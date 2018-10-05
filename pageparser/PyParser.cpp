
/************************************************************************
||
|| DATE:                $Date: 2006/04/20 19:06:52 $
|| SOURCE:              $Source: /repository/projects/Canpx/pageparser/PyParser.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: PyParser.cpp,v 1.5 2006/04/20 19:06:52 nickz Exp $
|| REVISION:    $Revision: 1.5 $
|| LOG:                 $Log: PyParser.cpp,v $
|| LOG:                 Revision 1.5  2006/04/20 19:06:52  nickz
|| LOG:                 .
|| LOG:
|| LOG:                 Revision 1.4  2006/03/23 19:30:17  nickz
|| LOG:                 testing...
|| LOG:
|| LOG:                 Revision 1.3  2004/12/03 18:45:08  mikhailt
|| LOG:                 handles HTTP POST
|| LOG:
|| LOG:                 Revision 1.2  2004/11/15 17:40:09  mikhailt
|| LOG:                 more-less stable
|| LOG:
|| LOG:                 Revision 1.1  2004/10/06 19:44:25  mikhailt
|| LOG:                 Initial revision
|| LOG:
|| LOG:                 Revision 1.1  2004/10/06 19:42:08  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/


#include "Logger.hpp"
#include "PyParser.hpp"
#include "ConfigMap.hpp"


PyParser::PyParser(const URLNameSpace& ns):_proxy(NULL)
{
    cout <<"Find and load PyParser from "<<ns.parser<<endl;
    _proxy = new PyProxy();
    _address = ns.parser;
    _parser_map = ns.parser_map;
    
    ConfigMap* config = ConfigMap::getConfig();
    
    _source = config->getValue("PAGEPARSER_NAME");
    
    if(_proxy->loadFrom(_address))
    {
	setMySlot();
    }
}

PyParser::~PyParser()
{
	if (_proxy){
		delete _proxy;
		_proxy = NULL;
	}
	//delete all slots from the map
	map<DataPage::PageSlot *, string>::iterator it = _mySlotMap.begin();
	for (it; it != _mySlotMap.end(); it++){
		delete (*it).first;

	}

}
void
PyParser::notify( Datum *datum)
{

	//cout<<"PyParser notified"<<endl;
}

void
PyParser::onData(DataPage *page, DataPage::PageSlot slot)
{
	if (page){
		string slot_str = page->slotAsString(slot);
		cout<<"PyParser="<<this<< " ,onData:"<<slot_str<<", for "<<page->address()<<endl;
		map<DataPage::PageSlot *, string>::iterator it = _mySlotMap.begin();
		for (it; it != _mySlotMap.end(); it++){
			DataPage::PageSlot *rs = (*it).first;
			if (*rs == slot){

				cout << "Run "<<(*it).second<<" with ["<<slot_str<<"]"<<endl;
				vector<string> in, out;

				in.push_back(_source);
				in.push_back(_myPyName);
				in.push_back(page->address());
				in.push_back(slot_str); 
				DataPage::serializeSlot(in, slot);
				if(!_proxy->runFunctionAsTupleString((*it).second.c_str(), in, out)){
					Logger::getLogger()->log(Logger::TYPE_ERROR, "PyParser::onData() failed %s for %s", (*it).second.c_str(), slot_str.c_str());
				}
				break;
			}
		}
	}

}

void
PyParser::buildHtmlAttr(map<int, string>& htmlAttr, const DataPage::PageSlot& slot)
{

	//cout << "PyParser::buildHtmlAttr slot "<<slot.row1<<", ";
	//cout <<slot.row2<<", "<<slot.column1<<", "<<slot.column2<<endl;
	map<DataPage::PageSlot *, string>::iterator it = _myHtmlAttr.begin();
	for (it; it != _myHtmlAttr.end(); it++){

		DataPage::PageSlot *mySlot = (*it).first;
		//cout << "Processing register slot "<<mySlot->row1<<", ";
		//cout <<mySlot->row2<<", "<<mySlot->column1<<", "<<mySlot->column2<<endl;
		if (slot.row1 == mySlot->row1 && slot.row2 == mySlot->row2){
			for (int i = mySlot->column1; i <= mySlot->column2; i++){
				htmlAttr[i] = (*it).second;
			}

		}


	}
}


vector<DataPage::PageSlot>
PyParser::getMySlot()const
{
	return _my_slot;

}

void
PyParser::setMySlot()
{
	if (_proxy){
		vector<string> vin, vout;
		vin.push_back(_parser_map);
		Logger::getLogger()->log(Logger::TYPE_INFO, "PyParser::setMySlot() load pyLoadMyMap %s , for %p", _parser_map.c_str(), this);
		if(!_proxy->runFunctionAsTupleString("pyLoadMyMap", vin, vout)){
			Logger::getLogger()->log(Logger::TYPE_ERROR, "PyParser::setMySlot() failed pyLoadMyMap");
			return;
		}

		vector<long> lout = 0;
		if (vout.size() == 1)
			_myPyName = vout[0];
		if(!_proxy->runFunctionAsTupleString("pyGetMySlotNum", vout, lout)){
			Logger::getLogger()->log(Logger::TYPE_ERROR, "PyParser::setMySlot() failed pyGetMySlotNum");
			return;
		}

		cout << "Found slots "<<lout[0]<<endl;
		char buf[5]; 
		for (int j = 0; j  < lout[0]; j ++){
			vector<string> in;
			in.push_back(_myPyName);
			sprintf(buf, "%d", j);
			in.push_back(string(buf));
			vector<long> out;
			if(_proxy->runFunctionAsTupleString("pyGetSlot", in, out)){
				if (out.size() == 4){
					DataPage::PageSlot *slot = new DataPage::PageSlot();
					vector <string> svin;
					svin.push_back(_myPyName);
					slot->row1 = out[0];
					slot->row2 = out[1];
					slot->column1 = out[2];
					slot->column2 = out[3];
					DataPage::serializeSlot(svin, *slot);
					_my_slot.push_back(*slot);
					vector<string> svec;
					if(_proxy->runFunctionAsTupleString("pyGetParserBySlot", svin, svec)){
						if(svec.size() == 2){
							cout <<" Found parser "<<svec[0]<<endl;
							_mySlotMap[slot] = svec[0];
							_myHtmlAttr[slot] = svec[1];
						}
					} else {
						Logger::getLogger()->log(Logger::TYPE_ERROR, "PyParser::setMySlot() failed pyGetParserBySlot");

					}
				} else {
					Logger::getLogger()->log(Logger::TYPE_ERROR, "PyParser::setMySlot() failed pyGetSlot returns corrupted slot size != 4");
					break;
				}
			
			} else {
				Logger::getLogger()->log(Logger::TYPE_ERROR, "PyParser::setMySlot() failed pyGetSlot");

			}
		}

	}
}
