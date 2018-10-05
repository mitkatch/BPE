
/************************************************************************
||
|| DATE:                $Date: 2006/04/07 19:31:50 $
|| SOURCE:              $Source: /export/home/tomcat/mlsrc/ml2tfeed/RCS/ML2TPageListener.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: ML2TPageListener.cpp,v 1.1 2006/04/07 19:31:50 mikhailt Exp $
|| REVISION:    $Revision: 1.1 $
|| LOG:                 $Log: ML2TPageListener.cpp,v $
|| LOG:                 Revision 1.1  2006/04/07 19:31:50  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#include "Logger.hpp"
#include "ML2TPageListener.hpp"

ML2TPageListener::ML2TPageListener(ML2TFeedSession* s, const URLNameSpace& ns): _mySession(s), _activated(false)
{
	//cout <<"Find and load ML2TPageListener from "<<ns.security_id<<endl;
	_security_id = ns.security_id;
	setMySlot(ns.slot, _security_id);
}

ML2TPageListener::~ML2TPageListener()
{
	//delete all slots from the map
	map<DataPage::PageSlot *, string>::iterator it = _mySlotMap.begin();
	for (it; it != _mySlotMap.end(); it++){
		delete (*it).first;
	}

}
void
ML2TPageListener::notify( Datum *datum)
{
	//the whole page got updated, we are not interested
	//cout<<"ML2TPageListener notified"<<endl;
}

void
ML2TPageListener::onData(DataPage *page, DataPage::PageSlot slot)
{
	if (_mySession == NULL) return;
	//for (int i = 0; i < _my_slot.size(); i++){
	//	if (_my_slot[i] >= slot){
			if (_activated == false){
				//cout << "onData::Image "<<page->address()<<"slot r1="<< slot.row1<<", r2="<<slot.row2<<", c1="<<slot.column1<<", c2="<<slot.column2<<endl;	
				//cout << "onData::mySlot "<<0<<"slot r1="<< _my_slot[0].row1<<", r2="<<_my_slot[0].row2<<", c1="<<_my_slot[0].column1<<", c2="<<_my_slot[0].column2<<endl;	
				_mySession->publishPageImage(page, slot,  _security_id);	
				_activated = true;
			}else{
	
				DataPage::Update upd = page->getLastUpdate();
				_mySession->publishPageUpdate(upd, _security_id);
				//cout <<"Update Published ["<<upd.data<<"]"<<endl;
			}
	//	}
	//}
	
}

vector<DataPage::PageSlot>
ML2TPageListener::getMySlot()const
{
	return _my_slot;

}

void
ML2TPageListener::setMySlot(const string& slot, const string& id)
{
	// slot "r1:0 & r2:14 & c1:0 & c2:40"

	vector<string> chanks;
	DataPage::PageSlot *myslot = new DataPage::PageSlot();
	Formatter::split(slot, "&", chanks);
	if (chanks.size() < 4){
		Logger::getLogger()->log(Logger::TYPE_ERROR, "ML2TPageListener::setMySlot() slot is not sufficient: %i chanks. The default Slot is used", chanks.size());
		myslot->row1 = DataPage::MIN_ROW;
		myslot->row2 = DataPage::MAX_ROW;
		myslot->column1 = DataPage::MIN_COLUMN;
		myslot->column2 = DataPage::MAX_COLUMN;

	}else{
		for ( int i = 0; i < chanks.size(); i++){
			string ch1 = Formatter::trim(chanks[i]);

			vector<string>  v2;
			Formatter::split(ch1, ":", v2);
			if (v2.size() == 2){
				//cout << "ML2TPageListener::setMySlot "<<v2[0]<<"["<<v2[1]<<"]"<<endl;
				if (v2[0] == "r1")
					myslot->row1 = (int)Formatter::ATOF(v2[1].c_str());
				else if (v2[0] == "r2")
					myslot->row2 = (int)Formatter::ATOF(v2[1].c_str());
				else if (v2[0] == "c1")
					myslot->column1 = (int)Formatter::ATOF(v2[1].c_str());
				else if (v2[0] == "c2")
					myslot->column2 = (int)Formatter::ATOF(v2[1].c_str());
				else
					Logger::getLogger()->log(Logger::TYPE_ERROR, "ML2TPageListener::setMySlot() unrecognized %s. [r1|r2|c1|c2]", v2[0].c_str());

			}
		}
	}
	_my_slot.push_back(*myslot);
	_mySlotMap[myslot] = id;

	/*********
	map<DataPage::PageSlot *, string>::iterator it = _mySlotMap.begin();
	for (it; it != _mySlotMap.end(); it++){
		_mySlotMap[myslot] = id;
	}else{
		Logger::getLogger()->log(Logger::TYPE_ERROR, "ML2TPageListener::setMySlot() duplicated SECURITY_ID found %s. ", id.c_str());
	}
	*********/
}
