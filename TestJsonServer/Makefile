# Makefile for TestJsonServer
#
# programmer : yee young han ( websearch@naver.com )
#            : http://blog.naver.com/websearch
# start date : 2019/04/04

include ../Makefile.mk

INC=-I../HttpStack -I../TcpStack -I../Http2Parser -I../HttpParser \
	-I../XmlParser -I../SipPlatform
LIB=-L../HttpStack -lhttpstack \
	-L../TcpStack -ltcpstack \
	-L../Http2Parser -lhttp2parser \
	-L../HttpParser -lhttpparser \
	-L../XmlParser -lxmlparser \
	-L../SipPlatform -lsipplatform \
	$(OPENSSL_LIB) -lpthread

.SUFFIXES: .o .cpp
%.o : %.cpp
	$(CC) $(CFLAGS) -c $< $(INC)

all: TestJsonServer.exe

SRV_OBJ=Main.o JsonServer.o Setup.o

TestJsonServer.exe: $(SRV_OBJ)
	$(CC) $(CFLAGS) -o $@ $(SRV_OBJ) $(INC) $(LIB)

clean:
	rm -f *.exe
	rm -f *.o
	rm -f core.*

