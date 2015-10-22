#pragma once

#include <webrtc/libjingle/xmpp/xmpptask.h>
#include <webrtc/libjingle/xmpp/xmppclient.h>

#include <codecvt>

class IXmppMessageInfo;

class XmppMessageReciveTask : public buzz::XmppTask
{
public:
    static std::string QN_EXTENTION_SPACE;
    static buzz::StaticQName QN_EXTENTION;
public:
    XmppMessageReciveTask(buzz::XmppClient *client);
    virtual ~XmppMessageReciveTask();

    virtual int ProcessStart(void) override;

protected:
    virtual bool HandleStanza(const buzz::XmlElement *stanza) override;

public:
    sigslot::signal1<const IXmppMessageInfo &> MessageReceived;
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
};

