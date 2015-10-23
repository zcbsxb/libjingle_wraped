#pragma once

#include "XmppDef.h"

class XmppMessageInfo : public IXmppMessageInfo
{
public:
    XmppMessageInfo();
    virtual ~XmppMessageInfo();

public:
    std::wstring GetUid(){ return uid; };
    void SetUid(const std::wstring &uid){ this->uid = uid; };

    std::wstring GetType(){ return type; };
    void SetType(const std::wstring &type){ this->type = type; };

    bool GetIsOfflineMsg(){ return isOfflineMsg; };
    void SetIsOfflineMsg(bool isofflineMsg){ this->isOfflineMsg = isofflineMsg; };

    std::wstring GetFrom(){ return from; };
    std::wstring GetFromResource(){ return fromResource; };

    void SetFrom(const std::wstring &from){ this->from = from; };
    void SetFromResource(const std::wstring &fromResource){ this->fromResource = fromResource; };

    std::wstring GetTo(){ return to; };
    std::wstring GetToResource(){ return toResource; };

    void SetTo(const std::wstring &to){ this->to = to; };
    void SetToResource(const std::wstring &toResource){ this->toResource = toResource; };

    std::wstring GetContent() const { return content; };
    void SetContent(const std::wstring &content){ this->content = content; };

    std::wstring GetTime(){ return time; };
    void SetTime(const std::wstring &time){ this->time = time; };

    std::wstring GetSubject(){ return subject; };
    void SetSubject(const std::wstring &subject){ this->subject = subject; };

    std::wstring GetSubjectValue(){ return subjectValue; };
    void SetSubjectValue(const std::wstring &subjectValue){ this->subjectValue = subjectValue; };

    std::map<std::wstring, std::wstring> GetExtention(){ return mapExtention; };
    void SetExtention(const std::map<std::wstring, std::wstring> &mapExtention){ this->mapExtention = mapExtention; };

public:
    std::wstring uid;
    std::wstring type;
    bool isOfflineMsg;
    std::wstring from;
    std::wstring fromResource;
    std::wstring to;
    std::wstring toResource;
    std::wstring content;
    std::wstring time;
    std::wstring subject;
    std::wstring subjectValue;

    std::map<std::wstring, std::wstring> mapExtention;
};

