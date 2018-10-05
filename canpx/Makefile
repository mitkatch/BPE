# $Id: Makefile,v 1.5 2006/06/05 13:18:36 nickz Exp $

MODULE := canpx

DEPENDS :=	\
	LIBUTIL	\
	LIBAPPS	\
	LIBRMDS	\
	LIBDBLDAPUTIL


SOURCES := \
	CanpxApp.cpp		\
	CanpxOpenServer.cpp	\
	CanpxHttpClient.cpp	\
	CanpxInstrument.cpp	\
	CanpxAlgorithm.cpp	\
	CanpxEngineGOCImpl.cpp	\
	CanpxEngineTBillImpl.cpp\
	CanpxEngineProvImpl.cpp	\
	CanpxEngine.cpp		\
	CompiledInstrument.cpp	\
	CanpxTrade.cpp		\
	ChainManager.cpp	\
	CanpxSubjectSubMgr.cpp  \
	canpxRecordProducer.cpp


LIBRARIES = 
OBJECTS := $(SOURCES:.cpp=.o)

include $(BASE_DIR)/make/common.mk

