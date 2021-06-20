# TestHttpStack 프로그램

### 개요

* HttpStack 라이브러리를 이용한 HTTP 서버 개발
* HttpStack 라이브러리를 이용한 WebSocket echo 서버 개발

### 개발자 정보

* 이메일 : websearch@naver.com
* 블로그 : http://blog.naver.com/websearch

### 라이선스

* 본 프로젝트의 라이선스는 GPLv3 이고 기업용 라이선스는 websearch@naver.com 으로 문의해 주세요.

### 실행 방법
HTTP/1.1 프로토콜만 사용할 경우, TestHttpStack.exe 프로그램은 Document root 폴더를 인자로 아래와 같이 실행하면 됩니다.

```
# TestHttpStack.exe {Document root}
```

HTTP/2 프로토콜을 사용할 경우, TestHttpStack.exe 프로그램은 Document root 폴더 및 인증서 파일 인자로 아래와 같이 실행하면 됩니다.

```
# TestHttpStack.exe {Document root} {pem file}
```

### WebSocket echo 테스트 방법

* TestHttpStack.exe 를 실행한 호스트에서 본 폴더에 존재하는 WebSocketEchoClient.html 를 브라우저로 실행하면 WebSocket echo 테스트를 하실 수 있습니다.

 