
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

#ifndef MKLMSG_HPP
#define MKLMSG_HPP

#include <vector>
using namespace std;

class MKLMsg
{
	public:
		MKLMsg():p_msg(NULL), _msgID(0L){};
		MKLMsg (const char* buf, int len);
		MKLMsg (const vector<char>& v);
		virtual ~MKLMsg();
		MKLMsg(const MKLMsg& msg);

		MKLMsg& operator=(const MKLMsg& msg);
		virtual void push_back(int i);
		virtual void push_back(char ch);
		virtual void push_back(const string& str);
		virtual void push_back(unsigned char ch);
		virtual void push_back(unsigned short s);
		virtual void push_back(const MKLMsg& msg);

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
			
		virtual void MKLMsg::printMsg() const;

	protected:
		char * p_msg;
		vector<char> v_msg;
		unsigned long _msgID;

};

#endif //MKLMSG_HPP
