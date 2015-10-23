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
        std::wstring uidStr;
        std::wstringstream os(uidStr);

        uidStr.append(L"status:");
        uidStr.append(s.GetFrom());
        uidStr.append(L" ");
        uidStr.append(s.GetStatusName());
        uidStr.append(L"\r\n");

        OutputDebugString(uidStr.c_str());
    }

    void OnMessage(const IXmppMessageInfo& messageInfo)
    {
        OutputDebugString(messageInfo.GetContent().c_str());
        OutputDebugString(L"\r\n");
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

    void OnLogInput(const std::wstring &loginfo)
    {
        std::wstring outputInfo(loginfo);
        outputInfo.append(L"\r\n");

        ::OutputDebugString(outputInfo.c_str());
    }

    void OnLogOutput(const std::wstring &loginfo)
    {
        std::wstring outputInfo(loginfo);
        outputInfo.append(L"\r\n");

        ::OutputDebugString(outputInfo.c_str());
    }

    void OnTimeOut()
    {
        ::OutputDebugString(L"timeout\r\n");
    }
};

std::string GetUid()
{
    return "";
}

int _tmain(int argc, _TCHAR* argv[])
{
    OutputInfo outputinfo;
    XmppManager xmppManager;

    xmppManager.SetChangeStatusCallback(std::bind(&OutputInfo::UpdateStatus, &outputinfo, _1, _2));
    xmppManager.SetUpdatePresenceStatusCallback(std::bind(&OutputInfo::UpdatePresence, &outputinfo, _1));
    xmppManager.SetOnMessageCallback(std::bind(&OutputInfo::OnMessage, &outputinfo, _1));
    xmppManager.SetOnLogCallback(std::bind(&OutputInfo::OnLogInput, &outputinfo, _1),
        std::bind(&OutputInfo::OnLogOutput, &outputinfo, _1));

    xmppManager.EnablePingServer(true);
    xmppManager.SetPingServerOption(20*1000, 15*1000, std::bind(&OutputInfo::OnTimeOut, &outputinfo));

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
            xmppManager.Login(L"XXX",
                L"XXX", L"XXX", 5222);
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

            std::wstring uidStr;
            std::wstringstream os(uidStr);
            os << uid;
            auto msgInfo = xmppManager.BuildMessage(uidStr);
            msgInfo->SetTo(L"XXX");
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

