///////////////////////////////////////////////////////////////
// IOCallback.C
//////////////////////////////////////////////////////////////
/************************************************************************
||
|| DATE:                $Date: 2004/01/08 20:51:46 $
|| SOURCE:              $Source: /export/home/tomcat/mlsrc/libapps/RCS/IOCallback.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: IOCallback.cpp,v 1.1 2004/01/08 20:51:46 tomcat Exp $
|| REVISION:    $Revision: 1.1 $
|| LOG:                 $Log: IOCallback.cpp,v $
|| LOG:                 Revision 1.1  2004/01/08 20:51:46  tomcat
|| LOG:                 Initial revision
|| LOG:
************************************************************************/


#include "IOCallback.hpp"
#include "Application.hpp"

IOCallback::IOCallback( ):
		_read(0),
		_write(0),
		_except(0),
		_fd(-1),
		_readyMask(IOCallback::IO_NONE),
		_mask(IOCallback::IO_NONE)
{

}

IOCallback::~IOCallback()
{
	removeIO(this->_fd);
}

void
IOCallback::newIO(int ifd, int mask, IOCallback * cb)
{
	if (cb == NULL)
		return;

	this->_fd = ifd;

	if(mask & IOCallback::IO_READ){	
		if (!(this->_mask & IOCallback::IO_READ)){
			this->_read = XtAppAddInput(theApplication->appContext(), ifd, 
			(XtPointer) XtInputReadMask, cb->regIOCallback, (XtPointer)this);
		}
	} else {
		if (this->_mask & IOCallback::IO_READ) {
			XtRemoveInput(this->_read);
		}
	}

	if(mask & IOCallback::IO_WRITE){	
		if (!(this->_mask & IOCallback::IO_WRITE)){
			this->_write = XtAppAddInput(theApplication->appContext(), ifd, 
			(XtPointer) XtInputWriteMask, cb->regIOCallback, (XtPointer)this);
		}
	} else {
		if (this->_mask & IOCallback::IO_WRITE) {
			XtRemoveInput(this->_write);
		}
	}

	if(mask & IOCallback::IO_EXCEPT){	
		if (!(this->_mask & IOCallback::IO_EXCEPT)){
			this->_except = XtAppAddInput(theApplication->appContext(), ifd, 
			(XtPointer) XtInputExceptMask, cb->regIOCallback, (XtPointer)this);
		}
	} else {
		if (this->_mask & IOCallback::IO_EXCEPT) {
			XtRemoveInput(this->_except);
		}
	}

	this->_mask = mask;
}

void
IOCallback::regIOCallback(XtPointer client_data, int *fid, InputId *id)
{
	//check what kind event happend

	IOCallback *cb = (IOCallback *)client_data;
	int mask = IOCallback::IO_NONE;

	if (cb){
		if (*id == cb->_read)
			mask = IOCallback::IO_READ;
		else if (*id == cb->_write)
			mask = IOCallback::IO_WRITE;
		else if (*id == cb->_except)
			mask = IOCallback::IO_EXCEPT;

		//Ignoring unwanted or duplicated events

		if (!(cb->_mask & mask) || (cb->_readyMask & mask))
			return;
	
		//this interesting event

		cb->_readyMask |= mask;
		mask = cb->_readyMask & cb->_mask; // if status has been changed we use ADD
		cb->_readyMask = IOCallback::IO_NONE;
		switch (mask){
			case IOCallback::IO_READ:
 				cb->onRead(*fid, *id);
				break;
			case IOCallback::IO_WRITE:
				cb->onWrite(*fid, *id);
				break;
			case IOCallback::IO_EXCEPT:
				cb->onExcept(*fid, *id);
				break;
			default:
				cout<<"Unknown Event captured mask="<<mask<<endl;
		}
	}
}

void
IOCallback::removeIO(int fid)
{
	if (fid == this->_fd){
		//cout<<"Before Input _read ="<<this->_read<<endl;
		//cout<<"Before Input _write ="<<this->_write<<endl;
		if (this->_mask & IOCallback::IO_READ){
			if (this->_read){
				XtRemoveInput(this->_read);
				this->_read = 0;
				this->_mask ^= IOCallback::IO_READ;
			}
		}
		if (this->_mask & IOCallback::IO_WRITE){
			if (this->_write){
				XtRemoveInput(this->_write);
				this->_write = 0;
				this->_mask ^= IOCallback::IO_WRITE;
			}
		}
		if (this->_mask & IOCallback::IO_EXCEPT){
			if (this->_except) {
				XtRemoveInput(this->_except);
				this->_except = 0;
				this->_mask ^= IOCallback::IO_EXCEPT;
			}
		}
		//cout<<"After Input _read ="<<this->_read<<endl;
		//cout<<"After Input _write ="<<this->_write<<endl;
	}	
}




