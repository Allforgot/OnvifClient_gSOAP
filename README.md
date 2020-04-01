# OnvifClient_gSOAP

An ONVIF client base on gSOAP

## Getting Started

This project will help you develop a onvif client with C++. An IPC support onvif is needed to run the samples.

### Prerequisites

- gSOAP & ONVIF client code

  reference [GENERATE_ONVIF_CODE](./GENERATE_ONVIF_CODE.md)

- cmake 3.4.1 +

### Installing

1. modify `samples/common/common.h` , change `USERNAME` `PASSWORD` `HOSTNAME` `PROFILETOKEN` for your IPC device

2. make

   ```bash
   $ mkdir build
   $ cd build
   $ cmake .. && make
   $ make install
   ```

### Running samples

- deviceBiding

  Get device information

  ```bash
  $ ./samples/deviceBinding/deviceBinding
  
  ====================== DeviceBinding DeviceInformation ======================
  Manufacturer:    ONVIF
  Model:           DS-2DC4223IW-D
  FirmwareVersion: V5.6.11 build 190426
  SerialNumber:    DS-2DC4223IW-*********************
  HardwareId:      88
  ====================== DeviceBinding Capabilities ======================
  Analytics address: http://192.168.1.164/onvif/Analytics
  Device address:    http://192.168.1.164/onvif/device_service
  Events address:    http://192.168.1.164/onvif/Events
  Imaging address:   http://192.168.1.164/onvif/Imaging
  Media address:     http://192.168.1.164/onvif/Media
  PTZ address:       http://192.168.1.164/onvif/PTZ
  ====================== DeviceBinding Scopes ======================
  onvif://www.onvif.org/type/video_encoder
  onvif://www.onvif.org/Profile/Streaming
  onvif://www.onvif.org/Profile/G
  onvif://www.onvif.org/Profile/T
  onvif://www.onvif.org/type/ptz
  onvif://www.onvif.org/MAC/****************
  onvif://www.onvif.org/hardware/DS-2DC4223IW-D
  onvif://www.onvif.org/name/HIKVISION%20DS-2DC4223IW-D
  onvif://www.onvif.org/location/city/hangzhou
  ```

- mediaBinding

  Get profile, snapshoturi, stramuri etc..

  ```bash
  $ ./samples/mediaBinding/mediaBinding
  
  ====================== MediaBinding Profiles ======================
  Profile name         Profile token
  mainStream         Profile_1
  subStream         Profile_2
  thirdStream         Profile_3
  ====================== MediaBinding GetSnapshotUri ======================
  SnapshotUri: http://192.168.1.164/onvif-http/snapshot?Profile_1
  ====================== MediaBinding GetStreamUri ======================
  StreamUri: rtsp://192.168.1.164:554/Streaming/Channels/101?transportmode=unicast&profile=Profile_1
  ```

- ptzBinding

  ptz control, your device must support ptz control

  ```bash
  $ ./samples/ptzBinding/ptzBinding
  
  ====================== PTZBinding Configurations ======================
  NodeToken: PTZNODETOKEN
  DefaultAbsolutePantTiltPositionSpace: http://www.onvif.org/ver10/tptz/PanTiltSpaces/PositionGenericSpace
  DefaultAbsoluteZoomPositionSpace: http://www.onvif.org/ver10/tptz/ZoomSpaces/PositionGenericSpace
  DefaultRelativePanTiltTranslationSpace: http://www.onvif.org/ver10/tptz/PanTiltSpaces/TranslationGenericSpace
  DefaultRelativeZoomTranslationSpace: http://www.onvif.org/ver10/tptz/ZoomSpaces/TranslationGenericSpace
  DefaultContinuousPanTiltVelocitySpace: http://www.onvif.org/ver10/tptz/PanTiltSpaces/VelocityGenericSpace
  DefaultContinuousZoomVelocitySpace: http://www.onvif.org/ver10/tptz/ZoomSpaces/VelocityGenericSpace
  DefaultPTZSpeed Pan:     0.1
  DefaultPTZSpeed Tilt:    0.1
  DefaultPTZSpeed Zoom:    1
  DefaultPTZTimeout:       300000
  PanTiltLimits Pan Min:   -1
  PanTiltLimits Pan Max:   1
  PanTiltLimits Tilt Min:  -1
  PanTiltLimits Tilt Max:  1
  ZoomLimits Min:          0
  ZoomLimits Max:          1
  Extension:               0
  MoveRamp:                0
  PresetRamp:              0
  PresetTourRamp:          0
  ====================== PTZBinding Status ======================
  Pan:  0.303444
  Tilt: 0.4
  Zoom: 0
  MovingStatusPanTilt: 0
  MovingStatusZoom: 0
  ====================== PTZBinding AbsoluteMove ======================
  Moving
  stop
  ====================== PTZBinding Status ======================
  Pan:  0.3
  Tilt: 0.4
  Zoom: 0
  MovingStatusPanTilt: 0
  MovingStatusZoom: 0
  ====================== PTZBinding ContinuousMove ======================
  Moving
  stop
  ====================== PTZBinding Status ======================
  Pan:  0.303778
  Tilt: 0.4
  Zoom: 0
  MovingStatusPanTilt: 0
  MovingStatusZoom: 0
  ```

- remoteDiscovery

  Find  devices

  **NOT FINISH**

  