# Generate ONVIF client code base on gSOAP

Generate ONVIF client code base on gSOAP 2.8.98 according to [GENIVIA ONVIF sample](https://www.genivia.com/examples/onvif/index.html)

### Prerequisites

- Ubuntu 16.04

### Installing

1. [Install gSOAP](https://www.genivia.com/downloads.html#unix)

   1. Install dependences

      - bison flex

        ```bash
        $ sudo apt-get install flex bison
        ```

      - Install OpenSSL to enable HTTPS for gSOAP clients

        1. download from https://www.openssl.org/source/old/1.1.0/

        2. install openssl follow its guide

        3. the default installing path is `/usr/local/ssl`

   2. gSOAP

      ```bash
      $ wget https://sourceforge.net/projects/gsoap2/files/gsoap-2.8/gsoap_2.8.98.zip/download
      $ unzip gsoap_2.8.98.zip
      $ cd gsoap-2.8
      $ ./configure --prefix=path_to_install --with-openssl=path_to_ssl
      $ make
      $ make install
      ```

      `wsdl2h` and `soapcpp2` can be found in `path_to_install/bin`

2. Generate ONVIF code

   1. modify `typemap.dat`

      To use XSD type `xsd:duration` , uncomment the following line in `typemap.dat`

      ```
      xsd__dateTime = #import "custom/struct_timeval.h" | xsd__dateTime
      xsd__duration = #import "custom/duration.h" | xsd__duration
      ```

      > Modified `typemap.dat` reference `res/typemap.dat`

   2. generate `onvif.h` for `C++`

      ```bash
      $ cd gsoap-2.8/gsoap && mkdir samples/onvif
      $ ./path_to_install/bin/wsdl2h -P -x -s -t typemap.dat -o samples/onvif/onvif.h http://www.onvif.org/onvif/ver10/device/wsdl/devicemgmt.wsdl http://www.onvif.org/onvif/ver10/event/wsdl/event.wsdl https://www.onvif.org/ver10/advancedsecurity/wsdl/advancedsecurity.wsdl https://www.onvif.org/ver20/imaging/wsdl/imaging.wsdl https://www.onvif.org/ver10/media/wsdl/media.wsdl https://www.onvif.org/ver20/ptz/wsdl/ptz.wsdl https://www.onvif.org/ver10/deviceio.wsdl http://www.onvif.org/onvif/ver10/network/wsdl/remotediscovery.wsdl
      ```

      > The `onvif.h` generated if for `C++`, for `C` you should add `-c`
      >
      > The wsdl link can be found in https://www.onvif.org/profiles/specifications/ , choose your wsdl according to your project

   3. modify `onvif.h`

      To use `WS-Security` , modify `onvif.h` 

      change 

      ```c++
      #import "wsdd10.h"	// wsdd10 = <http://schemas.xmlsoap.org/ws/2005/04/discovery>
      #import "xop.h"	// xop = <http://www.w3.org/2004/08/xop/include>
      #import "wsa5.h"	// wsa5 = <http://www.w3.org/2005/08/addressing>
      ```

      to

      ```c++
      #import "wsdd5.h"
      #import "xop.h"	
      #import "wsa5.h"
      #import "wsse.h"
      ```

      > Modified `onvif.h` reference `res/onvif.h`

   4. generate `C++` code

      ```bash
      $ ./path_to_install/bin/soapcpp2 -2 -C -d samples/onvif -I import -j -x samples/onvif/onvif.h
      ```

      > where option `-2` forces SOAP 1.2, option `-C` generates client code without service code, option `-j` generates C++ proxy classes and option `-x` omits the generation of sample XML messages (which are a lot!)

   5. generate `wsddClient.cpp`

      ```bash
      $ ./path_to_install/bin/soapcpp2 -a -x -L -pwsdd -d samples/onvif -I import import/wsdd5.h
      ```

   6. copy needed files

      files list below are also needed:

      ```bash
      stdsoap2.cpp
      stdsoap2.h
      dom.cpp
      dom.h
      plugin/wsaapi.c
      plugin/wsaapi.h
      plugin/mecevp.c
      plugin/mecevp.h
      plugin/smdevp.c
      plugin/smdevp.h
      plugin/threads.c
      plugin/threads.h
      custom/duration.c
      custom/duration.h
      custom/struct_timeval.c
      custom/struct_timeval.h
      ```

### Reference

- [GENIVIA ONVIF sample](https://www.genivia.com/examples/onvif/index.html)

- [ONVIF协议网络摄像机（IPC）客户端程序开发（6）：使用gSOAP生成ONVIF框架代码](https://blog.csdn.net/benkaoya/article/details/72466827)