# $Id: Makefile,v 1.2 2006/03/23 18:53:58 nickz Exp $

MODULE := libpy.so

DEPENDS := PYTHON \
	RFA

INTERFACES := \
	PyProxy.hpp	\
	PyDataCache.hpp	\
	PyLocalModule.hpp

SOURCES := \
	PyProxy.cpp	\
	PyDataCache.cpp	\
	PyLocalModule.cpp

#LIBRARIES = 
#INCLUDES = 
INCLUDES += -I$(BASE_DIR)/$(ARCH)/include


OBJECTS := $(SOURCES:.cpp=.o)

include $(BASE_DIR)/make/common.mk
