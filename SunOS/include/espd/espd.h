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

#if !defined(_CFETI_H_)
#define _CFETI_H_

#include <espd/types.h>

#if defined(__cplusplus)
extern "C" {
#endif

/*
 * Include files
 */
#include <espd/consts.h>
#include <espd/fields.h>
#include <espd/types.h>
#include <espd/dllexport.h>

/*
 * CFETI Application Interfaces
 */

/*
 * CFETIOpenSession : Open an eSpeed Session
 */
ESPD_SHARE CFETI_RC CFETIOpenSession(
	const char* primary,
	const char* secondary,
	CFETI_IDENTIFICATION ident
);

/*
 * CFETICloseConnection : Close the open eSpeed session
 */
ESPD_SHARE void CFETICloseSession();

/*
 * CFETILoginUser : Establish a login on an open session.
 */
ESPD_SHARE CFETI_RC CFETILogin(
	const char *userName,
	const char *userPassword,
	CFETI_PREF systemPreferences,
	CFETI_SYSTEM_CB systemCallback,
	CFETI_UD userData,
#if defined(__cplusplus)
	const char *szUserTag = 0
#else
	const char *szUserTag
#endif
);

/*
 * CFETIAuthenticate : Session creation and login authentication (deprecated)
 */
ESPD_SHARE CFETI_RC CFETIAuthenticate(
	const char *userName,
	const char *userPassword,
	const char *primarySessMgr,
	const char *secondarySessMgr,
	CFETI_PREF systemPreferences,
	CFETI_SYSTEM_CB systemCallback,
	CFETI_UD userData,
#if defined(__cplusplus)
	const char *szUserTag = 0
#else
	const char *szUserTag
#endif
);

/*
 * CFETILogout : System logout
 */
ESPD_SHARE CFETI_RC CFETILogout(
	CFETI_SESSION_ID sessId,
	CFETI_PREF systemPreferences
);

/*
 * CFETIConnect : Trading system connection
 */
ESPD_SHARE CFETI_RC CFETIConnect(
	const CFETI_SESSION_ID sessId,
	const char *userPassword,
	CFETI_TRADING_SYSTEM tradeSys,
	CFETI_PREF tradingSysPreferences,
	CFETI_CONNECT_CB tradingSysCallback,
	CFETI_UD userData,
	CFETI_TRADE_SETTINGS_DESC* pTradeSettings
);

/*
 * CFETIDisconnect : Trading System Disconnect
 */
ESPD_SHARE CFETI_RC CFETIDisconnect(
	const CFETI_SESSION_ID sessId,
	CFETI_TRADE_SESS_ID trdSysSessId,
	CFETI_PREF tradingSysPreferences
);


/*
 * CFETIPostMessage : Post a message to a trading system
 */
ESPD_SHARE CFETI_RC CFETIPostMessage(
	const CFETI_SESSION_ID sessId,
	CFETI_TRADE_SESS_ID trdSysSessId,
	CFETI_CMD cmd,
	CFETI_CMDDATA cmdData,
	CFETI_CMDPREF cmdPreferences
);


/*
 * CFETI Message flow
 */

/*
 * CFETIMessageLoop : Loop forever processing messages
 */
ESPD_SHARE void CFETIMessageLoop();

/*
 * CFETIWaitMessage : Wait until the next message arrives and process it
 */
ESPD_SHARE CFETI_RC CFETIWaitMessage(unsigned long timeOut);

/*
 * CFETIGetMessage : Get and process the next message - don't wait
 */
ESPD_SHARE CFETI_RC CFETIGetMessage();

/*
 * CFETIPeekMessage : Determine whether there are messages to be processed
 */
ESPD_SHARE CFETI_RC CFETIPeekMessage();

/*
 * Other interfaces
 */

/*
 * CFETIGetLastError : Retrieve error text associated with the last CFETI error
 */
ESPD_SHARE const char* CFETIGetLastError();

/**
 * CFETIDecodeDataField: Decode the field and data supplied in CFETI_DECODE_DATA_DESC
 */
ESPD_SHARE CFETI_RC CFETIDecodeDataField(
	const CFETI_SESSION_ID sessId,
	CFETI_TRADE_SESS_ID trdSysSessId,
	const CFETI_DECODE_DATA_DESC* decodeData,
	void** result
);

/*
 * CFETIDecodeField : Decode the supplied CFETI_FIELD (Deprecated)
 */
ESPD_SHARE CFETI_RC CFETIDecodeField(
	CFETI_SESSION_ID sessId,
	CFETI_TRADE_SESS_ID trdSysSessId,
	CFETI_FIELD field,
	void** result
);


/**
 * CFETIDecodeDataField: Decode the field and data supplied in CFETI_DECODE_DATA_DESC
 */
ESPD_SHARE CFETI_RC CFETIDecodeDataField(
	const CFETI_SESSION_ID sessId,
	CFETI_TRADE_SESS_ID trdSysSessId,
	const CFETI_DECODE_DATA_DESC* decodeData,
	void** result
);

/*
 * CFETIDecodeField : Decode the supplied CFETI_FIELD (Deprecated)
 */
ESPD_SHARE CFETI_RC CFETIDecodeFieldExt(
	const CFETI_SESSION_ID sessId,
	CFETI_TRADE_SESS_ID trdSysSessId,
	CFETI_FIELD field,
    void* instClass, 
	void** result
);

/**
 * Set a new password for the user
 */
ESPD_SHARE CFETI_RC CFETISetPassword(
	const CFETI_SESSION_ID sessionId,
	const char *oldPassword,
	const char *newPassword,
	const char *confirmedPassword
);

/*
 * CFETIVersion : Retrieve version string
 */
ESPD_SHARE const char* CFETIVersion();

/*
 * CFETIGetAPIConnectionInfo : Retrieve structure with available connection modes
 */
ESPD_SHARE const CFETI_API_CONNECTION_INFO CFETIGetAPIConnectionInfo();

/*
 * CFETISelectConnectionMode : Select which connection mode to use.
 */
ESPD_SHARE CFETI_RC CFETISelectConnectionMode(const char* szConnectionMode);


/*
 * CFETISetProxy : Set a proxy to go through "hostname:port"
 * Set userAgent when required by the proxy, else set to NULL 
 */
ESPD_SHARE void CFETISetProxy(const char* hostAddr, const char* userAgent);

/*
 * Helper macros
 */

/*
 * Macro to check for bit(s) set 
 */
#define CFETI_IS_SET(v,b) \
	((v) & (b))

/*
 * Macros to check for state in field CFETF_STATE 
 */
#define CFETI_IS_NO_MARKET(v) \
	((v) == CFETI_STATE_NO_MARKET)

#define CFETI_IS_BID(v) \
	CFETI_IS_SET(v, CFETI_STATE_BID)

#define CFETI_IS_UNCLEAR_BID(v) \
	(CFETI_IS_SET(v, CFETI_STATE_BID) && CFETI_IS_SET(v, CFETI_STATE_UNCLEAR_BID))

#define CFETI_IS_SELL(v) \
	CFETI_IS_SET(v, CFETI_STATE_SELL)

#define CFETI_IS_OFFER(v) \
	CFETI_IS_SET(v, CFETI_STATE_OFFER)

#define CFETI_IS_UNCLEAR_OFFER(v) \
	(CFETI_IS_SET(v, CFETI_STATE_OFFER) && CFETI_IS_SET(v, CFETI_STATE_UNCLEAR_OFFER))

#define CFETI_IS_BUY(v) \
	CFETI_IS_SET(v, CFETI_STATE_BUY)

#define CFETI_IS_CHECK_CREDIT(v) \
	CFETI_IS_SET(v, CFETI_STATE_CHECK_CREDIT)

#define CFETI_IS_ERROR(v) \
	CFETI_IS_SET(v, CFETI_STATE_ERROR)

#define CFETI_IS_TRADABLE(v) \
	!CFETI_IS_SET(v, CFETI_STATE_NON_TRADABLE)

#define CFETI_IS_BID_OR_OFFER(v) \
	CFETI_IS_SET(v, CFETI_STATE_BID|CFETI_STATE_OFFER)

#define CFETI_IS_BID_AND_OFFER(v) \
	(CFETI_IS_SET(v, CFETI_STATE_BID) && CFETI_IS_SET(v, CFETI_STATE_OFFER))

#define CFETI_IS_BID_AND_ASK(v) \
	CFETI_IS_BID_AND_OFFER(v)

#define CFETI_BID_IS_INDICATIVE(v) \
	CFETI_IS_SET(v, CFETI_STATE_INDICATIVE_BID)

#define CFETI_OFFER_IS_INDICATIVE(v) \
	CFETI_IS_SET(v, CFETI_STATE_INDICATIVE_OFFER)

#define CFETI_BID_IS_LAST(v) \
	CFETI_IS_SET(v, CFETI_STATE_EXT_LAST_BID)

#define CFETI_OFFER_IS_LAST(v) \
	CFETI_IS_SET(v, CFETI_STATE_EXT_LAST_OFFER)

#define CFETI_IS_TRADING(v) \
	CFETI_IS_SET(v, CFETI_STATE_BUY|CFETI_STATE_SELL)

#define CFETI_IS_TRADE_ENDING(v) \
	(CFETI_IS_TRADING(v) && CFETI_IS_SET(v, CFETI_STATE_TRADE_ENDING))

/*
 * Macros to check for state in field CFETF_STATE_EXT 
 */
#define CFETI_IS_PI_BID(v) \
	CFETI_IS_SET(v, CFETI_STATE_EXT_PI_BID)

#define CFETI_IS_PI_ASK(v) \
	CFETI_IS_SET(v, CFETI_STATE_EXT_PI_ASK)

#define CFETI_IS_PI_BUY(v) \
	CFETI_IS_SET(v, CFETI_STATE_EXT_PI_BUY)

#define CFETI_IS_PI_SELL(v) \
	CFETI_IS_SET(v, CFETI_STATE_EXT_PI_SELL)

#define CFETI_IS_REACHABLE_BUY(v) \
	CFETI_IS_SET(v, CFETI_STATE_EXT_REACHABLE_BUY)

#define CFETI_IS_REACHABLE_SELL(v) \
	CFETI_IS_SET(v, CFETI_STATE_EXT_REACHABLE_SELL)

/*
 * Macros to check for price code in bid/offer list participants 
 */
#define CFETI_PRICE_IS_GOOD_TILL_CANCEL(code) \
	CFETI_IS_SET(code, (unsigned char)CFETI_PRICE_GOOD_TILL_CANCEL)

#define CFETI_PRICE_IS_ALL_OR_NONE(code) \
	CFETI_IS_SET(code, (unsigned char)CFETI_PRICE_ALL_OR_NONE)

#define CFETI_PRICE_IS_LIMIT(code) \
	CFETI_IS_SET(code, (unsigned char)CFETI_PRICE_LIMIT)

#define CFETI_PRICE_IS_CHECK_CREDIT(code) \
	CFETI_IS_SET(code, (unsigned char)CFETI_PRICE_CHECK_CREDIT)

#define CFETI_PRICE_IS_CLEARING_LCH(code) \
	CFETI_IS_SET(code, (unsigned char)CFETI_PRICE_CLEARING_LCH)

#define CFETI_PRICE_IS_CLEARING_RESTRICTED(code) \
	CFETI_IS_SET(code, (unsigned char)CFETI_PRICE_CLEARING_RESTRICTED)

#define CFETI_PRICE_IS_CLEARING_CLEARNET         /**< @depricated Use CFETI_PRICE_IS_CLEARING_RESTRICTED instead */
#define CFETI_PRICE_IS_CLEARING_INTERBANK(code) \
	CFETI_IS_SET(code, (unsigned char)CFETI_PRICE_CLEARING_INTERBANK)

#define CFETI_PRICE_IS_DERIVED(code) \
	CFETI_IS_SET(code, (unsigned char)CFETI_PRICE_EXCHANGE_DERIVED)

#define CFETI_PRICE_IS_DERIVED_SPREAD(code) \
	CFETI_IS_SET(code, (unsigned char)CFETI_PRICE_DERIVED_SPREAD)

#define CFETI_PRICE_IS_ESPEED_DERIVED(code) \
	CFETI_IS_SET(code, (unsigned char)CFETI_PRICE_ESPEED_DERIVED)

#define CFETI_PRICE_IS_RESTRICTED(code) \
	CFETI_IS_SET(code, (unsigned char)CFETI_PRICE_OPTION_RESTRICTED)

#define CFETI_PRICE_IS_EXCLUDED(code) \
	CFETI_IS_SET(code, CFETI_PRICE_OPTION_EXCLUDE)

/*
 * Macros to check for price types 
 */
#define CFETI_PRICETYPE_IS_32ND(v) \
	CFETI_IS_SET(v, CFETI_PRICETYPE_32ND)

#define CFETI_PRICETYPE_IS_DECIMAL(v) \
	CFETI_IS_SET(v, CFETI_PRICETYPE_DECIMAL)
#if defined(__cplusplus)
}
#endif


#endif
