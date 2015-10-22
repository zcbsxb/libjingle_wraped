// XmppApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

//#include "XmppConn.h"
//#include "XmppPresence.h"

#include "XmppManager.h"
using namespace std::placeholders;

class OutputInfo
{
public:
    void UpdatePresence(const IXmppPresenceStatus& s)
    {
        int i = 0;
        i++;
    }

    void OnMessage(const IXmppMessageInfo& messageInfo)
    {
        OutputDebugStringW(messageInfo.GetContent().c_str());
    }

    void UpdateStatus(const XmppStatus& status, const XmppError& error)
    {
        switch (status)
        {
        case XmppStatus::STATE_OPEN:
            ::OutputDebugStringA("open\r\n");
            break;
        case XmppStatus::STATE_CLOSE:
            ::OutputDebugStringA("closed\r\n");
            break;
        }
    }
};

std::string GetUid()
{
    return "";
}

int _tmain(int argc, _TCHAR* argv[])
{
    /*XmppConn xmppConn;
    xmppConn.Conn("3_zhe800_waiter_05@im.zhe800.com/4ee4dff6-f678-3c4a-8029-fc769cd44b60_WindowsDesktop", "732061744", "192.168.100.222", 5222);

    XmppPresence xmppPresence(&xmppConn);
    xmppPresence.Start();*/

    OutputInfo outputinfo;
    XmppManager xmppManager;

    xmppManager.SetChangeStatusCallback(std::bind(&OutputInfo::UpdateStatus, &outputinfo, _1, _2));
    xmppManager.SetUpdatePresenceStatusCallback(std::bind(&OutputInfo::UpdatePresence, &outputinfo, _1));
    xmppManager.SetOnMessageCallback(std::bind(&OutputInfo::OnMessage, &outputinfo, _1));

    while (true)
    {
        int i;
        std::cin >> i;
        if (i == 1)
        {
            xmppManager.Logout();
        }
        else if (i == 2)
        {
            xmppManager.Login("XXXXX", "XXX", "XXXX", 5222);
        }else if (i == 3)
        {
            xmppManager.SendStatus(IXmppPresenceStatus::SHOW_ONLINE);
        }
        else if (i == 4)
        {
            xmppManager.SendStatus(IXmppPresenceStatus::SHOW_DND);
        }
        else if (i == 5)
        {
            xmppManager.SendStatus(IXmppPresenceStatus::SHOW_AWAY);
        }
        else if (i == 6)
        {
            static int64 uid = 11112313123123;
            uid++;

            std::string uidStr;
            std::stringstream os(uidStr);
            os << uid;
            auto msgInfo = xmppManager.BuildMessage(uidStr);
            msgInfo->SetTo("XXXX");
            msgInfo->SetContent(L"²âÊÔ·¢ËÍ×Ö·û");
            xmppManager.SendMsg(msgInfo);
        }
        else
        {
            return 0;
        }
    }

	return 0;
}

