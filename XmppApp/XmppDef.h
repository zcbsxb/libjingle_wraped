#pragma once

#include <functional>
#include <map>

/*******************************************************************
*  类目：XmppStatus
*  描述：XMPP状态接口
******************************************************************/
enum XmppStatus
{
    STATE_OPEN,
    STATE_CLOSE,
};

enum XmppError
{
    ERROR_NONE = 0,         //!< No error
    ERROR_XML,              //!< Malformed XML or encoding error
    ERROR_STREAM,           //!< XMPP stream error - see GetStreamError()
    ERROR_VERSION,          //!< XMPP version error
    ERROR_UNAUTHORIZED,     //!< User is not authorized (rejected credentials)
    ERROR_TLS,              //!< TLS could not be negotiated
    ERROR_AUTH,             //!< Authentication could not be negotiated
    ERROR_BIND,             //!< Resource or session binding could not be negotiated
    ERROR_CONNECTION_CLOSED,//!< Connection closed by output handler.
    ERROR_DOCUMENT_CLOSED,  //!< Closed by </stream:stream>
    ERROR_SOCKET,           //!< Socket error
    ERROR_NETWORK_TIMEOUT,  //!< Some sort of timeout (eg., we never got the roster)
    ERROR_MISSING_USERNAME, //!< User has a Google Account but no nickname
    ERROR_STREAM_CONFLICT,  //!< XMPP stream error, error by conflict
};

/*******************************************************************
*  类目：IXmppPresenceStatus
*  描述：用户状态接口
******************************************************************/
class IXmppPresenceStatus
{
public:
    enum Status
    {
        SHOW_NONE = 0,
        SHOW_OFFLINE = 1,
        SHOW_XA = 2,
        SHOW_AWAY = 3,
        SHOW_DND = 4,
        SHOW_ONLINE = 5,
        SHOW_CHAT = 6,
    };

public:
    virtual std::wstring GetFrom() const = 0;

    virtual Status GetStatus() const = 0;

    virtual std::wstring GetStatusName() const = 0;
};

/*******************************************************************
*  类目：IXmppMessageInfo
*  描述：消息内容描述接口
******************************************************************/
class IXmppMessageInfo
{
public:
    virtual std::wstring GetUid() = 0;
    virtual void SetUid(const std::wstring &) = 0;

    virtual std::wstring GetType() = 0;
    virtual void SetType(const std::wstring &) = 0;

    virtual bool GetIsOfflineMsg() = 0;
    virtual void SetIsOfflineMsg(bool) = 0;

    virtual std::wstring GetFrom() = 0;
    virtual std::wstring GetFromResource() = 0;

    virtual void SetFrom(const std::wstring &) = 0;
    virtual void SetFromResource(const std::wstring &) = 0;

    virtual std::wstring GetTo() = 0;
    virtual std::wstring GetToResource() = 0;

    virtual void SetTo(const std::wstring &) = 0;
    virtual void SetToResource(const std::wstring &) = 0;

    virtual std::wstring GetContent() const = 0;
    virtual void SetContent(const std::wstring &) = 0;

    virtual std::wstring GetTime() = 0;
    virtual void SetTime(const std::wstring &) = 0;

    virtual std::wstring GetSubject() = 0;
    virtual void SetSubject(const std::wstring &) = 0;

    virtual std::wstring GetSubjectValue() = 0;
    virtual void SetSubjectValue(const std::wstring &subjectValue) = 0;

    virtual std::map<std::wstring, std::wstring> GetExtention() = 0;
    virtual void SetExtention(const std::map<std::wstring, std::wstring> &) = 0;
};

typedef std::function<void(const XmppStatus&, const XmppError&)> ChangeStatusCallback;
typedef std::function<void(const IXmppPresenceStatus&)> UpdatePresenceStatusCallback;
typedef std::function<void(const IXmppMessageInfo&)> OnMessageCallback;
typedef std::function<void(const std::wstring&)> OnLogCallcack;
typedef std::function<void()> OnPingTimerOut;

/*******************************************************************
*  类目：IXmppManager
*  描述：XMPP管理类
******************************************************************/
class IXmppManager
{
public:
    IXmppManager(){ };
    virtual ~IXmppManager(){ };

public:
    virtual void SetChangeStatusCallback(ChangeStatusCallback callback) = 0;

    virtual void SetUpdatePresenceStatusCallback(UpdatePresenceStatusCallback callback) = 0;

    virtual void SetOnMessageCallback(OnMessageCallback callback) = 0;

    virtual void SetOnLogCallback(OnLogCallcack logInputCallback, OnLogCallcack logOuputCallback) = 0;

    virtual void EnablePingServer(bool enable) = 0;

    virtual void SetPingServerOption(int periodMillis, int timeoutMillis, OnPingTimerOut timeOut) = 0;

    virtual bool Login(const std::wstring &userJid, const std::wstring &userPassword, const std::wstring &serverIp, int serverPort) = 0;

    virtual void Logout() = 0;

    virtual bool SendStatus(const IXmppPresenceStatus::Status &status) = 0;

    virtual bool SendMsg(IXmppMessageInfo* messageInfo) = 0;

    virtual IXmppMessageInfo* BuildMessage(const std::wstring &uid) = 0;
};

