/////////////////////////////////////////
//	IOCallback.h
//////////////////////////////////////////
/************************************************************************
||
|| DATE:                $Date: 2004/01/08 20:51:46 $
|| SOURCE:              $Source: /export/home/tomcat/mlsrc/libapps/RCS/IOCallback.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: IOCallback.hpp,v 1.1 2004/01/08 20:51:46 tomcat Exp $
|| REVISION:    $Revision: 1.1 $
|| LOG:                 $Log: IOCallback.hpp,v $
|| LOG:                 Revision 1.1  2004/01/08 20:51:46  tomcat
|| LOG:                 Initial revision
|| LOG:
************************************************************************/


#ifndef IOCALLBACK_H
#define IOCALLBACK_H
#include "BasicComponent.hpp"
#include <assert.h>

typedef XtInputId       InputId;

class IOCallback 
{
	public: 
		static enum { 	IO_NONE = 0x0000, 
				IO_READ = 0x0001, 
				IO_WRITE = 0x0010, 
				IO_EXCEPT = 0x0100 
			} IOMask;

		IOCallback();
		virtual ~IOCallback();

		virtual void newIO(int ifd, int mask, IOCallback *);

	public:

		InputId		_read; 	//read handler
		InputId		_write; //write handler
		InputId		_except; //ecxept handler
		int		_fd; // file fd
		int		_mask; // mask of desired event
		int		_readyMask; //events that have been since the last time

	protected:

		static void regIOCallback(XtPointer client_data, int *fid, InputId *id); //input callback

		virtual void removeIO(int fid);

		//Calbacks
                // callback on IO event
                virtual void onRead(int fid, InputId id){
			cout <<"IOCallback::onRead for fd="<<fid<<endl ;
		};

                virtual void onWrite(int fid, InputId id){
			cout <<"IOCallback::onWrite for fd="<<fid<<endl ;
		};

                virtual void onExcept(int fid, InputId id){
			cout <<"IOCallback::onExcept for fd="<<fid<<endl ;
		};

			
};
#endif //IOCALLBACK_H

