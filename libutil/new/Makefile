# $Id: Makefile,v 1.7 2005/06/29 18:55:58 mikhailt Exp $

MODULE := libutil.so

DEPENDS := 

INTERFACES := \
	Logger.hpp	\
	MsgGraph.hpp	\
	DataGraph.hpp	\
	Subscriber.hpp	\
	Formatter.hpp	\
	Runnable.hpp	\
	CreateThread.hpp	\
	Scheduler.hpp	\
	Datum.hpp	\
	DataPage.hpp	\
	GliphDef.hpp	\
	PageListener.hpp	\
	HtmlMap.hpp	\
	HttpDataMap.hpp	\
	Converter.hpp	\
	Reflection.hpp	\
	StorageManager.hpp

SOURCES := \
	Logger.cpp	\
	MsgGraph.cpp	\
	DataGraph.cpp	\
	Subscriber.cpp	\
	Formatter.cpp	\
	CreateThread.cpp	\
	Scheduler.cpp	\
	DataPage.cpp	\
	HtmlMap.cpp	\
	HttpDataMap.cpp	\
	Reflection.cpp	\
	StorageManager.cpp

#LIBRARIES += -R/usr/local/BerkeleyDB.4.1/lib -L/usr/local/BerkeleyDB.4.1/lib -ldb 
INCLUDES += -I/usr/local/BerkeleyDB.4.1/include 


OBJECTS := $(SOURCES:.cpp=.o)

include $(BASE_DIR)/make/common.mk
