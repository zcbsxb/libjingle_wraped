#include "stdafx.h"
#include "XmppMessage.h"
#include "XmppConn.h"
#include "XmppDef.h"
#include "XmppMessageReciveTask.h"

#include <memory>
#include <webrtc/libjingle/xmllite/xmlelement.h>
#include <webrtc/libjingle/xmpp/constants.h>

using namespace std;

XmppMessage::XmppMessage(XmppConn* pXmppConn)
    : m_pXmppConn(pXmppConn)
    , onMessageCallback(nullptr)
{
}

XmppMessage::~XmppMessage()
{
}

void XmppMessage::SetOnMessageCallback(OnMessageCallback callback)
{
    onMessageCallback = callback;
}

void XmppMessage::Start()
{
    // xmpptask对象的释放由xmppclient处理
    auto messageReciveTask = new XmppMessageReciveTask(m_pXmppConn->GetClient());
    messageReciveTask->MessageReceived.connect(this, &XmppMessage::OnMessage);
    messageReciveTask->Start();
}

void XmppMessage::OnMessage(const IXmppMessageInfo &messageInfo)
{
    if (onMessageCallback != nullptr)
    {
        onMessageCallback(messageInfo);
    }
}

void XmppMessage::Send(IXmppMessageInfo* pMsgInfo)
{
    auto body = make_unique<buzz::XmlElement>(buzz::QN_BODY);
    body->SetBodyText(conv.to_bytes(pMsgInfo->GetContent()));

    std::wostringstream oss;
    oss << pMsgInfo->GetTo();

    auto toResource = pMsgInfo->GetToResource();
    if (!toResource.empty())
    {
        oss << '/';
        oss << toResource;
    }

    auto stanza = new (buzz::XmlElement)(buzz::QN_MESSAGE);
    stanza->SetAttr(buzz::QN_TO, WStrToUtf8(oss.str()));
    stanza->SetAttr(buzz::QN_TYPE, "chat");
    stanza->SetAttr(buzz::QN_ID, WStrToUtf8(pMsgInfo->GetUid()));
    stanza->AddElement(body.release());

    auto subject = pMsgInfo->GetSubject();
    if (!subject.empty())
    {
        auto subjectEl = make_unique<buzz::XmlElement>(buzz::QN_SUBJECT);
        subjectEl->SetBodyText(WStrToUtf8(subject));

        auto subjectValue = pMsgInfo->GetSubjectValue();
        if (!subjectValue.empty())
        {
            subjectEl->AddAttr(buzz::QN_VALUE, WStrToUtf8(subjectValue));
        }

        stanza->AddElement(subjectEl.release());
    }

    // 扩展数据
    auto extention = pMsgInfo->GetExtention();
    if (!extention.empty())
    {
        auto el = make_unique<buzz::XmlElement>(XmppMessageReciveTask::QN_EXTENTION);

        for (auto it : extention)
        {
            buzz::StaticQName QN_NAME = { XmppMessageReciveTask::QN_EXTENTION_SPACE.c_str(), WStrToUtf8(it.first).c_str() };
            auto elChild = make_unique<buzz::XmlElement>(QN_NAME);

            elChild->SetBodyText(WStrToUtf8(it.second));

            el->AddElement(elChild.release());
        }

        stanza->AddElement(el.release());
    }

    m_pXmppConn->QueueStanza(stanza);
}
