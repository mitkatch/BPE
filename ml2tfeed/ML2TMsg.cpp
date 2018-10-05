
/************************************************************************
||
|| DATE:                $Date: 2006/04/07 19:31:50 $
|| SOURCE:              $Source: /export/home/tomcat/mlsrc/ml2tfeed/RCS/ML2TMsg.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: ML2TMsg.cpp,v 1.1 2006/04/07 19:31:50 mikhailt Exp $
|| REVISION:    $Revision: 1.1 $
|| LOG:                 $Log: ML2TMsg.cpp,v $
|| LOG:                 Revision 1.1  2006/04/07 19:31:50  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#include <stdlib.h>
#include <strings.h>
#include "ML2TMsg.hpp"
#include "Logger.hpp"

ML2TMsg::~ML2TMsg()
{
	if (p_msg != NULL){
		delete [] p_msg;	
		p_msg = NULL;
	}
}

ML2TMsg::ML2TMsg(const ML2TMsg& msg)
{
	if (msg.p_msg != NULL){
		delete [] msg.p_msg;	
	}	
	p_msg = NULL;
	v_msg = msg.v_msg;
}

void
ML2TMsg::push_back(int i)
{
	char buf[25];
	bzero(buf, sizeof(buf));	
	sprintf(buf, "%i", i);
	push_back(buf);

}


void
ML2TMsg::push_back(char ch)
{
	v_msg.push_back(ch);

}

void
ML2TMsg::push_back(DataPage::PageRow& row)
{
	vector<DataPage::PageGliph>::iterator it2 = row.begin();
	for(it2; it2!= row.end(); it2++){
		v_msg.push_back((*it2).data);
	}
}

void
ML2TMsg::push_back(const string& str)
{
	for (int i = 0; i < str.size(); i++){

		v_msg.push_back(str[i]);

	}
}

void
ML2TMsg::push_back(unsigned char ch)
{
		v_msg.push_back((char)ch);
}

void
ML2TMsg::push_back(const ML2TMsg& msg)
{
	vector<char> vmsg = msg.getMsgAsVector();
	for (int i = 0; i < vmsg.size(); i++){
		v_msg.push_back(vmsg[i]);
	}


}

char *
ML2TMsg::getMsg()
{
	if (p_msg != NULL){
		delete [] p_msg;
		p_msg = NULL;
	}

	//p_msg = (unsigned char *)malloc(v_msg.size());
	p_msg = new char[v_msg.size()];
	char * frwd = p_msg;
	for (int i = 0; i < v_msg.size(); i++){
		*frwd = v_msg[i];
		frwd++;		
	}

	return p_msg;
}
