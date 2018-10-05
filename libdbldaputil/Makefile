
# $Id: Makefile,v 1.1 2005/06/29 18:11:58 mikhailt Exp $

MODULE := libdbldaputil.so

DEPENDS := 	\
		LDAP	
		

INTERFACES := \
	DBConnectorLdap.hpp


SOURCES := \
	DBConnectorLdap.cpp


#LIBRARIES = 

INCLUDES += -I$(BASE_DIR)/$(ARCH)/include


OBJECTS := $(SOURCES:.cpp=.o)

include $(BASE_DIR)/make/common.mk

