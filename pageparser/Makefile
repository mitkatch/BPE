# $Id: Makefile,v 1.4 2006/06/04 11:27:05 nickz Exp $

MODULE := pageparser 

DEPENDS :=	\
	EXPAT	\
	LIBUTIL	\
	LIBAPPS	\
	LIBRMDS	\
	LIBPY


SOURCES := \
	PageParserApp.cpp	\
	ParserOpenServer.cpp	\
	ParserHttpClient.cpp	\
	PyParser.cpp		\
	ppRecordProducer.cpp

LIBRARIES = 
OBJECTS := $(SOURCES:.cpp=.o)

include $(BASE_DIR)/make/common.mk

