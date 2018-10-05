# $Id: Makefile,v 1.9 2006/06/04 11:34:14 nickz Exp $

MODULE := librmds.so


DEPENDS :=	\
	RFA	\
	TIBRV


INTERFACES := \
	rmdsRecordHandler.hpp	\
	rmdsPageHandler.hpp	\
	rmdsRecordProducer.hpp	\
	rmdsPageProducer.hpp	\
	subjectMgr.hpp		\
	subjectSubMgr.hpp	\
	subjectPubMgr.hpp	\
	rmdsManagedHandler.hpp	\
	rmdsHandler.hpp


SOURCES := \
	rmdsRecordHandler.cpp 	\
	rmdsPageHandler.cpp	\
	rmdsRecordProducer.cpp	\
	rmdsPageProducer.cpp	\
	rmdsManagedHandler.cpp	\
	rmdsHandler.cpp		\
	subjectMgr.cpp		\
	subjectSubMgr.cpp	\
	subjectPubMgr.cpp


LIBRARIES = 

INCLUDES += -I$(BASE_DIR)/$(ARCH)/include 

OBJECTS := $(SOURCES:.cpp=.o)

include $(BASE_DIR)/make/common.mk
