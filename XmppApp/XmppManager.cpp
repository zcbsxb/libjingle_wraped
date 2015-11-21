#include "stdafx.h"
#include "XmppManager.h"
#include "XmppMessageInfo.h"

using namespace std;
using namespace std::placeholders;

XmppManager::XmppManager()
    : xmppConnected(false)
    , xmppConn(std::bind(&XmppManager::OnChangeStatus, this, _1, _2))
    , xmppPresence(&xmppConn)
    , xmppMessage(&xmppConn)
    , changeStatusCallback(nullptr)
{
}

XmppManager::~XmppManager()
{
}

void XmppManager::SetChangeStatusCallback(ChangeStatusCallback callback)
{
    changeStatusCallback = callback;
}

void XmppManager::SetUpdatePresenceStatusCallback(UpdatePresenceStatusCallback callback)
{
    xmppPresence.SetUpdatePresenceStatusCallback(callback);
}

void XmppManager::SetOnMessageCallback(OnMessageCallback callback)
{
    xmppMessage.SetOnMessageCallback(callback);
}

void XmppManager::SetOnLogCallback(OnLogCallcack logInputCallback, OnLogCallcack logOuputCallback)
{
    xmppConn.SetOnLogCallback(logInputCallback, logOuputCallback);
}

void XmppManager::EnablePingServer(bool enable)
{
    xmppConn.EnablePingServer(enable);
}

void XmppManager::SetPingServerOption(int periodMillis, int timeoutMillis, OnPingTimerOut timeOut)
{
    xmppConn.SetPingServerOption(periodMillis, timeoutMillis, timeOut);
}

bool XmppManager::Login(const std::wstring &userJid, const std::wstring &userPassword, const std::wstring &serverIp, int serverPort, int timerOutSeconds)
{
    return xmppConn.Conn(userJid, userPassword, serverIp, serverPort, timerOutSeconds);
}

void XmppManager::Logout()
{
    xmppConn.DisConn();
}

bool XmppManager::SendStatus(const IXmppPresenceStatus::Status &status)
{
    if (!xmppConnected)
    {
        return false;
    }

    return xmppPresence.Send(status);
}

bool XmppManager::SendMsg(IXmppMessageInfo* messageInfo)
{
    if (!xmppConnected)
    {
        return false;
    }

    xmppMessage.Send(messageInfo);

    return true;
}

IXmppMessageInfo* XmppManager::BuildMessage(const std::wstring &uid)
{
    auto pMsgInfo = new XmppMessageInfo();
    pMsgInfo->SetUid(uid);

    return pMsgInfo;
}

void XmppManager::OnChangeStatus(const XmppStatus& status, const XmppError& error)
{
    switch (status)
    {
    case STATE_OPEN:
        xmppConnected = true;

        xmppPresence.Start();
        xmppMessage.Start();

        break;
    case STATE_CLOSE:
        xmppConnected = false;
        break;
    }

    if (changeStatusCallback != nullptr)
    {
        changeStatusCallback(status, error);
    }
}
