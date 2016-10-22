# Makefile for WsdlParser
#
# programmer : yee young han ( websearch@naver.com )
#            : http://blog.naver.com/websearch
# start date : 2016/10/22

include ../Makefile.mk

INC=-I../HttpStack -I../TcpStack -I../XmlParser -I../SipPlatform

.SUFFIXES: .o .cpp
%.o : %.cpp
	$(CC) $(CFLAGS) -c $< $(INC)

all: libwsdlparser.a

LIB_OBJ=SoapArg.o SoapClass.o SoapMethod.o \
	WsdlMessage.o

libwsdlparser.a: $(LIB_OBJ)
	ar crv $@ $(LIB_OBJ)

clean:
	rm -f *.a
	rm -f *.o
	rm -f core.*

