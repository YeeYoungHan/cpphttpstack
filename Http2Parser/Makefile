# Makefile for Http2Parser
#
# programmer : yee young han ( websearch@naver.com )
#            : http://blog.naver.com/websearch
# start date : 2021/05/28

include ../Makefile.mk

INC=-I../HttpParser -I../SipPlatform

.SUFFIXES: .o .cpp
%.o : %.cpp
	$(CC) $(CFLAGS) -c $< $(INC)

all: libhttp2parser.a

LIB_OBJ=Http2Conversion.o Http2Frame.o Http2FrameList.o \
	Http2Header.o Http2HpackHeader.o Http2HpackHeaderList.o \
	Http2Packet.o Http2Settings.o Http2HuffmanCode.o

libhttp2parser.a: $(LIB_OBJ)
	ar crv $@ $(LIB_OBJ)

clean:
	rm -f *.a
	rm -f *.o
	rm -f core.*

