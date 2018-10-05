
/************************************************************************
||
|| DATE:                $Date: 2006/05/12 15:58:17 $
|| SOURCE:              $Source: /export/home/tomcat/mlsrc/ml2tfeed/RCS/ML2TMsg.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: ML2TMsg.hpp,v 1.2 2006/05/12 15:58:17 mikhailt Exp $
|| REVISION:    $Revision: 1.2 $
|| LOG:                 $Log: ML2TMsg.hpp,v $
|| LOG:                 Revision 1.2  2006/05/12 15:58:17  mikhailt
|| LOG:                 switch to BLOCKing mode
|| LOG:
|| LOG:                 Revision 1.1  2006/04/07 19:31:50  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#ifndef ML2TMSG_HPP
#define ML2TMSG_HPP

#include <vector>
#include "DataPage.hpp"
using namespace std;

class ML2TMsg
{
	public:
		ML2TMsg():p_msg(NULL), _msgID(0L){};
		virtual ~ML2TMsg();
		ML2TMsg(const ML2TMsg& msg);
		virtual void push_back(char ch);
		virtual void push_back(const string& str);
		virtual void push_back(unsigned char ch);
		virtual void push_back(DataPage::PageRow& row);
		virtual void push_back(const ML2TMsg& msg);

		virtual char* getMsg();

		virtual vector<char> getMsgAsVector()const{
			return v_msg;
		}

		virtual int size() const{
			return v_msg.size();
		}

		virtual void msgID(unsigned long id){
			_msgID = id;
		}

		virtual unsigned long msgID(){
			return _msgID;
		}
			
	protected:
		char * p_msg;
		vector<char> v_msg;
		unsigned long _msgID;

};

#endif //ML2TMSG_HPP
