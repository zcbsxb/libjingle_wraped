#pragma once

#include "XmppDef.h"

#include "webrtc/base/sigslot.h"
#include <codecvt>

class XmppConn;

class XmppMessage : public sigslot::has_slots<>
{
public:
    XmppMessage(XmppConn* pXmppConn);
    virtual ~XmppMessage();

    void SetOnMessageCallback(OnMessageCallback callback);

    void Start();

    void Send(IXmppMessageInfo* pMsgInfo);

    void OnMessage(const IXmppMessageInfo &messageInfo);
private:
    XmppConn* m_pXmppConn;
    OnMessageCallback onMessageCallback;
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
};

