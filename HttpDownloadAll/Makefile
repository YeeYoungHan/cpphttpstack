# Makefile for HttpDownload
#
# programmer : yee young han ( websearch@naver.com )
#            : http://blog.naver.com/websearch
# start date : 2023/06/02

include ../Makefile.mk

INC=-I../HtmlParser -I../HttpStack -I../TcpStack -I../Http2Parser -I../HttpParser \
	-I../XmlParser -I../SipPlatform
LIB=-L../HtmlParser -lhtmlparser \
	-L../HttpStack -lhttpstack \
	-L../TcpStack -ltcpstack \
	-L../Http2Parser -lhttp2parser \
	-L../HttpParser -lhttpparser \
	-L../XmlParser -lxmlparser \
	-L../SipPlatform -lsipplatform \
	$(OPENSSL_LIB) -lpthread

.SUFFIXES: .o .cpp
%.o : %.cpp
	$(CC) $(CFLAGS) -c $< $(INC)

all: HttpDownloadAll.exe

SRV_OBJ=HttpDownloadAll.o

HttpDownloadAll.exe: $(SRV_OBJ)
	$(CC) $(CFLAGS) -o $@ $(SRV_OBJ) $(INC) $(LIB)

clean:
	rm -f *.exe
	rm -f *.o
	rm -f core.*

