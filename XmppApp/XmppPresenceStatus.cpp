#include "stdafx.h"
#include "XmppPresenceStatus.h"

XmppPresenceStatus::XmppPresenceStatus(Status s)
    : status(s)
{
}

XmppPresenceStatus::~XmppPresenceStatus()
{
}

XmppPresenceStatus::Status XmppPresenceStatus::GetStatus()
{
    return status;
}

std::wstring XmppPresenceStatus::GetStatusName()
{
    std::wstring name;
    switch (status)
    {
    case IXmppPresenceStatus::SHOW_NONE:
        break;
    case IXmppPresenceStatus::SHOW_OFFLINE:
        name = L"����";
        break;
    case IXmppPresenceStatus::SHOW_XA:
        name = L"�뿪";
        break;
    case IXmppPresenceStatus::SHOW_AWAY:
        name = L"�뿪";
        break;
    case IXmppPresenceStatus::SHOW_DND:
        name = L"æµ";
        break;
    case IXmppPresenceStatus::SHOW_ONLINE:
        name = L"����";
        break;
    case IXmppPresenceStatus::SHOW_CHAT:
        name = L"����";
        break;
    default:
        break;
    }

    return name;
}