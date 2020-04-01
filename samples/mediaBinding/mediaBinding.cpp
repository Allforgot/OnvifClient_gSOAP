//
// Created by allforgot on 2020/3/28.
//

#include <iostream>
#include <string>
#include <stdsoap2.h>
#include "wsseapi.h"
#include "soapMediaBindingProxy.h"
#include "common.h"

using namespace std;

const string username = USERNAME;
const string password =  PASSWORD;
const string hostname =  HOSTNAME;

vector<pair<string, string>> getProfiles() {

//    std::cout << "====================== MediaBinding Profiles ======================" << std::endl;

    MediaBindingProxy mediaBindingProxy;
    mediaBindingProxy.soap_endpoint = hostname.c_str();
    if (SOAP_OK != soap_wsse_add_UsernameTokenDigest(mediaBindingProxy.soap, nullptr, username.c_str(), password.c_str())) {
        std::cerr << "Error: soap_wsse_add_UsernameTokenDigest" << std::endl;
        report_error(mediaBindingProxy.soap);
        return vector<pair<string, string>>();
    }

    struct soap* soap = soap_new();
    _trt__GetProfiles *GetProfiles = soap_new__trt__GetProfiles(soap);
    _trt__GetProfilesResponse *GetProfilesResponse = soap_new__trt__GetProfilesResponse(soap);

    vector<pair<string, string>> profiles;
    cout << "Profile name" << "         " << "Profile token" << endl;
    if (SOAP_OK == mediaBindingProxy.GetProfiles(GetProfiles, *GetProfilesResponse)) {
        for (auto & Profile : GetProfilesResponse->Profiles) {
            cout << Profile->Name << "         " << Profile->token << endl;
            profiles.emplace_back(pair<string, string>(Profile->Name, Profile->token));
        }
    } else {
        std::cerr <<"Error: getProfiles" << endl;
        report_error(mediaBindingProxy.soap);
    }

    CLEANUP_SOAP(soap);

    return profiles;
}

void getSnapshotUri(string &profileToken) {

//    std::cout << "====================== MediaBinding GetSnapshotUri ======================" << std::endl;

    MediaBindingProxy mediaBindingProxy;
    mediaBindingProxy.soap_endpoint = hostname.c_str();
    if (SOAP_OK != soap_wsse_add_UsernameTokenDigest(mediaBindingProxy.soap, nullptr, username.c_str(), password.c_str())) {
        std::cerr << "Error: soap_wsse_add_UsernameTokenDigest" << std::endl;
        report_error(mediaBindingProxy.soap);
        return;
    }

    struct soap* soap = soap_new();
    _trt__GetSnapshotUri *GetSnapshotUri = soap_new__trt__GetSnapshotUri(soap);
    GetSnapshotUri->ProfileToken = profileToken;
    _trt__GetSnapshotUriResponse *GetSnapshotUriResponse = soap_new__trt__GetSnapshotUriResponse(soap);

    if (SOAP_OK == mediaBindingProxy.GetSnapshotUri(GetSnapshotUri, *GetSnapshotUriResponse)) {
        cout << "SnapshotUri: " << GetSnapshotUriResponse->MediaUri->Uri << endl;
    } else {
        std::cerr <<"Error: getSnapshotUri" << endl;
        report_error(mediaBindingProxy.soap);
    }

    CLEANUP_SOAP(soap);
}

void getStreamUri(string &profileToken) {

//    std::cout << "====================== MediaBinding GetStreamUri ======================" << std::endl;

    MediaBindingProxy mediaBindingProxy;
    mediaBindingProxy.soap_endpoint = hostname.c_str();
    if (SOAP_OK != soap_wsse_add_UsernameTokenDigest(mediaBindingProxy.soap, nullptr, username.c_str(), password.c_str())) {
        std::cerr << "Error: soap_wsse_add_UsernameTokenDigest" << std::endl;
        report_error(mediaBindingProxy.soap);
        return;
    }

    struct soap* soap = soap_new();
    _trt__GetStreamUri *GetStreamUri = soap_new__trt__GetStreamUri(soap);
    GetStreamUri->ProfileToken = profileToken;
    _trt__GetStreamUriResponse *GetStreamUriResponse = soap_new__trt__GetStreamUriResponse(soap);

    if (SOAP_OK == mediaBindingProxy.GetStreamUri(GetStreamUri, *GetStreamUriResponse)) {
        cout << "StreamUri: " << GetStreamUriResponse->MediaUri->Uri << endl;
    } else {
        std::cerr <<"Error: getStreamUri" << endl;
        report_error(mediaBindingProxy.soap);
    }

    CLEANUP_SOAP(soap);
}

int main() {
    std::cout << "====================== MediaBinding Profiles ======================" << std::endl;
    vector<pair<string, string>> profiles = getProfiles();

    if (!profiles.empty()) {
        std::cout << "====================== MediaBinding GetSnapshotUri ======================" << std::endl;
        getSnapshotUri(profiles[0].second);

        std::cout << "====================== MediaBinding GetStreamUri ======================" << std::endl;
        getStreamUri(profiles[0].second);
    }
    
    

    return 0;
}