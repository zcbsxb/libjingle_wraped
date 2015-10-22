#pragma once

#include "XmppDef.h"

class XmppMessageInfo : public IXmppMessageInfo
{
public:
    XmppMessageInfo();
    virtual ~XmppMessageInfo();

public:
    std::string GetUid(){ return uid; };
    void SetUid(const std::string &uid){ this->uid = uid; };

    std::string GetType(){ return type; };
    void SetType(const std::string &type){ this->type = type; };

    bool GetIsOfflineMsg(){ return isOfflineMsg; };
    void SetIsOfflineMsg(bool isofflineMsg){ this->isOfflineMsg = isofflineMsg; };

    std::string GetFrom(){ return from; };
    std::string GetFromResource(){ return fromResource; };

    void SetFrom(const std::string &from){ this->from = from; };
    void SetFromResource(const std::string &fromResource){ this->fromResource = fromResource; };

    std::string GetTo(){ return to; };
    std::string GetToResource(){ return toResource; };

    void SetTo(const std::string &to){ this->to = to; };
    void SetToResource(const std::string &toResource){ this->toResource = toResource; };

    std::wstring GetContent() const { return content; };
    void SetContent(const std::wstring &content){ this->content = content; };

    std::string GetTime(){ return time; };
    void SetTime(const std::string &time){ this->time = time; };

    std::string GetSubject(){ return subject; };
    void SetSubject(const std::string &subject){ this->subject = subject; };

    std::string GetSubjectValue(){ return subjectValue; };
    void SetSubjectValue(const std::string &subjectValue){ this->subjectValue = subjectValue; };

    std::map<std::string, std::string> GetExtention(){ return mapExtention; };
    void SetExtention(const std::map<std::string, std::string> &mapExtention){ this->mapExtention = mapExtention; };

public:
    std::string uid;
    std::string type;
    bool isOfflineMsg;
    std::string from;
    std::string fromResource;
    std::string to;
    std::string toResource;
    std::wstring content;
    std::string time;
    std::string subject;
    std::string subjectValue;

    std::map<std::string, std::string> mapExtention;
};

