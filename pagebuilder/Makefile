# $Id: Makefile,v 1.7 2006/06/08 15:29:48 nickz Exp $

MODULE := pagebuilder

DEPENDS :=	\
	LIBUTIL	\
	LIBAPPS	\
	LIBDBLDAPUTIL	\
	LIBRMDS


SOURCES := \
	PBuilderApp.cpp		\
	PageArrayConverter.cpp	\
	PageConverter.cpp	\
	RowConverter.cpp	\
	CellConverter.cpp	\
	CFormat.cpp		\
	CVideo.cpp		\
	ChainManager.cpp	\
	PBrmdsRecordHandler.cpp \
	pbPageProducer.cpp

LIBRARIES =
OBJECTS := $(SOURCES:.cpp=.o)

include $(BASE_DIR)/make/common.mk

