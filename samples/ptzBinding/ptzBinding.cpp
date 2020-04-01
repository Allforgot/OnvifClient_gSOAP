//
// Created by allforgot on 2020/3/28.
//

#include <iostream>
#include <string>
#include <unistd.h>
#include <stdsoap2.h>
#include "wsseapi.h"
#include "soapPTZBindingProxy.h"
#include "common.h"

using namespace std;

const string username = USERNAME;
const string password =  PASSWORD;
const string hostname =  HOSTNAME;

typedef struct {
    float pan;
    float tilt;
    float zoom;

    /*enum tt__MoveStatus {
        tt__MoveStatus__IDLE = 0,
        tt__MoveStatus__MOVING = 1,
        tt__MoveStatus__UNKNOWN = 2
    };*/
    tt__MoveStatus move_status_pan_tilt;
    tt__MoveStatus move_status_zoom;
} _ocp_PTZStatus;

float panLimitsMin = -1;
float panLimitsMax = 1;
float tiltLimitsMin = -1;
float tiltLimitsMax = 1;
float zoomLimitsMin = 0;
float zoomLimitsMax = 1;

bool getConfiguraions() {

//    std::cout << "====================== PTZBinding Configurations ======================" << std::endl;

    PTZBindingProxy ptzBindingProxy;
    ptzBindingProxy.soap_endpoint = hostname.c_str();
    if (SOAP_OK != soap_wsse_add_UsernameTokenDigest(ptzBindingProxy.soap, nullptr, username.c_str(), password.c_str())) {
        std::cerr << "Error: soap_wsse_add_UsernameTokenDigest" << std::endl;
        report_error(ptzBindingProxy.soap);
        return false;
    }

    struct soap* soap = soap_new();
    _tptz__GetConfigurations *GetConfigurations = soap_new__tptz__GetConfigurations(soap);
    _tptz__GetConfigurationsResponse *GetConfigurationsResponse = soap_new__tptz__GetConfigurationsResponse(soap);

    bool executeResult = false;
    if (SOAP_OK == ptzBindingProxy.GetConfigurations(GetConfigurations, *GetConfigurationsResponse)) {
        for (auto PTZConfiguration : GetConfigurationsResponse->PTZConfiguration) {
            cout << "NodeToken: " << PTZConfiguration->NodeToken << endl;
            cout << "DefaultAbsolutePantTiltPositionSpace: " << *PTZConfiguration->DefaultAbsolutePantTiltPositionSpace << endl;
            cout << "DefaultAbsoluteZoomPositionSpace: " << *PTZConfiguration->DefaultAbsoluteZoomPositionSpace << endl;
            cout << "DefaultRelativePanTiltTranslationSpace: " << *PTZConfiguration->DefaultRelativePanTiltTranslationSpace << endl;
            cout << "DefaultRelativeZoomTranslationSpace: " << *PTZConfiguration->DefaultRelativeZoomTranslationSpace << endl;
            cout << "DefaultContinuousPanTiltVelocitySpace: " << *PTZConfiguration->DefaultContinuousPanTiltVelocitySpace << endl;
            cout << "DefaultContinuousZoomVelocitySpace: " << *PTZConfiguration->DefaultContinuousZoomVelocitySpace << endl;
            cout << "DefaultPTZSpeed Pan:     " << PTZConfiguration->DefaultPTZSpeed->PanTilt->x << endl;
            cout << "DefaultPTZSpeed Tilt:    " << PTZConfiguration->DefaultPTZSpeed->PanTilt->y << endl;
            cout << "DefaultPTZSpeed Zoom:    " << PTZConfiguration->DefaultPTZSpeed->Zoom->x << endl;
            cout << "DefaultPTZTimeout:       " << *PTZConfiguration->DefaultPTZTimeout << endl;
            cout << "PanTiltLimits Pan Min:   " << PTZConfiguration->PanTiltLimits->Range->XRange->Min << endl;
            panLimitsMin = PTZConfiguration->PanTiltLimits->Range->XRange->Min;
            cout << "PanTiltLimits Pan Max:   " << PTZConfiguration->PanTiltLimits->Range->XRange->Max << endl;
            panLimitsMax = PTZConfiguration->PanTiltLimits->Range->XRange->Max;
            cout << "PanTiltLimits Tilt Min:  " << PTZConfiguration->PanTiltLimits->Range->YRange->Min << endl;
            tiltLimitsMin = PTZConfiguration->PanTiltLimits->Range->YRange->Min;
            cout << "PanTiltLimits Tilt Max:  " << PTZConfiguration->PanTiltLimits->Range->YRange->Max << endl;
            tiltLimitsMax = PTZConfiguration->PanTiltLimits->Range->YRange->Max;
            cout << "ZoomLimits Min:          " << PTZConfiguration->ZoomLimits->Range->XRange->Min << endl;
            zoomLimitsMin = PTZConfiguration->ZoomLimits->Range->XRange->Min;
            cout << "ZoomLimits Max:          " << PTZConfiguration->ZoomLimits->Range->XRange->Max << endl;
            zoomLimitsMax = PTZConfiguration->ZoomLimits->Range->XRange->Max;
            cout << "Extension:               " << PTZConfiguration->Extension << endl;
            cout << "MoveRamp:                " << PTZConfiguration->MoveRamp << endl;
            cout << "PresetRamp:              " << PTZConfiguration->PresetRamp << endl;
            cout << "PresetTourRamp:          " << PTZConfiguration->PresetTourRamp << endl;
        }
        executeResult = true;
    } else {
        std::cerr <<"Error: getConfiguraions" << endl;
        report_error(ptzBindingProxy.soap);
    }

    return executeResult;
}

bool getStatus(const string &profileToken, _ocp_PTZStatus &ptzStatus) {

//    std::cout << "====================== PTZBinding Status ======================" << std::endl;

    PTZBindingProxy ptzBindingProxy;
    ptzBindingProxy.soap_endpoint = hostname.c_str();
    if (SOAP_OK != soap_wsse_add_UsernameTokenDigest(ptzBindingProxy.soap, nullptr, username.c_str(), password.c_str())) {
        std::cerr << "Error: soap_wsse_add_UsernameTokenDigest" << std::endl;
        report_error(ptzBindingProxy.soap);
        return false;
    }

    struct soap* soap = soap_new();
    _tptz__GetStatus *GetStatus = soap_new__tptz__GetStatus(soap);
    _tptz__GetStatusResponse *GetStatusResponse = soap_new__tptz__GetStatusResponse(soap);

    const tt__ReferenceToken& token = profileToken;
    GetStatus->ProfileToken = token;

    bool executeResult = false;
    if (SOAP_OK == ptzBindingProxy.GetStatus(GetStatus, *GetStatusResponse)) {
        if (GetStatusResponse->PTZStatus) {
            if (GetStatusResponse->PTZStatus->Position) {
                ptzStatus.pan = GetStatusResponse->PTZStatus->Position->PanTilt->x;
                ptzStatus.tilt = GetStatusResponse->PTZStatus->Position->PanTilt->y;
                ptzStatus.zoom = GetStatusResponse->PTZStatus->Position->Zoom->x;
            } else {
                cerr << "Error get ptz position" << endl;
            }
            if (GetStatusResponse->PTZStatus->MoveStatus) {
                if (GetStatusResponse->PTZStatus->MoveStatus->PanTilt) {
                    ptzStatus.move_status_pan_tilt = *(GetStatusResponse->PTZStatus->MoveStatus->PanTilt);
                } else {
                    ptzStatus.move_status_pan_tilt = tt__MoveStatus::tt__MoveStatus__UNKNOWN;
                }
                if (GetStatusResponse->PTZStatus->MoveStatus->Zoom) {
                    ptzStatus.move_status_zoom = *(GetStatusResponse->PTZStatus->MoveStatus->Zoom);
                } else {
                    ptzStatus.move_status_zoom = tt__MoveStatus::tt__MoveStatus__UNKNOWN;
                }
            } else {
                ptzStatus.move_status_pan_tilt = tt__MoveStatus::tt__MoveStatus__UNKNOWN;
                ptzStatus.move_status_zoom = tt__MoveStatus::tt__MoveStatus__UNKNOWN;
            }
            executeResult = true;
        }
    } else {
        std::cerr <<"Error: getStatus" << endl;
        report_error(ptzBindingProxy.soap);
    }

    CLEANUP_SOAP(soap);

    return executeResult;
}

bool continuousMove(const string &profileToken, const float &speedPTX, const float &speedPTY, const float &speed_zoom,
                    const long long &timeout) {
//    std::cout << "====================== PTZBinding ContinuousMove ======================" << std::endl;

    PTZBindingProxy ptzBindingProxy;
    ptzBindingProxy.soap_endpoint = hostname.c_str();
    if (SOAP_OK != soap_wsse_add_UsernameTokenDigest(ptzBindingProxy.soap, nullptr, username.c_str(), password.c_str())) {
        std::cerr << "Error: soap_wsse_add_UsernameTokenDigest" << std::endl;
        report_error(ptzBindingProxy.soap);
        return false;
    }

    struct soap* soap = soap_new();
    _tptz__ContinuousMove *ContinuousMove = soap_new__tptz__ContinuousMove(soap);
    _tptz__ContinuousMoveResponse *ContinuousMoveResponse = soap_new__tptz__ContinuousMoveResponse(soap);

    const tt__ReferenceToken& token = profileToken;
    ContinuousMove->ProfileToken = token;
    ContinuousMove->Velocity = soap_new_tt__PTZSpeed(soap);
    ContinuousMove->Velocity->PanTilt = soap_new_tt__Vector2D(soap);
    ContinuousMove->Velocity->PanTilt->x = speedPTX;
    ContinuousMove->Velocity->PanTilt->y = speedPTY;
    ContinuousMove->Velocity->Zoom = soap_new_tt__Vector1D(soap);
    ContinuousMove->Velocity->Zoom->x = speed_zoom;
    ContinuousMove->Timeout = (xsd__duration *)soap_malloc(soap, sizeof(xsd__duration *));
    *ContinuousMove->Timeout = timeout;

    bool executeResult = false;
    if (SOAP_OK == ptzBindingProxy.ContinuousMove(ContinuousMove, *ContinuousMoveResponse)) {
        executeResult = true;
    } else {
        std::cerr <<"Error: continuousMove" << endl;
        report_error(ptzBindingProxy.soap);
    }

    CLEANUP_SOAP(soap);

    return executeResult;
}

bool stop(const string &profileToken, const bool &stopPanTilt, const bool &stopZoom) {
//    std::cout << "====================== PTZBinding Stop ======================" << std::endl;

    PTZBindingProxy ptzBindingProxy;
    ptzBindingProxy.soap_endpoint = hostname.c_str();
    if (SOAP_OK != soap_wsse_add_UsernameTokenDigest(ptzBindingProxy.soap, nullptr, username.c_str(), password.c_str())) {
        std::cerr << "Error: soap_wsse_add_UsernameTokenDigest" << std::endl;
        report_error(ptzBindingProxy.soap);
        return false;
    }

    struct soap* soap = soap_new();
    _tptz__Stop *Stop = soap_new__tptz__Stop(soap);
    _tptz__StopResponse *StopResponse = soap_new__tptz__StopResponse(soap);

    const tt__ReferenceToken& token = profileToken;
    Stop->ProfileToken = token;
    Stop->PanTilt = (bool*)soap_malloc(soap, sizeof(bool*));
    Stop->Zoom = (bool*)soap_malloc(soap, sizeof(bool*));
    *Stop->PanTilt = stopPanTilt;
    *Stop->Zoom = stopZoom;

    bool executeResult = false;
    if (SOAP_OK == ptzBindingProxy.Stop(Stop, *StopResponse)) {
        executeResult = true;
    } else {
        std::cerr <<"Error: stop" << endl;
        report_error(ptzBindingProxy.soap);
    }

    CLEANUP_SOAP(soap);

    return executeResult;
}

bool absoluteMove(const std::string & profileToken, const float & pantiltX, const float & pantiltY, const float & zoom,
                  const float & speedPTX, const float & speedPTY, const float & speedZoom) {
//    std::cout << "====================== PTZBinding AbsoluteMove ======================" << std::endl;

    if (pantiltX < panLimitsMin || pantiltX > panLimitsMax || pantiltY < tiltLimitsMin || pantiltY > tiltLimitsMax
        || zoom < zoomLimitsMin || zoom > zoomLimitsMax) {
        cerr << "Destination out of bounds" << endl;
        return false;
    }

    PTZBindingProxy ptzBindingProxy;
    ptzBindingProxy.soap_endpoint = hostname.c_str();
    if (SOAP_OK != soap_wsse_add_UsernameTokenDigest(ptzBindingProxy.soap, nullptr, username.c_str(), password.c_str())) {
        std::cerr << "Error: soap_wsse_add_UsernameTokenDigest" << std::endl;
        report_error(ptzBindingProxy.soap);
        return false;
    }

    struct soap* soap = soap_new();
    _tptz__AbsoluteMove *AbsoluteMove = soap_new__tptz__AbsoluteMove(soap);
    _tptz__AbsoluteMoveResponse *AbsoluteMoveResponse = soap_new__tptz__AbsoluteMoveResponse(soap);

    const tt__ReferenceToken& token = profileToken;
    AbsoluteMove->ProfileToken = token;

    AbsoluteMove->Position = soap_new_tt__PTZVector(soap);
    AbsoluteMove->Position->PanTilt = soap_new_tt__Vector2D(soap);
    AbsoluteMove->Position->PanTilt->x = pantiltX;
    AbsoluteMove->Position->PanTilt->y = pantiltY;
    AbsoluteMove->Position->Zoom = soap_new_tt__Vector1D(soap);
    AbsoluteMove->Position->Zoom->x = zoom;
    AbsoluteMove->Speed = soap_new_tt__PTZSpeed(soap);
    AbsoluteMove->Speed->PanTilt = soap_new_tt__Vector2D(soap);
    AbsoluteMove->Speed->PanTilt->x = speedPTX;
    AbsoluteMove->Speed->PanTilt->y = speedPTY;
    AbsoluteMove->Speed->Zoom = soap_new_tt__Vector1D(soap);
    AbsoluteMove->Speed->Zoom->x = speedZoom;

    bool executeResult = false;
    if (SOAP_OK == ptzBindingProxy.AbsoluteMove(AbsoluteMove, *AbsoluteMoveResponse)) {
        executeResult = true;
    } else {
        std::cerr <<"Error: absoluteMove" << endl;
        report_error(ptzBindingProxy.soap);
    }

    CLEANUP_SOAP(soap);

    return executeResult;
}

int main() {

    std::cout << "====================== PTZBinding Configurations ======================" << std::endl;
    getConfiguraions();

    string profileToken = PROFILETOKEN;
    _ocp_PTZStatus status;
    bool getStatusResult = getStatus(profileToken, status);
    if (getStatusResult) {
        std::cout << "====================== PTZBinding Status ======================" << std::endl;
        cout << "Pan:  " << status.pan << endl;
        cout << "Tilt: " << status.tilt << endl;
        cout << "Zoom: " << status.zoom << endl;
        cout << "MovingStatusPanTilt: " << status.move_status_pan_tilt << endl;
        cout << "MovingStatusZoom: " << status.move_status_zoom << endl;
    }

    std::cout << "====================== PTZBinding AbsoluteMove ======================" << std::endl;
    absoluteMove(profileToken, 0.3, 0.4, 0, 0.1, 0.1, 0.1);
    cout << "Moving" << endl;
    do {
        usleep(100000);
        getStatusResult = getStatus(profileToken, status);
    } while (getStatusResult && (status.move_status_pan_tilt || status.move_status_zoom));
    cout << "stop" << endl;
    if (getStatusResult) {
        std::cout << "====================== PTZBinding Status ======================" << std::endl;
        cout << "Pan:  " << status.pan << endl;
        cout << "Tilt: " << status.tilt << endl;
        cout << "Zoom: " << status.zoom << endl;
        cout << "MovingStatusPanTilt: " << status.move_status_pan_tilt << endl;
        cout << "MovingStatusZoom: " << status.move_status_zoom << endl;
    }

    std::cout << "====================== PTZBinding ContinuousMove ======================" << std::endl;
    // It will stop after 5000ms
    continuousMove(profileToken, 0.1, 0, 0, 5000);
    cout << "Moving" << endl;
    do {
        usleep(100000);
        getStatusResult = getStatus(profileToken, status);
    } while (getStatusResult && (status.move_status_pan_tilt || status.move_status_zoom));
    cout << "stop" << endl;
    if (getStatusResult) {
        std::cout << "====================== PTZBinding Status ======================" << std::endl;
        cout << "Pan:  " << status.pan << endl;
        cout << "Tilt: " << status.tilt << endl;
        cout << "Zoom: " << status.zoom << endl;
        cout << "MovingStatusPanTilt: " << status.move_status_pan_tilt << endl;
        cout << "MovingStatusZoom: " << status.move_status_zoom << endl;
    }

    return 0;
}