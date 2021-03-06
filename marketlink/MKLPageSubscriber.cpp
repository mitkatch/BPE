
/************************************************************************
||
|| DATE:                $Date:$
|| SOURCE:              $Source:$
|| STATE:               $State:$
|| ID:                  $Id:$
|| REVISION:            $Revision:$
|| LOG:                 $Log:$
|| LOG:
************************************************************************/

#include "MKLFeedApp.hpp"
#include "Logger.hpp"
#include "MKLPageSubscriber.hpp"
#include "MKLFeedSession.hpp"
#include "Scheduler.hpp"
#include "DataGraph.hpp"
#include "MKLThrottleHandler.hpp"

MKLPageSubscriber::MKLPageSubscriber(FeedSession* s, const URLNameSpace& ns): _mySession(s), _activated(false), _myPage(NULL), _locked(false)
{
	//_updates = new vector<DataGraph::DataMap>();
	_pageImage = new MKLPage;
	_mklImage  = new MKLPage;
	_isRefreshRequired = false; 
	_security_id = ns.security_id;
	_address = ns.source + "." + ns.page;
	_row_tmpl_str = ns.row_tmpl;
	_throttle_delay = ((ns.throttle_delay).size()? Formatter::ATOF((ns.throttle_delay).c_str()): 1);
	if (setRowTmpl() == false)
		setDefaultRowMap();
	setMySlot(ns.slot, _security_id);
	_watermark = 500;
	string w = ConfigMap::getConfig()->getValue("SESSION_WATERMARK");
	if (w.size())
		_watermark = Formatter::ATOF(w.c_str());
}

MKLPageSubscriber::~MKLPageSubscriber()
{
	map<DataPage::PageSlot *, string>::iterator it = _mySlotMap.begin();
	for (it; it != _mySlotMap.end(); it++){
		delete (*it).first;
	}
	_pageImage->clear();
	delete _pageImage;

	_mklImage->clear();
	delete _mklImage;
}

void
MKLPageSubscriber::notify( Datum *datum)
{

}


void
MKLPageSubscriber::onData(DataPage *page, DataPage::PageSlot slot)
{
	if (_mySession == NULL) return;
	Logger::getLogger()->log(Logger::TYPE_INFO,"MKLPageSubscriber::onData processing %s ", _address.c_str());
	string tmstmp = Scheduler::getCurrentTime(CANPX_TIME_FORMAT); 
	theMKLFeedApp->setTimestamp(_address, tmstmp);

	if (_activated == false){
		_myPage = page;
		MKLThrottleHandler::addRic(_security_id, this);
		publishPageImage(page, slot);
		_activated = true;

	}else{
		DataPage::Update upd = page->getLastUpdate();
		publishPageUpdate(upd);	
	}
}


void
MKLPageSubscriber::publishPageImage(DataPage *page, DataPage::PageSlot& slot)
{
	if (page == _myPage){
		publishRefresh();
	}
}

void
MKLPageSubscriber::publishRefresh()
{
	MKLFeedSession *feed = dynamic_cast<MKLFeedSession *>(_mySession);
	_mklImage->clear();
	if (feed && !_locked){
		buildPageImage();
		_isRefreshRequired = false;
		MKLPage::iterator it = _pageImage->begin();
		for(it; it != _pageImage->end(); it++){
			//if (clearMKLImage((*it).first) ){
				MKLUpdates ups = buildUpdates((*it).first, (*it).second);
				for (int i = 0; i < ups.size(); i++){
					feed->publishInsertRequest(ups[i], _security_id);
				}
			//}
		}
	}else{
		_isRefreshRequired = true;
		//send next time;
	}
}

bool
MKLPageSubscriber::clearMKLImage(const string& rowID)
{
	/****
	DataPage::PageRow prow;
	for (int i = 0; i <= DataPage::MAX_COLUMN ; i++){
		DataPage::PageGliph g;
		g.data = 0x00;
		g.video = GLIPH_VIDEO_UNKNOWN;
		g.font = GLIPH_FONT_UNKNOWN;
		g.bg = GLIPH_BG_UNKNOWN;
		g.fg = GLIPH_FG_UNKNOWN;
		prow.push_back(g);
	}
	****/
	MKLPage::iterator it = _mklImage->find(rowID);
	if (it != _mklImage->end()){
		_mklImage->erase(it);
	}

	return true;
}

MKLUpdates
MKLPageSubscriber::buildUpdates(const string& rowID, const DataPage::PageRow& prow)
{
	MKLUpdates ups;
	MKLPage::iterator it = _mklImage->find(rowID);
	if (it == _mklImage->end()){
		_mklImage->insert(MKLPage::value_type(rowID, prow));
		ups.push_back(rowAsDataMap(rowID, prow));	
	}else{
		DataPage::PageRow mklrow = (*it).second;
		compareRows(mklrow, prow, &ups, rowID);	
		_mklImage->erase(it);
		_mklImage->insert(MKLPage::value_type(rowID, prow));
	}

	return ups;
}

void
MKLPageSubscriber::compareRows(const DataPage::PageRow& mklrow, const DataPage::PageRow& prow, MKLUpdates* ups, const string& rowID)
{
	int mkllength = mklrow.size();
	int plength = prow.size();
	int col = -1;
	char col_ch[5];
	string rowVal;
	int j = 0;
	string csi = string(1, (char)0x1B) + string(1, (char)0x5B);
	do {
	//for(int j = 0; j < mkllength && j < plength; j++)
		DataPage::PageGliph pg = prow[j];
		DataPage::PageGliph mklg = mklrow[j];
		//cout << "compare "<<pg.data <<" != "<< mklg.data<< " on "<<j<<endl;
		if (pg.data != mklg.data){
			if (col == -1) col = j;
			rowVal += string(1, pg.data);
		}else{
			if (rowVal.size() != 0 && col != -1){
				sprintf(col_ch, "%i`", col);
				//cout << "Part Line "<<rowID<<" is updated ["<<rowVal<<"] col: "<<col<<endl;
				rowVal = csi + col_ch + rowVal;
				DataGraph::DataMap dmap;
				DataGraph::Variant v(DataGraph::SVAL, rowVal.c_str());
				dmap.insert(DataGraph::DataMap::value_type(rowID, v));
				ups->push_back(dmap);

			}
			rowVal.clear();
			col = -1;
		}
		j++;
	}while (j <= mkllength && j <= plength);
}

void 
MKLPageSubscriber::publishPageUpdate(DataPage::Update& up)
{
	buildImageUpdate(up);
	MKLFeedSession *feed = dynamic_cast<MKLFeedSession *>(_mySession);
	if (feed && !_locked){
		MKLPage::iterator it = _pageImage->begin();
		for(it; it != _pageImage->end(); it++){
			MKLUpdates ups = buildUpdates((*it).first, (*it).second);
			for (int i = 0; i < ups.size(); i++){
				feed->publishInsertRequest(ups[i], _security_id);
			}
		}
		for (int i = 0; i < _timersDone.size(); i++){
			delete _timersDone[i];
		}
		_timersDone.clear();
	}
}

void
MKLPageSubscriber::addRefresh()
{

	string tmstmp = Scheduler::getCurrentTime(CANPX_TIME_FORMAT); 
	Logger::getLogger()->log(Logger::TYPE_INFO, "MKLPageSubscriber::addRefresh at %s, %s, %i credit, %i rate, %i ceiling ",tmstmp.c_str(), _security_id.c_str(),  _credit, _rate, _ceiling);

	if (_isRefreshRequired == false){
		_isRefreshRequired = true;
		_mklImage->clear();
		Logger::getLogger()->log(Logger::TYPE_INFO, "MKLPageSubscriber::addRefresh requested");
	}
	//publishRefresh();	
}

void
MKLPageSubscriber::addWaitTime(int sec)
{
	string tmstmp = Scheduler::getCurrentTime(CANPX_TIME_FORMAT); 
	Logger::getLogger()->log(Logger::TYPE_INFO, "MKLPageSubscriber::addWaitTime at %s, %s, %i sec, %i credit, %i rate, %i ceiling ",tmstmp.c_str(), _security_id.c_str(), sec, _credit, _rate, _ceiling);
	/******/
	if (!_locked){
		_locked = true;
		MKLPageSubscriber::PageTimer *my_timer = new MKLPageSubscriber::PageTimer(this, _credit, _rate, _ceiling);
		my_timer->initTimer();
		_timers.push_back(my_timer);
		// deleted from _timersDone;
		tmstmp = Scheduler::getCurrentTime(CANPX_TIME_FORMAT); 
		Logger::getLogger()->log(Logger::TYPE_INFO, "MKLPageSubscriber::addWaitTime locked at %s, %s, %i sec ",tmstmp.c_str(), _security_id.c_str(), sec);
	} else {
		Logger::getLogger()->log(Logger::TYPE_ERROR, "MKLPageSubscriber::addWaitTime %s, already locked",_security_id.c_str());
	}
	/******/

}
vector<DataPage::PageSlot>
MKLPageSubscriber::getMySlot()const
{
        return _my_slot;

}

void
MKLPageSubscriber::buildPageImage()
{
	if (_myPage){
		_pageImage->clear();
		for (int i = DataPage::MIN_ROW; i <= DataPage::MAX_ROW; i++){
			map<int, string>::iterator its = _rowMap.find(i);
			if ( its != _rowMap.end()){
				string rowID = (*its).second;
				DataPage::PageRow r = _myPage->rowAsPageRow(i);
				_pageImage->insert(MKLPage::value_type(rowID, r));
			}
		}
		_isRefreshRequired = false;
	}
}

void
MKLPageSubscriber::buildImageUpdate(DataPage::Update &up)
{
	// apply update to the Image 
	if (_myPage){
		map<int, string>::iterator its = _rowMap.find(up.row);
		if ( its != _rowMap.end()){
			string rowID = (*its).second;
			MKLPage::iterator itprow = _pageImage->find(rowID);
			if (itprow != _pageImage->end()){
				_pageImage->erase(itprow);
				DataPage::PageRow newrow = _myPage->rowAsPageRow(up.row);
				_pageImage->insert(MKLPage::value_type(rowID, newrow));
			} 
			
		}
	}
}

DataGraph::DataMap
MKLPageSubscriber::rowAsDataMap(const string& rowID, const DataPage::PageRow& pagerow)
{
	DataGraph::DataMap dmap;
	string rowVal;
	for (int i = 0; i < pagerow.size(); i++){
		DataPage::PageGliph g = pagerow[i];
		rowVal += string(1, g.data);
	}	
	//cout << "Line "<<rowID<<" is updated ["<<rowVal<<"]"<<endl;
	DataGraph::Variant v(DataGraph::SVAL, rowVal.c_str());
	dmap.insert(DataGraph::DataMap::value_type(rowID, v));
	return dmap;
}

DataGraph::DataMap
MKLPageSubscriber::pageRowAsDataMap(const string& rowID, const DataPage::PageRow& pagerow)
{
	DataGraph::DataMap dmap;
	bool done = false;
	bool isLine = false;
	string rowVal;
	for (int i = 0; i < pagerow.size(); i++){
		DataPage::PageGliph g = pagerow[i];
		if (g.data){
			rowVal += string(1, g.data);
			isLine = true; //starts from 0, it's Line 
		}else{
			done = true;
			break;
		}
	} 
	if (isLine){
		//cout << "Line "<<rowID<<" is updated ["<<rowVal<<"]"<<endl;
		DataGraph::Variant v(DataGraph::SVAL, rowVal.c_str());
		dmap.insert(DataGraph::DataMap::value_type(rowID, v));
	} else {
		//partial update
		// <CSI>15`ABCDEFGHIJ --> offset to 15 col
		//col's are 0 based
		string rowVal;
		done = false;
		int col = -1;
		for (int i = 0; i < pagerow.size(); i++){
			DataPage::PageGliph g = pagerow[i];
			if (g.data){
				rowVal += string(1, g.data);
				done = true;
				if (col == -1)
					col = i;
			}else{
				if (done) break;
			}
		} 

		char col_ch[5];
		sprintf(col_ch, "%i`", col);
		string csi = string(1, (char)0x1B) + string(1, (char)0x5B);
		//cout << "Part Line "<<rowID<<" is updated ["<<rowVal<<"] col: "<<col<<endl;
		rowVal = csi + col_ch + rowVal;
		DataGraph::Variant v(DataGraph::SVAL, rowVal.c_str());
		dmap.insert(DataGraph::DataMap::value_type(rowID, v));

	}

	return dmap;
}

void
MKLPageSubscriber::setMySlot(const string& slot, const string& id)
{
	// slot "r1:0 & r2:14 & c1:0 & c2:40"

	vector<string> chanks;
	DataPage::PageSlot *myslot = new DataPage::PageSlot();
	Formatter::split(slot, "&", chanks);
	if (chanks.size() < 4){
		Logger::getLogger()->log(Logger::TYPE_ERROR, "MKLPageSubscriber::setMySlot slot is not sufficient: %i chanks. The default Slot is used", chanks.size());
		myslot->row1 = DataPage::MIN_ROW;
		myslot->row2 = DataPage::MAX_ROW;
		myslot->column1 = DataPage::MIN_COLUMN;
		myslot->column2 = DataPage::MAX_COLUMN;
	} else {
		for ( int i = 0; i < chanks.size(); i++){
			string ch1 = Formatter::trim(chanks[i]);
			vector<string>  v2;
			Formatter::split(ch1, ":", v2);
			if (v2.size() == 2){
				if (v2[0] == "r1")
					myslot->row1 = (int)Formatter::ATOF(v2[1].c_str());
					else if (v2[0] == "r2")
						myslot->row2 = (int)Formatter::ATOF(v2[1].c_str());
					else if (v2[0] == "c1")
						myslot->column1 = (int)Formatter::ATOF(v2[1].c_str());
					else if (v2[0] == "c2")
						myslot->column2 = (int)Formatter::ATOF(v2[1].c_str());
					else
						Logger::getLogger()->log(Logger::TYPE_ERROR, "MKLPageSubscriber::setMySlot() unrecognized %s. [r1|r2|c1|c2]", v2[0].c_str());
			}
		}
	}
        _my_slot.push_back(*myslot);
	_mySlotMap[myslot] = id;

}

bool
MKLPageSubscriber::setRowTmpl()
{
	bool ret = false;
	char buf[80];
	for (int j = 1; j <= 25; j++){
		sprintf(buf, "%s.ROW80_%i", _row_tmpl_str.c_str(), j);	
		string row = ConfigMap::getConfig()->getValue(buf);
		if (row.size()){
			_row_tmpl.push_back(row); 
			if (!ret) ret = true;
		}

	}

	for (int i = 0; i < _row_tmpl.size(); i++){
		_rowMap.insert(map<int, string>::value_type(i, _row_tmpl[i]));
	}
	return ret;
}

void
MKLPageSubscriber::setDefaultRowMap()
{
	// ONLY 25 LINES
	//_rowMap.insert(map<int, string>::value_type(0, _row_tmpl[0]));
	_rowMap.insert(map<int, string>::value_type(0, ROW80_2));
	_rowMap.insert(map<int, string>::value_type(1, ROW80_3));
	_rowMap.insert(map<int, string>::value_type(2, ROW80_4));
	_rowMap.insert(map<int, string>::value_type(3, ROW80_5));
	_rowMap.insert(map<int, string>::value_type(4, ROW80_6));
	_rowMap.insert(map<int, string>::value_type(5, ROW80_7));
	_rowMap.insert(map<int, string>::value_type(6, ROW80_8));
	_rowMap.insert(map<int, string>::value_type(7, ROW80_9));
	_rowMap.insert(map<int, string>::value_type(8, ROW80_10));
	_rowMap.insert(map<int, string>::value_type(9, ROW80_11));
	_rowMap.insert(map<int, string>::value_type(10, ROW80_12));
	_rowMap.insert(map<int, string>::value_type(11, ROW80_13));
	_rowMap.insert(map<int, string>::value_type(12, ROW80_14));
	_rowMap.insert(map<int, string>::value_type(13, ROW80_15));
	_rowMap.insert(map<int, string>::value_type(14, ROW80_16));
	_rowMap.insert(map<int, string>::value_type(15, ROW80_17));
	_rowMap.insert(map<int, string>::value_type(16, ROW80_18));
	_rowMap.insert(map<int, string>::value_type(17, ROW80_19));
	_rowMap.insert(map<int, string>::value_type(18, ROW80_20));
	_rowMap.insert(map<int, string>::value_type(19, ROW80_21));
	_rowMap.insert(map<int, string>::value_type(20, ROW80_22));
	_rowMap.insert(map<int, string>::value_type(21, ROW80_23));
	_rowMap.insert(map<int, string>::value_type(22, ROW80_24));
	_rowMap.insert(map<int, string>::value_type(23, ROW80_25));
	//_rowMap.insert(map<int, string>::value_type(24, ROW80_25));

}

MKLPageSubscriber::PageTimer::PageTimer(MKLPageSubscriber *s, int credit, int rate, int ceiling)
{
	_ps = s;
	_t_rate = rate;
	_t_credit = credit;
	_t_ceiling = ceiling;

}

void
MKLPageSubscriber::PageTimer::initTimer()
{

	addTimer(_ps->_throttle_delay * 1000, 1, this);
}

void 
MKLPageSubscriber::PageTimer::onTimer(TimerId id)
{
	// 2 updates per second, when done add to timersDone
	string tmstmp = Scheduler::getCurrentTime(CANPX_TIME_FORMAT); 

	MKLFeedSession *feed = dynamic_cast<MKLFeedSession *>(_ps->_mySession);
	MKLPage::iterator it = _ps->_pageImage->begin();
	int i = 0;
	static drop_loop = 15 * 60 / _ps->_throttle_delay; 
	//drop loop after 15 min if no msgs from the server
	int default_i = 1;
	int watermark = _ps->_credit - _ps->_watermark;
	MKLUpdates ups;
	Logger::getLogger()->log(Logger::TYPE_INFO, "MKLPageSubscriber::PageTimer::onTimer at %s for %s, credit %i, watermark %i, drop in %i",tmstmp.c_str(), (_ps->_security_id).c_str(), _ps->_credit, watermark, drop_loop);
	for(it; it != _ps->_pageImage->end(); it++){
		ups = _ps->buildUpdates((*it).first, (*it).second);
		MKLUpdates::iterator itu = ups.begin();
		while(itu != ups.end()){
			DataGraph::DataMap::const_iterator itr = (*itu).begin();
			for( itr; itr != (*itu).end(); itr++){
				string msg = DataGraph::serializeVariantValue((*itr).second);
				default_i = watermark - msg.size(); 
			}
			if (default_i <= 0) break;
			Logger::getLogger()->log(Logger::TYPE_INFO, "MKLPageSubscriber::onTimer publish %s, watermark %i, breach %i , updates %i",  (_ps->_security_id).c_str(), watermark, default_i, ups.size() );
			feed->publishInsertRequest((*itu), _ps->_security_id);
			itu = ups.erase(itu);
			i++;

		}
		if (default_i <= 0) break;
	}
	if ( default_i <= 0 && --drop_loop){
		addTimer (_ps->_throttle_delay * 1000, 1 , this);
	}else {
		_ps->_locked = false;
		drop_loop = 15 * 60 / _ps->_throttle_delay; 
		Logger::getLogger()->log(Logger::TYPE_INFO, "MKLPageSubscriber::onTimer unlock %s @ %s",  (_ps->_security_id).c_str(), tmstmp.c_str() );
		vector<MKLPageSubscriber::PageTimer *>::iterator itt = std::find((_ps->_timers).begin(), (_ps->_timers).end(), this);
		if ( itt != _ps->_timers.end()){
			_ps->_timers.erase(itt);
			_ps->_timersDone.push_back(this);
		}else{
			Logger::getLogger()->log(Logger::TYPE_ERROR, "MKLPageSubscriber::onTimer failed to erase timer %s",  (_ps->_security_id).c_str() );

		}
	}
}

