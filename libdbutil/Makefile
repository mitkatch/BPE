
# $Id: Makefile,v 1.3 2005/06/29 18:15:53 mikhailt Exp $

MODULE := libdbutil.so

DEPENDS := 	\
		LIBUTIL
		

INTERFACES := \
	DBConnector.hpp	\
	DBRecord.hpp	\
	DBRecordIterator.hpp	\
	DBRecordSet.hpp


SOURCES := \
	DBConnector.cpp		\
	DBRecordIterator.cpp	\
	DBRecord.cpp	\
	DBRecordSet.cpp


#LIBRARIES = 

#INCLUDES += -I$(BASE_DIR)/$(ARCH)/include


OBJECTS := $(SOURCES:.cpp=.o)

include $(BASE_DIR)/make/common.mk

