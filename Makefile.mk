# Makefile for all of C++ HttpStack
#
# programmer : yee young han ( websearch@naver.com )
#            : http://blog.naver.com/websearch
# start date : 2013/03/26

CC=g++
CFLAGS=-Wall -O2 -D_REENTRANT -g #-pg

OPENSSL_INC=-I../openssl/linux/64bit/include
OPENSSL_LIB=-L../openssl/linux/64bit/lib -lssl -lcrypto -ldl

