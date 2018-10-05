
/************************************************************************
||
|| DATE:                $Date: 2005/10/04 16:51:28 $
|| SOURCE:              $Source: /repository/projects/Canpx/canpx/CanpxAlgorithm.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: CanpxAlgorithm.cpp,v 1.2 2005/10/04 16:51:28 mikhailt Exp $
|| REVISION:    $Revision: 1.2 $
|| LOG:                 $Log: CanpxAlgorithm.cpp,v $
|| LOG:                 Revision 1.2  2005/10/04 16:51:28  mikhailt
|| LOG:                 prov added
|| LOG:
|| LOG:                 Revision 1.1  2005/06/24 16:21:53  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#include "DataGraph.hpp"
#include "CanpxAlgorithm.hpp"
#include "CanpxInstrument.hpp"
#include "Formatter.hpp"

/**/
bool
CanpxAlgorithm::operator< (const CanpxAlgorithm* ca)const
{
	cout <<"Man, dont call me!!!!"<<endl;
	return false;
}
/**/

bool
BestBidGOC::operator<(const BestBidGOC& ca)const
{
	CanpxInstrument *inst1 = ca.getInstrument();
	bool ret = true;
	string sb1, sb2;
	if (inst1->getDataGraph()){
		sb1 = _i->getDataGraph()->getValue(_BID_);
		sb2 = inst1->getDataGraph()->getValue(_BID_);

		if (sb1 == sb2){
			string addr2 = inst1->getDataGraph()->getValue(_ML_GRAPH_ADDRESS_);
			string addr1 = _i->getDataGraph()->getValue(_ML_GRAPH_ADDRESS_);
			ret = addr1 > addr2;
		}else{
			ret =  Formatter::ATOF(sb1.c_str()) > Formatter::ATOF(sb2.c_str());	
		}
	}else{
		string addr2 = inst1->getDataGraph()->getValue(_ML_GRAPH_ADDRESS_);
		string addr1 = _i->getDataGraph()->getValue(_ML_GRAPH_ADDRESS_);
		//cout << "Failed compare "<<addr1<<" and "<<addr2<<endl;
	}
	return ret;
}

bool
BestAskGOC::operator<(const BestAskGOC& ca)const
{
	CanpxInstrument *inst1 = ca.getInstrument();
	bool ret = true;
	string sb1, sb2;
	if (inst1->getDataGraph()){
		sb2 = inst1->getDataGraph()->getValue(_ASK_);
		sb1 = _i->getDataGraph()->getValue(_ASK_);

		if (sb1 == sb2){
			string addr2 = inst1->getDataGraph()->getValue(_ML_GRAPH_ADDRESS_);
			string addr1 = _i->getDataGraph()->getValue(_ML_GRAPH_ADDRESS_);
			ret = addr1 < addr2;
		}else{
			ret =  Formatter::ATOF(sb1.c_str()) < Formatter::ATOF(sb2.c_str());	
		}
	}
	return ret;
}

bool
BestBidTBill::operator<(const BestBidTBill& ca)const
{
	CanpxInstrument *inst1 = ca.getInstrument();
	bool ret = true;
	string sb1, sb2;
	if (inst1->getDataGraph()){
		sb1 = _i->getDataGraph()->getValue(_BID_);
		sb2 = inst1->getDataGraph()->getValue(_BID_);

		if (sb1 == sb2){
			string addr2 = inst1->getDataGraph()->getValue(_ML_GRAPH_ADDRESS_);
			string addr1 = _i->getDataGraph()->getValue(_ML_GRAPH_ADDRESS_);
			ret = addr1 < addr2;
		}else{
			ret =  Formatter::ATOF(sb1.c_str()) < Formatter::ATOF(sb2.c_str());	
		}
	}else{
		string addr2 = inst1->getDataGraph()->getValue(_ML_GRAPH_ADDRESS_);
		string addr1 = _i->getDataGraph()->getValue(_ML_GRAPH_ADDRESS_);
		//cout << "Failed compare "<<addr1<<" and "<<addr2<<endl;
	}
	return ret;
}

bool
BestAskTBill::operator<(const BestAskTBill& ca)const
{
	CanpxInstrument *inst1 = ca.getInstrument();
	bool ret = true;
	string sb1, sb2;
	if (inst1->getDataGraph()){
		sb2 = inst1->getDataGraph()->getValue(_ASK_);
		sb1 = _i->getDataGraph()->getValue(_ASK_);

		if (sb1 == sb2){
			string addr2 = inst1->getDataGraph()->getValue(_ML_GRAPH_ADDRESS_);
			string addr1 = _i->getDataGraph()->getValue(_ML_GRAPH_ADDRESS_);
			ret = addr1 > addr2;
		}else{
			ret =  Formatter::ATOF(sb1.c_str()) > Formatter::ATOF(sb2.c_str());	
		}
	}
	return ret;
}

bool
BestBidProv::operator<(const BestBidProv& ca)const
{
	CanpxInstrument *inst1 = ca.getInstrument();
	bool ret = true;
	string sb1, sb2;
	if (inst1->getDataGraph()){
		sb1 = _i->getDataGraph()->getValue(_BID_);
		sb2 = inst1->getDataGraph()->getValue(_BID_);

		if (sb1 == sb2){
			string addr2 = inst1->getDataGraph()->getValue(_ML_GRAPH_ADDRESS_);
			string addr1 = _i->getDataGraph()->getValue(_ML_GRAPH_ADDRESS_);
			ret = addr1 < addr2;
		}else{
			ret =  Formatter::ATOF(sb1.c_str()) < Formatter::ATOF(sb2.c_str());	
		}
	}else{
		string addr2 = inst1->getDataGraph()->getValue(_ML_GRAPH_ADDRESS_);
		string addr1 = _i->getDataGraph()->getValue(_ML_GRAPH_ADDRESS_);
		//cout << "Failed compare "<<addr1<<" and "<<addr2<<endl;
	}
	return ret;
}

bool
BestAskProv::operator<(const BestAskProv& ca)const
{
	CanpxInstrument *inst1 = ca.getInstrument();
	bool ret = true;
	string sb1, sb2;
	if (inst1->getDataGraph()){
		sb2 = inst1->getDataGraph()->getValue(_ASK_);
		sb1 = _i->getDataGraph()->getValue(_ASK_);

		if (sb1 == sb2){
			string addr2 = inst1->getDataGraph()->getValue(_ML_GRAPH_ADDRESS_);
			string addr1 = _i->getDataGraph()->getValue(_ML_GRAPH_ADDRESS_);
			ret = addr1 > addr2;
		}else{
			ret =  Formatter::ATOF(sb1.c_str()) > Formatter::ATOF(sb2.c_str());	
		}
	}
	return ret;
}
