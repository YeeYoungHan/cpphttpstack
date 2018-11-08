# HttpSpeed

### 개요

* 웹서버 성능 분석 프로그램

### 개발자 정보

* 이메일 : websearch@naver.com
* 블로그 : http://blog.naver.com/websearch

### 라이선스

* 본 프로젝트의 라이선스는 GPLv3 이고 기업용 라이선스는 websearch@naver.com 으로 문의해 주세요.

### 실행 방법
C++ HTTP stack 기반으로 HTTP 서버 성능 측정 프로그램의 실행 방법은 다음과 같습니다.

```
DOS> HttpSpeed.exe {url} {HTTP 요청 개수} {쓰레드 개수}
```

* {HTTP 요청 개수} 에 총 HTTP 요청할 개수를 입력한다.
* {쓰레드 개수} 에 동시에 HTTP 요청을 전송하고 응답을 수신할 쓰레드 개수를 입력한다.


### 결과 출력
HttpSpeed.exe 가 모든 HTTP 요청 개수를 처리 완료하면 아래의 항목을 출력합니다.

* 1개의 HTTP 요청/응답 처리에 소요된 최소/최대/평균 시간 ( ms 단위 )
* HTTP 응답 성공 개수
* HTTP 응답 오류 코드별 개수
  * HTTP 응답 오류가 존재하는 경우 출력된다.

### 관련 포스트

* https://blog.naver.com/websearch/221304621705 : HTTP 서버 성능 측정 프로그램 1차 개발 완료
