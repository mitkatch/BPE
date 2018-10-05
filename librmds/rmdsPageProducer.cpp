/************************************************************************
||
|| DATE:                $Date: 2006/06/19 21:32:58 $
|| SOURCE:              $Source: /repository/projects/Canpx/librmds/rmdsPageProducer.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: rmdsPageProducer.cpp,v 1.10 2006/06/19 21:32:58 mikhail Exp $
|| REVISION:		$Revision: 1.10 $
|| LOG:                 $Log: rmdsPageProducer.cpp,v $
|| LOG:                 Revision 1.10  2006/06/19 21:32:58  mikhail
|| LOG:                 cannot read the set of fileds from the parser
|| LOG:
|| LOG:                 Revision 1.9  2006/06/04 11:34:15  nickz
|| LOG:                 Changed record producer from Unmanaged to managed publisher
|| LOG:
|| LOG:                 Revision 1.8  2006/05/02 15:10:15  nickz
|| LOG:                 testing
|| LOG:
|| LOG:                 Revision 1.7  2006/04/10 17:50:03  nickz
|| LOG:                 .
|| LOG:
|| LOG:                 Revision 1.6  2006/04/07 16:03:52  nickz
|| LOG:                 testing...
|| LOG:
|| LOG:                 Revision 1.5  2006/04/06 17:59:08  nickz
|| LOG:                 tested Page Publishing
|| LOG:
|| LOG:                 Revision 1.4  2006/04/04 20:03:08  nickz
|| LOG:                 verified addressed to be correct
|| LOG:
|| LOG:                 Revision 1.3  2006/04/01 12:09:21  nickz
|| LOG:                 continue building and testing...
|| LOG:
|| LOG:                 Revision 1.2  2006/03/18 13:23:21  nickz
|| LOG:                 Added RMDS-based functionality
|| LOG:
|| LOG:                 Revision 1.1  2006/03/06 16:00:17  nickz
|| LOG:                 .
|| LOG:
|| LOG:
************************************************************************/

// System includes
#include <assert.h>

// RFA includes
#include "AnsiPage/Page.h"

// Application includes
#include "rmdsPageProducer.hpp"
#include "Logger.hpp"
#include "DataPage.hpp"

using namespace rfa::sessionLayer;
using namespace rfa::common;
using namespace rfa::AnsiPage;

char	rmdsPageProducer::s_strUpdateBuf[4096];

rmdsPageProducer::rmdsPageProducer(const string& name): rmdsManagedHandler(name)
{
    // Default Page cell
    m_modelCell = new PageCell( ' ',                  //The character itself
				PageCell::USAscii,    //Graphic Set being used
				PageCell::plain,      //Cell style attribute
				PageCell::underline,  //Cell fade style attribute
				PageCell::green,      //Cell foreground color
				PageCell::black,      //Cell background color
				PageCell::black,      //Cell foreground fade color
				PageCell::red );      //Cell background fade color
}

rmdsPageProducer::~rmdsPageProducer()
{
}


/*****************************************************************************
 * updatePage
 * Called when update occurs within DataCache.
 *
 *****************************************************************************/
void
rmdsPageProducer::updatePage(Converter* cv)
{
    pthread_mutex_lock(&s_DataMutex);

    // Publish out page
    DataPage* node = cv->getDataPage();
    
    if (node != NULL)
    {
	// Create a page on the stack
	Page ansiPage(node->height(), node->width());
	
	const long MAX_UPDATES = 1000;
	
	//This array will be used to hold a list of updates
	static PageUpdate updateList[MAX_UPDATES];
	
	long numUpdates = 0;
	
	//Initialize page using the ANSI "ESC c" sequence
	char strInit[] = "\033c";
	ansiPage.decode( strInit,
			 sizeof( strInit ),
			 updateList,
			 MAX_UPDATES,
			 &numUpdates );
	
	// Determine which section within the DataPage requires attention
	short pageRow = cv->row();
	short rowCnt  = cv->rowCnt();
	short endCol  = cv->col() + (cv->value()).size();

	// Build up ANSI Page
	for (int i=0; i < rowCnt; i++)
	{
	    // Iterate through data and build update page
	    DataPage::PageRow row = node->rowAsPageRow(pageRow+i);
	    
	    for (int j = cv->col(); j < endCol && j <= DataPage::MAX_COLUMN +1 ; j++)
	    {
		m_modelCell->setStyle( toVideo(row[j].video) );
		m_modelCell->setChar( row[j].data );
		ansiPage.setPageCell( pageRow+i +1, j +1, *m_modelCell );
	    }
	    
	    updateList[i].setRow( pageRow+i +1 );
	    updateList[i].setBeginningColumn( cv->col()  +1);
	    updateList[i].setEndingColumn( endCol  +1);
	}

	//Now, let's create an ANSI encoded update string
	//based upon the current contents of the page image
	//and the updates currently in the update list.
	memset(s_strUpdateBuf, ' ', sizeof(s_strUpdateBuf));

	long nTextLength = 0;

	string page;

	//Encodes page and stores in strUpdateBuf
	bool more = true;
	while (more)
	{
	    more =  ansiPage.encode( true,
				     updateList,
				     rowCnt,
				     s_strUpdateBuf,
				     sizeof( s_strUpdateBuf ),
				     &nTextLength );
	    page += s_strUpdateBuf;
	}

	// Publish out update/image
	publish(page, node);
    }

    pthread_mutex_unlock(&s_DataMutex);
}


/******
PageCell::CellStyle
rmdsPageProducer::toVideo(int v)
{
    PageCell::CellStyle newV = PageCell::plain;
    
    switch(v)
    {
    case DataPage::V_BLINK:
	newV = PageCell::blink;
	break;
    case DataPage::V_UNDERLINE:
	newV = PageCell::underline;
	break;
    case DataPage::V_HIGHLIGHT:
	newV = PageCell::reverse;
	break;
    }	
    
    return newV;
}
****/

PageCell::CellStyle
rmdsPageProducer::toVideo(int v)
{
    PageCell::CellStyle newV = PageCell::plain;
    
    if (v & DataPage::V_BLINK)
	newV = PageCell::blink;
    if (v & DataPage::V_UNDERLINE)
	newV = PageCell::underline;
    if (v & DataPage::V_HIGHLIGHT)
	newV = PageCell::reverse;
    
    return newV;
}

void
rmdsPageProducer::publish( const string& ansiPage, Datum* datum )
{
    if ( !connected() )
    {
	Logger::getLogger()->log(Logger::TYPE_WARNING,
				 "rmdsRecordProducer::publish Attempting to publish when publisher not connected.");
	return;
    }

    m_marketDataManagedItemPub.setItemName(datum->item());

    // Set publishing attributes
    MarketDataManagedItemPub::MarketDataMsgType type;

    if ( datum->initial() )
    {
	type = MarketDataManagedItemPub::Image;
	datum->initial(false);
    }
    else
	type = MarketDataManagedItemPub::Update;

    m_marketDataManagedItemPub.setMarketDataMsgType( type );

    // Set the data
    Buffer buffer;
    
    // Build ANSI page message buffer
    buffer.setFrom( (unsigned char *)ansiPage.c_str(), 
		    ansiPage.length(), ansiPage.length() );
    
    m_marketDataManagedItemPub.setBuffer( buffer );
    m_marketDataManagedItemPub.setDataFormat(MarketDataEnums::ANSI_Page);
    
    // Publish data
    try 
    {
	long id = m_pMarketDataManagedPublisher->publish(m_marketDataManagedItemPub);

	Logger::getLogger()->log(Logger::TYPE_INFO,
				 "rmdsPageProducer::publish %d, item: %s (%s)", id,
				 datum->item().c_str(), m_serviceName.c_str());

    } 
    catch (rfa::common::Exception& e)
    {
	rfaException(e);
    }
}
