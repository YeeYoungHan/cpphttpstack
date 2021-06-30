# HttpSimulator

### 개요

* C++ HTTP stack 을 이용하여서 HTTP 웹 시뮬레이터 프로그램 개발
  * HTTP GET, POST 전송 기능 개발
  * HTTP 응답 body 를 파일로 저장하는 기능 개발
  * HTTP POST body 에 저장할 내용을 출력하는 프로그램을 실행하는 기능 개발
  * HTTP 응답 html 에 포함된 모든 css, js, img 파일들을 다운로드하는 기능 개

### 개발자 정보

* 이메일 : websearch@naver.com
* 블로그 : http://blog.naver.com/websearch

### 라이선스

* 본 프로젝트의 라이선스는 GPLv3 이고 기업용 라이선스는 websearch@naver.com 으로 문의해 주세요.

### 실행 방법

* HttpSimulator.xml 파일에 아래와 같은 HTTP 웹서버 연동 명령을 저장한다.
  * 각 명령은 순서대로 실행된다.

```
<HttpSimulator>
	<!-- HTTP 웹서버 연동 명령 리스트 -->
	<CommandList>
	
		<!-- HTTP GET 명령 -->
		<Command>
			<Url>http://127.0.0.1/</Url>
		</Command>
		
		<!-- HTTP POST 명령 -->
		<Command>
			<Method>POST</Method>
			<Url>http://127.0.0.1/login</Url>
			<Body>userId=test&passWord=1234</Body>
		</Command>
		
		<!-- 수신한 HTTP body 를 파일로 저장하는 명령 -->
		<Command>
			<Url>http://127.0.0.1/</Url>
			<RecvFileName>c:\temp\index.html</RecvFileName>
		</Command>
		
		<!-- HTTP POST body 에 저장할 문자열을 출력하는 프로그램을 실행하여서 HTTP POST 명령을 수행한다. -->
		<Command>
			<Method>POST</Method>
			<Url>http://127.0.0.1/login</Url>
			<BodyExecute>java PrintLoginText c:\temp\index.html</BodyExecute>
		</Command>
		
		<!-- GET 명령: 수신한 HTML 페이지에 존재하는 css, js, img 파일을 모두 다운로드한다. -->
		<Command>
			<Url>http://127.0.0.1/</Url>
			<DownloadAll>true</DownloadAll>
		</Command>
		
	</CommandList>
	<Log>
		<Level Debug="true" Network="false" HttpHeader="true" />
	</Log>
</HttpSimulator>
```

* 아래와 같이 실행한다.

```
dos> HttpSimulator.exe HttpSimulator.xml
```
