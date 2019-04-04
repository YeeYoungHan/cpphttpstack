# TestJsonServer 프로그램

### 개요

* HttpStack 라이브러리를 이용한 Json 응답 서버 개발
* Json 요청에 대한 응답을 수신하는 앱을 개발할 때에 Json 요청에 대한 응답을 시뮬레이션할 수 있습니다.

### 개발자 정보

* 이메일 : websearch@naver.com
* 블로그 : http://blog.naver.com/websearch

### 라이선스

* 본 프로젝트의 라이선스는 GPLv3 이고 기업용 라이선스는 websearch@naver.com 으로 문의해 주세요.

### 실행 방법
TestJsonServer.exe 프로그램은 설정 파일을 인자로 아래와 같이 실행하면 됩니다.

```
# TestJsonServer.exe TestJsonServer.xml
```

### 설정 파일 예제
설정 파일에는 요청 URI 에 대한 응답을 전송할 json 파일 full path 를 아래와 같이 저장하면 됩니다.

```
<Setup>
	<ReqUriPathList>
		<ReqUriPath>
			<ReqUri>/test</ReqUri>
			<Path>c:\temp\test.json</Path>
		</ReqUriPath>
	</ReqUriPathList>
</Setup>
```
 