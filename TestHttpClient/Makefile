# Makefile for TestHttpClient
#
# programmer : yee young han ( websearch@naver.com )
#            : http://blog.naver.com/websearch
# start date : 2016/10/15

include ../Makefile.mk

INC=-I../HttpStack -I../TcpStack -I../HttpParser -I../SipPlatform
LIB=-L../HttpStack -lhttpstack \
	-L../TcpStack -ltcpstack \
	-L../HttpParser -lhttpparser \
	-L../SipPlatform -lsipplatform \
	$(OPENSSL_LIB) -lpthread

.SUFFIXES: .o .cpp
%.o : %.cpp
	$(CC) $(CFLAGS) -c $< $(INC)

all: TestHttpClient.exe

SRV_OBJ=Main.o \
	TestHttpClientGet.o TestHttpClientPost.o TestHttpClientSoap.o \
	TestHttpClientUpload.o \
	TestHttpClient2Get.o TestHttpClient2Loop.o

TestHttpClient.exe: $(SRV_OBJ)
	$(CC) $(CFLAGS) -o $@ $(SRV_OBJ) $(INC) $(LIB)

clean:
	rm -f *.exe
	rm -f *.o
	rm -f core.*

