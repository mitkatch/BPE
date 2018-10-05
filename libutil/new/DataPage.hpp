
/************************************************************************
||
|| DATE:                $Date: 2006/04/07 14:06:58 $
|| SOURCE:              $Source: /export/home/tomcat/mlsrc/libutil/RCS/DataPage.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: DataPage.hpp,v 1.6 2006/04/07 14:06:58 tomcat Exp $
|| REVISION:    $Revision: 1.6 $
|| LOG:                 $Log: DataPage.hpp,v $
|| LOG:                 Revision 1.6  2006/04/07 14:06:58  tomcat
|| LOG:                 24 /80 page
|| LOG:
|| LOG:                 Revision 1.5  2006/03/28 21:18:09  mikhailt
|| LOG:                 Update added
|| LOG:
|| LOG:                 Revision 1.4  2005/06/29 18:55:58  mikhailt
|| LOG:                 *** empty log message ***
|| LOG:
|| LOG:                 Revision 1.3  2004/12/10 16:46:10  mikhailt
|| LOG:                 setVideo
|| LOG:
|| LOG:                 Revision 1.2  2004/10/20 19:47:10  mikhailt
|| LOG:                 PageParser compatible
|| LOG:
|| LOG:                 Revision 1.1  2004/09/23 17:54:33  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#ifndef DATAPAGE_HPP
#define DATAPAGE_HPP
#include <stdio.h>
#include <pthread.h>
//#include <stdarg.h>

#include <map>
#include <string>
#include <vector>
#include <utility>
#include <set>
#include <algorithm>

using std::map;
using std::string;
using std::vector;
using std::set;

#include "MsgGraph.hpp"
#include "Datum.hpp"
#include "GliphDef.hpp"

class PageListener;
class Subscriber;

class DataPage: public Datum
{
	public:
		typedef struct {
			char data;
			int video;
			int font;
			int bg;
			int fg;
		} PageGliph; 
		typedef vector<PageGliph> PageRow;
		typedef map<int, PageRow> PageMap; 

		typedef struct PageSlot {
			PageSlot(){};
			PageSlot(int r1, int r2, int c1, int c2):
					row1(r1), row2(r2), 
					column1(c1), column2(c2){};
			int row1;
			int row2;
			int column1;
			int column2;
			bool operator>=(const PageSlot& slot){
				bool b_r = (row1 <= slot.row1 && row2 >= slot.row2);
				bool b_c = (column1 <= slot.column1 && column2 >= slot.column2); 
				return  b_r && b_c;
			}
			bool operator<=(const PageSlot& slot){
				bool b_r = (row1 <= slot.row1 && row2 >= slot.row2);
				//bool b_c = (column1 <= slot.column1 && column2 >= slot.column2); 
				bool b_c = (slot.column1 <= column1 && column2 <= slot.column2); 
				return  b_r && b_c;
			}
			bool operator==(const PageSlot& slot){
				bool b_r = (row1 == slot.row1 && row2 == slot.row2);
				bool b_c = (column1 == slot.column1 && column2 == slot.column2); 
				return  b_r && b_c;

			}
		} PageSlot;

		typedef struct {
			int row;
			int column;
			string data;
			int bg;
			int fg;
			int video;
			int font;
		} Update;


		static enum {	MIN_ROW = 0, 
				MAX_ROW = 23, //24, 
				MIN_COLUMN = 0,
				MAX_COLUMN = 79 //80
			};	
		static enum {	V_NONE =	0x00,
				V_BLINK =	0x001,
				V_HIGHLIGHT =	0x010,
				V_UNDERLINE =	0x100};

	public:
		explicit DataPage(const string& address, int row = DataPage::MAX_ROW +1, int columns = DataPage::MAX_COLUMN +1);
		virtual ~DataPage();

		virtual void getDataClone(Datum *datum){};
		virtual int getVideo(int r1, int c1);
		virtual bool setVideo(int r1, int c1, int video);
		virtual bool update(int row, int column, const string& data);
		virtual bool update(int row, int column, const string& data, int video);
		virtual bool update(int row, int column, const string& data, int video, int bg, int fg);
		virtual bool update(int row, int column, const string& data, int bg, int fg, int video, int font);

		virtual bool registerSubscriber(Subscriber *subscriber, bool b_notify=true);
		virtual bool unregisterSubscriber(Subscriber *subscriber);
		virtual int numOfSubscribers();
		virtual void updated();

		virtual bool addListener(PageListener *, PageSlot slot);
		virtual set<PageListener *> getListenersSet(const PageSlot& slot);
		virtual bool removeListener(PageListener *);
		virtual void notifyListeners(int row1, int row2, int column1, int column2);
		virtual string columnAsString(int column);
		virtual DataPage::PageRow rowAsPageRow(int row);
		virtual string rowAsString(int row);
		virtual string rowAsHtmlRow(int row);
		virtual string rowAsHtmlRow(int row, const map<int, string> &htmlAttr);
		virtual string slotAsString( DataPage::PageSlot slot);
		virtual int height() const{
			return _rows;
		}
		virtual int width() const {
			return _columns;
		}

		static void serializeSlot(vector<string>& vec, const DataPage::PageSlot& slot);
		static void deserializeSlot(const vector<string>& vec, DataPage::PageSlot& slot);

		virtual DataPage::Update getLastUpdate(){
			return _lastUpdate;
		}

	protected:
		virtual void init();
		string toFlatUpdate(int row, int column, const string& data, int bg, int fg, int video, int font);
		PageMap _image;
		string _address;
		int _columns, _rows;
		Update _lastUpdate;
		set<Subscriber *> _subscribers;
		map<PageListener *, vector<PageSlot> > _listeners;
		pthread_mutex_t m_DataMutex;
		pthread_mutex_t m_SubsctiberMutex;
		pthread_mutex_t m_ListenerMutex;
};

#endif //DATAPAGE_HPP
