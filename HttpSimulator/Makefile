# Makefile for HttpSimulator
#
# programmer : yee young han ( websearch@naver.com )
#            : http://blog.naver.com/websearch
# start date : 2021/06/29

include ../Makefile.mk

INC=-I../HttpStack -I../TcpStack -I../HttpParser -I../HtmlParser \
	-I../XmlParser -I../SipPlatform
LIB=-L../HttpStack -lhttpstack \
	-L../TcpStack -ltcpstack \
	-L../HttpParser -lhttpparser \
	-L../HtmlParser -lhtmlparser \
	-L../XmlParser -lxmlparser \
	-L../SipPlatform -lsipplatform \
	$(OPENSSL_LIB) -lpthread

.SUFFIXES: .o .cpp
%.o : %.cpp
	$(CC) $(CFLAGS) -c $< $(INC)

all: HttpSimulator.exe

SRV_OBJ=HttpSimulator.o HttpSimulatorSetup.o HttpExecute.o

HttpSimulator.exe: $(SRV_OBJ)
	$(CC) $(CFLAGS) -o $@ $(SRV_OBJ) $(INC) $(LIB)

clean:
	rm -f *.exe
	rm -f *.o
	rm -f core.*

