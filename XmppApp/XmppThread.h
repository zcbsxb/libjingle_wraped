#pragma once

#include "webrtc/libjingle/xmpp/xmppclientsettings.h"
#include "webrtc/libjingle/xmpp/xmppengine.h"
#include "webrtc/libjingle/xmpp/xmpppump.h"
#include "webrtc/libjingle/xmpp/xmppsocket.h"
#include "webrtc/base/thread.h"

#include <atomic>

class XmppThread:
    public rtc::Thread, buzz::XmppPumpNotify, rtc::MessageHandler, public sigslot::has_slots<>
{
public:
    XmppThread();
    virtual ~XmppThread();

    void Conn(const buzz::XmppClientSettings & xcs, int timerOutSeconds);
    void DisConn();

    bool QueueStanza(buzz::XmlElement* pEle);

    buzz::XmppClient* client();
    void ProcessMessages(int cms);
private:
    void OnStateChange(buzz::XmppEngine::State state);

    void OnMessage(rtc::Message* pmsg);

    void OnMessageLogin(rtc::Message* pmsg);
    void OnMessageDisconnect(rtc::Message* pmsg);
    void OnMessageStanza(rtc::Message* pmsg);
    void OnMessageLoginTimeOut(rtc::Message* pmsg);

public:
    sigslot::signal0<> SignalPreConn;
    sigslot::signal0<> SignalConnTimeOut;

private:
    buzz::XmppPump* pump_;

    std::wstring connId;
};

