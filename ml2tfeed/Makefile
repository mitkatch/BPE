# $Id: Makefile,v 1.1 2006/04/07 19:32:15 mikhailt Exp $

MODULE := ml2tfeed 

DEPENDS :=	\
	LIBUTIL	\
	LIBAPPS	\
	LIBRMDS	


SOURCES := \
	RMDSFeedSession.cpp	\
	ML2TFeedSession.cpp	\
	ML2TPageListener.cpp	\
	ML2TMsg.cpp	\
	ML2TFeedApp.cpp	\
	ML2TConverter.cpp

LIBRARIES += -lrtc 
#LIBRARIES = 
OBJECTS := $(SOURCES:.cpp=.o)

include $(BASE_DIR)/make/common.mk

