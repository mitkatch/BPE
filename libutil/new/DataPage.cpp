/************************************************************************
||
|| DATE:                $Date: 2006/03/28 21:18:09 $
|| SOURCE:              $Source: /export/home/tomcat/mlsrc/libutil/RCS/DataPage.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: DataPage.cpp,v 1.7 2006/03/28 21:18:09 mikhailt Exp $
|| REVISION:    $Revision: 1.7 $
|| LOG:                 $Log: DataPage.cpp,v $
|| LOG:                 Revision 1.7  2006/03/28 21:18:09  mikhailt
|| LOG:                 Update added
|| LOG:
|| LOG:                 Revision 1.6  2005/09/12 16:31:08  tomcat
|| LOG:                 testing
|| LOG:
|| LOG:                 Revision 1.5  2005/06/29 18:55:58  mikhailt
|| LOG:                 *** empty log message ***
|| LOG:
|| LOG:                 Revision 1.4  2004/12/15 20:22:18  mikhailt
|| LOG:                 exclude one char from data (begins from zero)
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

#include <strings.h>

#include "DataPage.hpp"
#include "Subscriber.hpp"
#include "PageListener.hpp"
#include "HtmlMap.hpp"
#include "StorageManager.hpp"
#include "Scheduler.hpp"

DataPage::DataPage(const string& address, int rows , int columns ):
	Datum(address), _rows(rows), _columns(columns)
{
	pthread_mutex_init(&m_DataMutex, NULL);
	pthread_mutex_init(&m_SubsctiberMutex, NULL);
	pthread_mutex_init(&m_ListenerMutex, NULL);

	init();
}

DataPage::~DataPage()
{
	pthread_mutex_destroy(&m_DataMutex);
	pthread_mutex_destroy(&m_SubsctiberMutex);
	pthread_mutex_destroy(&m_ListenerMutex);

}

void
DataPage::init()
{
	pthread_mutex_lock(&m_DataMutex);
	for (int i = 0; i < _rows; i++){
		PageRow page_row;
		for (int j = 0; j < _columns; j++){
			PageGliph page;
			page.data = ' ';
			page.video = GLIPH_VIDEO_UNKNOWN;
			page.font = GLIPH_FONT_UNKNOWN;
			page.bg = GLIPH_BG_UNKNOWN;
			page.fg = GLIPH_FG_UNKNOWN;

			page_row.push_back(page);	
		}
		_image[i] = page_row;
	}
	pthread_mutex_unlock(&m_DataMutex);
}

DataPage::PageRow
DataPage::rowAsPageRow(int row)
{
	pthread_mutex_lock(&m_DataMutex);
	DataPage::PageRow pageRow;
	map<int, PageRow>::iterator it = _image.find(row);
	if (it != _image.end()){
		pageRow = (*it).second;
	}
	pthread_mutex_unlock(&m_DataMutex);
	return pageRow;
}

string
DataPage::rowAsString(int row)
{

	pthread_mutex_lock(&m_DataMutex);
	string str;
	map<int, PageRow>::iterator it = _image.find(row);

	if (it != _image.end()){
		PageRow &row = (*it).second;
		vector<PageGliph>::iterator it2 = row.begin();
		for(it2; it2!= row.end(); it2++){
			str.append(1, (*it2).data);
		}
	}

	pthread_mutex_unlock(&m_DataMutex);
	return str;
}

string
DataPage::rowAsHtmlRow(int row)
{

	pthread_mutex_lock(&m_DataMutex);
	string str;
	map<int, PageRow>::iterator it = _image.find(row);
	
	if (it != _image.end()){
		PageRow &row = (*it).second;
		vector<PageGliph>::iterator it2 = row.begin();
		for(it2; it2!= row.end(); it2++){
			str.append("<td>");
			str.append(HtmlMap::toHtml((*it2).data));
			str.append("</td>");
		}
	}

	pthread_mutex_unlock(&m_DataMutex);
	return str;
}

string
DataPage::rowAsHtmlRow(int row, const map<int, string> &htmlAttr)
{

	pthread_mutex_lock(&m_DataMutex);
	string str;
	map<int, PageRow>::iterator it = _image.find(row);
	
	if (it != _image.end()){
		PageRow &row = (*it).second;
		vector<PageGliph>::iterator it2 = row.begin();
		int i = 0;
		for(it2; it2!= row.end(); it2++, i++){
			str.append("<td ");
			map<int, string>::const_iterator it3 = htmlAttr.find(i);
			if (it3 != htmlAttr.end()){
				str.append((*it3).second);
			}
			str.append(" >"); //close attr
			str.append(HtmlMap::toHtml((*it2).data));
			str.append("</td>");
		}
	}

	pthread_mutex_unlock(&m_DataMutex);
	return str;
}


string
DataPage::columnAsString(int column)
{
	pthread_mutex_lock(&m_DataMutex);
	string str;
	map<int, PageRow>::iterator it = _image.begin();

	for (it; it != _image.end(); it++){
		PageRow row = (*it).second;
		if (row.size() > column){
			str.append(1, row[column].data);
			str.append(1, '\n');
		}	
	}

	pthread_mutex_unlock(&m_DataMutex);
	return str;

}

int
DataPage::getVideo(int row, int column)
{
	pthread_mutex_lock(&m_DataMutex);
	int video = -1;
	map<int, PageRow>::iterator it = _image.find(row);
	if (it != _image.end()){
		PageRow row = (*it).second;
		if (row.size() > column){
			video = row[column].video;
		}	
	}

	pthread_mutex_unlock(&m_DataMutex);
	return video;
}

bool
DataPage::setVideo(int row, int column, int video)
{
	pthread_mutex_lock(&m_DataMutex);
	bool done = false;
	map<int, PageRow>::iterator it = _image.find(row);
	if (it != _image.end()){
		PageRow row = (*it).second;
		if (row.size() > column){
			row[column].video = video;
			done = true;
		}	
	}

	pthread_mutex_unlock(&m_DataMutex);
	return done;
}

string
DataPage::slotAsString( DataPage::PageSlot slot)
{
	pthread_mutex_lock(&m_DataMutex);
	string str;
	map<int, PageRow>::iterator it = _image.begin();

	for (it; it != _image.end(); it++){
		if ((*it).first >= slot.row1 && (*it).first <= slot.row2){
			PageRow &row = (*it).second;
			for(int i= 0; i < row.size(); i++){
				if (i >= slot.column1 && i <= slot.column2){
					str.append(1, row[i].data);
				}
			}
			str.append(1, '\n');
		}
	}
	if (!str.empty()){
		if (str[str.size() -1] == '\n'){
			str.replace(str.size() -1, 1, "");
		}
	}


	pthread_mutex_unlock(&m_DataMutex);
	return str;


}

bool
DataPage::update(int row, int column, const string& data)
{
	return update(row, column, data, GLIPH_VIDEO_UNKNOWN,  GLIPH_BG_UNKNOWN, GLIPH_FG_UNKNOWN, GLIPH_FONT_UNKNOWN);  

}

bool
DataPage::update(int row, int column, const string& data, int video)
{
	//cout << "DataPage::update ["<<data<<"] col="<<column<<", row="<<row<<", video="<<video<<endl;
	return update(row, column, data, video, GLIPH_BG_UNKNOWN, GLIPH_FG_UNKNOWN, GLIPH_FONT_UNKNOWN);  


}

bool
DataPage::update(int row, int column, const string& data, int video, int bg, int fg)
{
	return update(row, column, data, video, bg, fg, GLIPH_FONT_UNKNOWN);  
}

bool
DataPage::update(int row, int column, const string& data, int video, int bg, int fg, int font)
{
	pthread_mutex_lock(&m_DataMutex);
	bool ret = false;
	map<int, PageRow>::iterator it = _image.find(row);
	//cout<<"Update Row:"<<row<<", Col:"<<column<<"["<<data<<"]"<<endl;
	if (it != _image.end()){
		_lastUpdate.row = row;
		_lastUpdate.column = column;
		_lastUpdate.data = data;
		_lastUpdate.video = video;
		_lastUpdate.bg = bg;
		_lastUpdate.fg = fg;
		_lastUpdate.font = font;

		PageRow &pagerow = (*it).second;
		//for (int j = column; j  < pagerow.size(); j++){
		//	pagerow[j].video = video;
		//}
		for(int i = 0; (pagerow.size() > column + i) && i < data.size(); i++ ){
			pagerow[column +i].data = data[i];
			pagerow[column +i].bg = bg;
			pagerow[column +i].fg = fg;
			pagerow[column +i].font = font;	
			pagerow[column +i].video = video;	
			ret = true;
		}
		if (StorageManager::activated()){
			string str = toFlatUpdate(row, column, data, video, bg, fg, font);
			StorageManager::getStorage(this->address())->save(str);
		}
		pthread_mutex_unlock(&m_DataMutex);
		notifyListeners(row, row, column, column+ data.size() -1);//begins from zero 
		pthread_mutex_lock(&m_DataMutex);
	}

	
	pthread_mutex_unlock(&m_DataMutex);
	//updated();
	return ret;
}

string
DataPage::toFlatUpdate(int row, int column, const string& data, int video, int bg, int fg, int font)
{
	string str;
	string end = "\n";
	char buf[24];
	bzero(buf, sizeof(buf));

	sprintf(buf, "%d", row);
	str += string("R1 : ") + string(buf) + end;

	sprintf(buf, "%d", column);
	str += string("C1 : ") + string(buf) + end;

	str += string("VAL : ") + data + end;

	sprintf(buf, "%d", data.size());
	str += string("LENGTH : ") + string(buf) + end;

	sprintf(buf, "%d", video);
	str += string("VIDEO : ") + string(buf) + end;

	sprintf(buf, "%d", bg);
	str += string("BG : ") + string(buf) + end;

	sprintf(buf, "%d", fg);
	str += string("FG : ") + string(buf) + end;

	sprintf(buf, "%d", font);
	str += string("FONT : ") + string(buf) + end;

	string timestamp = Scheduler::getCurrentTime("%x %X");
	str += string("TIMESTAMP : ") + timestamp + end;

	return str;
}

int
DataPage::numOfSubscribers()
{

	pthread_mutex_lock(&m_SubsctiberMutex);
	int i;
	i = _subscribers.size();

	pthread_mutex_unlock(&m_SubsctiberMutex);
	return i;
}

bool
DataPage::unregisterSubscriber(Subscriber *subscriber)
{
	//cout << "DataPage::unregisterSubscriber start "<<endl;
	pthread_mutex_lock(&m_SubsctiberMutex);
	bool ret = false;
	ret = _subscribers.erase(subscriber) == 1;

	pthread_mutex_unlock(&m_SubsctiberMutex);
	//cout<<" DataPage::unregisterSubscriber end "<<endl;
	return ret;
}

bool
DataPage::registerSubscriber(Subscriber *subscriber, bool b_notify)
{
	pthread_mutex_lock(&m_SubsctiberMutex);
	bool ret = false;
	ret = _subscribers.insert(subscriber).second;
	pthread_mutex_unlock(&m_SubsctiberMutex);

	if (ret && b_notify)
		subscriber->notify(this);


	return ret;

}

void
DataPage::updated()
{
	pthread_mutex_lock(&m_SubsctiberMutex);
	for (set<Subscriber *>::iterator it=_subscribers.begin();
		it != _subscribers.end(); it++){
			pthread_mutex_unlock(&m_SubsctiberMutex);
			(*it)->notify(this);
			pthread_mutex_lock(&m_SubsctiberMutex);
	}
	pthread_mutex_unlock(&m_SubsctiberMutex);
}

bool
DataPage::addListener(PageListener *listener, PageSlot slot)
{
	pthread_mutex_lock(&m_ListenerMutex);
	//bool ret = false;
	if (listener != NULL){
		map<PageListener *, vector<PageSlot> >::iterator it = _listeners.find(listener);

		if (it == _listeners.end()){
			vector<PageSlot> v;
			v.push_back(slot);
			_listeners[listener] = v;
		}
		else {
			(*it).second.push_back(slot);
		}
		pthread_mutex_unlock(&m_ListenerMutex);
		listener->onData(this, slot);
		pthread_mutex_lock(&m_ListenerMutex);
	}
	pthread_mutex_unlock(&m_ListenerMutex);
	return true;
}

bool
DataPage::removeListener(PageListener *listener)
{

	pthread_mutex_lock(&m_ListenerMutex);
	bool ret = false;
	ret = _listeners.erase(listener) == 1;
	pthread_mutex_unlock(&m_ListenerMutex);
	return ret;
}

void
DataPage::notifyListeners(int row1, int row2, int column1, int column2)
{
	pthread_mutex_lock(&m_ListenerMutex);

	PageSlot slot;
	slot.row1 = row1;
	slot.row2 = row2;
	slot.column1 = column1;
	slot.column2 = column2;

	map<PageListener *, vector<PageSlot> >::iterator it = _listeners.begin();
	
	for (it; it != _listeners.end(); it++){

		for (int j = 0; j < (*it).second.size(); j++){
			if ( (*it).second[j] >= slot){
				pthread_mutex_unlock(&m_ListenerMutex);
				(*it).first->onData(this, (*it).second[j]);
				pthread_mutex_lock(&m_ListenerMutex);
			}
		}
	}
	pthread_mutex_unlock(&m_ListenerMutex);
}

set<PageListener *> 
DataPage::getListenersSet(const PageSlot& slot)
{
	set<PageListener *> myset;
	pthread_mutex_lock(&m_ListenerMutex);
	map<PageListener *, vector<PageSlot> >::iterator it = _listeners.begin();
	for (it; it != _listeners.end(); it++){

		for (int j = 0; j < (*it).second.size(); j++){
			if ( (*it).second[j] <= slot){
				myset.insert((*it).first);
			}
		}
	}
	pthread_mutex_unlock(&m_ListenerMutex);
	return myset;
}

void
DataPage::serializeSlot(vector<string>& vec, const DataPage::PageSlot& slot)
{
	char buf[24];
	
	sprintf(buf, "%d", slot.row1);
	vec.push_back(buf);

	sprintf(buf, "%d", slot.row2);
	vec.push_back(buf);

	sprintf(buf, "%d", slot.column1);
	vec.push_back(buf);

	sprintf(buf, "%d", slot.column2);
	vec.push_back(buf);

}

void
DataPage::deserializeSlot(const vector<string>& vec, DataPage::PageSlot& slot)
{
	
	if (vec.size() == 4){
		slot.row1 = atoi(vec[0].c_str());
		slot.row2 = atoi(vec[1].c_str());
		slot.column1 = atoi(vec[2].c_str());
		slot.column2 = atoi(vec[3].c_str());
	}


}
