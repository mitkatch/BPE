
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

#include <stdlib.h>
#include <strings.h>
#include "MKLMsg.hpp"
#include "Logger.hpp"

MKLMsg::~MKLMsg()
{
	if (p_msg != NULL){
		delete [] p_msg;	
		p_msg = NULL;
	}
}

MKLMsg::MKLMsg(const char* buf, int len):p_msg(NULL), _msgID(0L)
{
	for (int i = 0; i < len; i++){
		push_back(buf[i]);
	}
}

MKLMsg::MKLMsg(const vector<char>& v):p_msg(NULL), _msgID(0L)
{
	v_msg = v;
}

MKLMsg::MKLMsg(const MKLMsg& msg)
{
	if (msg.p_msg != NULL){
		delete [] msg.p_msg;	
	}	
	p_msg = NULL;
	v_msg = msg.v_msg;
	_msgID = msg._msgID;
}

MKLMsg&
MKLMsg::operator=(const MKLMsg& msg)
{
	if (msg.p_msg != NULL){
		delete [] msg.p_msg;	
	}	
	p_msg = NULL;
	v_msg = msg.v_msg;
	_msgID = msg._msgID;

	return *this;
}

void
MKLMsg::push_back(int i)
{
	char buf[25];
	bzero(buf, sizeof(buf));	
	sprintf(buf, "%i", i);
	push_back(buf);

}


void
MKLMsg::push_back(char ch)
{
	v_msg.push_back(ch);

}

void
MKLMsg::push_back(const string& str)
{
	for (int i = 0; i < str.size(); i++){

		v_msg.push_back(str[i]);

	}
}

void
MKLMsg::push_back(unsigned char ch)
{
	v_msg.push_back((char)ch);
}

void
MKLMsg::push_back(unsigned short s)
{
	char ch[2] = {0x00, 0x00};	
	ch[0] = s >> 8;
	ch[1] = s;
	v_msg.push_back(ch[0]);
	v_msg.push_back(ch[1]);
}

void
MKLMsg::push_back(const MKLMsg& msg)
{
	vector<char> vmsg = msg.getMsgAsVector();
	for (int i = 0; i < vmsg.size(); i++){
		v_msg.push_back(vmsg[i]);
	}


}

char *
MKLMsg::getMsg()
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

void
MKLMsg::printMsg() const
{
	printf("MKLMsg::printMsg size %i {\n", v_msg.size());
	for (int i = 0; i < v_msg.size(); i++){
		if (isprint(v_msg[i]))
			printf("%c", v_msg[i]);
		else
			printf("{%X}", v_msg[i]);
		if ((i != 0) && !(i%80))
			printf("\n");
	}
	printf( "} MKLMsg::printMsg() \n");

}
