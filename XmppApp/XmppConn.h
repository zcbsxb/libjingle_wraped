#pragma once

#include "webrtc/libjingle/xmpp/pingtask.h"

#include "XmppDef.h"
#include "XmppThread.h"

class XmppConn : public sigslot::has_slots<>
{
public:
    XmppConn(ChangeStatusCallback changeStatusCallback);
    virtual ~XmppConn();

    void SetOnLogCallback(OnLogCallcack logInputCallback, OnLogCallcack logOuputCallback);

    void EnablePingServer(bool enable);

    void SetPingServerOption(int periodMillis, int timeoutMillis, OnPingTimerOut timeOut);

    bool Conn(const std::wstring &userJid, const std::wstring &userPassword, const std::wstring &serverIp, int serverPort, int timerOutSeconds);

    void DisConn();

    buzz::XmppClient* GetClient();

    void QueueStanza(buzz::XmlElement* pEle);
private:
    void OnPreStart();

    void OnConnTimeOut();

    void OnStateChange(buzz::XmppEngine::State state);

    void OnStarted();

    void OnClosed();

    void OnLogInput(const char * pLog, int iLen);

    void OnLogOutput(const char * pLog, int iLen);

    void OnTimeOut();

    void StartPing();
private:
    const int PING_PERIOD_MILLIS = 20 * 1000;
    const int PING_TIMEROUT_MILLIS = 10 * 1000;

    ChangeStatusCallback m_changeStatusCallback;

    XmppThread xmppThread;

    OnLogCallcack logInputCallback;
    OnLogCallcack logOutputCallback;

    bool enablePingTask;
    int pingTaskPeroidMillis;
    int pingTaskTimeroutMillis;
    OnPingTimerOut onPingTimerOutCallback;
};

