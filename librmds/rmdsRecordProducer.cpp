/************************************************************************
||
|| DATE:                $Date: 2006/06/19 21:32:58 $
|| SOURCE:              $Source: /repository/projects/Canpx/librmds/rmdsRecordProducer.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: rmdsRecordProducer.cpp,v 1.9 2006/06/19 21:32:58 mikhail Exp $
|| REVISION:		$Revision: 1.9 $
|| LOG:                 $Log: rmdsRecordProducer.cpp,v $
|| LOG:                 Revision 1.9  2006/06/19 21:32:58  mikhail
|| LOG:                 cannot read the set of fileds from the parser
|| LOG:
|| LOG:                 Revision 1.8  2006/06/04 11:34:16  nickz
|| LOG:                 Changed record producer from Unmanaged to managed publisher
|| LOG:
|| LOG:                 Revision 1.7  2006/05/02 15:10:16  nickz
|| LOG:                 testing
|| LOG:
|| LOG:                 Revision 1.6  2006/04/03 19:36:40  nickz
|| LOG:                 testing...
|| LOG:
|| LOG:                 Revision 1.5  2006/04/01 12:09:22  nickz
|| LOG:                 continue building and testing...
|| LOG:
|| LOG:                 Revision 1.4  2006/03/28 20:11:40  nickz
|| LOG:                 .
|| LOG:
|| LOG:                 Revision 1.3  2006/03/23 18:55:47  nickz
|| LOG:                 testing...
|| LOG:
|| LOG:                 Revision 1.2  2006/03/18 13:23:23  nickz
|| LOG:                 Added RMDS-based functionality
|| LOG:
|| LOG:                 Revision 1.1  2006/03/06 16:00:18  nickz
|| LOG:                 .
|| LOG:
|| LOG:
************************************************************************/

// System includes
#include <sstream>
#include <assert.h>

// Application includes
#include "rmdsRecordProducer.hpp"
#include "Logger.hpp"
#include "DataGraph.hpp"

using namespace std;
using namespace rfa::sessionLayer;
using namespace rfa::common;


// Reuters MarketFeed field separators
#define		RTR_FS	 '\x1C'
#define		RTR_GS	 '\x1D'
#define		RTR_RS	 '\x1E'
#define		RTR_US	 '\x1F'
#define		RTR_ESC	 '\x1B'



rmdsRecordProducer::rmdsRecordProducer(const string& name): rmdsManagedHandler(name)
{
}


rmdsRecordProducer::~rmdsRecordProducer()
{
}


/*****************************************************************************
 * notify
 * Called when update occurs within DataCache.
 *
 *****************************************************************************/
void
rmdsRecordProducer::notify(Datum *datum)
{
    // Publish out record
    DataGraph* data = dynamic_cast<DataGraph *>(datum);
    
    if (data)
    {
	// Iterate through data and build TibMsg
	TibMsg msg;

	// First grab the field list
	set<string> fset = data->getFieldSet();

	// Iterate through list to build output
	set<string>::iterator it = fset.begin();

	string  fieldname;
	DataGraph::Variant value;
	char buf[12];
	for (it; it != fset.end(); it++)
	{
	    fieldname = (*it);
	    value = data->getVariant(fieldname);

	    switch(value.type)
	    {
	    case DataGraph::SVAL:
		assert( value.data.sval != NULL );
		msg.Append(fieldname.c_str(), value.data.sval);
		break;

	    case DataGraph::DVAL:
		if (value.data.dval)
			sprintf(buf, "%.3f", value.data.dval);
		else
			sprintf(buf, "");
		//msg.Append(fieldname.c_str(), value.data.dval);
		msg.Append(fieldname.c_str(), buf);
		break;

	    case DataGraph::LVAL:
		msg.Append(fieldname.c_str(), value.data.lval);
		break;

	    default:
		Logger::getLogger()->log(Logger::TYPE_WARNING,
					 "rmdsRecordProducer::notify unknown Variant type %d", 
					 value.type);
	    }
	}

	// Publish out update/image
	publish(msg, data);
    }
}

void
rmdsRecordProducer::publish( TibMsg& msg, Datum* datum )
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

    if ( !tibMsg2MFbuffer(datum->item(), msg, buffer, type) )
    {
	Logger::getLogger()->log(Logger::TYPE_WARNING,
				 "rmdsPageProducer::publish failed.  Item: %s (Failed to convert to MF format)", 
				 datum->item().c_str());

	return;
    }
    
    m_marketDataManagedItemPub.setBuffer( buffer );
    m_marketDataManagedItemPub.setDataFormat(MarketDataEnums::Marketfeed);

    // Publish data
    try 
    {
	long id = m_pMarketDataManagedPublisher->publish(m_marketDataManagedItemPub);

	Logger::getLogger()->log(Logger::TYPE_INFO,
				 "rmdsRecordProducer::publish %d, item: %s (%s)", id,
				 datum->item().c_str(), m_serviceName.c_str());

    } 
    catch (rfa::common::Exception& e)
    {
	rfaException(e);
    }
}

bool 
rmdsRecordProducer::tibMsg2MFbuffer( const string& item, TibMsg& msg, Buffer& buffer,
				     MarketDataManagedItemPub::MarketDataMsgType type)
{
    static char dfield[512];
    ostringstream strStr;
    
    // Build the MF header
    switch (type)
    {
    	case MarketDataManagedItemPub::Image:
    	case MarketDataManagedItemPub::UnsolicitedImage:
	    strStr << RTR_FS << "340" << RTR_US << "XX" << RTR_GS << item.c_str() 
		   << RTR_US << RTR_US;
	    break;
	
    	case MarketDataManagedItemPub::Update:
	    strStr << RTR_FS << "316" << RTR_US << "XX" << RTR_GS << item.c_str() << RTR_US;
	    break;
	
    	default:
	    Logger::getLogger()->log(Logger::TYPE_WARNING,
				     "rmdsRecordProducer::tibMsg2MFbuffer error.  Invalid Msg Type: %d encountered.",
				     type);
	    return(false);
    }
    
    // Iterate through the msg and build up fields based on the loaded data dictionary
    TibField field;
    TibMsg_mfeed* fid;
    //cout << "Print Item "<<item <<" { "<<endl;
    msg.PrintTib(); 
    //cout << "} "<<endl;
    for (field.First(&msg); field.status == TIBMSG_OK; field.Next())
    {
	// nested message fields shouldn't exist (ignore if so)
	if (field.Type() != TIBMSG_MESSAGE)
	{
	    fid = msg.Fname2Mfeed(field.Name());

	    if ( fid != NULL ){
		int x = msg.Mfeed2Fid(fid);
		field.Convert(dfield, sizeof(dfield));
		
		strStr << RTR_RS << x << RTR_US << dfield;
	    } else {
	    	Logger::getLogger()->log(Logger::TYPE_WARNING,
				     "rmdsRecordProducer::tibMsg2MFbuffer error.  msg.Fname2Mfeed failed for %s.", field.Name());
	    }
	}
    }
    
    strStr << RTR_FS;
    
    // Assign string
    const string& str = strStr.str();
    
    // Assign RFA buffer
    buffer.setFrom( (unsigned char* )str.c_str(), str.length(), str.length() );
    
    return(true);
}

