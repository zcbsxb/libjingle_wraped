#pragma once

#include "XmppDef.h"

class XmppPresenceStatus : public IXmppPresenceStatus
{
public:
    XmppPresenceStatus(Status status);
    virtual ~XmppPresenceStatus();

public:
    virtual Status GetStatus() override;

    virtual std::wstring GetStatusName() override;

private:
    Status status;
};

