#pragma once

#include "XmppDef.h"

class XmppPresenceStatus : public IXmppPresenceStatus
{
public:
    XmppPresenceStatus(Status status);
    virtual ~XmppPresenceStatus();

public:
    virtual std::wstring GetFrom() const override;

    virtual Status GetStatus() const override;

    virtual std::wstring GetStatusName() const override;

    void SetFrom(const std::wstring &jid);
private:
    Status status;
    std::wstring jid;
};

