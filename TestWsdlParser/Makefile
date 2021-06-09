# Makefile for TestWsdlParser
#
# programmer : yee young han ( websearch@naver.com )
#            : http://blog.naver.com/websearch
# start date : 2016/10/22

include ../Makefile.mk

INC=-I../WsdlParser -I../HttpStack -I../TcpStack \
	-I../HttpParser -I../XmlParser -I../SipPlatform
LIB=-L../WsdlParser -lwsdlparser \
	-L../HttpStack -lhttpstack \
	-L../TcpStack -ltcpstack \
	-L../HttpParser -lhttpparser \
	-L../XmlParser -lxmlparser \
	-L../SipPlatform -lsipplatform \
	$(OPENSSL_LIB) -lpthread

.SUFFIXES: .o .cpp
%.o : %.cpp
	$(CC) $(CFLAGS) -c $< $(INC)

all: TestWsdlParser.exe

SRV_OBJ=TestWsdlParser.o \
	TestGlobalWeatherSoap.o SoapGlobalWeatherSoap.o \
	TestWebService1.o SoapService1Soap.o

TestWsdlParser.exe: $(SRV_OBJ)
	$(CC) $(CFLAGS) -o $@ $(SRV_OBJ) $(INC) $(LIB)

clean:
	rm -f *.exe
	rm -f *.o
	rm -f core.*

