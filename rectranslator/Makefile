# $Id:  $

MODULE := rectranslator 

DEPENDS :=	\
	LIBUTIL	\
	LIBAPPS	\
	LIBRMDS	\
	LIBDBLDAPUTIL


SOURCES := \
	RTApp.cpp	\
	RTRecordProducer.cpp	\
	RTrmdsRecordHandler.cpp	\
	RecordArrayConverter.cpp	\
	RecordConverter.cpp	\
	FieldTranslator.cpp	\
	FieldFormatter.cpp


LIBRARIES = 
OBJECTS := $(SOURCES:.cpp=.o)

include $(BASE_DIR)/make/common.mk

