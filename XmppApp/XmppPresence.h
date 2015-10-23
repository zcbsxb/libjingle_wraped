#pragma once

#include "XmppDef.h"

#include "webrtc/libjingle/xmpp/presencestatus.h"
#include "webrtc/libjingle/xmllite/xmlelement.h"
#include "webrtc/base/sigslot.h"

class XmppConn;

class XmppPresence : public sigslot::has_slots<>
{
public:
    XmppPresence(XmppConn* pXmppConn);
    virtual ~XmppPresence();

    void SetUpdatePresenceStatusCallback(UpdatePresenceStatusCallback callback);

    void Start();

    bool Send(const IXmppPresenceStatus::Status &status);

private:
    void PresenceUpdate(const buzz::PresenceStatus& status);

    buzz::XmlElement* TranslateStatus(const buzz::PresenceStatus & s);
private:
    XmppConn* m_pXmppConn;
    UpdatePresenceStatusCallback updatePresenceStatusCallback;
};

