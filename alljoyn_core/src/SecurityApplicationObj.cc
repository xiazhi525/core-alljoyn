/**
 * @file
 * This implements the SecurityApplicationObj class
 */
/******************************************************************************
 * Copyright AllSeen Alliance. All rights reserved.
 *
 *    Permission to use, copy, modify, and/or distribute this software for any
 *    purpose with or without fee is hereby granted, provided that the above
 *    copyright notice and this permission notice appear in all copies.
 *
 *    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *    WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *    MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *    ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *    WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *    ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 ******************************************************************************/

#include <alljoyn/Status.h>
#include <alljoyn/AllJoynStd.h>
#include "SecurityApplicationObj.h"

#define QCC_MODULE "ALLJOYN_SECURITY"

using namespace qcc;
namespace ajn {

const uint16_t SecurityApplicationObj::APPLICATION_VERSION = 1;
const uint16_t SecurityApplicationObj::SECURITY_APPLICATION_VERSION = 1;
const uint16_t SecurityApplicationObj::SECURITY_CLAIMABLE_APPLICATION_VERSION = 1;
const uint16_t SecurityApplicationObj::SECURITY_MANAGED_APPLICATION_VERSION = 1;


SecurityApplicationObj::SecurityApplicationObj() :
    BusObject(org::alljoyn::Bus::Security::ObjectPath),
    m_bus(NULL)
{
}

QStatus SecurityApplicationObj::Init(BusAttachment* bus)
{
    QStatus status = ER_OK;
    {
        /* Add org.alljoyn.Bus.Peer.Authentication interface */
        const InterfaceDescription* ifc = bus->GetInterface(org::alljoyn::Bus::Application::InterfaceName);
        if (ifc == NULL) {
            QCC_LogError(status, ("Failed to get the %s interface", org::alljoyn::Bus::Application::InterfaceName));
            return ER_BUS_INTERFACE_MISSING;
        }
        status = AddInterface(*ifc);
        if (ER_OK != status) {
            QCC_LogError(status, ("Failed to add the %s interface", org::alljoyn::Bus::Application::InterfaceName));
            return status;
        }
    }
    {
        /* Add org.alljoyn.Bus.Security.Application interface */
        const InterfaceDescription* ifc = bus->GetInterface(org::alljoyn::Bus::Security::Application::InterfaceName);
        if (ifc == NULL) {
            QCC_LogError(status, ("Failed to get the %s interface", org::alljoyn::Bus::Security::Application::InterfaceName));
            return ER_BUS_INTERFACE_MISSING;
        }
        status = AddInterface(*ifc);
        if (ER_OK != status) {
            QCC_LogError(status, ("Failed to add the %s interface", org::alljoyn::Bus::Security::Application::InterfaceName));
        }
    }
    {
        /* Add org.alljoyn.Bus.Security.ClaimableApplication interface */
        const InterfaceDescription* ifc = bus->GetInterface(org::alljoyn::Bus::Security::ClaimableApplication::InterfaceName);
        if (ifc == NULL) {
            QCC_LogError(status, ("Failed to get the %s interface", org::alljoyn::Bus::Security::ClaimableApplication::InterfaceName));
            return ER_BUS_INTERFACE_MISSING;
        }
        status = AddInterface(*ifc);
        if (ER_OK != status) {
            QCC_LogError(status, ("Failed to add the %s interface", org::alljoyn::Bus::Security::Application::InterfaceName));
            return status;
        }
        status = AddMethodHandler(ifc->GetMember("Claim"), static_cast<MessageReceiver::MethodHandler>(&SecurityApplicationObj::Claim));
        if (ER_OK != status) {
            QCC_LogError(status, ("Failed to add method handler for  %s.Claim", org::alljoyn::Bus::Security::ClaimableApplication::InterfaceName));
            return status;
        }
    }
    {
        /* Add org.alljoyn.Bus.Security.ManagedApplication interface */
        const InterfaceDescription* ifc = bus->GetInterface(org::alljoyn::Bus::Security::ManagedApplication::InterfaceName);
        if (ifc == NULL) {
            QCC_LogError(status, ("Failed to get the %s interface", org::alljoyn::Bus::Security::ManagedApplication::InterfaceName));
            return ER_BUS_INTERFACE_MISSING;
        }
        status = AddInterface(*ifc);
        if (ER_OK != status) {
            QCC_LogError(status, ("Failed to add the %s interface", org::alljoyn::Bus::Security::ManagedApplication::InterfaceName));
            return status;
        }
        status = AddMethodHandler(ifc->GetMember("Reset"), static_cast<MessageReceiver::MethodHandler>(&SecurityApplicationObj::Reset));
        if (ER_OK != status) {
            QCC_LogError(status, ("Failed to add method handler for  %s.Reset", org::alljoyn::Bus::Security::ManagedApplication::InterfaceName));
            return status;
        }
        status = AddMethodHandler(ifc->GetMember("UpdateIdentity"), static_cast<MessageReceiver::MethodHandler>(&SecurityApplicationObj::UpdateIdentity));
        if (ER_OK != status) {
            QCC_LogError(status, ("Failed to add method handler for  %s.UpdateIdentity", org::alljoyn::Bus::Security::ManagedApplication::InterfaceName));
            return status;
        }
        status = AddMethodHandler(ifc->GetMember("UpdatePolicy"), static_cast<MessageReceiver::MethodHandler>(&SecurityApplicationObj::UpdatePolicy));
        if (ER_OK != status) {
            QCC_LogError(status, ("Failed to add method handler for  %s.UpdatePolicy", org::alljoyn::Bus::Security::ManagedApplication::InterfaceName));
            return status;
        }
        status = AddMethodHandler(ifc->GetMember("ResetPolicy"), static_cast<MessageReceiver::MethodHandler>(&SecurityApplicationObj::ResetPolicy));
        if (ER_OK != status) {
            QCC_LogError(status, ("Failed to add method handler for  %s.ResetPolicy", org::alljoyn::Bus::Security::ManagedApplication::InterfaceName));
            return status;
        }
        status = AddMethodHandler(ifc->GetMember("InstallMembership"), static_cast<MessageReceiver::MethodHandler>(&SecurityApplicationObj::InstallMembership));
        if (ER_OK != status) {
            QCC_LogError(status, ("Failed to add method handler for  %s.InstallMembership", org::alljoyn::Bus::Security::ManagedApplication::InterfaceName));
            return status;
        }
        status = AddMethodHandler(ifc->GetMember("RemoveMembership"), static_cast<MessageReceiver::MethodHandler>(&SecurityApplicationObj::RemoveMembership));
        if (ER_OK != status) {
            QCC_LogError(status, ("Failed to add method handler for  %s.RemoveMembership", org::alljoyn::Bus::Security::ManagedApplication::InterfaceName));
            return status;
        }
    }
    status = bus->RegisterBusObject(*this);
    if (ER_OK == status) {
        m_bus = bus;
    }
    return status;
}
QStatus SecurityApplicationObj::State(qcc::ECCPublicKey publicKey, ApplicationState state)
{
    QCC_DbgTrace(("SecurityApplication::%s", __FUNCTION__));
    QStatus status = ER_OK;
    if (m_bus == NULL) {
        return ER_BUS_OBJECT_NOT_REGISTERED;
    }
    const InterfaceDescription* ifc = m_bus->GetInterface(org::alljoyn::Bus::Application::InterfaceName);
    if (!ifc) {
        return ER_BUS_INTERFACE_MISSING;
    }

    const ajn::InterfaceDescription::Member* stateSignalMember = ifc->GetMember("State");
    assert(stateSignalMember);

    size_t stateArgsSize = 2;
    MsgArg stateArgs[2];
    //TODO remove magic numbers should use algorithm, curveIdentifier, xCoordinate, yCoordinate
    status = MsgArg::Set(stateArgs, stateArgsSize, "(yyayay)q", 0, 0, 32, publicKey.x, 32, publicKey.y, static_cast<uint16_t>(state));
    status = ER_NOT_IMPLEMENTED; // TODO remove on implementation
    if (status != ER_OK) {
        QCC_DbgPrintf(("SecurityApplication::%s Failed to set state arguments %s", __FUNCTION__, QCC_StatusText(status)));
        return status;
    }

    Message msg(*m_bus);
    status = Signal(NULL, 0, *stateSignalMember, stateArgs, stateArgsSize, 0, ALLJOYN_FLAG_SESSIONLESS, &msg);
    QCC_DbgPrintf(("Sent org.alljoyn.Bus.Peer.Authentication.State signal from %s  = %s", m_bus->GetUniqueName().c_str(), QCC_StatusText(status)));

    return status;
}

void SecurityApplicationObj::Claim(const ajn::InterfaceDescription::Member* member, ajn::Message& msg)
{
    QCC_UNUSED(member); //TODO remove on implementation
    QCC_UNUSED(msg); //TODO remove on implementation
    QCC_DbgTrace(("SecurityApplicationObj::%s", __FUNCTION__));
    //TODO
    MethodReply(msg, ER_NOT_IMPLEMENTED);
}

void SecurityApplicationObj::Reset(const ajn::InterfaceDescription::Member* member, ajn::Message& msg)
{
    QCC_UNUSED(member); //TODO remove on implementation
    QCC_UNUSED(msg); //TODO remove on implementation
    QCC_DbgTrace(("SecurityApplicationObj::%s", __FUNCTION__));
    //TODO
    MethodReply(msg, ER_NOT_IMPLEMENTED);
}

void SecurityApplicationObj::UpdateIdentity(const ajn::InterfaceDescription::Member* member, ajn::Message& msg)
{
    QCC_UNUSED(member); //TODO remove on implementation
    QCC_UNUSED(msg); //TODO remove on implementation
    QCC_DbgTrace(("SecurityApplicationObj::%s", __FUNCTION__));
    //TODO
    MethodReply(msg, ER_NOT_IMPLEMENTED);
}

void SecurityApplicationObj::UpdatePolicy(const ajn::InterfaceDescription::Member* member, ajn::Message& msg)
{
    QCC_UNUSED(member); //TODO remove on implementation
    QCC_UNUSED(msg); //TODO remove on implementation
    QCC_DbgTrace(("SecurityApplicationObj::%s", __FUNCTION__));
    //TODO
    MethodReply(msg, ER_NOT_IMPLEMENTED);
}

void SecurityApplicationObj::ResetPolicy(const ajn::InterfaceDescription::Member* member, ajn::Message& msg)
{
    QCC_UNUSED(member); //TODO remove on implementation
    QCC_UNUSED(msg); //TODO remove on implementation
    QCC_DbgTrace(("SecurityApplicationObj::%s", __FUNCTION__));
    //TODO
    MethodReply(msg, ER_NOT_IMPLEMENTED);
}

void SecurityApplicationObj::InstallMembership(const ajn::InterfaceDescription::Member* member, ajn::Message& msg)
{
    QCC_UNUSED(member); //TODO remove on implementation
    QCC_UNUSED(msg); //TODO remove on implementation
    QCC_DbgTrace(("SecurityApplicationObj::%s", __FUNCTION__));
    //TODO
    MethodReply(msg, ER_NOT_IMPLEMENTED);
}

void SecurityApplicationObj::RemoveMembership(const ajn::InterfaceDescription::Member* member, ajn::Message& msg)
{
    QCC_UNUSED(member); //TODO remove on implementation
    QCC_UNUSED(msg); //TODO remove on implementation
    QCC_DbgTrace(("SecurityApplicationObj::%s", __FUNCTION__));
    //TODO
    MethodReply(msg, ER_NOT_IMPLEMENTED);
}

QStatus SecurityApplicationObj::Get(const char* ifcName, const char* propName, MsgArg& val)
{
    QCC_DbgTrace(("SecurityApplicationObj::%s", __FUNCTION__));
    QStatus status = ER_BUS_NO_SUCH_PROPERTY;
    if (0 == strcmp(org::alljoyn::Bus::Security::Application::InterfaceName, ifcName)) {
        if (0 == strcmp("Version", propName)) {
            status = val.Set("q", SecurityApplicationObj::SECURITY_APPLICATION_VERSION);
        } else if (0 == strcmp("ApplicationState", propName)) {
            status = val.Set("q", applicationState);
        } else if (0 == strcmp("ManifestTemplateDigest", propName)) {
            /* status = val.Set("(yay)", manifestTemplateDigest.algorithm,
                     manifestTemplateDigest.digestDataSize,
                     manifestTemplateDigest.digestData); */
            status = ER_NOT_IMPLEMENTED; // TODO remove on implementation
        } else if (0 == strcmp("EccPublicKey", propName)) {
            /* status = val.Set("(yyayay)", eccPublicKey.algorithm,
                    eccPublicKey.curveIdentifier,
                    eccPublicKey.xCoordinateSize, eccPublicKey.xCoordinate,
                    eccPublicKey.yCoordinateSize, eccPublicKey.yCoordinate); */
            status = ER_NOT_IMPLEMENTED; // TODO remove on implementation
        } else if (0 == strcmp("ManufacturerCertificate", propName)) {
            //status = val.Set("a(yay)", /*ManufacturerCertificate*/);
            status = ER_NOT_IMPLEMENTED; // TODO remove on implementation
        } else if (0 == strcmp("ManifestTemplate", propName)) {
            //status = val.Set("a(ssa(syy))", /*ManifestTemplate*/);
            status = ER_NOT_IMPLEMENTED; // TODO remove on implementation
        } else if (0 == strcmp("ClaimCapabilities", propName)) {
            status = val.Set("q", claimCapabilities);
        } else if (0 == strcmp("ClaimCapabilityAdditionalInfo", propName)) {
            status = val.Set("q", claimCapabilityAdditionalInfo);
        }
    } else if (0 == strcmp(org::alljoyn::Bus::Security::ManagedApplication::InterfaceName, ifcName)) {
        if (0 == strcmp("Version", propName)) {
            status = val.Set("q", SecurityApplicationObj::SECURITY_MANAGED_APPLICATION_VERSION);
        } else if (0 == strcmp("Identity", propName)) {
            //status = val.Set("a(yay)", /*Identity*/);
            status = ER_NOT_IMPLEMENTED; // TODO remove on implementation
        } else if (0 == strcmp("Manifest", propName)) {
            //status = val.Set("a(ssa(syy))", /*Manifest*/);
            status = ER_NOT_IMPLEMENTED; // TODO remove on implementation
        } else if (0 == strcmp("IdentityCertificateId", propName)) {
            //status = val.Set("(ayyyayay)", /* IdentityCertificateId */);
            status = ER_NOT_IMPLEMENTED; // TODO remove on implementation
        } else if (0 == strcmp("PolicyVersion", propName)) {
            status = val.Set("u", policyVersion);;
        } else if (0 == strcmp("Policy", propName)) {
            //status = val.Set("(qua(a(ya(yyayay)ay)a(ssa(syy))))", /*Policy*/);
            status = ER_NOT_IMPLEMENTED; // TODO remove on implementation
        } else if (0 == strcmp("DefaultPolicy", propName)) {
            //status = val.Set("(qua(a(ya(yyayay)ay)a(ssa(syy))))", /*DefaultPolicy*/);
            status = ER_NOT_IMPLEMENTED; // TODO remove on implementation
        } else if (0 == strcmp("MembershipSummaries", propName)) {
            //status = val.Set("a(ayyyayay)", /*MembershipSummaries*/);
            status = ER_NOT_IMPLEMENTED; // TODO remove on implementation
        }
    } else if (0 == strcmp(org::alljoyn::Bus::Application::InterfaceName, ifcName)) {
        if (0 == strcmp("Version", propName)) {
            status = val.Set("q", SecurityApplicationObj::APPLICATION_VERSION);
        }
    } else if (0 == strcmp(org::alljoyn::Bus::Security::ClaimableApplication::InterfaceName, ifcName)) {
        if (0 == strcmp("Version", propName)) {
            status = val.Set("q", SecurityApplicationObj::SECURITY_CLAIMABLE_APPLICATION_VERSION);
        }
    }
    return status;
}

} //namespace ajn
