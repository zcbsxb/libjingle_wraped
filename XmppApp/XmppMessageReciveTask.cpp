#include "stdafx.h"
#include "XmppMessageReciveTask.h"
#include "XmppMessageInfo.h"

#include <webrtc/libjingle/xmllite/xmlelement.h>
#include <webrtc/libjingle/xmpp/constants.h>
#include <webrtc/libjingle/xmpp/xmppclient.h>

std::string XmppMessageReciveTask::QN_EXTENTION_SPACE = "http://im.zhe800.com";
buzz::StaticQName XmppMessageReciveTask::QN_EXTENTION = { QN_EXTENTION_SPACE.c_str(), "extension" };

XmppMessageReciveTask::XmppMessageReciveTask(buzz::XmppClient *client)
    : XmppTask(client, buzz::XmppEngine::HL_TYPE)
{
}

XmppMessageReciveTask::~XmppMessageReciveTask()
{
}

int XmppMessageReciveTask::ProcessStart(void)
{
    static buzz::StaticQName QN_DELAY = { "urn:xmpp:delay", "delay" };

    // ��ȡ��һ��stanza
    const auto stanza = NextStanza();
    if (stanza == nullptr)
    {
        return STATE_BLOCKED;
    }

    // ��ȡ��Ϣ��
    const auto body = stanza->FirstNamed(buzz::QN_BODY);
    if (body == nullptr)
    {
        return STATE_BLOCKED;
    }

    XmppMessageInfo message;

    // ��Ϣid
    message.SetUid(Utf8ToWStr(stanza->Attr(buzz::QN_ID)));
    // ��Ϣ����
    message.SetType(Utf8ToWStr(stanza->Attr(buzz::QN_TYPE)));

    // ��ȡ������
    std::wstring from(Utf8ToWStr(stanza->Attr(buzz::QN_FROM)));
    message.SetFrom(from);
    message.SetFromResource(from);

    // ��ȡ������
    message.SetTo(Utf8ToWStr(stanza->Attr(buzz::QN_TO)));
    message.SetToResource(Utf8ToWStr(stanza->Attr(buzz::QN_TO)));

    // ��ȡ��Ϣ
    message.SetContent(conv.from_bytes(body->BodyText()));

    // ��ȡʱ��
    const auto delay = stanza->FirstNamed(QN_DELAY);
    if (delay != nullptr)
    {
        message.SetTime(Utf8ToWStr(delay->Attr(buzz::kQnStamp)));
    }

    // �Ƿ�������Ϣ
    message.SetIsOfflineMsg(delay != nullptr);

    // ��ȡ����
    const auto subject = stanza->FirstNamed(buzz::QN_SUBJECT);
    if (subject != nullptr)
    {
        message.SetSubject(Utf8ToWStr(subject->BodyText()));

        // ��������
        if (subject->HasAttr(buzz::QN_VALUE))
        {
            message.SetSubjectValue(Utf8ToWStr(subject->Attr(buzz::QN_VALUE)));
        }
    }

    // ��ȡ��չ����
    auto extention = stanza->FirstNamed(QN_EXTENTION);
    if (extention != nullptr)
    {
        std::map<std::wstring, std::wstring> mapValue;

        auto elChild = extention->FirstElement();
        while (elChild != nullptr)
        {
            auto name = elChild->Name().LocalPart();
            mapValue.emplace(Utf8ToWStr(name), Utf8ToWStr(elChild->BodyText().c_str()));

            elChild = elChild->NextElement();
        }

        message.SetExtention(mapValue);
    }

    MessageReceived(message);

    return STATE_START;
}

bool XmppMessageReciveTask::HandleStanza(const buzz::XmlElement *stanza)
{
    if (stanza->Name() != buzz::QN_MESSAGE)
    {
        return false;
    }

    QueueStanza(stanza);

    return true;
}