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

bool XmppManager::Login(const std::string &userJid, const std::string &userPassword, const std::string &serverIp, int serverPort)
{
    return xmppConn.Conn(userJid, userPassword, serverIp, serverPort);
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

IXmppMessageInfo* XmppManager::BuildMessage(const std::string &uid)
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