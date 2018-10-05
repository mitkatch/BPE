# $Id: Makefile,v 1.1 2005/11/17 21:57:58 mikhailt Exp $

MODULE := espeedfeedII
 

DEPENDS :=	\
	CFETI	\
	LIBUTIL	\
	LIBAPPS	\
	LIBMCMTRS	\
	LIBDBLDAPUTIL



SOURCES := \
	ESHttpClient.cpp	\
	ESOpenServer.cpp	\
	ESpeedApp.cpp		\
	ESCFETISession.cpp	\
	ESCFETIProcessor.cpp	\
	ESInstrument.cpp	\
	AliasConverter.cpp

LIBRARIES = 
OBJECTS := $(SOURCES:.cpp=.o)

include $(BASE_DIR)/make/common.mk

