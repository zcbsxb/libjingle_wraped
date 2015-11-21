#include "stdafx.h"
#include "XmppThread.h"
#include "PubTools.h"

#include "webrtc/libjingle/xmpp/xmppauth.h"
#include "webrtc/libjingle/xmpp/xmppclientsettings.h"

namespace 
{
    const uint32 MSG_LOGIN = 1;
    const uint32 MSG_DISCONNECT = 2;
    const uint32 MSG_SENDSTANZA = 3;
    const uint32 MSG_LOGINTIMEOUT = 4;

    struct LoginData : public rtc::MessageData 
    {
        LoginData(const buzz::XmppClientSettings& s, int time) : xcs(s), timeoutTimeSecond(time)
        {
        };

        virtual ~LoginData() {};

        buzz::XmppClientSettings xcs;
        int timeoutTimeSecond;
    };

    struct StanzaData : public rtc::MessageData
    {
        StanzaData(buzz::XmlElement* ele) : pEle(ele) 
        {
        };

        virtual ~StanzaData() 
        { 
            delete pEle;
        };

        buzz::XmlElement* pEle;
    };

    struct LoginTimeOutData : public rtc::MessageData
    {
        LoginTimeOutData(std::wstring id) : connId(id)
        {

        };

        std::wstring connId;
    };
}

XmppThread::XmppThread()
    : pump_(nullptr)
    , connId()
{
}

XmppThread::~XmppThread()
{
    Stop();

    OnMessageDisconnect(nullptr);
}

void XmppThread::Conn(const buzz::XmppClientSettings& xcs, int timerOutSeconds)
{
    Post(this, MSG_LOGIN, new LoginData(xcs, timerOutSeconds));
}

void XmppThread::DisConn()
{
    Post(this, MSG_DISCONNECT);
}

bool XmppThread::QueueStanza(buzz::XmlElement* pEle)
{
    Post(this, MSG_SENDSTANZA, new StanzaData(pEle));

    return true;
}

buzz::XmppClient* XmppThread::client()
{
    if (pump_ == nullptr)
    {
        return nullptr;
    }

    return pump_->client();
}

void XmppThread::ProcessMessages(int cms)
{
    rtc::Thread::ProcessMessages(cms);
}

void XmppThread::OnStateChange(buzz::XmppEngine::State state)
{
    switch (state)
    {
    case buzz::XmppEngine::STATE_OPEN:
    {
        connId.clear();

        break;
    }
    case buzz::XmppEngine::STATE_CLOSED:
    {
        break;
    }
    default:
        break;
    }
}

void XmppThread::OnMessage(rtc::Message* pmsg) 
{
    if (pmsg->message_id == MSG_LOGIN)
    {
        OnMessageLogin(pmsg);
    }
    else if (pmsg->message_id == MSG_DISCONNECT) 
    {
        OnMessageDisconnect(pmsg);
        
    }
    else if (pmsg->message_id == MSG_SENDSTANZA)
    {
        OnMessageStanza(pmsg);
    }
    else if (pmsg->message_id == MSG_LOGINTIMEOUT)
    {
        OnMessageLoginTimeOut(pmsg);
    }
    else 
    {
        ASSERT(false);
    }
}

void XmppThread::OnMessageLogin(rtc::Message* pmsg)
{
    if (pump_ != nullptr)
    {
        delete pump_;
        pump_ = nullptr;
    }

    pump_ = new buzz::XmppPump(this);

    SignalPreConn();

    LoginData* data = reinterpret_cast<LoginData*>(pmsg->pdata);
    auto xmppSocket = new buzz::XmppSocket(buzz::TLS_DISABLED);

    pump_->DoLogin(data->xcs, xmppSocket, new XmppAuth());

    // 连接超时处理
    connId = PubTools::GenerateUUID();

    PostDelayed(data->timeoutTimeSecond * 1000, this, MSG_LOGINTIMEOUT, new LoginTimeOutData(connId));

    delete data;
}

void XmppThread::OnMessageDisconnect(rtc::Message* pmsg)
{
    if (pump_ != nullptr)
    {
        pump_->DoDisconnect();

        delete pump_;
        pump_ = nullptr;
    }
}

void XmppThread::OnMessageStanza(rtc::Message* pmsg)
{
    if (pump_ != nullptr)
    {
        auto pData = reinterpret_cast<StanzaData*>(pmsg->pdata);

        pump_->SendStanza(pData->pEle);

        delete pData;
    }
}

void XmppThread::OnMessageLoginTimeOut(rtc::Message* pmsg)
{
    LoginTimeOutData* data = reinterpret_cast<LoginTimeOutData*>(pmsg->pdata);
    if (connId != data->connId)
    {
        return;
    }

    OnMessageDisconnect(nullptr);

    // 通知登陆超时
    SignalConnTimeOut();
}
