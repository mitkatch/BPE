
/************************************************************************
||
|| DATE:                $Date: 2005/10/04 16:51:28 $
|| SOURCE:              $Source: /repository/projects/Canpx/canpx/CanpxAlgorithm.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: CanpxAlgorithm.hpp,v 1.2 2005/10/04 16:51:28 mikhailt Exp $
|| REVISION:    $Revision: 1.2 $
|| LOG:                 $Log: CanpxAlgorithm.hpp,v $
|| LOG:                 Revision 1.2  2005/10/04 16:51:28  mikhailt
|| LOG:                 prov added
|| LOG:
|| LOG:                 Revision 1.1  2005/06/24 16:21:53  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#ifndef CANPXALGORITHM_HPP
#define CANPXALGORITHM_HPP
#include <algorithm>

using std::binary_function;

class CanpxInstrument;

class CanpxAlgorithm
{
	public:
		CanpxAlgorithm(CanpxInstrument *inst):_i(inst){};
		virtual ~CanpxAlgorithm(){};
		virtual bool operator< (const CanpxAlgorithm* ca)const;// = 0;  
		virtual CanpxInstrument* getInstrument() const {
			return _i;
		}
	protected:
		CanpxInstrument* _i;
};

class BestBidGOC 
{
	public:
		BestBidGOC(CanpxInstrument *inst):_i(inst){};//:CanpxAlgorithm(inst){};
		virtual ~BestBidGOC(){};
		const char* name(){return "BestBidGOC";};
		virtual bool operator<(const BestBidGOC& ca)const;
		virtual CanpxInstrument* getInstrument() const {
			return _i;
		}
	protected:
		CanpxInstrument* _i;

};

class BestAskGOC 
{
	public:
		BestAskGOC(CanpxInstrument *inst):_i(inst){};//:CanpxAlgorithm(inst){};
		virtual ~BestAskGOC(){};
		const char* name(){return "BestAskGOC";};
		virtual bool operator<(const BestAskGOC& ca)const;
		virtual CanpxInstrument* getInstrument() const {
			return _i;
		}
	protected:
		CanpxInstrument* _i;

};

class BestBidTBill 
{
	public:
		BestBidTBill(CanpxInstrument *inst):_i(inst){};//:CanpxAlgorithm(inst){};
		virtual ~BestBidTBill(){};
		const char* name(){return "BestBidTBill";};
		virtual bool operator<(const BestBidTBill& ca)const;
		virtual CanpxInstrument* getInstrument() const {
			return _i;
		}
	protected:
		CanpxInstrument* _i;

};

class BestAskTBill 
{
	public:
		BestAskTBill(CanpxInstrument *inst):_i(inst){};//:CanpxAlgorithm(inst){};
		virtual ~BestAskTBill(){};
		const char* name(){return "BestAskTBill";};
		virtual bool operator<(const BestAskTBill& ca)const;
		virtual CanpxInstrument* getInstrument() const {
			return _i;
		}
	protected:
		CanpxInstrument* _i;

};

class BestBidProv 
{
	public:
		BestBidProv(CanpxInstrument *inst):_i(inst){};//:CanpxAlgorithm(inst){};
		virtual ~BestBidProv(){};
		const char* name(){return "BestBidProv";};
		virtual bool operator<(const BestBidProv& ca)const;
		virtual CanpxInstrument* getInstrument() const {
			return _i;
		}
	protected:
		CanpxInstrument* _i;

};

class BestAskProv 
{
	public:
		BestAskProv(CanpxInstrument *inst):_i(inst){};//:CanpxAlgorithm(inst){};
		virtual ~BestAskProv(){};
		const char* name(){return "BestAskProv";};
		virtual bool operator<(const BestAskProv& ca)const;
		virtual CanpxInstrument* getInstrument() const {
			return _i;
		}
	protected:
		CanpxInstrument* _i;

};


#endif //CANPXALGORITHM_HPP
