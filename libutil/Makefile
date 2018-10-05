# $Id: Makefile,v 1.9 2006/04/11 16:03:19 nickz Exp $

MODULE := libutil.so

DEPENDS := \
	RFA \
	BERKELEYDB


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


OBJECTS := $(SOURCES:.cpp=.o)

include $(BASE_DIR)/make/common.mk
