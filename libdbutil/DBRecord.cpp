
/************************************************************************
||
|| DATE:                $Date: 2004/08/26 16:58:12 $
|| SOURCE:              $Source: /repository/projects/Canpx/libdbutil/DBRecord.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: DBRecord.cpp,v 1.2 2004/08/26 16:58:12 mikhailt Exp $
|| REVISION:    $Revision: 1.2 $
|| LOG:                 $Log: DBRecord.cpp,v $
|| LOG:                 Revision 1.2  2004/08/26 16:58:12  mikhailt
|| LOG:                 use non-block connection
|| LOG:
|| LOG:                 Revision 1.1  2004/03/18 16:57:51  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/


#include "DBRecordSet.hpp"
#include "DBRecord.hpp"

DBRecord::DBRecord(DBRecordSet *rset):_myrset(rset)
{
}

DBRecord::~DBRecord()
{
	//cout<<"Delete DBRecord with fields "<<_vrec.size()<<endl;
	_vrec.clear();
}

DBRecord::DBRecord(const DBRecord& rec)
{
	_vrec = rec._vrec;
}

bool
DBRecord::addVal(string& val)
{
	_vrec.push_back(val);
	return true;
}

string
DBRecord::toString()
{
	string str;
	vector<string>::iterator it = _vrec.begin();
	for (it; it != _vrec.end(); it++){
		str.append((*it));
		str.append(", ");
	}
	int last_com = str.rfind(", ");

	if (last_com != string::npos)
		str.replace(last_com, 2, "");
 
	return str;	

}

string
DBRecord::toString(const string &delim)
{
	string str;
	vector<string>::iterator it = _vrec.begin();
	for (it; it != _vrec.end(); it++){
		str.append((*it));
		str.append(delim);
	}
	int last_com = str.rfind(delim);

	if (last_com != string::npos)
		str.replace(last_com, delim.size(), "");
 
	return str;	

}


void
DBRecord::toVector(vector<string> &vec)
{
	vec = _vrec; 

}

const char*
DBRecord::getField(const char * header)const
{
	if (_myrset && header){
		int n = _myrset->getFieldEnum(header);

		if (n >=0 && n < _vrec.size()){
			return _vrec[n].c_str();
		}
	}

	return NULL;
}
