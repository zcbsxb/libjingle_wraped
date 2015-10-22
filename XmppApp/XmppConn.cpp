#include "stdafx.h"
#include "XmppConn.h"

#include "webrtc/libjingle/xmpp/presenceouttask.h"
#include "webrtc/libjingle/xmpp/presencereceivetask.h"

#include <codecvt>

using namespace buzz;

XmppConn::XmppConn(ChangeStatusCallback changeStatusCallback)
    : m_changeStatusCallback(changeStatusCallback)
    , xmppThread()
    , pPingTask(nullptr)
{
    xmppThread.SignalPreConn.connect(this, &XmppConn::OnPreStart);

    xmppThread.Start();
}

XmppConn::~XmppConn()
{
    xmppThread.Stop();
}

bool XmppConn::Conn(const std::string &userJid, const std::string &userPassword, const std::string &serverIp, int serverPort)
{
    // 解析域名
    addrinfo *result = nullptr;
    addrinfo hints = { 0 };
    hints.ai_family = AF_INET;
    hints.ai_flags = AI_ADDRCONFIG;
    auto ret = ::getaddrinfo(serverIp.c_str(), nullptr, &hints, &result);
    if (ret != 0)
    {
        return false;
    }

    // 查找对应IP
    rtc::IPAddress ipAddress;
    for (auto cursor = result; cursor != nullptr; cursor = cursor->ai_next)
    {
        if (cursor->ai_family == AF_INET)
        {
            if (rtc::IPFromAddrInfo(cursor, &ipAddress))
            {
                break;
            }
        }
    }
    ::freeaddrinfo(result);

    // 判断是否有满足条件的IP
    if (ipAddress.family() != AF_INET)
    {
        return false;
    }
    rtc::SocketAddress server{ ipAddress, serverPort };

    // 设置密码
    rtc::InsecureCryptStringImpl csi;
    csi.password() = userPassword;

    // 客户端设置
    buzz::Jid jid(userJid);
    buzz::XmppClientSettings xcs;
    xcs.set_user(jid.node());
    xcs.set_host(jid.domain());
    xcs.set_resource(jid.resource());
    xcs.set_pass(rtc::CryptString(csi));
    xcs.set_use_tls(buzz::TlsOptions::TLS_DISABLED);
    xcs.set_server(server);

    xmppThread.Conn(xcs);

    return true;
}

void XmppConn::DisConn()
{
    xmppThread.DisConn();
}

buzz::XmppClient* XmppConn::GetClient()
{
    return xmppThread.client();
}

void XmppConn::QueueStanza(buzz::XmlElement* pEle)
{
    xmppThread.QueueStanza(pEle);
}

void XmppConn::OnPreStart()
{
    auto client = xmppThread.client();
    client->SignalStateChange.connect(this, &XmppConn::OnStateChange);
    client->SignalLogInput.connect(this, &XmppConn::OnLogInput);
    client->SignalLogOutput.connect(this, &XmppConn::OnLogOutput);
}

void XmppConn::OnStateChange(buzz::XmppEngine::State state)
{
    switch (state)
    {
    case buzz::XmppEngine::STATE_OPEN:
    {
        OnStarted();

        break;
    }
    case buzz::XmppEngine::STATE_CLOSED:
    {
        OnClosed();

        break;
    }
    default:
        break;
    }
}

void XmppConn::OnStarted()
{
    StartPing();

    m_changeStatusCallback(STATE_OPEN, ERROR_NONE);
}

void XmppConn::OnClosed()
{
    int subCode;
    auto error = xmppThread.client()->GetError(&subCode);
    if (error == buzz::XmppEngine::Error::ERROR_STREAM)
    {
        auto stanzaError = xmppThread.client()->GetStreamError();
        if (stanzaError != nullptr && 
            stanzaError->Name() == buzz::QN_STREAM_ERROR &&
            nullptr != stanzaError->FirstNamed(buzz::QN_XSTREAM_CONFLICT))
        {
            m_changeStatusCallback(STATE_CLOSE, ERROR_STREAM_CONFLICT);

            return;
        }
    }

    m_changeStatusCallback(STATE_CLOSE, static_cast<XmppError>(error));
}

void XmppConn::OnLogInput(const char * pLog, int iLen)
{
    std::string output = "[IN ]" + std::string(pLog, iLen) + "\r\n";

    ::OutputDebugStringA(output.c_str());
}

void XmppConn::OnLogOutput(const char * pLog, int iLen)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
    std::string output = "[OUT]" + std::string(pLog, iLen) + "\r\n";

    ::OutputDebugStringA(output.c_str());
}

void XmppConn::StartPing()
{
    pPingTask = new PingTask(xmppThread.client(), &xmppThread, PING_PERIOD_MILLIS, PING_TIMEROUT_MILLIS);

    pPingTask->SignalTimeout.connect(this, &XmppConn::OnTimeOut);
    pPingTask->Start();
}

void XmppConn::OnTimeOut()
{
    //DisConn();

    ::OutputDebugStringA("OnTimerOut\r\n");
}