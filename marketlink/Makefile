# $Id: $

MODULE := mklfeed 

DEPENDS :=	\
	LIBUTIL	\
	LIBAPPS	\
	LIBDBLDAPUTIL	\
	LIBRMDS	


SOURCES := \
	MKLFeedApp.cpp	\
	MKLHttpClient.cpp	\
	MKLOpenServer.cpp	\
	MKLMsg.cpp	\
	MKLRecordSubscriber.cpp	\
	MKLPageSubscriber.cpp	\
	MKLFeedSession.cpp	\
	MKLTag.cpp		\
	MKLThrottleHandler.cpp	\
	MKLSubjectSubMgr.cpp

#	MKLrmdsRecordHandler.cpp


#LIBRARIES += -lrtc 
#LIBRARIES = 
OBJECTS := $(SOURCES:.cpp=.o)

include $(BASE_DIR)/make/common.mk

