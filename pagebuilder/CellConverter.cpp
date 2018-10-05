
/************************************************************************
||
|| DATE:                $Date: 2006/06/19 21:32:59 $
|| SOURCE:              $Source: /repository/projects/Canpx/pagebuilder/CellConverter.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: CellConverter.cpp,v 1.4 2006/06/19 21:32:59 mikhail Exp $
|| REVISION:    $Revision: 1.4 $
|| LOG:                 $Log: CellConverter.cpp,v $
|| LOG:                 Revision 1.4  2006/06/19 21:32:59  mikhail
|| LOG:                 cannot read the set of fileds from the parser
|| LOG:
|| LOG:                 Revision 1.3  2006/04/04 13:19:31  nickz
|| LOG:                 ported to rmds
|| LOG:
|| LOG:                 Revision 1.2  2005/09/21 17:49:55  mikhailt
|| LOG:                 activated only when loaded()
|| LOG:
|| LOG:                 Revision 1.1  2005/06/29 19:00:48  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

// System includes
#include <iostream>
#include <strstream>
#include <iomanip>

// Application includes
#include "PageConverter.hpp"
#include "Logger.hpp"
#include "CellConverter.hpp"
#include "RowConverter.hpp"
#include "DataPage.hpp"



CellConverter::CellConverter(const string &key, MsgGraph *rset, RowConverter* rc):
			_myRow(rc), _address(key), 
			_dynamic(false), _cFormat(NULL),
			_cVideo(NULL), _currentVideo(DataPage::V_NONE)
{
	setDataPage(_myRow->myPage()->getDataPage());

	if (rset){
		string val;
		string defVal = "N/A";
		rset->getAttributeValue("cLength@" + _address, 0, val);
		_cLength = (val.size() ? atoi(val.c_str()) : 0);

		if (rset->getAttributeValue("cX@" + _address, 0, val))
			_cX = (val.size() ? atoi(val.c_str()) : -1); //0 based

		if (rset->getAttributeValue("cY@" + _address, 0, val))
			_cY = (val.size() ? atoi(val.c_str()) : -1);

		if (rset->getAttributeValue("colNum@" + _address, 0, val))
			_colNum = (val.size() ? atoi(val.c_str()) : -1);

		if (rset->getAttributeValue("cAlignment@" + _address, 0, val))
			_cAlignment = (val.size() ? atoi(val.c_str()) : 1); //left's default
		val = "";
		rset->getAttributeValue("cVideo@" + _address, 0, val);
		//create video in any cases
		_cVideo = CVideoFactory::getVideo(val); 

		if (rset->getAttributeValue("cFormat@" + _address, 0, val)){
			_cFormatStr = val;
			if (!_cFormatStr.empty()){
				int par = _cFormatStr.find_first_of('@');
				if (par != string::npos){
					//split HANDLER_COLLECTED@TRADE_PRICE
					_formatParameter = _cFormatStr.substr(par + 1, _cFormatStr.size() - (par + 1)); 	
					 _cFormatStr =  _cFormatStr.substr(0, par);
				}
				_cFormat = CFormatFactory::getFormat(_cFormatStr);
			}
		}

		if (rset->getAttributeValue("cValue@" + _address, 0, val)){
			val = (val.size() ? val : defVal);
			setValue(val);
		}
	}

	//cout<<_address<<" "<<_cLength<<" ["<<_cX<<" "<<_cY<<"] "<<_colNum<<" "<<_cAlignment<<" "<<_cVideo->value()<<" " <<_cValue<<endl;

}

CellConverter::~CellConverter()
{
}

void
CellConverter::setValue(const string& val)
{
	// _cValue looks like rec://ML_BID@feedSource
	// rec://cFormat[BID|ASK]@feedSource
	string rec = "rec://";
	int rec_id = val.find(rec);
	if (rec_id != string::npos){
		_cValue = val;
		int at = _cValue.find_last_of("@");//, rec.size());
		string datumField = _cValue.substr(rec_id + rec.size(), at - (rec_id + rec.size())); 
		setDatumField(datumField);
		_dynamic = true;
		_datumAddress = _myRow->source();
	}else {
		_cVideo->video(NULL, this); //ugly set video
		update(val); //Static field
	} 


}

void
CellConverter::setDatumField(const string& field)
{
	// ML_BID
	// cFormat[BID|ASK]
	// using vector to keep order specified

	string fmtStart = "cFormat[";
	string fmtEnd = "]";

	int ffound1 = field.find(fmtStart);
	int ffound2 = field.find(fmtEnd, ffound1);
	if (ffound1 != string::npos && ffound2 != string::npos){
		string ss = field.substr(ffound1 + fmtStart.size(), ffound2 - (ffound1 + fmtStart.size()) );
		//cout<< "Fields found "<<ss<<endl;
		Formatter::split(ss, "|", _datumField);

	}else{
		//cout<< "Fields found "<<field<<endl;
		_datumField.push_back(field);

	}
}

void
CellConverter::update(DataGraph *data)
{
	//cout << "CellConverter::update "<<data->address()<<endl;
	string val;
	if (_cFormat){
		val = _cFormat->compile(data, this);
	} else {
		for (int i = 0; i < _datumField.size(); i++){
			val = data->getValue(_datumField[i]);
			//cout<<"CellConverter::update ["<<_datumField[i]<<"] = "<<val<<endl;

		}
	}
	_cVideo->video(data, this); //ugly set video
	this->update(val);
}

void
CellConverter::update(const string& val)
{
	if (_cValue != val || _cVideo->value() != _currentVideo){
		setDisplayValue(val);
		DataPage *dp = getDataPage();//_myRow->myPage()->getDataPage();
		if (dp){
			cout << "Update DataPage row="<<this->row()<<", col="<<this->col()<<", value="<<this->value()<<endl; 
			dp->update(this->row(), this->col(), this->value(), _cVideo->value());
			_myRow->myPage()->publishUpdate(this);
		}
		_cValue = val;
		_currentVideo = _cVideo->value();
	}
}

void
CellConverter::update(const string& val, int video)
{
	/***
	if (_cValue != val || _cVideo->value() != video){

	}
	***/
}

string
CellConverter::setDisplayValue(const string& val)
{
	strstream tmpstr;
	switch(_cAlignment){
		case 0:
			tmpstr.setf(ios_base::internal, ios_base::adjustfield);
			break;
		case 2:
			tmpstr.setf(ios_base::right, ios_base::adjustfield);
			break;
		default:
			tmpstr.setf(ios_base::left, ios_base::adjustfield);
	}
	tmpstr.width(_cLength);
	tmpstr.fill(' ');
	tmpstr<<val<<ends;
	string retstr = tmpstr.str();	
	if (retstr.size() > _cLength){
		_displayValue = retstr.substr(0, _cLength);
	} else {
		_displayValue = retstr;
	}
	return _displayValue;

}

int
CellConverter::row()
{
	//return _cY;
	return _myRow->rowNum();

}

void
CellConverter::setDispStream()
{

}
