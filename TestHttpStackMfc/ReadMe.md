# TestHttpStackMFC 프로그램

### 개요

* HttpStack 라이브러리를 이용한 HTTP 서버 개발
* MFC 기반 빌드 환경 예제 제

### 개발자 정보

* 이메일 : websearch@naver.com
* 블로그 : http://blog.naver.com/websearch

### 라이선스

* 본 프로젝트의 라이선스는 GPLv3 이고 기업용 라이선스는 websearch@naver.com 으로 문의해 주세요.

### 주의사

* TestHttpStackMfcDlg.cpp 소스 코드의 아래의 라인을 운영할 웹서버의 root 폴더로 수정한 후, 빌드해 주세요.

'''
	// HTTP 서버에서 사용할 Document root 폴더를 설정한다.
	m_clsServer.m_strDocumentRoot = "C:\\OpenProject\\CppHttpStack\\trunk\\doc";
'''
 