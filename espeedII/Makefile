# $Id: Makefile,v 1.3 2006/06/07 14:29:12 nickz Exp $

MODULE := espeedfeedII
 

DEPENDS :=	\
	CFETI	\
	LIBUTIL	\
	LIBAPPS	\
	LIBRMDS	\
	LIBDBLDAPUTIL



SOURCES := \
	ESHttpClient.cpp	\
	ESOpenServer.cpp	\
	ESpeedApp.cpp		\
	ESCFETISession.cpp	\
	ESCFETIProcessor.cpp	\
	ESInstrument.cpp	\
	AliasConverter.cpp	\
	esRecordProducer.cpp

LIBRARIES = 
OBJECTS := $(SOURCES:.cpp=.o)

include $(BASE_DIR)/make/common.mk

