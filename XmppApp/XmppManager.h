#pragma once

#include "XmppDef.h"
#include "XmppConn.h"
#include "XmppPresence.h"
#include "XmppMessage.h"

#include <atomic>

class XmppManager : public IXmppManager
{
public:
    XmppManager();
    virtual ~XmppManager();

    virtual void SetChangeStatusCallback(ChangeStatusCallback callback) override;

    virtual void SetUpdatePresenceStatusCallback(UpdatePresenceStatusCallback callback) override;

    virtual void SetOnMessageCallback(OnMessageCallback callback) override;

    virtual bool Login(const std::string &userJid, const std::string &userPassword, const std::string &serverIp, int serverPort) override;

    virtual void Logout() override;

    virtual bool SendStatus(const IXmppPresenceStatus::Status &status) override;

    virtual bool SendMsg(IXmppMessageInfo* messageInfo) override;

    virtual IXmppMessageInfo* BuildMessage(const std::string &uid) override;
private:
    void OnChangeStatus(const XmppStatus& status, const XmppError& error);

private:
    std::atomic<bool> xmppConnected;
    XmppConn xmppConn;
    XmppPresence xmppPresence;
    XmppMessage xmppMessage;

    ChangeStatusCallback changeStatusCallback;
};