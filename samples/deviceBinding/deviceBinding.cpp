//
// Created by allforgot on 2020/3/28.
//

#include <iostream>
#include <string>
#include <stdsoap2.h>
#include "wsseapi.h"
#include "soapDeviceBindingProxy.h"
#include "common.h"

using namespace std;

const string username = USERNAME;
const string password =  PASSWORD;
const string hostname =  HOSTNAME;

void getDeviceInformation() {

//    std::cout << "====================== DeviceBinding DeviceInformation ======================" << std::endl;

    DeviceBindingProxy deviceBindingProxy;
    deviceBindingProxy.soap_endpoint = hostname.c_str();
    if (SOAP_OK != soap_wsse_add_UsernameTokenDigest(deviceBindingProxy.soap, nullptr, username.c_str(), password.c_str())) {
        std::cerr << "Error: soap_wsse_add_UsernameTokenDigest" << std::endl;
        report_error(deviceBindingProxy.soap);
        return;
    }

    struct soap* soap = soap_new();
    _tds__GetDeviceInformation *GetDeviceInfo = soap_new__tds__GetDeviceInformation(soap);
    _tds__GetDeviceInformationResponse *GetDeviceInformationResponse = soap_new__tds__GetDeviceInformationResponse(soap);

    if (SOAP_OK == deviceBindingProxy.GetDeviceInformation(GetDeviceInfo, *GetDeviceInformationResponse)) {
        std::cout << "Manufacturer:    " << GetDeviceInformationResponse->Manufacturer << std::endl;
        std::cout << "Model:           " << GetDeviceInformationResponse->Model << std::endl;
        std::cout << "FirmwareVersion: " << GetDeviceInformationResponse->FirmwareVersion << std::endl;
        std::cout << "SerialNumber:    " << GetDeviceInformationResponse->SerialNumber << std::endl;
        std::cout << "HardwareId:      " << GetDeviceInformationResponse->HardwareId << std::endl;
    } else {
        std::cerr <<"Error: getDeviceInformation" << endl;
        report_error(deviceBindingProxy.soap);
    }

    CLEANUP_SOAP(soap);
}

void getCapabilities() {

//    std::cout << "====================== DeviceBinding Capabilities ======================" << std::endl;

    DeviceBindingProxy deviceBindingProxy;
    deviceBindingProxy.soap_endpoint = hostname.c_str();
    if (SOAP_OK != soap_wsse_add_UsernameTokenDigest(deviceBindingProxy.soap, nullptr, username.c_str(), password.c_str())) {
        std::cerr << "Error: soap_wsse_add_UsernameTokenDigest" << std::endl;
        report_error(deviceBindingProxy.soap);
        return;
    }

    struct soap* soap = soap_new();
    _tds__GetCapabilities *GetCapabilities = soap_new__tds__GetCapabilities(soap);
    _tds__GetCapabilitiesResponse *GetCapabilitiesResponse = soap_new__tds__GetCapabilitiesResponse(soap);

    if (SOAP_OK == deviceBindingProxy.GetCapabilities(GetCapabilities, *GetCapabilitiesResponse)) {
        if (GetCapabilitiesResponse->Capabilities->Analytics != nullptr) {
            cout << "Analytics address: " + GetCapabilitiesResponse->Capabilities->Analytics->XAddr << endl;
        }
        if (GetCapabilitiesResponse->Capabilities->Device != nullptr) {
            cout << "Device address:    " + GetCapabilitiesResponse->Capabilities->Device->XAddr << endl;
        }
        if (GetCapabilitiesResponse->Capabilities->Events != nullptr) {
            cout << "Events address:    " + GetCapabilitiesResponse->Capabilities->Events->XAddr << endl;
        }
        if (GetCapabilitiesResponse->Capabilities->Imaging != nullptr) {
            cout << "Imaging address:   " + GetCapabilitiesResponse->Capabilities->Imaging->XAddr << endl;
        }
        if (GetCapabilitiesResponse->Capabilities->Media != nullptr) {
            cout << "Media address:     " + GetCapabilitiesResponse->Capabilities->Media->XAddr << endl;
        }
        if (GetCapabilitiesResponse->Capabilities->PTZ != nullptr) {
            cout << "PTZ address:       " + GetCapabilitiesResponse->Capabilities->PTZ->XAddr << endl;
        }
    } else {
        std::cerr <<"Error: getCapabilities" << endl;
        report_error(deviceBindingProxy.soap);
    }

    CLEANUP_SOAP(soap);
}

void getScopes() {

//    std::cout << "====================== DeviceBinding Scopes ======================" << std::endl;

    DeviceBindingProxy deviceBindingProxy;
    deviceBindingProxy.soap_endpoint = hostname.c_str();
    if (SOAP_OK != soap_wsse_add_UsernameTokenDigest(deviceBindingProxy.soap, nullptr, username.c_str(), password.c_str())) {
        std::cerr << "Error: soap_wsse_add_UsernameTokenDigest" << std::endl;
        report_error(deviceBindingProxy.soap);
        return;
    }

    struct soap* soap = soap_new();
    _tds__GetScopes *GetScopes = soap_new__tds__GetScopes(soap);
    _tds__GetScopesResponse *GetScopesResponse = soap_new__tds__GetScopesResponse(soap);

    if (SOAP_OK == deviceBindingProxy.GetScopes(GetScopes, *GetScopesResponse)) {
        for (int i = 0; i < GetScopesResponse->Scopes.size(); ++ i) {
            cout << GetScopesResponse->Scopes[i]->ScopeItem << endl;
        }
    } else {
        std::cerr <<"Error: getScopes" << endl;
        report_error(deviceBindingProxy.soap);
    }

}

int main() {

    std::cout << "====================== DeviceBinding DeviceInformation ======================" << std::endl;
    getDeviceInformation();

    std::cout << "====================== DeviceBinding Capabilities ======================" << std::endl;
    getCapabilities();

    std::cout << "====================== DeviceBinding Scopes ======================" << std::endl;
    getScopes();

    return 0;
}