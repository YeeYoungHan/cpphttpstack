# Makefile for all of C++ HttpStack
#
# programmer : yee young han ( websearch@naver.com )
#            : http://blog.naver.com/websearch
# start date : 2012/07/12

all:
	cd SipPlatform && make
	cd HttpParser && make
	cd Http2Parser && make
	cd TcpStack && make
	cd HttpStack && make
	cd XmlParser && make
	cd WsdlParser && make
	cd JsonParser && make
	cd HtmlParser && make
	cd TestHttpClient && make
	cd TestHttpParser && make
	cd TestHttpStack && make
	cd TestWsdlParser && make
	cd TestJsonParser && make
	cd TestHtmlParser && make
	cd HttpSpeed && make
	cd HttpSimulator && make

clean:
	cd SipPlatform && make clean
	cd HttpParser && make clean
	cd Http2Parser && make clean
	cd TcpStack && make clean
	cd HttpStack && make clean
	cd XmlParser && make clean
	cd WsdlParser && make clean
	cd JsonParser && make clean
	cd HtmlParser && make clean
	cd TestHttpClient && make clean
	cd TestHttpParser && make clean
	cd TestHttpStack && make clean
	cd TestWsdlParser && make clean
	cd TestJsonParser && make clean
	cd TestHtmlParser && make clean
	cd HttpSpeed && make clean
	cd HttpSimulator && make clean

install:

