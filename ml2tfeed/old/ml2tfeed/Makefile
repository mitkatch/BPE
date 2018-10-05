# $Id: Makefile,v 1.2 2006/04/13 14:36:51 tomcat Exp $

MODULE := ml2tfeed 

DEPENDS :=	\
	LIBUTIL	\
	LIBAPPS	\
	LIBMCMTRS


SOURCES := \
	ML2TFeedApp.cpp	\
	ML2TFeedSession.cpp	\
	ML2TPageListener.cpp	\
	ML2TMsg.cpp

#LIBRARIES += -lrtc 
OBJECTS := $(SOURCES:.cpp=.o)

include $(BASE_DIR)/make/common.mk

