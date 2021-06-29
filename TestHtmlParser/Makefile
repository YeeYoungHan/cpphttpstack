# Makefile for TestHtmlParser
#
# programmer : yee young han ( websearch@naver.com )
#            : http://blog.naver.com/websearch
# start date : 2020/02/07

include ../Makefile.mk

INC=-I../HtmlParser -I../SipPlatform
LIB=-L../HtmlParser -lhtmlparser \
	-L../SipPlatform -lsipplatform \
	-lpthread

.SUFFIXES: .o .cpp
%.o : %.cpp
	$(CC) $(CFLAGS) -c $< $(INC)

all: TestHtmlParser.exe

SRV_OBJ=Main.o \
	TestHtmlAttribute.o TestHtmlClass.o TestHtmlElement.o TestHtmlGetText.o \
	TestHtmlUrlList.o

TestHtmlParser.exe: $(SRV_OBJ)
	$(CC) $(CFLAGS) -o $@ $(SRV_OBJ) $(INC) $(LIB)

clean:
	rm -f *.exe
	rm -f *.o
	rm -f core.*

