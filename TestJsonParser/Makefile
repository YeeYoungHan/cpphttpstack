# Makefile for TestJsonParser
#
# programmer : yee young han ( websearch@naver.com )
#            : http://blog.naver.com/websearch
# start date : 2016/11/09

include ../Makefile.mk

INC=-I../JsonParser -I../SipPlatform
LIB=-L../JsonParser -ljsonparser \
	-L../SipPlatform -lsipplatform \
	-lpthread

.SUFFIXES: .o .cpp
%.o : %.cpp
	$(CC) $(CFLAGS) -c $< $(INC)

all: TestJsonParser.exe

SRV_OBJ=Main.o \
	TestJsonObject.o TestJsonArray.o \
	TestJsonObjectNewLine.o \
	SampleJsonObject.o SampleJsonArray.o

TestJsonParser.exe: $(SRV_OBJ)
	$(CC) $(CFLAGS) -o $@ $(SRV_OBJ) $(INC) $(LIB)

clean:
	rm -f *.exe
	rm -f *.o
	rm -f core.*

