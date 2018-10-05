
/************************************************************************
||
|| DATE:                $Date: 2006/03/30 21:54:41 $
|| SOURCE:              $Source: /export/home/tomcat/mlsrc/espeedII/RCS/ESInstrument.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: ESInstrument.hpp,v 1.4 2006/03/30 21:54:41 tomcat Exp $
|| REVISION:    $Revision: 1.4 $
|| LOG:                 $Log: ESInstrument.hpp,v $
|| LOG:                 Revision 1.4  2006/03/30 21:54:41  tomcat
|| LOG:                 handle Prov price normilized
|| LOG:
|| LOG:                 Revision 1.3  2006/02/17 22:12:15  tomcat
|| LOG:                 normalization of the Prov Labels
|| LOG:
|| LOG:                 Revision 1.2  2006/02/08 16:38:27  mikhailt
|| LOG:                 normalization for PROV
|| LOG:
|| LOG:                 Revision 1.1  2005/11/17 21:57:58  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#ifndef ESINSTRUMENT_HPP
#define ESINSTRUMENT_HPP

#include <map>
#include <string>


using std::map;
using std::string;

#include "DataGraph.hpp"

class ESInstrument
{
	public:
		struct MInstr {
        		MInstr();
        		~MInstr(){};
        		double lastBid;
        		string initTime;
        		int counter;
		};
        	typedef map<string, ESInstrument::MInstr> MMAP;
	public:
		ESInstrument(const string& label);
		virtual ~ESInstrument();

		virtual bool activate();
	
		virtual void setValByIndex(int val, unsigned short index);	
		virtual void setValByIndex(double val, unsigned short index);	
		virtual void setValByIndex(long val, unsigned short index);	
		virtual void setValByIndex(const string& val, unsigned short index);	
		virtual void update();
	protected:
		bool setAskYield(double val){return true;};
		bool setBidYield(double val){return true;};
		bool setAskSize(double val){return true;};
		bool setBidSize(double val){return true;};
		bool setAsk(double val){return true;};
		bool setBid(double val){return true;};
		bool setBidChg(double val);
		bool setClose(double val);
		bool setTick(double val);
		bool setMatDate(long val){return true;};
		bool setTradeTone(int t);
		bool setTradeYield(int t);
		bool setSeqCount(); //on update
		bool setTimeStamp(); //on update

		bool setSubtype(const string& t);
		void normPriceProv();
		string normalizeLabel(const string& label); 
		void buildName(const string& label);
		static string getField(unsigned short index);
		static void setFieldMap();

		//History
	        //bool addHistory(const char* instr, const MInstr& hist);
        	//MInstr getHistory(const char* instr);
		//MMAP _history;
		MInstr _hist;
	private:
		DataGraph* _data;
		string _label;
		string _name;
		string _inst_code;
		unsigned short _currentState;
		bool _isProv;
		bool _isNorm;
		//static map<unsigned short, string> _fieldMap;
};
#endif //ESINSTRUMENT_HPP
