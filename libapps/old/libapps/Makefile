# $Id: Makefile,v 1.7 2006/03/31 15:18:58 mikhailt Exp $

MODULE := libapps.so

DEPENDS := 	EXPAT	\

INTERFACES := \
	IOCallback.hpp	\
	TimerCallback.hpp	\
	Application.hpp \
	BasicComponent.hpp \
	ConfigMap.hpp	\
	OpenServer.hpp	\
	MsgParser.hpp	\
	MsgProcessor.hpp	\
	MessageFactory.hpp	\
	DataCache.hpp		\
	DataFeed.hpp		\
	InteractiveDataFeed.hpp	\
	UDPSession.hpp		\
	HeartbeatSession.hpp	\
	MCMsg.hpp		\
	ControlSession.hpp	\
	HtmlBeanProcessor.hpp	\
	WorkerCallback.hpp	\
	OpenClient.hpp


SOURCES := \
	IOCallback.cpp	\
	TimerCallback.cpp	\
	Application.cpp  \
	BasicComponent.cpp  \
	ConfigMap.cpp	\
	OpenServer.cpp	\
	MsgProcessor.cpp	\
	MsgParser.cpp		\
	MessageFactory.cpp	\
	DataCache.cpp		\
	Main.cpp		\
	UDPSession.cpp		\
	HeartbeatSession.cpp	\
	MCMsg.cpp		\
	ControlSession.cpp	\
	HtmlBeanProcessor.cpp	\
	WorkerCallback.cpp	\
	OpenClient.cpp

LIBRARIES = 

INCLUDES += -I$(BASE_DIR)/$(ARCH)/include 

OBJECTS := $(SOURCES:.cpp=.o)

include $(BASE_DIR)/make/common.mk
