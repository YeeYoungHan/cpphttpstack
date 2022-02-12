# C++ HTTP stack 라이브러리
C++ STL 기반으로 HTTP 서버 및 클라이언트 개발용 라이브러리를 개발하는 프로젝트입니다.

### 개요

* C++ STL 라이브러리를 이용한 HTTP 클라이언트 및 서버 라이브러리 개발

### 개발자 정보
본 프로젝트를 진행하는 개발자 정보는 다음과 같습니다.

* 이메일 : websearch@naver.com
* 블로그 : http://blog.naver.com/websearch

### 라이선스

* 본 프로젝트의 라이선스는 GPLv3 이고 기업용 라이선스는 websearch@naver.com 으로 문의해 주세요.

### 개발툴

* 윈도우 : Visual C++ 2008, 2017
* 리눅스 : g++

### API 문서

* https://yeeyounghan.github.io/doc/CppHttpStack/html/index.html

### 폴더 설명

* doc
  * 본 프로젝트와 관련된 문서 저장 폴더

* HtmlParser
  * HTML 문서를 파싱/생성하는 라이브러리

* Http2Parser
  * HTTP/2 프로토콜 파싱/생성하는 라이브러리

* HttpNoCacheProxy
  * HTTP 캐시 기능을 사용하지 않는 HTTP Proxy 서비스

* HttpParser
  * HTTP 프로토콜 파서/생성 라이브러리

* HttpSimulator
  * HTTP 시뮬레이터 프로그램

* HttpSpeed
  * 웹서버 성능 테스트 프로그램

* HttpStack
  * HTTP 클라이언트/서버 라이브러리
  * WebSocket 서버 라이브러리

* JsonParser
  * JSON 문자열 파싱 및 생성 라이브러리

* openssl
  * MS 윈도우용 openssl 라이브러리
  * 리눅스 64bit openssl 라이브러리

* Publish
  * MS 윈도우용 배포 프로그램 저장 폴더

* SendFirebaseMessage
  * HTTP stack 기반으로 FirebaseMessage 를 전송하는 예제 프로그램

* SipPlatform
  * C++ SIP stack 에서 사용되는 OS 독립적인 유틸리티 라이브러리
  * https://github.com/YeeYoungHan/cppsipstack

* TcpStack
  * TCP 클라이언트/서버 라이브러리
  * HttpStack 에서 HTTP 서버 개발에 사용된다.

* WsdlParser
  * WSDL 문서를 파싱하는 라이브러리

* XmlParser
  * XML 문서를 파싱하는 라이브러리

* TestHttp2Client
  * HTTP/2 클라이언트 테스트 프로그램

* TestHttpClient
  * HTTP/1.1 클라이언트 테스트 프로그램

* TestHttpStack
  * HttpStack 을 이용한 간단한 HTTP/1.1 & HTTP/2 서버 샘플 프로그램
  * HttpStack 을 이용한 WebSocket echo 서버 프로그램

* TestHttpStackMfc
  * MFC 기반으로 HttpStack 을 이용한 간단한 HTTP 서버 샘플 프로그램

* TestWebRtc
  * WebSocket 을 이용한 WebRTC 연동 샘플 프로그램
  * 사용법은 http://blog.naver.com/websearch/221142356741 포스트를 참고하세요.

### 컴파일 방법
본 프로젝트의 컴파일 방법은 다음과 같습니다.

* VC++ 2008
  * HttpStack.sln 더블클릭한 후, 빌드한다.
  * Visual Studio 2008 버전 9.0.30729.1 SP 에서 빌드할 것을 권장합니다.

* VC++ 2017
  * HttpStack-vs2017.sln 더블클릭한 후, 빌드한다.

* 64bit 리눅스
  * make 를 실행하여서 빌드한다.

* 32bit 리눅스
  * openssl 1.0.2 버전 이상의 개발 라이브러리를 설치한다.
  * make 를 실행하여서 빌드한다.

### HTTP GET 요청/응답 개발 방법
HttpStack 라이브러리의 CHttpClient 클래스를 이용한 HTTP GET 프로토콜 연동하는 소스 코드 예제는 다음과 같습니다.
테스트용 샘플 소스 코드는 TestHttpClient 프로젝트의 TestHttpClientGet.cpp 파일입니다.

```
 CHttpClient clsClient;
 std::string strBodyType, strBody;

 clsClient.DoGet( "http://www.naver.com", strBodyType, strBody );
```

### HTTP POST 요청/응답 개발 방법
HttpStack 라이브러리의 CHttpClient 클래스를 이용한 HTTP POST 프로토콜 연동하는 소스 코드 예제는 다음과 같습니다.
테스트용 샘플 소스 코드는 TestHttpClient 프로젝트의 TestHttpClientPost.cpp 파일입니다.

```
 std::string strSendBody, strRecvBodyType, strRecvBody;
 CHttpClient clsClient;
 HTTP_HEADER_LIST clsHeaderList;
 CHttpHeader clsHeader( "SOAPAction", "http://www.webserviceX.NET/GetWeather" );

 strSendBody = "<soapenv:Envelope xmlns:soapenv=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:web=\"http://www.webserviceX.NET\">"
   "<soapenv:Header/>"
   "<soapenv:Body>"
      "<web:GetWeather>"
         "<web:CityName>seoul</web:CityName>"
         "<web:CountryName>kr</web:CountryName>"
      "</web:GetWeather>"
   "</soapenv:Body>"
  "</soapenv:Envelope>";

  clsClient.DoPost( "http://www.webserviceX.NET/globalweather.asmx", &clsHeaderList, "text/xml;charset=UTF-8", strSendBody.c_str(), strRecvBodyType, strRecvBody );
```

### HTTP SOAP 요청/응답 개발 방법
HttpStack 라이브러리의 CHttpClient 클래스를 이용한 웹 서비스 연동하는 소스 코드 예제는 다음과 같습니다.
테스트용 샘플 소스 코드는 TestHttpClient 프로젝트의 TestHttpClientSoap.cpp 파일입니다.

```
 std::string strSendBody, strRecvBody;
 CHttpClient clsClient;

 strSendBody = "<soapenv:Envelope xmlns:soapenv=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:web=\"http://www.webserviceX.NET\">"
   "<soapenv:Header/>"
   "<soapenv:Body>"
      "<web:GetWeather>"
         "<web:CityName>seoul</web:CityName>"
         "<web:CountryName>kr</web:CountryName>"
      "</web:GetWeather>"
   "</soapenv:Body>"
  "</soapenv:Envelope>";

 clsClient.DoSoap( "http://www.webserviceX.NET/globalweather.asmx", "http://www.webserviceX.NET/GetWeather", strSendBody.c_str(), strRecvBody );
```

### 초간단 HTTP 서버 개발 방법
초간단 HTTP 서버 개발 소스 코드 예제는 TestHttpStack 프로젝트에 있습니다. TestHttpStack 프로젝트 소스 코드를 참고하시면 CHttpStack 을 이용하여서 어떻게 HTTP 서버를 개발할 수 있을지 확인하실 수 있습니다.

### WSDL 문서로 SOAP 통신용 클래스 생성 방법
WSDL 파일을 읽어서 SOAP 통신용 클래스 생성하는 소스 코드 예제는 다음과 같습니다.
테스트용 샘플 소스 코드는 TestWsdlParser 프로젝트를 확인해 보세요.

```
 CWsdlMessage clsWsdl;

 if( clsWsdl.ParseFile( "globalweather.xml" ) == false )
 {
  printf( "clsWsdl.Parse error\n" );
 }
 else
 {
  clsWsdl.GetSoap()->MakeSource( NULL );
 }
```

### 생성한 SOAP 통신용 클래스로 SOAP 통신하는 방법
위에서 WSDL 문서로 생성한 SOAP 클래스로 SOAP 통신하여서 응답을 수신하는 소스 코드는 다음과 같습니다.

```
 CSoapGlobalWeatherSoap clsSoap;
 std::string strInput, strOutput;

 strInput = "korea";

 if( clsSoap.GetCitiesByCountry( strInput, strOutput ) == false ) return false;

 printf( "[%s]\n", strOutput.c_str() );
```

### JSON 문자열 파싱 및 생성하는 방법
TestJsonParser 폴더의 소스 코드를 참고하세요.

### 관련 포스트

* https://blog.naver.com/websearch/222645809188 : 리눅스에서 빌드할 때에 nonrepresentable section on output 오류 해결하는 방법
* https://blog.naver.com/websearch/222414136421 : HTTP 시뮬레이터 프로그램 1차 개발 완료
* https://blog.naver.com/websearch/222404707445 : HTTP/2 프로토콜 기능 1차 개발 완료
* https://blog.naver.com/websearch/221511179768 : 읽기 편한 json 문자열 생성 기능 추가 완료
* https://blog.naver.com/websearch/221416268609 : 안드로이드 FCM 푸시 알림 전송 샘플 프로그램 개발 완료
* https://blog.naver.com/websearch/221406617987 : HTTP no cache proxy 프로그램 개발 완
* https://blog.naver.com/websearch/221361595770 : POST multipart/form-data 처리 기능 추가 완료
* https://blog.naver.com/websearch/221359386562 : POST 파라미터 파싱 예제 추가 완료
* https://blog.naver.com/websearch/221358225884 : GET 파라미터 파싱 기능 추가 완료
* https://blog.naver.com/websearch/221355453609 : HTTP 클라이언트에 파일 업로드 기능 추가 완료
* https://blog.naver.com/websearch/221304621705 : HTTP 서버 성능 측정 프로그램 1차 개발 완료
* https://blog.naver.com/websearch/221207718670 : VisualStudio 2017 에서 C++ HTTP stack 빌드하는 방법
* https://blog.naver.com/websearch/221142356741 : WebRTC 테스트 프로그램 개발 완료
* https://blog.naver.com/websearch/221137780592 : 초간단 C++ WebSocket 서버 개발 방법
* https://blog.naver.com/websearch/221137776985 : C++ HTTP stack 에 WebSocket 서버 기능 추가 완료
* https://blog.naver.com/websearch/220986809154 : C++ 기반 고성능 REST API 서버 개발 방법
* https://blog.naver.com/websearch/220986770727 : C++ HTTP 서버 성능 테스트 #1
* https://blog.naver.com/websearch/220982955113 : C++ JsonParser 로 Json Array 생성하는 방법
* https://blog.naver.com/websearch/220982797106 : C++ JsonParser 로 Json Object 생성하는 방법
* https://blog.naver.com/websearch/220937602762 : C++ HTTP stack 아키텍처
* https://blog.naver.com/websearch/220937584573 : C++ HTTP stack 및 TCP stack 설정 파일
* https://blog.naver.com/websearch/220937571044 : C++ HTTP stack 및 TCP stack 설정
* https://blog.naver.com/websearch/220902758709 : C++ TCP stack 라이브러리 소개
* https://blog.naver.com/websearch/220857574782 : C++ Json 문자열 파서/생성 라이브러리
* https://blog.naver.com/websearch/220842522297 : WSDL 문서로 SOAP 연동 C++ 클래스 생성 방법
* https://blog.naver.com/websearch/220836975654 : C++ HTTP POST 요청 예제
* https://blog.naver.com/websearch/220836973582 : C++ HTTP GET 요청 예제
* https://blog.naver.com/websearch/220836971178 : 초간단 C++ HTTP 서버 개발 방법
