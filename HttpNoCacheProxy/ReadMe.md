# HttpNoCacheProxy 프로그램

### 개요

* 웹 프로그램 개발할 때에 웹 브라우저의 캐쉬 때문에 현재 개발된 웹 페이지가 보이지 않는 경우가 발생합니다.
* HttpNoCacheProxy 를 이용하면 항상 현재 개발된 웹 페이지를 웹 브라우저에서 확인할 수 있습니다.
* 웹 브라우저 <-> HttpNoCacheProxy <-> HTTP Server 구조로 연동됩니다.
* 웹 브라우저에서는 HttpNoCacheProxy 의 IP:8080 으로 접속합니다.
* HttpNoCacheProxy 는 HTTP Server IP, Port 를 실행 인자로 입력받습니다.

### 개발자 정보

* 이메일 : websearch@naver.com
* 블로그 : http://blog.naver.com/websearch

### 라이선스

* 본 프로젝트의 라이선스는 GPLv3 이고 기업용 라이선스는 websearch@naver.com 으로 문의해 주세요.

### 실행 방법
HttpNoCacheProxy.exe 프로그램은 HTTP Server IP, Port 인자로 아래와 같이 실행하면 됩니다.

```
# HttpNoCacheProxy.exe {HTTP server IP} {HTTP Server Port}
# HttpNoCacheProxy.exe 127.0.0.1
```

 