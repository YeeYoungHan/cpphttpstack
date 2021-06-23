# Makefile for TestHttpParser
#
# programmer : yee young han ( websearch@naver.com )
#            : http://blog.naver.com/websearch
# start date : 2016/10/15

include ../Makefile.mk

INC=-I../Http2Parser -I../HttpParser -I../SipPlatform
LIB=-L../Http2Parser -lhttp2parser \
	-L../HttpParser -lhttpparser \
	-L../SipPlatform -lsipplatform \
	-lpthread

.SUFFIXES: .o .cpp
%.o : %.cpp
	$(CC) $(CFLAGS) -c $< $(INC)

all: TestHttpParser.exe

SRV_OBJ=Main.o \
	TestHttpPacket.o TestHttpParameter.o TestHttpMultipart.o \
	TestHttpSetCookie.o \
	TestHttp2Conversion.o TestHttp2Header.o TestHttp2HuffmanCode.o \

TestHttpParser.exe: $(SRV_OBJ)
	$(CC) $(CFLAGS) -o $@ $(SRV_OBJ) $(INC) $(LIB)

clean:
	rm -f *.exe
	rm -f *.o
	rm -f core.*

