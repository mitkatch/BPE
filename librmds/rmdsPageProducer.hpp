/************************************************************************
||
|| DATE:                $Date: 2006/06/04 11:34:15 $
|| SOURCE:              $Source: /repository/projects/Canpx/librmds/rmdsPageProducer.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: rmdsPageProducer.hpp,v 1.7 2006/06/04 11:34:15 nickz Exp $
|| REVISION:		$Revision: 1.7 $
|| LOG:                 $Log: rmdsPageProducer.hpp,v $
|| LOG:                 Revision 1.7  2006/06/04 11:34:15  nickz
|| LOG:                 Changed record producer from Unmanaged to managed publisher
|| LOG:
|| LOG:                 Revision 1.6  2006/05/02 15:10:16  nickz
|| LOG:                 testing
|| LOG:
|| LOG:                 Revision 1.5  2006/04/07 16:03:52  nickz
|| LOG:                 testing...
|| LOG:
|| LOG:                 Revision 1.4  2006/04/06 17:59:08  nickz
|| LOG:                 tested Page Publishing
|| LOG:
|| LOG:                 Revision 1.3  2006/04/01 12:09:22  nickz
|| LOG:                 continue building and testing...
|| LOG:
|| LOG:                 Revision 1.2  2006/03/18 13:23:22  nickz
|| LOG:                 Added RMDS-based functionality
|| LOG:
|| LOG:                 Revision 1.1  2006/03/06 16:00:17  nickz
|| LOG:                 .
|| LOG:
|| LOG:
************************************************************************/

#ifndef _RMDS_PAGE_PRODUCER_HPP_
#define _RMDS_PAGE_PRODUCER_HPP_

// System includes
#include <string>

// RFA includes
#include "AnsiPage/Page.h"

// Application includes
#include "rmdsManagedHandler.hpp"
#include "Datum.hpp"
#include "Subscriber.hpp"
#include "Converter.hpp"


class rmdsPageProducer: public rmdsManagedHandler
{
public:	
    rmdsPageProducer(const string& name);
    virtual ~rmdsPageProducer();

public:
    // Update and publish ANSI page (Canpx Subscriber)
    void updatePage(Converter* cv);

private:
    rfa::AnsiPage::PageCell::CellStyle toVideo(int v);
    void publish( const std::string& ansiPage, Datum* datum );

private:
    rfa::AnsiPage::PageCell* 	m_modelCell;

    // Page buffer
    static char			s_strUpdateBuf[4096];
};


#endif // _RMDS_PAGE_PRODUCER_HPP_
