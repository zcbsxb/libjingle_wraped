#include "stdafx.h"
#include "XmppPresence.h"
#include "XmppConn.h"
#include "XmppPresenceStatus.h"

#include "webrtc/libjingle/xmpp/presenceouttask.h"
#include "webrtc/libjingle/xmpp/presencereceivetask.h"
#include "webrtc/base/stringencode.h"

using namespace buzz;

XmppPresence::XmppPresence(XmppConn* pXmppConn)
    : m_pXmppConn(pXmppConn)
    , updatePresenceStatusCallback(nullptr)
{
}

XmppPresence::~XmppPresence()
{
}

void XmppPresence::SetUpdatePresenceStatusCallback(UpdatePresenceStatusCallback callback)
{
    updatePresenceStatusCallback = callback;
}

void XmppPresence::Start()
{
    // 接受状态改变,xmpptask对象的释放由xmppclient处理
    auto pXmppPresenceReveive = new PresenceReceiveTask(m_pXmppConn->GetClient());
    pXmppPresenceReveive->PresenceUpdate.connect(this, &XmppPresence::PresenceUpdate);

    pXmppPresenceReveive->Start();
}

void XmppPresence::PresenceUpdate(const buzz::PresenceStatus& status)
{
    XmppPresenceStatus presenceStatus(static_cast<IXmppPresenceStatus::Status>(status.show()));

    if (updatePresenceStatusCallback != nullptr)
    {
        updatePresenceStatusCallback(presenceStatus);
    }
}

bool XmppPresence::Send(const IXmppPresenceStatus::Status &s)
{
    XmppPresenceStatus presentStatus(s);

    buzz::PresenceStatus status;
    status.set_jid(m_pXmppConn->GetClient()->jid());
    status.set_available(presentStatus.GetStatus() != buzz::PresenceStatus::SHOW_OFFLINE);
    status.set_show(static_cast<buzz::PresenceStatus::Show>(presentStatus.GetStatus()));
    status.set_priority(0);
    status.set_know_capabilities(true);
    status.set_video_capability(false);
    status.set_camera_capability(false);
    status.set_version("1.0.0.0");

    status.set_status(WStrToUtf8(presentStatus.GetStatusName()));

    auto pEle = TranslateStatus(status);
    m_pXmppConn->QueueStanza(pEle);

    return true;
}

XmlElement* XmppPresence::TranslateStatus(const PresenceStatus & s)
{
    XmlElement * result = new XmlElement(QN_PRESENCE);
    if (!s.available()) {
        result->AddAttr(QN_TYPE, STR_UNAVAILABLE);
    }
    else {
        if (s.show() != PresenceStatus::SHOW_ONLINE &&
            s.show() != PresenceStatus::SHOW_OFFLINE) {
            result->AddElement(new XmlElement(QN_SHOW));
            switch (s.show()) {
            default:
                result->AddText(STR_SHOW_AWAY, 1);
                break;
            case PresenceStatus::SHOW_XA:
                result->AddText(STR_SHOW_XA, 1);
                break;
            case PresenceStatus::SHOW_DND:
                result->AddText(STR_SHOW_DND, 1);
                break;
            case PresenceStatus::SHOW_CHAT:
                result->AddText(STR_SHOW_CHAT, 1);
                break;
            }
        }

        result->AddElement(new XmlElement(QN_STATUS));
        result->AddText(s.status(), 1);

        if (!s.nick().empty()) {
            result->AddElement(new XmlElement(QN_NICKNAME));
            result->AddText(s.nick(), 1);
        }

        std::string pri;
        rtc::ToString(s.priority(), &pri);

        result->AddElement(new XmlElement(QN_PRIORITY));
        result->AddText(pri, 1);

        if (s.know_capabilities()) {
            result->AddElement(new XmlElement(QN_CAPS_C, true));
            result->AddAttr(QN_NODE, s.caps_node(), 1);
            result->AddAttr(QN_VER, s.version(), 1);

            std::string caps;
            caps.append(s.voice_capability() ? "voice-v1" : "");
            caps.append(s.pmuc_capability() ? " pmuc-v1" : "");
            caps.append(s.video_capability() ? " video-v1" : "");
            caps.append(s.camera_capability() ? " camera-v1" : "");

            result->AddAttr(QN_EXT, caps, 1);
        }

        // Put the delay mark on the presence according to JEP-0091
    {
        result->AddElement(new XmlElement(kQnDelayX, true));

        // This here is why we *love* the C runtime
        time_t current_time_seconds;
        time(&current_time_seconds);
        struct tm* current_time = gmtime(&current_time_seconds);
        char output[256];
        strftime(output, ARRAY_SIZE(output), "%Y%m%dT%H:%M:%S", current_time);
        result->AddAttr(kQnStamp, output, 1);
    }
    }

    return result;
}