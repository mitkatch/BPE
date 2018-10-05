/****************************************************************************
 *
 * eSpeed, Inc.
 * 
 *
 * This system and/or its use is protected under one or more of the following 
 * United States patents:  5,905,974; 6,560,580.  All rights reserved.  Other 
 * patents pending 
 * 
 ***************************************************************************/

#if !defined(ESPD_TYPES_H)
#define ESPD_TYPES_H

/*
 * Include system types file (for time_t)
 */
#include <sys/types.h>

#if defined(__cplusplus)
extern "C" {
#endif


/*
 * CFETI_DATE: Date representation of local date. Can handle beyond year 2038.
 * Format CCYYMMDD where CC=century, YY=year (0-99), MM=month (1-12), DD=day (1-31)
 */
typedef unsigned int CFETI_DATE;

/*
 * CFETI_TIME: Time representation of local time. Can handle beyond year 2038.
 * HHMMSSCC where HH=hour (0-23), MM=minute (0-59), SS=seconds (0-59), CC=1/100 seconds (0-99). 
 */
typedef unsigned int CFETI_TIME;

/*
 * CFETI_RC: Return codes returned by CFETI routines
 */
typedef int CFETI_RC;

/*
 * CFETI_CMD: Commands issued by CFETI
 */
typedef unsigned int CFETI_CMD;

/*
 * CFETI_CMD_STATUS: Status associated with given CFETI commands
 */
typedef struct CFETI_CMD_STATUS_DESC CFETI_CMD_STATUS_DESC;
struct CFETI_CMD_STATUS_DESC {
	int cmdStatus;
	const char *statusText;
};
typedef CFETI_CMD_STATUS_DESC* CFETI_CMD_STATUS;

/*
 * CFETI_PREF: System preferences
 */
typedef unsigned int CFETI_PREF;

/*
 * CFETI_CMDPREF: Command preferences
 */
typedef unsigned int CFETI_CMDPREF;

/*
 * CFETI_SESSION_ID: Session identifier
 */
typedef const char* CFETI_SESSION_ID;

/*
 * CFETI_TRADE_SESS_ID: Trading System Session identifier
 */
typedef unsigned int CFETI_TRADE_SESS_ID;

/*
 * CFETI_TRADING_SYSTEM: Trading system
 * CFETI_TRADING_SUBSYSTEM: Trading sub-system
 */
typedef unsigned int CFETI_TRADING_SYSTEM;
typedef unsigned int CFETI_TRADING_SUBSYSTEM;

/*
 * CFETI_TRADING_SYS: Trading system definition
 */
typedef struct CFETI_TRADING_SYS_DESC CFETI_TRADING_SYS_DESC;
struct CFETI_TRADING_SYS_DESC {
	CFETI_TRADING_SYSTEM tsId;
	const char*          tsDescription;
};
typedef CFETI_TRADING_SYS_DESC* CFETI_TRADING_SYS;

/*
 * CFETI_USER_TYPE: User type
 */
typedef unsigned int CFETI_USER_TYPE;

/*
 * CFETI_TRADE_CONTROL_FLAGS: Trade control flags
 */
typedef unsigned int CFETI_TRADE_CONTROL_FLAGS;

/*
 * Trade settings
 */
struct CFETI_TRADE_SETTINGS_DESC {
	unsigned int retainRule;
	unsigned int priorityFollowRule;
};
typedef struct CFETI_TRADE_SETTINGS_DESC CFETI_TRADE_SETTINGS_DESC;

/*
 * CFETI_UD: User data
 */
typedef void* CFETI_UD;

/*
 * CFETI_CMDDATA: Generic command data
 */
typedef void *CFETI_CMDDATA;


struct CFETI_LOGIN_INFO {
	CFETI_SESSION_ID  sessionId;
	CFETI_TRADING_SYS *ts;
    const char*       szConnectionMode;
	const char*       szUserId;             /**< eSpeed user identity */
};
typedef struct CFETI_LOGIN_INFO CFETI_LOGIN_INFO;


/**
 * CFETI_API_CONNECTION_INFO_DESC: Connection Info for this API build
 * A list of id strings for available connection modes (socket types)
 */
struct CFETI_API_CONNECTION_INFO_DESC
{
	unsigned int numConnectionModes;  /**< Number of connection modes available. */
	char** connectionModeNames; /**< Id strings for available connection modes */ 
	char** connectionModeDescs; /**< Description strings for available connection modes */ 
};
typedef struct CFETI_API_CONNECTION_INFO_DESC CFETI_API_CONNECTION_INFO_DESC;
typedef CFETI_API_CONNECTION_INFO_DESC* CFETI_API_CONNECTION_INFO;

/*
 * CFETI_CONNECT_INFO: Trading system connection command data - session id
 *                     trading system description
 */
typedef struct CFETI_CONNECT_INFO CFETI_CONNECT_INFO;
struct CFETI_CONNECT_INFO {
	CFETI_TRADE_SESS_ID         sessionId;
	CFETI_TRADING_SYS_DESC      tradingSystem;
	CFETI_TRADE_CONTROL_FLAGS   tradeFlags;
	CFETI_TRADE_CONTROL_FLAGS   tradeFlags2;
	CFETI_TRADE_SETTINGS_DESC   tradeSettings;
};

/*
 * CFETI_TRADE_ID: Trading system trade identifier
 */
typedef const char* CFETI_TRADE_ID;

/*
 * CFETI_INSTRUMENT: Instrument description
 */
typedef const char * CFETI_INSTRUMENT;

/*
 * CFETI_PRICECODE: instrument price code
 */
typedef unsigned int CFETI_PRICECODE;

/*
 * CFETI_PRICE: Price of instrument
 */
typedef double CFETI_PRICE;

/*
 * CFETI_SIZE: Size of market or order
 */
typedef double CFETI_SIZE;

/*
 * CFETI_ID: Market or Order identifier
 */
typedef unsigned int CFETI_ID;

/*
 * CFETI_COUNTERPARTY_NAME: Name of counterparty in a trade
 */
typedef const char* CFETI_COUNTERPARTY_NAME;

/*
 * CFETI_COUNTERPARTY_STATE: Counterparty state
 */
typedef unsigned int CFETI_COUNTERPARTY_STATE;

/*
 * CFETI_MARKET: Market description
 * (NB. tradeId is unspecified unless market has been executed)
 */
typedef unsigned char CFETI_MARKET_SIDE;
typedef struct CFETI_MARKET_DESC CFETI_MARKET_DESC;
struct CFETI_MARKET_DESC {
	CFETI_INSTRUMENT            tradeInstrument;
	CFETI_PRICE                 price;
	CFETI_SIZE                  size;
	CFETI_MARKET_SIDE           side;
	CFETI_PREF                  preferences;          /**< 32 bit mask for market preferences */
	CFETI_PREF                  preferences2;		  /**< 32 bit mask for additional market preferences */
	CFETI_TRADE_ID              tradeId;
	CFETI_SIZE                  tradeSize;
	time_t                      tradeTime;
	CFETI_ID                    id;
	CFETI_TRADING_SUBSYSTEM     subsystem;
	void*                       userData;
	unsigned short              userDataSize;
	void*                       appUserData;
	unsigned short              appUserDataSize;
	const char*                 shortCode;
	CFETI_ID                    requestId;
	CFETI_TRADING_SYSTEM        tsId;
	unsigned int                priceImprovement;
	unsigned int                orderInfoType;  
	void*                       orderInfo;       
	CFETI_PRICE                 altPrice1;
	CFETI_PRICE                 altPrice2;
	unsigned int                basketId;
	unsigned int                basketActions;
	const char*                 userName;
	const char*                 requestorId;
	const char*                 originatorId;
	time_t                      creationTime; /**< Market creation time (responses & notifications only) */
	const char*					allocationInfo; /**< User specified allocation instructions (free text) */
	CFETI_PRICE                 executionPrice; /**< Execution price */
	unsigned int                timeOffset; /**< No. of minutes for which order is valid when pref is GOOD_UNTIL_TIME */
	const CFETI_TRADE_SETTINGS_DESC* tradeSettings; /**< Trade settings */
	CFETI_SIZE                  reserveMinSize; /**< Min clip size for reserve order */
	CFETI_SIZE                  reserveMaxSize; /**< Max clip size for reserve order */
	CFETI_SIZE                  reserveInitialSize; /**< Initial clip size for reserve order */
	const char*                 tradeReference; /**< Trade reference common to all legs of a trade */
};
typedef CFETI_MARKET_DESC* CFETI_MARKET;

/*
 * CFETI_ORDER: Order description
 * (NB. tradeId is unspecified unless order is executed or trade is confirmed)
 */
struct CFETI_INSTRUMENT_DATA_DESC;
struct CFETI_PI_BENEFIT_DESC;
typedef unsigned char CFETI_ORDER_INDICATOR;
typedef unsigned char CFETI_TRADE_SIDE;
typedef CFETI_PREF CFETI_ORDER_PREF;
typedef struct CFETI_ORDER_DESC CFETI_ORDER_DESC;
struct CFETI_ORDER_DESC {
	CFETI_INSTRUMENT            tradeInstrument;
	CFETI_PRICE                 price;
	CFETI_SIZE                  size;
	CFETI_ORDER_INDICATOR       indicator;
	CFETI_PREF                  preferences;          /**< 32 bit mask for order preferences */
	CFETI_PREF                  preferences2;		  /**< 32 bit mask for additional order preferences */
	CFETI_TRADE_ID              tradeId;
	CFETI_SIZE                  tradeSize;
	time_t                      tradeTime;
	CFETI_TRADE_SIDE            tradeSide;
	CFETI_PRICE                 tradePrice;
	time_t                      tradeSettlement;
	unsigned int                settlementDate; /**< CCYYMMDD settlement date */
	const char*                 tradeReference;		   /**< Trade reference common to all legs of a trade */
	unsigned short              tradeConfirmOperation; /**< Enumerated trade confirm operation */ 
	unsigned int                legId;				   /**< Trade leg number for trade confirms  consisting of multiple legs */ 
	unsigned int                legCount;			   /**< The number of legs in this trade */
	unsigned int                recordVersion;		   /**< Trade confirmation version number */
	CFETI_ID                    id;
	CFETI_TRADING_SUBSYSTEM     subsystem;
	void*                       userData;
	unsigned short              userDataSize;
	void*                       appUserData;
	unsigned short              appUserDataSize;
	const char*                 userName;
	const char*                 shortCode;
	CFETI_PRICE                 toPrice; /**< Deprecated. Replaced by altPrice1 */
	CFETI_PRICE                 altPrice1;
	CFETI_PRICE                 altPrice2;
	CFETI_ID                    requestId;
	time_t                      tradeRepoEndDate; /**< Deprecated. Replaced by endDate */
	time_t                      endDate;
	unsigned int                instrumentIdType;
	CFETI_INSTRUMENT            instrumentId;
	const char*                 tradeComments;
	unsigned int                tradeInfoType;
	const char*                 tradeInfo;
	unsigned int                settlementType;
	unsigned int                orderInfoType;  
	void*                       orderInfo;       
	CFETI_COUNTERPARTY_NAME     counterpartyName;
	unsigned int                counterpartyID;
	const char*                 contactName;
	const char*                 contactTelephoneNumber;
	unsigned int                rejectionId;
	CFETI_TRADING_SYSTEM        tsId;
	const char*                 settlementMethod;
	double                      brokerage;	
	time_t                      paymentDate;
	unsigned int                instProperties;
	unsigned int                tradeProperties;
	unsigned int                priceImprovement;
	unsigned int                checkoutPermissions;
	const char*                 reserved3;        /**< Reserved for future feature */
	unsigned int                basketId;
	unsigned int                basketActions;
	const char*                 requestorId;
	const char*                 originatorId;
	const CFETI_INSTRUMENT_DATA_DESC*	instrumentData;
	const char*	                clearerTradeId;
	const CFETI_PI_BENEFIT_DESC* pPIBenefit; /**< PI Benefit Data (trade confirms only) */
	time_t                      creationTime; /**< Order creation time (responses & notifications only) */
	const char*					allocationInfo; /**< User specified allocation instructions (free text) */
	unsigned int                dealStructure; /**< Enumerated deal structure */
	unsigned int                tradeType; /**< Enumerated trade type */
	unsigned int                pricingMethod; /**< Enumerated pricing method */
	CFETI_PRICE                 executionPrice; /**< Execution price */
    unsigned int                timeOffset; /**< No. of minutes for which order is valid when pref is GOOD_UNTIL_TIME */
	const CFETI_TRADE_SETTINGS_DESC* tradeSettings; /**< Trade settings */
	double						assetSwapLevel; /**< The corresponding asset swap level at the time of the trade */
	CFETI_SIZE                  reserveMinSize; /**< Min clip size for reserve order */
	CFETI_SIZE                  reserveMaxSize; /**< Max clip size for reserve order */
	CFETI_SIZE                  reserveInitialSize; /**< Initial clip size for reserve order */
	double                      yield; /**< Yield corresponding to screen price CFETI_ORDER_DESC::price (where available in trade confirmations) */
    const char*                 bicCode; /**< BIC code (where available in trade confirmations) */
};
typedef CFETI_ORDER_DESC* CFETI_ORDER;

/*
 * CFETI_BLOCK_TRADE_DESC: Block trade details
 */
typedef struct CFETI_BLOCK_TRADE_DESC CFETI_BLOCK_TRADE_DESC;
struct CFETI_BLOCK_TRADE_DESC {
	time_t      executionTime;
	const char* buyShortCode;
	const char* buyClearingMember;
	const char* buyCTICode;
	const char* sellShortCode;
	const char* sellClearingMember;
	const char* sellCTICode;
};
typedef CFETI_BLOCK_TRADE_DESC* CFETI_BLOCK_TRADE;

/*
 * CFETI_FIELD_VALUE: Value of a market data field
 */ 
typedef union CFETI_FIELD_VALUE CFETI_FIELD_VALUE;
union CFETI_FIELD_VALUE {
	char int8;
	unsigned char byte;
	short int16;
	unsigned short uint16;
	int int32;
	unsigned int uint32;
	double decimal;
	time_t dateTime;
	struct {
		unsigned short len;
		const char *bp;
	} buffer;
	struct {
		unsigned short len;
		unsigned short fields;
		const char *bp;
	} bytestream;

	const char *string;
};

/*
 * CFETI_FIELD_DESC: Description of a market data field
 */
typedef struct CFETI_FIELD_DESC CFETI_FIELD_DESC;
struct CFETI_FIELD_DESC {
	unsigned short    fieldId;
	unsigned char     fieldType;
	CFETI_FIELD_VALUE fieldValue;
};
typedef CFETI_FIELD_DESC* CFETI_FIELD;

/*
 * CFETI_INSTRUMENT_DATA : Market data
 */
typedef struct CFETI_INSTRUMENT_DATA_DESC CFETI_INSTRUMENT_DATA_DESC;
struct CFETI_INSTRUMENT_DATA_DESC {
	CFETI_INSTRUMENT         instName;
	CFETI_TRADING_SYSTEM     tsId;
	unsigned short           numFields;
	CFETI_FIELD              fieldTable;
};
typedef CFETI_INSTRUMENT_DATA_DESC* CFETI_INSTRUMENT_DATA;

/*
 * CFETI_SUBMIT_INSTRUMENT_DATA: For submitting new market data
 */
typedef struct CFETI_SUBMIT_INSTRUMENT_DATA_DESC CFETI_SUBMIT_INSTRUMENT_DATA_DESC;
struct CFETI_SUBMIT_INSTRUMENT_DATA_DESC {
	CFETI_INSTRUMENT_DATA_DESC  instrumentData;
	unsigned int                id;
};
typedef CFETI_SUBMIT_INSTRUMENT_DATA_DESC* CFETI_SET_INSTRUMENT_DATA;

/*
 * CFETI_EXT_PROPERTY_DESC: Preference struct for exchanges
 */
typedef struct CFETI_EXT_PROPERTY_DESC CFETI_EXT_PROPERTY_DESC;
struct CFETI_EXT_PROPERTY_DESC {
	unsigned int propertyId;
	unsigned int propertyType;
	CFETI_FIELD_VALUE propertyValue;
};
typedef CFETI_EXT_PROPERTY_DESC* CFETI_EXT_PROPERTY;

/*
 * CFETI_EXT_PROPERTY_ORDERINFO_DESC: Order info struct for exchanges
 */
typedef struct CFETI_EXT_PROPERTY_ORDERINFO_DESC CFETI_EXT_PROPERTY_ORDERINFO_DESC;
struct CFETI_EXT_PROPERTY_ORDERINFO_DESC {
	unsigned int numProperties;
	CFETI_EXT_PROPERTY propertyList;
};
typedef CFETI_EXT_PROPERTY_ORDERINFO_DESC* CFETI_EXT_PROPERTY_ORDERINFO;

/*
 * CFETI_QUERY_TYPE : Query type
 */
typedef unsigned char CFETI_QUERY_TYPE;

/*
 * CFETI_INSTRUMENT_TYPE : Query instrument type
 */
typedef unsigned short CFETI_INSTRUMENT_TYPE;

/*
 * CFETI_INSTRUMENT_DESC : Query instrument details
 */
typedef struct CFETI_INSTRUMENT_DESC CFETI_INSTRUMENT_DESC;
struct CFETI_INSTRUMENT_DESC {
	CFETI_INSTRUMENT name;
	const char*      description;
	const char*      id;               /* e.g. CUSIP or ISIN */
	const char*      classification;
	const char*      componentId;
	unsigned int     properties; // default to 0
	unsigned int     sortRule;
};

/*
 * CFETI_INSTRUMENT_LIST_DESC : Instrument list description
 */
typedef struct CFETI_INSTRUMENT_LIST_DESC CFETI_INSTRUMENT_LIST_DESC;
struct CFETI_INSTRUMENT_LIST_DESC {
	CFETI_TRADING_SYSTEM     tsId;
	unsigned int             numInstruments;
	CFETI_INSTRUMENT_DESC   *instList;
};
typedef CFETI_INSTRUMENT_LIST_DESC* CFETI_INSTRUMENT_LIST;

/*
 * CFETI_INSTRUMENT_QUERY_DESC : Instrument query description
 */
typedef struct CFETI_INSTRUMENT_QUERY_DESC CFETI_INSTRUMENT_QUERY_DESC;
struct CFETI_INSTRUMENT_QUERY_DESC {
	CFETI_INSTRUMENT_TYPE    trdType;
	unsigned int             numIndicativeFields;
	CFETI_FIELD              indicativeFields;
};

/*
 * CFETI_TRADE_QUERY_DESC : Trade query description
 */
typedef struct CFETI_TRADE_QUERY_DESC CFETI_TRADE_QUERY_DESC;
struct CFETI_TRADE_QUERY_DESC {
	time_t                   startTime;
	time_t                   endTime;
	CFETI_TRADE_ID           tradeId;
};

/*
 * CFETI_TRADE_LE_QUERY_DESC : Trade query (legal entity) description
 */ 
typedef struct CFETI_TRADE_LE_QUERY_DESC CFETI_TRADE_LE_QUERY_DESC;
struct CFETI_TRADE_LE_QUERY_DESC {
	time_t                   startTime;
	time_t                   endTime;
	CFETI_TRADE_ID           tradeId;
	unsigned int             numUsers;
	char**                   userList;
};

/*
 *
 */
typedef struct CFETI_CALCULATION_QUERY_DESC CFETI_CALCULATION_QUERY_DESC;
struct CFETI_CALCULATION_QUERY_DESC {
	unsigned int             uiCalculationId;
	unsigned int             uiRequestId;
	unsigned int             uiNumInputFields;
	CFETI_FIELD              pInputs;
};

/*
 * CFETI_QUERY_DESC : Query description
 */
typedef struct CFETI_QUERY_DESC CFETI_QUERY_DESC;
struct CFETI_QUERY_DESC {
	CFETI_QUERY_TYPE         queryType;
	union {
		CFETI_TRADE_QUERY_DESC       trade;
		CFETI_INSTRUMENT_QUERY_DESC  instrument;
		CFETI_TRADE_LE_QUERY_DESC    tradeLE;
		CFETI_CALCULATION_QUERY_DESC calculation;
	} query;
	void*                    extend; /* Reserved for future use */
};

/*
 * CFETI_QUERY : Query
 */
typedef CFETI_QUERY_DESC* CFETI_QUERY;

/*
 * CFETI_INSTRUMENT_QUERY_RESULT : Instrument query result
 */
typedef struct CFETI_INSTRUMENT_QUERY_RESULT CFETI_INSTRUMENT_QUERY_RESULT;
struct CFETI_INSTRUMENT_QUERY_RESULT {
	unsigned int             numInstruments;
	CFETI_INSTRUMENT_DESC   *instrumentList;
};

/*
 * CFETI_TRADE_QUERY_RESULT & CFETI_TRADE_LE_QUEURY_RESULT : Trade query results
 */
typedef struct CFETI_TRADE_QUERY_RESULT CFETI_TRADE_QUERY_RESULT;
typedef struct CFETI_TRADE_QUERY_RESULT CFETI_TRADE_LE_QUERY_RESULT;
struct CFETI_TRADE_QUERY_RESULT {
	unsigned int             numOrders;
	CFETI_ORDER              orderList;
};

/*
 * CFETI_USERLIST_QUERY_RESULT : Userlist query result
 */
typedef struct CFETI_USERLIST_QUERY_RESULT CFETI_USERLIST_QUERY_RESULT;
struct CFETI_USERLIST_QUERY_RESULT {
	unsigned int             numUsers;
	char**                   userList;
};

/*
 * CFETI_COMMISSION_TABLE_ENTRY : Entry in commission table query result
 */
typedef struct CFETI_COMMISSION_TABLE_ENTRY CFETI_COMMISSION_TABLE_ENTRY;
struct CFETI_COMMISSION_TABLE_ENTRY {
	unsigned int type;         /* When-issue or Bill      */
	double yearsToMaturity;    /* Years to maturity       */
	double size;               /* Size                    */
	double adjustment;         /* Adjustment per 100 face */
};

/*
 * CFETI_COMMISSION_TABLE_QUERY_RESULT : Commission table query result
 */
typedef struct CFETI_COMMISSION_TABLE_QUERY_RESULT CFETI_COMMISSION_TABLE_QUERY_RESULT;
struct CFETI_COMMISSION_TABLE_QUERY_RESULT {
	unsigned int                    commissionType;
	double                          flatFee;
	unsigned int                    numFeeTableEntries;
	CFETI_COMMISSION_TABLE_ENTRY*   feeTable;
};

/*
 * CFETI_CALCULATION_QUERY_RESULT : Commission table query result
 */
typedef struct CFETI_CALCULATION_QUERY_RESULT CFETI_CALCULATION_QUERY_RESULT;
struct CFETI_CALCULATION_QUERY_RESULT {
	unsigned int                    uiNumOutputFields;
	CFETI_FIELD                     pOutputs;
};

/*
 * CFETI_QUERY_RESULT_DESC : Query result
 */
typedef struct CFETI_QUERY_RESULT_DESC CFETI_QUERY_RESULT_DESC;
struct CFETI_QUERY_RESULT_DESC {
	CFETI_QUERY_DESC         query;
	union {
		CFETI_TRADE_QUERY_RESULT               trade;
		CFETI_INSTRUMENT_QUERY_RESULT          instrument;
		CFETI_USERLIST_QUERY_RESULT            user;
		CFETI_TRADE_LE_QUERY_RESULT            tradeLE;
		CFETI_COMMISSION_TABLE_QUERY_RESULT    commissionTable;
		CFETI_CALCULATION_QUERY_RESULT         calculation;
	} result;
	void*                    extend; /* Reserved for future use */
};

/*
 * CFETI_QUERY_RESULT : Query result
 */
typedef CFETI_QUERY_RESULT_DESC* CFETI_QUERY_RESULT;

/*
 * CFETI_INSTRUMENTNOTIFY_DESC
 */
typedef struct CFETI_INSTRUMENTNOTIFY_DESC CFETI_INSTRUMENTNOTIFY_DESC;
struct CFETI_INSTRUMENTNOTIFY_DESC {
	CFETI_TRADING_SYSTEM        tsId;
	CFETI_INSTRUMENT_DESC*      instList;
	int                         numInstruments;
	unsigned int                notifyType;
	time_t                      sendTime;
	char**                      text;
	int                         textLines;
	double                      countdownTime;
};
typedef struct CFETI_INSTRUMENTNOTIFY_DESC* CFETI_INSTRUMENTNOTIFY;

/*
 * Bid/Offer lists
 */
typedef struct CFETI_PARTICIPANT_DESC CFETI_PARTICIPANT_DESC;
struct CFETI_PARTICIPANT_DESC {
    CFETI_TRADE_SESS_ID tsId;
    CFETI_SIZE size;
    CFETI_PRICECODE code;
    CFETI_PRICECODE code2;
    CFETI_COUNTERPARTY_STATE counterpartyState;
	unsigned int attributes; /**< reserved for future use */
	void* extendedInfo; /**< reserved for future use */
};
typedef CFETI_PARTICIPANT_DESC* CFETI_PARTICIPANT;

typedef struct CFETI_PARTICIPANT_LIST_DESC CFETI_PARTICIPANT_LIST_DESC;
struct CFETI_PARTICIPANT_LIST_DESC {
    unsigned int numParticipants;
    CFETI_PARTICIPANT participant;
};
typedef CFETI_PARTICIPANT_LIST_DESC* CFETI_PARTICIPANT_LIST;

/*
 * CFETI_ENERGY_TRADE_DESC: Energy trade details
 */
typedef struct CFETI_ENERGY_TRADE_DESC CFETI_ENERGY_TRADE_DESC;
struct CFETI_ENERGY_TRADE_DESC {
	const char*      location;	             /* Physical location */
	const char*      commodity;              /* Commodity Code */
	unsigned char    settlementType;         /* Product type. 'F' = 70 for financial and 'P' = 80 for physical */
	const char*      indexType;	             /* Value "NYM" for some NTGAS products */
	unsigned int     indexTypeID;	         /* Enumerated index type */
	double           durationDays;           /* The total number of days of flow */
	double           durationHours;          /* The total number of hours of flow */
	unsigned char    flowBeginHour;          /* Hour beginning of flow (0-23) */
	unsigned char    flowEndHour;            /* Hour ending of flow    (0-23) */
	const char*      counterpartyName;       /* Legal Entity Name of Counterparty */
	const char*      contactName;            /* Name of counterpart's trader */
	unsigned int     contactNameId;          /* Counterpart's traders page number. For cross reference */
	const char*      contactTelephoneNumber; /* Counterpart's traders phone number */
	CFETI_DATE       startCalendarDate;      /* First day of first month of flow CCYYMMDD */
	CFETI_DATE       endCalendarDate;        /* Last day of last month of flow CCYYMMDD */
	unsigned int     counterpartyID;         /* Numeric identifier of counterparty */
	unsigned char    contractType;           /* Contract type */
	unsigned int     traderID;               /* ID of trader for cross-reference */
	const char*      traderName;             /* Name of user doing trade for entity */
	unsigned int     counterpartyTraderID;   /* ID of counterparty trader for cross-reference */
	const char*      counterpartyTraderName; /* Name of user at counerparty doing trade. */
	const char*      pointName;      /* Pipeline meter name */
	unsigned int     pointNumber;    /* Pipeline meter number */
	double           totalQuantity;  /* Total quantity of trade */
	unsigned char    quantityUnit;   /* Enumerated value to identify quantity unit */
	const char*      currency;       /* ISO standard 3 character mnemonic for currency */
	unsigned char    priceType;      /* Price type */
	const char*      indexName;      /* Name of index (GD = Gas Daily, IF = InsideFerc etc) */
	const char*      loadProfile;    /* Load profile e.g. 5X16, 6X16,2X24 etc */
	unsigned char    holidayFlag;    /* Boolean value used to denote whether to include Holiday (non-zero) or not (zero). */
	unsigned char    productPeriod;  /* Enumerated value used to denote product period: */
	const char*      shortLocation;	 /* Short description of full location name */
	const char*      shortIndexName; /* Short description of full index name */
	const char*      timeZone;       /* 3 character mnemonic for prevailing time zone */
	const char*      priceMechanism; /* Price mechanism (e.g. Fixed, SWAP, NGI Chicago). */
	const char*      indexType2;             /* Index type of 2nd instrument (swap trades only) */
	unsigned int     indexTypeID2;	         /* Enumerated index type of 2nd instrument (swap trades only)*/
	const char*      indexName2;             /* Name of index of 2nd instrument (swap trades only) */
	const char*      shortIndexName2;        /* Index description of 2nd instrument (swap trades only)*/
};
typedef CFETI_ENERGY_TRADE_DESC* CFETI_ENERGY_TRADE;

/*
 * CFETI_MMTS_ORDER_DESC: MMTS Trade Details
 */
typedef struct CFETI_MMTS_ORDER_DESC CFETI_MMTS_ORDER_DESC;
struct CFETI_MMTS_ORDER_DESC {
	CFETI_PRICE allInPrice;          /**< all-in-price */
	CFETI_PRICE accruedInterest;     /**< accured interest */
	CFETI_PRICE repoPriceAdjustment; /**< repo price adjustment > **/
};
typedef CFETI_MMTS_ORDER_DESC* CFETI_MMTS_ORDER;

/*
 * CFETI_FX_TRADE_DESC: FX trade details
 */
typedef struct CFETI_FX_TRADE_DESC CFETI_FX_TRADE_DESC;
struct CFETI_FX_TRADE_DESC {
	unsigned int productType;        /**< FX Product Type (enumerated) */
    unsigned int dealType;           /**< FX Deal Type */
	unsigned int contractDate;	     /**< Trading Date CCYYMMDD */
	unsigned int deliveryDate;	     /**< Settlement Date CCYYMMDD */
	unsigned short legType;          /**< FX Leg Type (enumerated) */ 
	unsigned int cutoffTime;	     /**< Cutoff time HHMMSSCC (FX Options only) */
	const char*  cutoffTimeRegion;   /**< Cutoff region (FX Options only) */
	unsigned int expiryDate;	     /**< Expiry date CCYYMMDD (FX Options only) */
	double       referencePrice;     /**< Reference price */ 
	const char*  receivingAgent;     /**< Receiving Agent */
	const char*  beneficiary;        /**< Beneficiary */
	const char*  beneficiaryAccount; /**< Benficiary Account */
	const char*  buyCurrency;        /**< Mnemonic of buy currency */
	double       buyAmount;          /**< Buy currency amount */
	const char*  sellCurrency;       /**< Mnemonic of sell currency */
	double       sellAmount;         /**< Sell currency amount */
	const char*  currency1Buyer;     /**< Buyer of currency 1 */
	const char*  currency1Seller;    /**< Seller of currency 1 */
	double       forwardRate;        /**< Formard Rate for FX Swap, FX Forward and non-deliverable forwards */
	const char*  currencySettlement; /**< Mnemonic of settlement currency for non-deliverable forwards */
	unsigned int fixingDate;         /**< Date to be used for fixing for non-deliverable forwards */
	const char*  fixingSource;       /**< Fixing source for non-deliverable forwards */
	const char*  hedgeId;            /**< Hedge id */
	unsigned char hedgeType;         /**< Enumerated hedge type */
	const char*  brokerageCurrency;  /**< Brokerage currency */
};

/*
 * CFETI_FX_TRADE_DESC: FX trade details
 */
typedef struct CFETI_FX_OPTION_TRADE_DESC CFETI_FX_OPTION_TRADE_DESC;
struct CFETI_FX_OPTION_TRADE_DESC {
	CFETI_FX_TRADE_DESC	fxTrade;    /**< Core FX trade structure */
	unsigned char  exerciseStyle;	/**< Enumerated exercise style */
	unsigned short optionClass;	    /**< Enumerated option class */
	unsigned short optionStyle;     /**< Enumerated FX Option style */
	unsigned short optionStrategy;  /**< Enumerated option strategy */
	unsigned short deliveryTerms;   /**< Enumerated delivery terms */
	double         lowTrigger;
	double         highTrigger;
	unsigned short lowTriggerBasis;  /**< Enumerated Amount Method */
    unsigned short highTriggerBasis; /**< Enumerated Amount Method */
	double		   lowTriggerRebate;
	double		   highTriggerRebate;
	const char*	   triggerAgent;
	unsigned char  putCallIndicator;  /**< Enumerated put/call */
	double         strikePrice;		  /**< Exercise price for option leg */
	unsigned short strikeBasis;		  /**< Enumerated Amount Method */
	const char*    premiumCurrency;	  /**< currency for payment of premium */
	double         premiumQuote;	  /**< Premium ratio */
	unsigned short premiumQuoteBasis; /**< Terms of premium quote.  Enumerated Amount Method */
	double         premiumAmount;	  /**< Amount of premium for leg 1 */
	double         volatility;
	double         spotRate;
	double         swapPoints;
	double         depositRateCurrency1;
	double         depositRateCurrency2;
	double         delta;
};

/*
 * CFETI_TSWAP_DESC: OrderInfo structure for US Treasury Swap details
 */
struct CFETI_TSWAP_DESC {
	double   dTSwapRatio;        /**< User’s selected TSWAP ratio */
	double   dLockPrice;         /**< User’s selected benchmark lock price */
	double   dCurrentTSwapRatio; /**< Prevailing PV01 */
	double   dCurrentLockPrice;  /**< Prevailing lock price */
};
typedef struct CFETI_TSWAP_DESC CFETI_TSWAP_DESC;
typedef struct CFETI_TSWAP_DESC CFETI_PV01_LOCK_DESC;
typedef CFETI_TSWAP_DESC* CFETI_TSWAP;

/**
 * @brief Contains rank information for a quote
 */
struct CFETI_DD_QUOTE_RANK_DESC
{
    unsigned int uiQuoteId;		/**< The id of the ranked Quote */
	unsigned int uiSortRank;	/**< The display rank for the Quote */
	unsigned int uiBidRank;		/**< The bid rank for this Quote */
	unsigned int uiAskRank;		/**< The ask rank for this Quote */
};
typedef struct CFETI_DD_QUOTE_RANK_DESC CFETI_DD_QUOTE_RANK_DESC;
typedef CFETI_DD_QUOTE_RANK_DESC* CFETI_DD_QUOTE_RANK;

/**
 * @brief Contains rank informations for all quotes against an Direct Dealing Request
 */
struct CFETI_DD_RANK_DESC
{
	const char*	 szInstName; /**< The instrument for the DD Request */
	unsigned int uiRFQId;	 /**< The id of the DD Request the quotes rank information belongs to */
	unsigned int uiBOLSMatchedAsk; /**< Id of quote that matched the ask side of the DD BOLS Request spread */
	unsigned int uiBOLSMatchedBid; /**< Id of quote that matched the bid side of the DD BOLS Request spread */
	unsigned int uiCountdownTime;     /**< Countdown to buy or sell */
	unsigned int uiNumQuoteRanks;        /**< The number of quote rank information structures */
	CFETI_DD_QUOTE_RANK_DESC*	pQuoteRanks; /**< The rank information for each quote */
};
typedef struct CFETI_DD_RANK_DESC CFETI_DD_RANK_DESC;
typedef CFETI_DD_RANK_DESC* CFETI_DD_RANK;


/**
 * CFETI_MARKET_AVAILABILITY_DESC
 * Example on how to access the availability state and size:for a bid on tier1
 * desc.tier[CFETI_INDEX_MARKET_BID][0] - Bid on tier 1
 * desc.tier[CFETI_INDEX_MARKET_ASK][1] - Ask on tier 2
 * desc.tier[CFETI_INDEX_ORDER_SELL][2] - Sell on tier 3
 * desc.tier[CFETI_INDEX_PRDER_BUY][3]  - Buy on tier 4
 */
typedef struct CFETI_TIER_AVAILABILITY_DESC   CFETI_TIER_AVAILABILITY_DESC;
struct CFETI_TIER_AVAILABILITY_DESC {
		 unsigned int availability;
		 double size;
 }; /**< Availability for Bid, Ask, Sell and buy for five tiers */

typedef struct CFETI_MARKET_AVAILABILITY_DESC CFETI_MARKET_AVAILABILITY_DESC;
struct CFETI_MARKET_AVAILABILITY_DESC {
     const char* instrumentName;              /**< eSpeed instrument identifier */
	 CFETI_TIER_AVAILABILITY_DESC tier[4][5]; /**< Availability for Bid, Ask, Sell and buy for five tiers */
};


/*
 * CFETI_CANTOR_REPO_DESC: REPO specific details
 */
typedef struct CFETI_CANTOR_REPO_DESC CFETI_CANTOR_REPO_DESC;
struct CFETI_CANTOR_REPO_DESC {
	unsigned int startDate;             /**< Start date (CCYYMMDD) */
	unsigned int endDate;               /**< End date (CCYYMMDD) */
	unsigned int rightsOfSubstitution;  /**< Rights of substitution */
	const char* annotation;             /**< Annotation text */
	unsigned int attributes;            /**< Attributes */
};

/*
 * CFETI_IRD_TRADE: Interest Rate Swaps
 */
typedef struct CFETI_IRD_IRS_TRADE_DESC CFETI_IRD_IRS_TRADE_DESC;
struct CFETI_IRD_IRS_TRADE_DESC {
	const char*	    bond1;							/**< First bond for IRS traded as spread */
	double			bond1Price;						/**< Price of first bond */
	unsigned int 	bond1PriceType;					/**< Price Type of first bond */
	double			bond1Qty;						/**< Quantity of first bond */
	const char*     bond2;							/**< Second bond for IRS traded as spread */
	double			bond2Price;						/**< Price of second bond */
	unsigned int 	bond2PriceType;					/**< Price Type of second bond */
	double			bond2Qty;						/**< Quantity of second bond */
	unsigned char 	breakAtUnit;					/**< Specifies when break occurs */
	unsigned short	breakAtQty;						/**< Number of above units */
	unsigned char 	breakEveryUnit;					/**< Defines break occurrence (if optional) */
	unsigned short	breakEveryQty;					/**< Number of above units */
	unsigned short	compoundingMethod;				/**< Compounding method (enumeration) */
	unsigned short	compoundingFrequency;			/**< Compounding frequency (enumeration) */
	unsigned int	endDate;						/**< End date (CCYYMMDD) */
	double			firstFixingRate;				/**< First floating rate fixing (%) */
	unsigned short	fixedBasis;						/**< Fixed interest payment calc basis (enum) */
	unsigned short	fixedConvention;				/**< Convention for payment date (enum) */
	unsigned char 	fixedPaymentFrequencyUnit;		/**< Fixed interest payment interval */
	unsigned short	fixedPaymentFrequencyQty;		/**< Number of above units */
	double			fixedRate;						/**< Fixed interest rate (%) */
	unsigned short	floatingConvention;				/**< Convention for payment date (enum) */
	const char*		floatingCurrency;				/**< Currency for floating interest payments */
	double			floatingNotional;				/**< Notional for calc of floating payments */
	unsigned char 	floatingPaymentFrequencyUnit;	/**< Floating interest payment interval */
	unsigned short	floatingPaymentFrequencyQty;	/**< Number of above units */
	unsigned char   floatingRollFrequencyUnit;		/**< Floating rate index fixing interval */
	unsigned short  floatingRollFrequencyQty;		/**< Number of above units */
	const char*		rollDatesHolidayCenters;		/**< Roll dates holiday calendar */
	unsigned int	rolls;							/**< Day of month that floating rate is fixed */
	double			spreadOverFloating;				/**< +ve/-ve spread in basis points */
	unsigned int	startDate;						/**< Start date (CCYYMMDD) */
	double			swapSpread;						/**< Swap spread (basis points) */
	unsigned char 	stubInterpolationStartUnit;		/**< Stub interpolation start unit */
	unsigned short	stubInterpolationStartQty;		/**< Number of above units */
	unsigned char 	stubInterpolationEndUnit;		/**< Stub interpolation end unit */
	unsigned short	stubInterpolationEndQty;		/**< Number of above units */
};

/*
 * CFETI_IRD_TRADE: Overnight Index Swaps
 */
typedef struct CFETI_IRD_OIS_TRADE_DESC CFETI_IRD_OIS_TRADE_DESC;
struct CFETI_IRD_OIS_TRADE_DESC {
	unsigned int	endDate;						/**< End date (CCYYMMDD) */
	unsigned short	fixedBasis;						/**< Fixed interest payment calc basis (enum) */
	unsigned short	fixedConvention;				/**< Convention for payment date (enum) */
	unsigned char 	fixedPaymentFrequencyUnit;		/**< Fixed interest payment interval */
	unsigned short	fixedPaymentFrequencyQty;		/**< Number of above units */
	double			fixedRate;						/**< Fixed interest rate (%) */
	unsigned short  floatingConvention;				/**< Convention for payment date (enum) */
	const char*		floatingCurrency;				/**< Currency for floating interest payments */
	double			floatingNotional;				/**< Notional for calc of floating payments */
	unsigned char   floatingPaymentFrequencyUnit;   /**< Payment interval (enum) */
	unsigned short  floatingPaymentFrequencyQty;	/**< Number of above units */
	unsigned char   floatingRollFrequencyUnit;		/**< Floating rate index fixing interval */
	unsigned short  floatingRollFrequencyQty;		/**< Number of above units */
	const char*		rollDatesHolidayCenters;		/**< Roll dates holiday calendar */
	unsigned int	rolls;							/**< Day of month that floating rate is fixed */
	unsigned int	startDate;						/**< Start date (CCYYMMDD) */
};

/*
 * CFETI_IRD_TRADE: Fixed Rate Agreements
 */
typedef struct CFETI_IRD_FRA_TRADE_DESC CFETI_IRD_FRA_TRADE_DESC;
struct CFETI_IRD_FRA_TRADE_DESC {
	unsigned int	calcPeriodDays;				/**< Num days from value date to maturity date */
	double			fixedRate;					/**< Fixed interest rate (%) */
	unsigned int	fixingDate;					/**< Fixing date (CCYYMMDD) */
	unsigned char 	indexTenor1Unit;			/**< First index tenor */
	unsigned short	indexTenor1Qty;				/**< Number of above units */
	unsigned char 	indexTenor2Unit;			/**< Second index tenor */
	unsigned short	indexTenor2Qty;				/**< Number of above units */
	unsigned int	matDate;					/**< End date of FRA (CCYYMMDD) */
	unsigned int	matDateTenor;				/**< Months to end of FRA */
	unsigned char	paymentDateConvention;		/**< Convention for payment date (enum) */
	unsigned int	valueDate;					/**< Value date (CCYYMMDD) */
	unsigned int	valueDateTenor;				/**< Months to start of FRA */
};

/*
 * CFETI_IRD_TRADE: Swaptions
 */
typedef struct CFETI_IRD_SWAPTION_TRADE_DESC CFETI_IRD_SWAPTION_TRADE_DESC;
struct CFETI_IRD_SWAPTION_TRADE_DESC {
	unsigned char 	breakAtUnit;					/**< Specifies when break occurs */
	unsigned short	breakAtQty;						/**< Number of above units */
	unsigned char 	breakEveryUnit;					/**< Defines break occurrence (if optional) */
	unsigned short	breakEveryQty;					/**< Number of above units */
	unsigned short	calcAgent;						/**< Calculation agent (enumeration) */
	unsigned short	compoundingMethod;				/**< Compounding method (enumeration) */
	unsigned short	compoundingFrequency;			/**< Compounding frequency (enumeration) */
	unsigned int    earliestTime;					/**< Earliest time to exercise swaption on expiry date */
	unsigned int	endDate;						/**< End date (CCYYMMDD) */
	const char*     exerciseHolidayCenters;			/**< Holiday calendar name */
	const char*		exerciseLocation;				/**< Physical location for expiry of SWAPTION */
	unsigned int	expiryDate;						/**< Date on which SWAPTION expires (CCYYMMDD) */
	unsigned int	expiryTime;						/**< Time at which SWAPTION expires (HHMMSSCC) */
	unsigned short	fixedBasis;						/**< Fixed interest payment calc basis (enum) */
	unsigned short	fixedConvention;				/**< Convention for payment date (enum) */
	unsigned char 	fixedPaymentFrequencyUnit;		/**< Fixed interest payment interval */
	unsigned short	fixedPaymentFrequencyQty;		/**< Number of above units */
	double			fixedRate;						/**< Fixed interest rate (%) */
	unsigned short  floatingConvention;				/**< Convention for payment date (enum) */
	const char*		floatingCurrency;				/**< Currency for floating interest payments */
	double			floatingNotional;				/**< Notional for calc of floating payments */
	unsigned char   floatingPaymentFrequencyUnit;   /**< Floating interest payment interval */
	unsigned short  floatingPaymentFrequencyQty;	/**< Number of above units */
	unsigned char   floatingRollFrequencyUnit;		/**< Floating rate index fixing interval */
	unsigned short  floatingRollFrequencyQty;		/**< Number of above units */
	double			notional;						/**< Notional amount on the SWAPTION */
	unsigned char	optionStyle;					/**< Exercise style (enum) */
	double			premiumAmount;					/**< Amount to be paid from buyer to seller */
	double			premiumBasis;					/**< Option premium in basis points */
	const char*		premiumCpty;					/**< Seller of the option */
	const char*		premiumPayer;					/**< Buyer of the option */
	unsigned int	premiumPaymentDate;				/**< Date for premium payment (CCYYMMDD) */
	const char*		rollDatesHolidayCenters;		/**< Roll dates holiday calendar */
	unsigned int	rolls;							/**< Day of month that floating rate is fixed */
	unsigned short	subType;						/**< Enumerated sub-type */

	struct
	{
		unsigned short 	deliveryTerms;				/**< Cash or Physical (enum) */
		const char*		currency;					/**< Settlement cash currency */
		unsigned short	cashMethod;					/**< Payment method for cash */
		unsigned char	paymentOffset;				/**< Payment offset */
		unsigned short	quotationRate;				/**< Quotation rate (enum) */
		const char*		rateSource;					/**< Source of settlement rate */
		const char*		banks;						/**< Reference banks */
		unsigned char	valuationOffset;			/**< Valuation offset */
		unsigned int	valuationTime;				/**< (HHSSMMCC) */
	} settlement;

	double			spreadOverFloating;				/**< +ve/-ve spread in basis points */
	unsigned int	startDate;						/**< Start date (CCYYMMDD) */
	double			strikePrice;					/**< Strike price of the option */
};

/*
 * CFETI_IRD_TRADE: Caps/Floors
 */
typedef struct CFETI_IRD_CAPSFLOORS_TRADE_DESC CFETI_IRD_CAPSFLOORS_TRADE_DESC;
struct CFETI_IRD_CAPSFLOORS_TRADE_DESC {
	unsigned short  basis;							/**< Basis for calculation of net payment */
	unsigned int	endDate;						/**< End date (CCYYMMDD) */
	unsigned short  floatingConvention;				/**< Convention for payment date (enum) */
	unsigned char   floatingPaymentFrequencyUnit;   /**< Floating interest payment interval */
	unsigned short  floatingPaymentFrequencyQty;	/**< Number of above units */
	unsigned char 	floatingRateIndexTenorUnit;		/**< Floating rate index tenor */
	unsigned short	floatingRateIndexTenorQty;		/**< Number of above units */
	unsigned char   floatingRollFrequencyUnit;		/**< Floating rate index fixing interval */
	unsigned short  floatingRollFrequencyQty;		/**< Number of above units */
	double			premiumAmount;					/**< Amount to be paid from buyer to seller */
	double			premiumBasis;					/**< Option premium in basis points */
	const char*		premiumCpty;					/**< Seller of the option */
	const char*		premiumPayer;					/**< Buyer of the option */
	unsigned int	premiumPaymentDate;				/**< Date for premium payment (CCYYMMDD) */
	const char*		rollDatesHolidayCenters;		/**< Roll dates holiday calendar */
	unsigned int	rolls;							/**< Day of month that floating rate is fixed */
	double			spreadOverFloating;				/**< +ve/-ve spread in basis points */
	unsigned int	startDate;						/**< Start date (CCYYMMDD) */
	double          strikePrice;					/**< Strike price of the option */
	unsigned short  subType;						/**< Enumerated sub-type */
};

/*
 * CFETI_IRD_TRADE_DESC: IRD Trade Leg description
 */
typedef union CFETI_IRD_TRADE_LEG_DESC CFETI_IRD_TRADE_LEG_DESC;
union CFETI_IRD_TRADE_LEG_DESC
{
	CFETI_IRD_IRS_TRADE_DESC		irs;
	CFETI_IRD_OIS_TRADE_DESC		ois;
	CFETI_IRD_FRA_TRADE_DESC		fra;
	CFETI_IRD_SWAPTION_TRADE_DESC	swaption;
	CFETI_IRD_CAPSFLOORS_TRADE_DESC capsfloors;
};

/*
 * CFETI_IRD_TRADE: IRD trade description
 * (N.B. When Used, it populates the orderInfo buffer of a CFETI_ORDER_DESC structure)
 */
typedef struct CFETI_IRD_TRADE_DESC CFETI_IRD_TRADE_DESC;
struct CFETI_IRD_TRADE_DESC {
	unsigned short				productType;				/**< IRD product type (trade leg) */
	unsigned int				tradeAttributes;			/**< IRD trade attributes (bit-mask) */
	double						brokerageAmount;			/**< Amount of brokerage charged */
	const char*					brokerageCurrency;			/**< Brokerage currency */
	unsigned short              contractState;              /**< ContractState (enumeration) */
	const char*					counterparty;				/**< Same as CFETI_ORDER_DESC::counterparty */
	const char*					fixedCurrency;				/**< Currency for fixed interest */
	double						fixedNotional;				/**< Notional amount for fixed interest */
	const char*					fixingDatesHolidayCenters;	/**< Fixing date holiday calendar */
	int							fixingDatesOffset;			/**< Fixing date offset */
	unsigned short				floatingBasis;				/**< Fixed interest payment calc basis (enum) */
	unsigned short				floatingRateIndex;			/**< Enumerated floating rate index */
	unsigned short				masterAgreement;			/**< Master agreement (enumeration) */
	const char*					myEntity;					/**< Entity under which user trades */
	const char*					paymentDatesHolidayCenters; /**< Payment date holiday calendar */
	unsigned int				paymentDatesOffset;			/**< Offset from end of calculation period */
	CFETI_IRD_TRADE_LEG_DESC	tradeLeg;				    /**< Union of IRD specific trade info, indexed by irdProductType */
};

/**
 * CFETI_INST_SPECIFICATION_DESC
 *
 * Description of individual instruments in a compound instrument list
 */
typedef struct CFETI_INST_SPECIFICATION_DESC CFETI_INST_SPECIFICATION_DESC;
struct CFETI_INST_SPECIFICATION_DESC
{
	unsigned int uiTradingSystemId; /**< eSpeed trading system identifier */
	const char*  szInstName; /**< eSpeed instrument identifier */
};
typedef struct CFETI_INST_SPECIFICATION_DESC* CFETI_INST_SPECIFICATION;

/**
 * CFETI_COMPOUND_INST_LIST_DESC
 *
 * Structure to represent a list of instruments in compound instrument list
 * market data field (e.g. CFETF_COMPOUND_INST_LIST)
 */
typedef struct CFETI_COMPOUND_INST_LIST_DESC CFETI_COMPOUND_INST_LIST_DESC;
struct CFETI_COMPOUND_INST_LIST_DESC
{
	unsigned int uiNumInstruments; /**< Number of instruments in the list */
	unsigned int uiAttributes; /**< special properties */
	CFETI_INST_SPECIFICATION pInstSpec; /**< Array of instrument specifications */
};
typedef struct CFETI_COMPOUND_INST_LIST_DESC* CFETI_COMPOUND_INST_LIST;

/*
 * CFETI_IDENTIFICATION_DESC : Structure to hold the identification details of
 * the client
 */
typedef struct CFETI_IDENTIFICATION_DESC CFETI_IDENTIFICATION_DESC;
struct CFETI_IDENTIFICATION_DESC {
    const char* Company;
    const char* Application;
    const char* Version;
	const char* Platform;
	const char* OperatingSystem;
};
typedef CFETI_IDENTIFICATION_DESC* CFETI_IDENTIFICATION;

/*
 * CFETI_TRADING_SESSION_INFO_DESC: Notification of the state of the current trading session
 */
typedef struct CFETI_TRADING_SESSION_INFO_DESC CFETI_TRADING_SESSION_INFO_DESC;
struct CFETI_TRADING_SESSION_INFO_DESC
{
	unsigned int uiTradingSystemId;  /**< Trading system id */
	unsigned int uiBusinessDate;     /**< Business date for this trading session(YYYYMMDD) */
	unsigned int uiNextBusinessDate; /**< Next business date after current */
	unsigned int uiNextBusinessDate2; /**< Two business days from current */
	unsigned int uiSessionStartDate; /**< Date of trading session (YYYYMMDD) */
	unsigned int uiSessionStartTime; /**< GMT time of start of trading session (HHMMSSCC) */
	unsigned int uiSessionEndDate;   /**< Date of end of trading session (YYYYMMDD) */
	unsigned int uiSessionEndTime;   /**< GMT time of end of trading session (HHMMSSCC) */
	unsigned int uiSessionState;     /**< The enumerated Trading Session State */
};
typedef struct CFETI_TRADING_SESSION_INFO_DESC* CFETI_TRADING_SESSION_INFO;


/*
 * CFETI_PRICE_BOUNDARY_DESC: Session price and yield boundaries (high/low/open/close)
 */
typedef struct CFETI_PRICE_BOUNDARY_DESC CFETI_PRICE_BOUNDARY_DESC;
struct CFETI_PRICE_BOUNDARY_DESC 
{
	double        dHigh;			/**< High price for trading session */
    double        dHighYield;       /**< Highest yield during this session */
    unsigned char highIndicator;    /**< Trade event for highest price */
    double        dLow;				/**< Low price for trading session */
    double        dLowYield;        /**< Lowest yield during this session */
    unsigned char lowIndicator;     /**< Trade event for lowest price */
    double        dOpen;            /**< Openning price for trading session */
    double        dOpenYield;       /**< Opening yield for this session */
    double        dClose;			/**< Closing price for trading session */
    double        dCloseYield;      /**< Closing yield of previous session*/
	unsigned int  uiSessionState;   /**< Enumerated Trading Session State */
};

/*
 * CFETI_PRICE_NET_CHANGE_DESC: Session price and yield net change (price and yield)
 */
typedef struct CFETI_PRICE_NET_CHANGE_DESC CFETI_PRICE_NET_CHANGE_DESC;
struct CFETI_PRICE_NET_CHANGE_DESC {
 double dNetChange;      /**< Net change for this session */
 double dNetChangeYield; /**< Net yield change for this session */
};

/*
 * CFETI_VWA_BOUNDARY_DESC: Session volume weighted average data
 */
typedef struct CFETI_VWA_BOUNDARY_DESC CFETI_VWA_BOUNDARY_DESC ;
struct CFETI_VWA_BOUNDARY_DESC 
{
	double        dVWAP ;			/**< Volume weighted average price */
    double        dVWAY ;			/**< Volume weighted average yield */
    double        dNetChangeVWAP ;	/**< Volume weighted average price net change */
    double        dNetChangeVWAY ;	/**< Volume weighted average yield net change */
	time_t        timestamp ;		/**< Timestamp of last update */
	unsigned int  uiMilliseconds ;	/**< milliseconds of the timestamp */
	unsigned int  uiSessionState;   /**< Enumerated Trading Session State */
} ;


/**
 * CFETI_DD_DETAILS_DESC
 * Data structure to specify details of an RFQ or Quote
 * Delivered by DD-server in a CFETI_RFQ_SERVER_INFO_DESC
 */
struct CFETI_DD_DETAILS_DESC {
	unsigned int maxTimeout;	/**< Max timeout in seconds */
	unsigned int minTimeout;	/**< Min timeout in seconds */
	unsigned int timeoutIncrement; /**< Timeout increment in seconds */
};

/**
 * CFETI_DD_SERVER_ATTRIBUTES_DESC
 * Data structure to specify attributes of the Direct Dealing system
 */
typedef struct CFETI_DD_SERVER_ATTRIBUTES_DESC CFETI_DD_SERVER_ATTRIBUTES_DESC;
struct CFETI_DD_SERVER_ATTRIBUTES_DESC {
	unsigned int uiServerFeatures;
	double dBOLSValidSpread;
	double dMinimumSize;
	unsigned int minAnonymousCalloutLE;
	CFETI_DD_DETAILS_DESC rfqDetails;
	CFETI_DD_DETAILS_DESC quoteDetails;
};
typedef CFETI_DD_SERVER_ATTRIBUTES_DESC* CFETI_DD_SERVER_ATTRIBUTES;

/**
 * CFETI_LEGAL_ENTITY_QUERY_DESC
 * Data structure to specify details of a legal entity query
 */
typedef struct CFETI_LEGAL_ENTITY_QUERY_DESC CFETI_LEGAL_ENTITY_QUERY_DESC;
struct CFETI_LEGAL_ENTITY_QUERY_DESC {
	unsigned int uiLegalEntityId; /**< Legal entity id of query issuer*/
	const char* szUsername; /**< eSpeed username of query issuer*/
};
typedef CFETI_LEGAL_ENTITY_QUERY_DESC* CFETI_LEGAL_ENTITY_QUERY;

/**
 * CFETI_LEGAL_ENTITY_DESC
 * Data structure to specify details of a legal entity
 */
typedef struct CFETI_LEGAL_ENTITY_DESC CFETI_LEGAL_ENTITY_DESC;
struct CFETI_LEGAL_ENTITY_DESC {
	unsigned int uiLegalEntityId; /**< Legal entity id */
	const char* szLegalEntityName; /**< Legal entity name */
	const char* szLEShortName; /**< Legal entity nickname */
};
typedef CFETI_LEGAL_ENTITY_DESC* CFETI_LEGAL_ENTITY;

/**
 * CFETI_LEGAL_ENTITY_LIST_DESC
 * Data structure to specify list of legal entities
 */
typedef struct CFETI_LEGAL_ENTITY_LIST_DESC CFETI_LEGAL_ENTITY_LIST_DESC;
struct CFETI_LEGAL_ENTITY_LIST_DESC {
	unsigned int uiNumEntities; /**< Number of entries in the list */
	CFETI_LEGAL_ENTITY_DESC* pEntity; /**< Array of legal entity descriptions */
};
typedef CFETI_LEGAL_ENTITY_LIST_DESC* CFETI_LEGAL_ENTITY_LIST;

/*
 * CFETI_TEXTNOTIFY_DESC
 */
typedef struct CFETI_TEXTNOTIFY_DESC CFETI_TEXTNOTIFY_DESC;
struct CFETI_TEXTNOTIFY_DESC {
	CFETI_TRADING_SYSTEM tsId;
	unsigned int         severity;
	time_t               sendTime;
	char**               text;
	int                  textLines;
	const char*          sender;
};
typedef struct CFETI_TEXTNOTIFY_DESC* CFETI_TEXTNOTIFY;

/**
 * CFETI_ACTION_TYPE: Enumerated type representing market actions. 
 */
typedef unsigned char CFETI_ACTION_TYPE; 

/**
 * @brief Command data structure to describe an Direct Dealing Request.
 * Contains command data for Direct Dealing Request commands.
 */
struct CFETI_DD_REQUEST_DESC {
	CFETI_ID         id;                  /**< Zero for cmd submit. Non zero for cancel or modify. */
	CFETI_ID         requestId;           /**< Non zero for cmd submit. Zero for cancel or modify. */
	CFETI_INSTRUMENT tradeInstrument;     /**< Instrument for which quote is requested */  
	CFETI_PREF       preferences;         /**< Direct Dealing specific options */
	CFETI_SIZE       size;                /**< Absolute size user is requesting  */
	CFETI_SIZE       minSize;             /**< Minimum size user is requesting */
	CFETI_PRICE      askPrice;            /**< Price that requester is quoting */
	CFETI_PRICE      bidPrice;            /**< Price that requester is quoting */
	CFETI_PRICE      askReservePrice;     /**< Reserve price. */
	CFETI_PRICE      bidReservePrice;     /**< Reserve price. */
	unsigned int     minPricesCount;      /**< Min number of prices user requires for Request */
	CFETI_TIME       timeLimit;           /**< Number of seconds for which the request/quote will be valid specified by creator */
	CFETI_TIME       timeRemaining;       /**< Number of seconds for which the request/quote will remain valid */
	CFETI_ACTION_TYPE action;             /**< Action: e.g. CFETI_DD_BUY, CFETI_DD_SELL etc. */
	unsigned int     uiOwner;             /**< Id of the DD Request owner */
	unsigned int     uiOwnerType;         /**< Owner of this DD Request is either receiving user, same legal entity or other counterpart */
	CFETI_TEXTNOTIFY pMessage;            /**< Text message associated with this Request (if command is CFETI_DD_SYSTEM_MESSAGE) */
	CFETI_PRICE      bolsSpread;          /**< BOLS Spread for use in DD_BOLS_2WAY Reqyests. */
	const char*      szClassification;    /**< Instrument classification string for trade instrument (for named callout DD Reqyests)*/
	unsigned int     uiNumCounterparties; /**< Number of counterparties in legal entity list (for named callout DD Requests) */
	unsigned int*    pCounterpartyList;   /**< List of legal entity ids for this DD Request (for named callout DD Requests) */
	const char*      szFullUsername;      /**< Full user name of DD Request owner (for named callout DD Requests) */
	const char*      szLegalEntityName;   /**< Full legal entity name of DD Request owner (for named callout DD Requests) */
	const char*      szLEShortName;       /**< Legal entity nickname of DD Request owner (for named callout DD Requests) */
	unsigned int     uiCounterpartyStateBid; /**< Enumerated counterparty state for the bid price (name giveup only) */
	unsigned int     uiCounterpartyStateAsk; /**< Enumerated counterparty state for the ask price (name giveup only) */
	unsigned short   usInstrumentClass;      /**< Instrument classification (name giveup only) */
	const char*      szUserName;          /**< Name of the user to which the request belongs */
	const char*      szRequestorId;       /**< Name of the user submitting the request */
	const char*      szOriginatorId;      /**< Name of the user that first created the request (responses only) */
	unsigned int     orderInfoType;		  /**< Enumerated type to describe the structure orderInfo points to */
	void*			 orderInfo;			  /**< Pointer to orderInfo type structure */
	time_t           creationTime;        /**< Request creation time (responses & notifications only) */
	const char*		 allocationInfo;	  /**< User specified allocation instructions (free text) */
};
typedef struct CFETI_DD_REQUEST_DESC CFETI_DD_REQUEST_DESC;
typedef CFETI_DD_REQUEST_DESC* CFETI_DD_REQUEST;

/**
 * @brief Command data structure for posting and receiving Direct Dealing Quotes
 * Contains command data for commmand messages for quotes and responses to quotes.
 */
struct CFETI_DD_QUOTE_DESC {
	CFETI_ID          id;                /**< Zero for cmd submit. Non zero for cancel or modify. */
	CFETI_ID          requestId;         /**< Non zero for cmd submit. Zero for cancel or modify. */
	CFETI_INSTRUMENT  tradeInstrument;   /**< Instrument for which quote is given/requested */  
	CFETI_PREF        preferences;       /**< Quote/Response specific options */
	CFETI_SIZE        size;              /**< Absolute size user is quoting */
	CFETI_PRICE       askPrice;	         /**< Price that user is quoting */
	CFETI_PRICE       bidPrice;	         /**< Price that user is quoting */
	CFETI_TIME        timeLimit;         /**< Number of seconds for which the request/quote will be valid */
	CFETI_TIME        timeRemaining;     /**< Number of seconds for which the request/quote will remain valid */
	CFETI_ACTION_TYPE action;            /**< Action: e.g. CFETI_DD_BUY, CFETI_DD_SELL etc. */
	CFETI_ID          RFQId;             /**< Unique id for DD Request. For quote and quote response */
	const char*       szFullUsername;    /**< Full user name of Quote owner (named callout only) */
	const char*       szLegalEntityName; /**< Full legal entity name of Quote owner (named callout only) */
	const char*       szLEShortName;     /**< Legal entity nickname of Quote owner (for named callout) */
	unsigned int      uiOwner;           /**< Id of the Quote owner */
	unsigned int      uiOwnerType;       /**< Owner of this Quote is either receiving user, same legal entity or other counterpart */
	CFETI_PRICE       quoteSpread;       /**< Bid/Offer spread for 2 Way and BOLS quotes */
	unsigned int      uiCounterpartyStateBid; /**< Enumerated counterparty state for the bid price (name giveup only) */
	unsigned int      uiCounterpartyStateAsk; /**< Enumerated counterparty state for the ask price (name giveup only) */
	unsigned short    usInstrumentClass;      /**< Instrument classification (name giveup only) */
	const char*       szUserName;         /**< Name of the user to which the request belongs */
	const char*       szRequestorId;      /**< Name of the user submitting the request */
	const char*       szOriginatorId;     /**< Name of the user that first created the request (responses only) */
	unsigned int      orderInfoType;	  /**< Enumerated type to describe the structure orderInfo points to */
	void*			  orderInfo;          /**< Pointer to orderInfo type structure */
	time_t            creationTime;       /**< Quote creation time (responses & notifications only) */
	const char*		  allocationInfo;	  /**< User specified allocation instructions (free text) */
};
typedef struct CFETI_DD_QUOTE_DESC CFETI_DD_QUOTE_DESC;
typedef CFETI_DD_QUOTE_DESC* CFETI_DD_QUOTE;




/**
 * The data structure CFETI_EXTENDED_MARKET_AVAILABILITY_DESC is populated when
 * applications decode the multi-user availability market data field
 * CFETF_MARKET_AVAILABILITY_MULTI_USER.
 */
typedef struct CFETI_EXTENDED_MARKET_AVAILABILITY_USER_DESC CFETI_EXTENDED_MARKET_AVAILABILITY_USER_DESC;
struct CFETI_EXTENDED_MARKET_AVAILABILITY_USER_DESC
{
		CFETI_TRADE_SESS_ID tsSession;
		CFETI_MARKET_AVAILABILITY_DESC availability;
};

typedef struct CFETI_EXTENDED_MARKET_AVAILABILITY_DESC CFETI_EXTENDED_MARKET_AVAILABILITY_DESC;
struct CFETI_EXTENDED_MARKET_AVAILABILITY_DESC {
	unsigned int numUsers;
	CFETI_EXTENDED_MARKET_AVAILABILITY_USER_DESC* pUsers;
};

/**
 * @brief Exchange specific trading session information. Delivered to
 * connection callback when the command is CFETI_SET_EXCHANGE_INFO.
 */
typedef struct CFETI_EXCHANGE_INFO_DESC CFETI_EXCHANGE_INFO_DESC;
struct CFETI_EXCHANGE_INFO_DESC {
    unsigned int exchangeId;          /**< Enumerated exchange identifier */
    unsigned int exchangePermissions; /**< Permissions bit-mask */
};

/**
 * @brief Description of an exchange account
 */
typedef struct CFETI_EXCHANGE_ACCOUNT_DESC CFETI_EXCHANGE_ACCOUNT_DESC;
struct CFETI_EXCHANGE_ACCOUNT_DESC{
	const char* accountName; /**< Name given to the account */
	unsigned int accountVersion; /**< Account version */
	CFETI_EXT_PROPERTY_ORDERINFO_DESC accountProperties; /** Attributes of the account */
};

/**
 * @brief List of accounts available to a trading user for an exchange
 */
typedef struct CFETI_EXCHANGE_ACCOUNT_LIST_DESC CFETI_EXCHANGE_ACCOUNT_LIST_DESC;
struct CFETI_EXCHANGE_ACCOUNT_LIST_DESC {
	unsigned int numAccounts;  /**< Number of accounts */
	CFETI_EXCHANGE_ACCOUNT_DESC* accounts; /**< List of accounts */
};


/**
 * PI Benefit Data
 * The amount of PI benefit afforded to the customer as a result of the trade
 */
typedef struct CFETI_PI_BENEFIT_DESC CFETI_PI_BENEFIT_DESC;
struct CFETI_PI_BENEFIT_DESC {
	double straight;     /**< The amount of PI Benefit */
	double held;         /**< The amount of PI Held Benefit */
	double betterFilled; /**< The amount of Better Filled Benefit */
};

/*
 * CFETI_SYSTEM_CB: Callback for all system related messages
 */
typedef void (*CFETI_SYSTEM_CB)(CFETI_CMD, CFETI_CMD_STATUS, CFETI_CMDDATA, CFETI_UD);

/*
 * CFETI_CONNECT_CB: Callback for all connection related messages
 */
typedef void (*CFETI_CONNECT_CB)(CFETI_CMD, CFETI_CMD_STATUS, CFETI_CMDDATA, CFETI_UD);

/**
 * Data structure for list of other eSpeed users
 */
struct CFETI_TRADER_USER_LIST_DESC {
	unsigned int numEntries;
	const char** traderList;
};
typedef struct CFETI_TRADER_USER_LIST_DESC CFETI_TRADER_USER_LIST_DESC;

/**
 * CFETI_API_SUBSCRIBED_USER_INFO_DESC: Connection Info for this API build
 * A list of id strings for available connection modes (socket types)
 */
struct CFETI_API_SUBSCRIBED_USER_INFO_DESC
{
	CFETI_TRADE_SESS_ID         sessionId;
	CFETI_TRADING_SYSTEM		tradingSystem;
	const char*					szSubscribedUsername;
	unsigned int				connectionStatus;
	unsigned int				userProperties;
};

/**
 * Data structure delivered to the client application trading system connection callback when the
 * command is CFETI_SET_CHECKOUT_BUSINESS_INFO
 */
typedef struct CFETI_CHECKOUT_BUSINESS_INFO_DESC CFETI_CHECKOUT_BUSINESS_INFO_DESC;
struct CFETI_CHECKOUT_BUSINESS_INFO_DESC
{
     const char* businessName;
     const char* businessShortname;
};

/**
 * CFETI_2WAY_ORDER_DESC : Definition of a 2-way order around the touch price.
 */
typedef struct CFETI_2WAY_ORDER_DESC CFETI_2WAY_ORDER_DESC;
struct CFETI_2WAY_ORDER_DESC {
	const char* instrument;        /**< eSpeed instrument name */
	unsigned int tsId;             /**< Trading system identifier */
	double priceSpread;            /**< Spread for 2-way market */
	unsigned short preferredSide;  /**< Preferred side */
	unsigned short propagation;    /**< Propagation preference bit-mask (bid, offer, both) */
	unsigned int attributes;       /**< Attributes applied to 2-way order */
	double bidSize;                /**< Size in which to propagate bid */
	double askSize;                /**< Size in which to propagate offer */
	double maxSpread;              /**< Maximum spread between bid/offer for propagation */
	unsigned int requestId;        /**< Request id (used in create request and respones) */
	unsigned int id;               /**< Order id assigned by trading system */
	unsigned int bidId;            /**< Order id of propagated bid */
	unsigned int askId;            /**< Order id of propagated offer */
	unsigned int priceImprovement; /**< Price improvement to apply to propagated markets */
};

/** 
 * Data structure used to specified field data to decode
 * in CFETIDecodeDataField
 */
typedef struct CFETI_DECODE_DATA_DESC CFETI_DECODE_DATA_DESC;
struct CFETI_DECODE_DATA_DESC
{
	CFETI_FIELD_DESC* field; /**< Field to decode */
	const char* instrumentName; /**< Instrument name */
	void* instClass; /**< Instrument classification data */
};

/**
 * Data structure used to describe credit status
 */
typedef struct CFETI_CREDIT_STATUS_DESC CFETI_CREDIT_STATUS_DESC;
struct CFETI_CREDIT_STATUS_DESC {
     double initialCredit;    /**< Initial trading limit (US$) */
     double currentCredit;  /**< Current trading limit (US$) */
     double lowCredit;       /**< Optional low credit value (US$) */
};

/*
 * CFETI_ESPD_REPO_TRADE_DESC: Trade Order info struct for Repos
 */
typedef struct CFETI_ESPD_REPO_TRADE_DESC CFETI_ESPD_REPO_TRADE_DESC;
struct CFETI_ESPD_REPO_TRADE_DESC {
     unsigned int properties; /**< Properties bit-mask */
     double allInPrice; /**< Calculated all-in-price */
     unsigned int startDate; /**< Start date (CCYYMMDD) */
     unsigned int endDate; /**< End date (CCYYMMDD) */
     const char* collateral; /**< Underlying instrument */
     unsigned short collateralIdType; /**< ISIN or CUSIP */
     const char* collateralId; /** Collateral inst CUSIP / ISIN */
	 double notionalAmount; /** Notional amount */
};

/*
 * CFETI_ESPD_REPO_TRADE_DESC: Market/Order order info struct for Repos
 */
typedef struct CFETI_ESPD_REPO_DESC CFETI_ESPD_REPO_DESC;
struct CFETI_ESPD_REPO_DESC {
     const char* collateral; /**< eSpeed instrument name */
     unsigned int properties; /**< Properties bit-mask */
     unsigned int startDate; /**< Start date (CCYYMMDD) */
     unsigned int endDate; /**< End date (CCYYMMDD) */
};

 


#if defined(__cplusplus)
}
#endif

#endif
