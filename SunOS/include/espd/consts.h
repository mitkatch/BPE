/*
 * eSpeed, Inc.
 * 
 * This system and/or its use is protected under one or more of the following
 * United States patents:  5,905,974; 6,560,580.  All rights reserved.  Other
 * patents pending.
 * 
 * cfeti_consts.h generated 02/21/2006 18:09:51 - do not edit directly
 */

#if !defined(_CFETI_CONSTS_)
#define _CFETI_CONSTS_

/*
 * Return code for success 
 */
#define CFETI_SUCCESS                           0

/*
 * Commands issued by user 
 */
#define CFETC_POST_MARKET                       0x1
#define CFETC_CANCEL_MARKET                     0x2
#define CFETC_SUBMIT_ORDER                      0x3
#define CFETC_CANCEL_ORDER                      0x4
#define CFETC_QUERY                             0x5
#define CFETC_SUBSCRIBE                         0x6
#define CFETC_UNSUBSCRIBE                       0x7
#define CFETC_REFRESH                           0x8
#define CFETC_MODIFY_CREDIT_VIEW                0xa
#define CFETC_TRADE_CHECKOUT_REQUEST            0xd /**< Sent by trading user to acknowledge a trade */
#define CFETC_TRADE_REFUSE_REQUEST              0xf /**< Sent by the user to reject a trade */
#define CFETC_DD_REQUEST_SUBMIT                 0x12
#define CFETC_DD_REQUEST_CANCEL                 0x13
#define CFETC_DD_REQUEST_MODIFY                 0x14
#define CFETC_DD_QUOTE_SUBMIT                   0x15
#define CFETC_DD_QUOTE_CANCEL                   0x16
#define CFETC_DD_QUOTE_MODIFY                   0x17
#define CFETC_DD_QUOTE_ACCEPT                   0x18
#define CFETC_DD_QUOTE_REJECT                   0x19
#define CFETC_SUBSCRIBE_LIST                    0x1a
#define CFETC_SUBSCRIBE_SNAPSHOT                0x1b
#define CFETC_DD_REGISTER                       0x1c
#define CFETC_DD_DEREGISTER                     0x1d
#define CFETC_EXCHANGE_LIST_ACCOUNTS            0x36 /**< Request a list of accounts for the exchange */
#define CFETC_EXCHANGE_GET_ACCOUNT              0x37 /**< Request the details of a specific exchange account */
#define CFETC_CREATE_2WAY_ORDER                 0x42 /**< Request to create a 2-way order */
#define CFETC_MODIFY_2WAY_ORDER                 0x43 /**< Request to modify a 2-way order */
#define CFETC_DELETE_2WAY_ORDER                 0x44 /**< Request to delete a 2-way order */
#define CFETC_MARKET_MAKE_IT_MINE               0x45 /**< Request to take control of a propagated market */

/*
 * Commands issued by eSpeed API 
 */
#define CFETI_LOGIN_ACCEPTED                    0x1
#define CFETI_LOGIN_REJECTED                    0x2
#define CFETI_LOGIN_TERMINATED                  0x3
#define CFETI_MKT_ACCEPTED                      0x4
#define CFETI_MKT_REJECTED                      0x5
#define CFETI_MKT_EXECUTED                      0x6
#define CFETI_MKT_CANCELLED                     0x7
#define CFETI_CANCEL_MARKET_ACCEPTED            0x8
#define CFETI_CANCEL_MARKET_REJECTED            0x9
#define CFETI_ORDER_QUEUED                      0xa
#define CFETI_ORDER_REJECTED                    0xb
#define CFETI_ORDER_EXECUTED                    0xc
#define CFETI_ORDER_CANCELLED                   0xd
#define CFETI_TRADE_CONFIRM                     0xe
#define CFETI_CANCEL_ORDER_ACCEPTED             0xf
#define CFETI_CANCEL_ORDER_REJECTED             0x10
#define CFETI_QUERY_ACCEPTED                    0x11
#define CFETI_QUERY_REJECTED                    0x12
#define CFETI_SUBSCRIBE_ACCEPTED                0x13
#define CFETI_SUBSCRIBE_REJECTED                0x14
#define CFETI_UNSUBSCRIBE_ACCEPTED              0x15
#define CFETI_UNSUBSCRIBE_REJECTED              0x16
#define CFETI_UPDATE                            0x17
#define CFETI_CONNECTION_ACCEPTED               0x18
#define CFETI_CONNECTION_REJECTED               0x19
#define CFETI_CONNECTION_TERMINATED             0x1a
#define CFETI_DISCONNECT_ACCEPTED               0x1b
#define CFETI_DISCONNECT_REJECTED               0x1c
#define CFETI_STATUS                            0x1d
#define CFETI_LOGOUT_ACCEPTED                   0x1e
#define CFETI_LOGOUT_REJECTED                   0x1f
#define CFETI_SUBSCRIBE_STATUS                  0x20
#define CFETI_MKT_REFRESH                       0x21
#define CFETI_ORDER_REFRESH                     0x22
#define CFETI_MKT_CREATED                       0x23
#define CFETI_REFRESH_COMPLETE                  0x24
#define CFETI_ORDER_TRIGGERED                   0x25
#define CFETI_TRADE_COMPLETE                    0x26
#define CFETI_CANCEL_ALL_COMPLETE               0x27
#define CFETI_CONNECTION_MODIFIED               0x29
#define CFETI_INST_QUERY_COMPLETE               0x2a
#define CFETI_PEER_LOGOUT_ACCEPTED              0x2c
#define CFETI_PEER_LOGOUT_REJECTED              0x2d
#define CFETI_INSTRUMENT_NOTIFY                 0x35
#define CFETI_GIVEUP_MATRIX_INITIAL_IMAGE       0x36
#define CFETI_GIVEUP_MATRIX_UPDATE              0x37
#define CFETI_GIVEUP_MATRIX_DELETE              0x38
#define CFETI_GIVEUP_MATRIX_RECEIVED            0x39
#define CFETI_GIVEUP_MATRIX_CHANGED             0x3a
#define CFETI_GIVEUP_MATRIX_DELETED             0x3b
#define CFETI_MKT_NOT_EXECUTED                  0x3c
#define CFETI_INQUIRE_DEPTH_ACCEPTED            0x3f
#define CFETI_INQUIRE_DEPTH_REJECTED            0x40
#define CFETI_TRADE_PENDING                     0x41
#define CFETI_TRADE_REJECTED                    0x42
#define CFETI_SET_PASSWORD_ACCEPTED             0x43
#define CFETI_SET_PASSWORD_REJECTED             0x44
#define CFETI_TRADE_CHECKOUT_QUEUED             0x45 /**< Trade checkout request is acknowledged */
#define CFETI_TRADE_CHECKOUT_ACCEPTED           0x46 /**< Trade checkout request is accepted */
#define CFETI_TRADE_CHECKOUT_REFUSED            0x47 /**< Trade checkout request is refused */
#define CFETI_TRADE_CHECKOUT_REJECTED           CFETI_TRADE_CHECKOUT_REFUSED /**< @deprecated Use CFETI_TRADE_CHECKOUT_REFUSED */
#define CFETI_TRADE_REFUSE_QUEUED               0x4c /**< Trade refusal request is acknowledged */
#define CFETI_TRADE_REFUSE_ACCEPTED             0x4d /**< Trade refusal request is accepted */
#define CFETI_TRADE_REFUSE_REJECTED             0x4e /**< Trade refusal request is rejected */
#define CFETI_DD_REQUEST_SUBMIT_ACCEPTED        0x4f
#define CFETI_DD_REQUEST_SUBMIT_REJECTED        0x50
#define CFETI_DD_REQUEST_CANCEL_ACCEPTED        0x51
#define CFETI_DD_REQUEST_CANCEL_REJECTED        0x52
#define CFETI_DD_REQUEST_MODIFY_ACCEPTED        0x53
#define CFETI_DD_REQUEST_MODIFY_REJECTED        0x54
#define CFETI_DD_REQUEST_CANCELLED              0x55
#define CFETI_DD_REQUEST_EXECUTED               0x56
#define CFETI_DD_QUOTE_SUBMIT_ACCEPTED          0x57
#define CFETI_DD_QUOTE_SUBMIT_REJECTED          0x58
#define CFETI_DD_QUOTE_CANCEL_ACCEPTED          0x59
#define CFETI_DD_QUOTE_CANCEL_REJECTED          0x5a
#define CFETI_DD_QUOTE_MODIFY_ACCEPTED          0x5b
#define CFETI_DD_QUOTE_MODIFY_REJECTED          0x5c
#define CFETI_DD_QUOTE_CANCELLED                0x5d
#define CFETI_DD_QUOTE_EXECUTED                 0x5e
#define CFETI_DD_QUOTE_ACCEPTED                 0x5f
#define CFETI_DD_QUOTE_REJECTED                 0x60
#define CFETI_DD_ACCEPT_QUOTE_ACCEPTED          0x61
#define CFETI_DD_ACCEPT_QUOTE_REJECTED          0x62
#define CFETI_DD_REJECT_QUOTE_ACCEPTED          0x63
#define CFETI_DD_REJECT_QUOTE_REJECTED          0x64
#define CFETI_DD_SUBMIT_TO_MARKET_ACCEPTED      0x65
#define CFETI_DD_SUBMIT_TO_MARKET_REJECTED      0x66
#define CFETI_QUERY_INVALIDATED                 0x67
#define CFETI_TRADE_QUERY_COMPLETE              0x69
#define CFETI_SUBSCRIBE_LIST_ACCEPTED           0x6a
#define CFETI_SUBSCRIBE_LIST_REJECTED           0x6b
#define CFETI_SUBSCRIBE_SNAPSHOT_ACCEPTED       0x6c
#define CFETI_SUBSCRIBE_SNAPSHOT_REJECTED       0x6d
#define CFETI_DD_REQUEST_CREATED                0x6e
#define CFETI_DD_REQUEST_MODIFIED               0x6f
#define CFETI_DD_REQUEST_TRADED                 0x70
#define CFETI_MKT_MOVED                         0x71 /**< The lock price of the benchmark to which the instrument is linked has been modified */
#define CFETI_MKT_MODIFIED                      CFETI_MKT_MOVED /**< Market has been modified by the system */
#define CFETI_ORDER_HELD                        0x72 /**< The order is held waiting for client checkout. */
#define CFETI_ORDER_UNHELD                      0x73 /**< The order is no longer held */
#define CFETI_TRADE_CHECKEDOUT                  0x74 /**< The order is checked out */
#define CFETI_DD_QUOTE_CREATED                  0x7a
#define CFETI_DD_QUOTE_MODIFIED                 0x7b
#define CFETI_DD_QUOTE_DELETED                  0x7c
#define CFETI_DD_SYSTEM_MESSAGE                 0x7d
#define CFETI_DD_REGISTER_ACCEPTED              0x7e /**< Direct Dealing registration request was accepted */
#define CFETI_DD_REGISTER_REJECTED              0x7f /**< Direct Dealing registration request was rejected */
#define CFETI_ORDER_BROKER_HELD                 0x80 /**< The order is held waiting for broker checkout */
#define CFETI_DD_SET_LEGAL_ENTITY_LIST          0x81 /**< List of legal entities for Direct Dealing Callout */
#define CFETI_DD_SET_SYSTEM_ATTRIBUTES          0x82 /**< Set Direct Dealing system attributes for this trading session */
#define CFETI_DD_UPDATE_QUOTE_RANK              0x86 /**< Update the rank information should be updated for the Quotes against the Direct Dealing Request this message represents */
#define CFETI_TRADING_SESSION_STATUS_NOTIFICATION 0x87 /**< The status of the current trading session */
#define CFETI_DD_REQUEST_REFRESH                0x9e /**< Refresh client view of this Direct Dealing request */
#define CFETI_DD_QUOTE_REFRESH                  0x9f /**< Refresh client view of this Direct Dealing quote */
#define CFETI_UNUSED_1                          0xa3 /**< Was: Notification of a priority change */
#define CFETI_SET_EXCHANGE_INFO                 0xa6 /**< The cmdData parameter contains exchange info */
#define CFETI_MARKET_AVAILABILITY_NOTIFICATION  0xb5 /**< Market availability change, data structure delivered in CFETI_MARKET_AVAILABILITY_DESC */
#define CFETI_MKT_EXECUTING                     0xb6 /**< An execution will be delivered for this market at this or a better price for the size indicated in the SIZE_DONE field */
#define CFETI_ORDER_EXECUTING                   0xb7 /**< An execution will be delivered for this order at this or a better price for the size indicated in the SIZE_DONE field */
#define CFETI_MKT_CANCEL_QUEUED                 0xb8 /**< The cancel request is acknowledged by the trading system but cannot be processed at this time */
#define CFETI_ORDER_CANCEL_QUEUED               0xb9 /**< The cancel request is acknowledged by the trading system but cannot be processed at this time */
#define CFETI_EXCHANGE_LIST_ACCOUNTS_ACCEPTED   0xbb /**< Exchange account list request accepted */
#define CFETI_EXCHANGE_LIST_ACCOUNTS_REJECTED   0xbc /**< Exchange account list request rejected */
#define CFETI_EXCHANGE_GET_ACCOUNT_ACCEPTED     0xbd /**< Exchange account details request accepted */
#define CFETI_EXCHANGE_GET_ACCOUNT_REJECTED     0xbe /**< Exchange account details request accepted */
#define CFETI_PRIVATE_UPDATE                    0xbf /**< Private market data update delivered only to specific users in the admin channel */
#define CFETI_QUERY_SUBSCRIBED_USERS_ACCEPTED   0xc0 /**< The Query Subscribed user request has been accepted by the auth manager */
#define CFETI_SUBSCRIBE_USER_ACCEPTED           0xc2 /**< The subscribe user request has been accepted by the trading system */
#define CFETI_SUBSCRIBE_USER_REJECTED           0xc3 /**< The subscribe user request has been rejected by the trading system */
#define CFETI_SUBSCRIBE_USER_UPDATE             0xc4 /**< The trading system has updated a subscribed users connection status */
#define CFETI_UNSUBSCRIBE_USER_ACCEPTED         0xce /**< The unsubscribe user request has been accepted by the trading system */
#define CFETI_UNSUBSCRIBE_USER_REJECTED         0xcf /**< The unsubscribe user request has been rejected by the trading system */
#define CFETI_CREATE_2WAY_ORDER_ACCEPTED        0xde /**< Request to create a 2-way order was accepted */
#define CFETI_CREATE_2WAY_ORDER_REJECTED        0xdf /**< Request to create a 2-way order was rejected */
#define CFETI_MODIFY_2WAY_ORDER_ACCEPTED        0xe0 /**< Request to modify a 2-way order was accepted */
#define CFETI_MODIFY_2WAY_ORDER_REJECTED        0xe1 /**< Request to modify a 2-way order was rejected */
#define CFETI_DELETE_2WAY_ORDER_ACCEPTED        0xe2 /**< Request to delete a 2-way order was accepted */
#define CFETI_DELETE_2WAY_ORDER_REJECTED        0xe3 /**< Request to delete a 2-way order was rejected */
#define CFETI_MKT_MAKE_IT_MINE_ACCEPTED         0xe4 /**< Request to take control of a propagated market is accepted */
#define CFETI_MKT_MAKE_IT_MINE_REJECTED         0xe5 /**< Request to take control of a propagated market is rejected */
#define CFETI_2WAY_ORDER_MODIFIED               0xe6 /**< Attributes of a 2-way order have been modified (e.g. order id of propagated markets) */
#define CFETI_2WAY_ORDER_SUSPENDED              0xe7 /**< Market conditions are such that propagation from the 2-way order are suspended */
#define CFETI_2WAY_ORDER_RESUMED                0xe8 /**< Market conditions are such that propagation can be resumed for a previously suspended 2-way order */
#define CFETI_SET_CHECKOUT_BUSINESS_INFO        0xe9 /**< To deliver business info data structure to the client app TS connection callback */
#define CFETI_CREDIT_STATUS_UPDATE              0xfc /**< Credit status update */

/*
 * Last command 
 */
#define CFETI_LAST_COMMAND                      0x104

/*
 * Login/Connection Preferences - can be combined (|) 
 */
#define CFETI_RETAIN_MKTS_ON_CLOSE              0x1
#define CFETI_RETAIN_ORDERS_ON_CLOSE            0x2

/*
 * Market and Order indicators 
 */
#define CFETI_MARKET_BID                        0x1
#define CFETI_MARKET_ASK                        0x2
#define CFETI_ORDER_SELL                        0x3
#define CFETI_ORDER_HIT                         CFETI_ORDER_SELL
#define CFETI_ORDER_RECEIVE                     CFETI_ORDER_SELL
#define CFETI_ORDER_BUY                         0x4
#define CFETI_ORDER_TAK                         CFETI_ORDER_BUY
#define CFETI_ORDER_PAY                         CFETI_ORDER_BUY
#define CFETI_BLOCK_BUY                         0x5
#define CFETI_BLOCK_SELL                        0x6
#define CFETI_BLOCK_BUY_SELL                    0x7
#define CFETI_ORDER_BUY_SELL                    0x8
#define CFETI_DD_BID                            0xb
#define CFETI_DD_ASK                            0xc
#define CFETI_DD_BUY                            0xd
#define CFETI_DD_SELL                           0xe
#define CFETI_DD_2WAY                           0xf
#define CFETI_DD_BOLS_2WAY                      0x10
#define CFETI_DD_2WAY_QUOTE                     0x11
#define CFETI_DD_BOLS_2WAY_QUOTE                0x12
#define CFETI_LAST_ORDER_FLAG                   0x12

/*
 * Trade side flags 
 */
#define CFETI_TRADE_PASSIVE                     0x1
#define CFETI_TRADE_ACTIVE                      0x2

/*
 * Market and Order preferences - bitmask 
 */
#define CFETI_MARKET_LIMIT_PRICE                0x1
#define CFETI_MARKET_SIZE_IS_INCREMENTAL        0x2
#define CFETI_MARKET_SIZE_IS_TOTAL              0x4
#define CFETI_MARKET_ALL_OR_NONE                0x8
#define CFETI_MARKET_CANCEL_ALL_FOR_ISSUE       0x10
#define CFETI_ORDER_COMPLETE_FILL_ONLY          0x20
#define CFETI_TRADE_AT_MARKET_PRICE             0x40
#define CFETI_TRADE_AT_MARKET_SIZE              0x80
#define CFETI_ORDER_CANCEL_ALL_FOR_ISSUE        0x100
#define CFETI_ORDER_SIZE_IS_INCREMENTAL         0x200
#define CFETI_ORDER_SIZE_IS_TOTAL               0x400
#define CFETI_ORDER_IS_TRIGGERED                0x800
#define CFETI_PRICE_IMPROVEMENT                 0x1000
#define CFETI_MARKET_INDICATIVE                 0x2000
#define CFETI_TOLERANCE_SET                     0x4000 /**< Market is specified with upper and lower tolerance values in altPrice1 and altPrice2 */
#define CFETI_MARKET_GOOD_TILL_CANCEL           0x8000
#define CFETI_ORDER_STOP                        0x10000
#define CFETI_ORDER_MIT                         0x20000
#define CFETI_TRADE_OPTION_1                    0x40000
#define CFETI_TRADE_OPTION_2                    0x80000
#define CFETI_TRADE_OPTION_3                    0x100000
#define CFETI_MARKET_CANCELS_MARKET             0x200000 /**< The submitted market cancels an existing market specified in the order id field of the submitted market */
#define CFETI_MARKET_DISABLE_DERIVATION         0x400000 /**< Derivation by TopSpeed from the submitted market should be prevented */
#define CFETI_MARKET_NO_RESPONSE                0x1000000
#define CFETI_MARKET_CANCEL_ALL_FOR_ISSUE_SAME_SIDE 0x2000000
#define CFETI_AGGRESS_WHEN_TOUCHED              0x4000000 /**< Market will automatically execute a contra market that is clearing to it if that unclear market is placed in a when-hit/when-take state */
#define CFETI_MARKET_PROPAGATED                 0x8000000 /**< Market is propagated from a 2-way order */
#define CFETI_AGGRESS_IF_LOCKED                 0x10000000 /**< Market will be promoted to aggressor if it is locked */
#define CFETI_MARKET_REDUCE_SIZE                0x20000000 /**< Modifier for market-cancels-market to reduce the new market by partially executed size */
#define CFETI_ORDER_PREF_NO_MORE                0x40000000 /**< No more size is required than the amount requested */
#define CFETI_ONLY_AT_BEST                      0x80000000 /**< Market or order shall work only at the best price available */

/*
 * Additional Market and Order preferences: preferences2 - bitmask 
 */
#define CFETI_GOOD_UNTIL_TIME                   0x1 /**< Market or order is good for a specified time */
#define CFETI_USE_RESERVE_SIZE                  0x2 /**< Market or order is specified with total size and min/max clip size */
#define CFETI_NO_BOND_CROSS                     0x10 /**< Customer has selected that there shall be no bond cross */

/*
 * Synonyms for trade options (European Repos) 
 */
#define CFETI_TRADE_CLEARING_LCH                CFETI_TRADE_OPTION_1
#define CFETI_TRADE_CLEARING_RESTRICTED         CFETI_TRADE_OPTION_2
#define CFETI_TRADE_CLEARING_CLEARNET           CFETI_TRADE_OPTION_2 /**< @depricated Use CFETI_TRADE_CLEARING_RESTRICTED instead */
#define CFETI_TRADE_CLEARING_INTERBANK          CFETI_TRADE_OPTION_3

/*
 * Synonyms for trade options (IRS). LCH and INTERBANK options are retained
 * from European REPOs. OPTION_2 is redefined as DERIVED_SPREAD 
 */
#define CFETI_TRADE_DERIVED_SPREAD              CFETI_TRADE_OPTION_2

/*
 * eSpeed API command status and error codes 
 */
#define CFETI_INVALID_CREDENTIALS               0x1
#define CFETI_USERNAME_CURRENTLY_CONNECTED      0x2
#define CFETI_FORCED_LOGOFF                     0x3
#define CFETI_UNKNOWN_TRD_INST                  0x4
#define CFETI_INVALID_AUTH                      0x5
#define CFETI_INVALID_MARKET                    0x6
#define CFETI_TRADE_IN_PROGRESS                 0x7
#define CFETI_MKT_BESTED                        0x8
#define CFETI_MKT_CUT                           0x9
#define CFETI_MKT_CLEARED                       0xa
#define CFETI_INSUFFICIENT_TIME                 0xb
#define CFETI_TRADE_SYS_OFFLINE                 0xc
#define CFETI_TRADE_SYS_ONLINE                  0xd
#define CFETI_FORCED_DISCONNECT                 0xe
#define CFETI_USERNAME_CURRENTLY_NOT_CONNECTED  0xf
#define CFETI_INVALID_SYNTAX                    0x10
#define CFETI_TRD_INST_NOT_SUBSCRIBED           0x11
#define CFETI_SESSION_LOST                      0x12
#define CFETI_SESSION_RESTORED                  0x13
#define CFETI_INSTRUMENT_LOST                   0x14
#define CFETI_INSTRUMENT_RESTORED               0x15
#define CFETI_MARKET_FEED_DOWN                  0x16
#define CFETI_MARKET_FEED_RESTORED              0x17
#define CFETI_INTERNAL_ERROR                    0x18
#define CFETI_INVALID_PREFERENCES               0x19
#define CFETI_PREFERENCES_NOT_SUPPORTED         0x1a
#define CFETI_CANNOT_AUTHENTICATE               0x1b
#define CFETI_INVALID_ORDER                     0x1c
#define CFETI_TRADING_SYSTEM_ERROR              0x1d
#define CFETI_MKT_CANCELLED_BY_SYSTEM           0x1e
#define CFETI_MKT_CANCELLED_BY_BROKER           CFETI_MKT_CANCELLED_BY_SYSTEM
#define CFETI_ORDER_CANCELLED_BY_SYSTEM         0x1f
#define CFETI_ORDER_CANCELLED_BY_BROKER         CFETI_ORDER_CANCELLED_BY_SYSTEM
#define CFETI_REFRESH_FAILED                    0x20
#define CFETI_PERMISSION_DENIED                 0x21
#define CFETI_DATABASE_ERROR                    0x22
#define CFETI_TRADE_SUBSYSTEM_OFFLINE           0x23
#define CFETI_TRADE_SUBSYSTEM_ONLINE            0x24
#define CFETI_TRADING_SUSPENDED                 0x25
#define CFETI_TRANSACTION_NOT_APPROVED          0x26
#define CFETI_TRANSACTION_DISAPPROVED           0x27
#define CFETI_INSTRUMENT_NOT_TRADEABLE          0x28
#define CFETI_MAX_USERDATA_SIZE_EXCEEDED        0x29
#define CFETI_SERVICE_NOT_AVAILABLE             0x2a
#define CFETI_INVALID_SHORTCODE                 0x2b
#define CFETI_INVALID_REQUEST_ID                0x2c
#define CFETI_INVALID_ID                        0x2d
#define CFETI_TRADE_CONTROL_FLAGS_MODIFIED      0x2e
#define CFETI_ACTION_PROHIBITED                 0x2f
#define CFETI_LOGGED_OUT_BY_PEER                0x30
#define CFETI_CONNECTION_TIMEOUT                0x31
#define CFETI_NO_REGISTERED_SOCKETS_ERROR       0x32 /**< Socket factory not initialized */
#define CFETI_SOCKET_TYPE_ERROR                 0x33 /**< Encryption handshake procedure failed */
#define CFETI_UNSUPPORTED_SOCKET_TYPE_ERROR     0x34 /**< Encryption type not supported by client or server */
#define CFETI_NO_CREDIT                         0x35
#define CFETI_OPERATION_NOT_SUPPORTED           0x36
#define CFETI_INVALID_PARAMETER                 0x3b
#define CFETI_CHECK_CREDIT                      0x3c
#define CFETI_RSASSL_MODE_NOT_AVAILABLE         0x3d
#define CFETI_NEW_PASSWORD_INVALID              0x3e
#define CFETI_NEW_PASSWORD_NOT_MATCHED          0x3f
#define CFETI_MAX_INSTRUMENTS_EXCEEDED          0x40
#define CFETI_CONNECTION_MODE_NOT_VALID         0x41
#define CFETI_CONNECTION_MODE_SELECT_FAILED     0x42
#define CFETI_CONNECTION_IN_PROGRESS            0x44 /**< Connection to the session manager in progress */
#define CFETI_NOT_PERMISSIONED_FOR_TRADING_SYSTEM 0x45 /**< User is not permissioned for the trading system */
#define CFETI_USER_NOT_AUTHENTICATED            0x46 /**< User has not authenticated / connected to the trading system */
#define CFETI_TRADING_SYSTEM_NOT_AVAILABLE      0x47 /**< The trading system is known but not currently available */
#define CFETI_LOGOUT_IN_PROGRESS                0x48 /**< Logout has already been requested */
#define CFETI_ALREADY_SUBSCRIBED                0x49 /**< Instrument has already been subscribed to */
#define CFETI_PRIOR_CONNECTION_IN_PROGRESS      0x4a /**< A prior connection to the session manager in still in progress */
#define CFETI_MARKET_WOULD_BACKWARDATE          0x4b /**< Tried to enter a market with bid price > offer */
#define CFETI_NOT_BEST_PRICE                    0x4c /**< Tried to enter a market below the current best price */
#define CFETI_SAME_QUANTITY_SPECIFIED           0x4d /**< Tried to modify a market but size specified was the same as current size */
#define CFETI_PRICE_CHANGE_TOO_GREAT            0x4e /**< Tried to modify a market but the price entered was too far from current price */
#define CFETI_QUANTITY_TOO_SMALL                0x4f /**< Tried to enter a market with a size below minimum */
#define CFETI_NO_MARKET                         0x50 /**< Tried to enter an order when there is no market to trade with */
#define CFETI_INVALID_SWAP_NUMBER               0x51 /**< Invalid Treasury Swap PV01 weighting. The weighting value must be greater than zero */
#define CFETI_INVALID_LOCK_PRICE                0x52 /**< The specified lock price is not a valid price */
#define CFETI_INVALID_CURRENT_LOCK_PRICE        0x53 /**< The specified lock price does not match the prevailing lock price */
#define CFETI_INVALID_CURRENT_SWAP_NUMBER       0x54 /**< Invalid value given for the current PV01 weighting value */
#define CFETI_MARKET_NOT_BEST_PRICE             0x55 /**< The benchmark lock price has changed and the market has been cancelled by the system because it is no longer at the best price */
#define CFETI_INVALID_PI_INCREMENT              0x56 /**< The number of increments specified in the market or order for price improvement falls outside of the defined bounds */
#define CFETI_PI_NOT_SPECIFIED                  0x57 /**< A market or order is specified with a number of price improvement increments to apply but price improvement is not specified */
#define CFETI_ACTION_NOT_PERMISSIONED           0x58 /**< The post action was not permissioned for the posting user id */
#define CFETI_INDICATIVE_PRICE_NOT_TRADABLE     0x59 /**< The aggressed market is indicative only */
#define CFETI_DD_REQUEST_EXPIRED                0x5a /**< The DD Request has expired - time limit reached */
#define CFETI_DD_QUOTE_EXPIRED                  0x5b /**< The DD Quote has expired - time limit reached */
#define CFETI_INDICATIVE_PRICE_TRADE_ATTEMPT    0x5d /**< Market cancelled because an attempt was made to agress it  */
#define CFETI_INDICATIVE_PRICE_INVERTED         0x5e /**< Market cancelled because indicative market is inverted with a tradable contra-market */
#define CFETI_DD_QUOTE_CANCELLED_BY_TRADE       0x60 /**< DD Quote is cancelled because DD Request has been traded */
#define CFETI_DD_REQUEST_INVALID_ID             0x61 /**< Invalid Direct Dealing Request id submitted */
#define CFETI_DD_NOT_OWNER                      0x62 /**< Not owner of this DD Request/Quote */
#define CFETI_DD_MAX_REACHED                    0x63 /**< Maximum number of DD Requests/Quotes reached */
#define CFETI_DD_OUTSTANDING_REQUEST            0x64 /**< Already have an outstanding DD Request/Quote */
#define CFETI_DD_INVALID_PARAMETERS             0x65 /**< Values in DD Request/Quote specification are invalid */
#define CFETI_DD_INVALID_COMMAND                0x66 /**< Invalid Direct Deql Request/Quote command */
#define CFETI_DD_INVALID_COUNTERPARTY           0x67 /**< Cannot quote to that counterparty */
#define CFETI_DD_INVALID_INSTRUMENT             0x68 /**< Unknown instrument */
#define CFETI_DD_USER_NOT_REGISTERED            0x69 /**< User that issued Direct Dealing Request/Quote is not registered */
#define CFETI_DD_UNABLE_TO_COMPLETE             0x6a /**< Unable to commit Direct Dealing operation */
#define CFETI_DD_LOCKED_DOWN                    0x6b /**< No modifications allowed during lock down period */
#define CFETI_API_VERSION_TOO_OLD               0x6c /**< API Version does not support this feature */
#define CFETI_DD_TRADE_NOT_APPROVED             0x6d /**< The GiveUp Matrix does not approve a Direct Dealing trade */
#define CFETI_DATA_CONTAINS_NAN_VALUE           0x6e /**< A field in the command data contained a NaN (a double was not a number) */
#define CFETI_PRICE_SPREAD_EXCEEDED             0x6f /**< Tried to enter a market too far from the contra-market price */
#define CFETI_NEW_PASSWORD_REUSE_ERROR          0x74 /**< Password violates ageing rule */
#define CFETI_NEW_PASSWORD_INVALID_LENGTH       0x75 /**< Password violates password format rules for length */
#define CFETI_NEW_PASSWORD_INVALID_FORMAT       0x76 /**< Password violates password format rules for character types */
#define CFETI_NEW_PASSWORD_NOT_SAVED            0x77 /**< New password could not be stored */
#define CFETI_PASSWORD_EXPIRED                  0x79 /**< Password has expired */
#define CFETI_USER_ID_EXPIRED                   0x7a /**< The user id has expired */

/*
 * eSpeed API command status and error codes 
 */
#define CFETI_BASE_ERROR                        0x80
#define CFETI_NOMEMORY                          (CFETI_BASE_ERROR + 0x0)
#define CFETI_SESSION_EXISTS                    (CFETI_BASE_ERROR + 0x1)
#define CFETI_MAX_INSTANCE                      (CFETI_BASE_ERROR + 0x2)
#define CFETI_MAX_SESSION                       (CFETI_BASE_ERROR + 0x3)
#define CFETI_NO_SESSION                        (CFETI_BASE_ERROR + 0x4)
#define CFETI_ADD_LOGIN                         (CFETI_BASE_ERROR + 0x5)
#define CFETI_NO_MESSAGE                        (CFETI_BASE_ERROR + 0x6)
#define CFETI_NO_CALLBACK                       (CFETI_BASE_ERROR + 0x7)
#define CFETI_NO_SUCH_LOGIN                     (CFETI_BASE_ERROR + 0x8)
#define CFETI_ENQUEUE                           (CFETI_BASE_ERROR + 0x9)
#define CFETI_UNKNOWN_COMMAND                   (CFETI_BASE_ERROR + 0xa)
#define CFETI_LOGIN_EXISTS                      (CFETI_BASE_ERROR + 0xb)
#define CFETI_NO_SUCH_CONNECTION                (CFETI_BASE_ERROR + 0xc)
#define CFETI_UNEXPECTED_MESSAGE                (CFETI_BASE_ERROR + 0xd)
#define CFETI_SESSMGR_SPEC                      (CFETI_BASE_ERROR + 0xe)
#define CFETI_CONNECT_FAIL                      (CFETI_BASE_ERROR + 0xf)
#define CFETI_INVALID_ARG                       (CFETI_BASE_ERROR + 0x10)
#define CFETI_CONNECTION_NOT_ON_LINE            (CFETI_BASE_ERROR + 0x11)
#define CFETI_TIMED_OUT                         (CFETI_BASE_ERROR + 0x12)
#define CFETI_FIELD_NOT_ENCODED                 (CFETI_BASE_ERROR + 0x13)
#define CFETI_FIELD_DECODE_FAILED               (CFETI_BASE_ERROR + 0x14)
#define CFETI_SDK_EXPIRED                       (CFETI_BASE_ERROR + 0x15)
#define CFETI_USERDATA_TOO_LARGE                (CFETI_BASE_ERROR + 0x16)
#define CFETI_SHORTCODE_TOO_LONG                (CFETI_BASE_ERROR + 0x17)
#define CFETI_GIVEUP_NOT_ENABLED                (CFETI_BASE_ERROR + 0x18)
#define CFETI_LOGIN_HAS_BEEN_TERMINATED         (CFETI_BASE_ERROR + 0x19)
#define CFETI_UNKNOWN_COUNTERPARTY_ID           (CFETI_BASE_ERROR + 0x1a) /**< Unknown counterparty id */
#define CFETI_APPLICATION_VERSION_TOO_OLD       (CFETI_BASE_ERROR + 0x1f) /**< Login rejected because the clients application version has expired */
#define CFETI_APPLICATION_RIGHTS_NONE           (CFETI_BASE_ERROR + 0x20) /**< The user and the legal entity the user belong to have not the right to connect to eSpeed with that particular application */
#define CFETI_TOLERANCE_EXCEEDED                (CFETI_BASE_ERROR + 0x21) /**< Market was cancelled because a tolerance specified for its validity was exceeded */
#define CFETI_BASKET_MARKET_EXECUTED            (CFETI_BASE_ERROR + 0x22) /**< Market was cancelled or rejected because another in the basket was executed */
#define CFETI_BASKET_MARKET_EXECUTED_PARTIAL    (CFETI_BASE_ERROR + 0x23) /**< Market was cancelled or created because another in the basket was executed in partial size */
#define CFETI_BASKET_MARKET_FOUND               (CFETI_BASE_ERROR + 0x24) /**< Market was specified with new basket action but another market with the same basket id was found */
#define CFETI_BASKET_MARKET_NOT_FOUND           (CFETI_BASE_ERROR + 0x25) /**< Market was not specified with new basket action, but no market in the same basket was found */
#define CFETI_BASKET_TRADE_IN_PROGRESS          (CFETI_BASE_ERROR + 0x26) /**< A trade is in progress for another market in thesame basket */
#define CFETI_BASKET_ACTION_NOT_RECOGNISED      (CFETI_BASE_ERROR + 0x27) /**< Unknown basket action specified */
#define CFETI_BASKET_CANCELLED                  (CFETI_BASE_ERROR + 0x28) /**< Market was cancelled because user requested cancel of all markets in the basket */
#define CFETI_SM_NOT_ENABLED_FOR_TS             (CFETI_BASE_ERROR + 0x2a) /**< SessionMgr is disabled for this TSID */
#define CFETI_ORDER_STATE_ERROR                 (CFETI_BASE_ERROR + 0x38) /**< Order is rejected because new buy/sell requests cannot be accepted for an instrument in its current state */
#define CFETI_INVALID_STATE                     (CFETI_BASE_ERROR + 0x3c) /**< Invalid state for requested operation */
#define CFETI_EXCHANGE_NO_ACCOUNTS              (CFETI_BASE_ERROR + 0x3d) /**< No exchange accounts available for the user */
#define CFETI_EXCHANGE_ACCOUNT_NOT_FOUND        (CFETI_BASE_ERROR + 0x3e) /**< Specified exchange account was not found */
#define CFETI_REQUEST_IN_PROGRESS               (CFETI_BASE_ERROR + 0x3f) /**< Request can not be processed because a previous request is in progress */
#define CFETI_DD_REQUEST_SPREAD_MET             (CFETI_BASE_ERROR + 0x40) /**< There is a quote that meets the qualifying spread stipulated by the direct deal requestor */
#define CFETI_ALLOCATION_INFO_TOO_LONG          (CFETI_BASE_ERROR + 0x4b) /**< The allocation information is too long. */
#define CFETI_MATCHED                           (CFETI_BASE_ERROR + 0x4c) /**< Customer is guaranteed to trade from an identified match not yet exexcuted. */
#define CFETI_MATCHED_BETTER_FILL               (CFETI_BASE_ERROR + 0x4d) /**< Customer is guaranteed to trade from an identified match not yet exexcuted by trading through the stack. */
#define CFETI_SEEKING_BETTER_FILL               (CFETI_BASE_ERROR + 0x4e) /**< Customer is trading through the stack behind a trade through the stack already matched. Customer is next. */
#define CFETI_ATTEMPTING_SUBSTITUTION           (CFETI_BASE_ERROR + 0x4f) /**< Sent when market or order cancel is queued */
#define CFETI_SUBSTITUTED                       (CFETI_BASE_ERROR + 0x50) /**< Sent when a queued cancel is accepted. */
#define CFETI_INVALID_SPREAD                    (CFETI_BASE_ERROR + 0x51) /**< Specified spread is invalid */
#define CFETI_CANNOT_PROPAGATE                  (CFETI_BASE_ERROR + 0x52) /**< Cannot propagate from a 2-way order */
#define CFETI_FEATURE_DISABLED                  (CFETI_BASE_ERROR + 0x53) /**< Feature not currently enabled in the trading system */
#define CFETI_PARENT_DELETED                    (CFETI_BASE_ERROR + 0x54) /**< Market cancelled because 2-way order it was derived from has been deleted */
#define CFETI_PARENT_MODIFIED                   (CFETI_BASE_ERROR + 0x55) /**< Market modified/cancelled because 2-way order it was derived from has been deleted */
#define CFETI_EXPIRED_UPGRADE_REJECTED          (CFETI_BASE_ERROR + 0x56) /**< Login rejected as mandatory upgrade rejected. Application has expired. */
#define CFETI_UPGRADE_SETUP_NOT_RUN             (CFETI_BASE_ERROR + 0x57) /**< Login rejected as user has not run upgrade Setup */
#define CFETI_MANDATORY_UPGRADE_CONFIG_FAILURE  (CFETI_BASE_ERROR + 0x58) /**< Login rejected as a mandatory upgrade failed. Please contact operations */
#define CFETI_PRICE_LEVELS_EXCEEDED             (CFETI_BASE_ERROR + 0x59) /**< Do not immediately resubmit market at the same price */
#define CFETI_DERIVATION_MANDATORY              (CFETI_BASE_ERROR + 0x5a) /**< Derivation is mandatory for this security and cannot be disabled */
#define CFETI_ORDER_CONVERTED_TO_MARKET         (CFETI_BASE_ERROR + 0x5b) /**< Market-make rule invoked */
#define CFETI_PRIORITY_REVEND_DISABLED          (CFETI_BASE_ERROR + 0x5c) /**< System does not allow an acquirer of priority to become a priority vendor */
#define CFETI_MARKET_TO_FOLLOW                  (CFETI_BASE_ERROR + 0x5f) /**< Market or order cancelled - create will follow */
#define CFETI_INVALID_RESERVE_SIZE              (CFETI_BASE_ERROR + 0x60) /**< Clip size in reserve order was invalid */
#define CFETI_PRICE_WOULD_CROSS_MM              (CFETI_BASE_ERROR + 0x64) /**< Price would cross market-maker price */
#define CFETI_NO_SUCH_TRADING_SYSTEM            (CFETI_BASE_ERROR + 0x65) /**< TODO */
#define CFETI_NO_SUCH_INSTRUMENT                (CFETI_BASE_ERROR + 0x66) /**< TODO */

/*
 * Last command status 
 */
#define CFETI_LAST_COMMAND_STATUS               (CFETI_BASE_ERROR + 0x66)

/*
 * Basket Market Actions 
 */
#define CFETI_BASKET_ACTION_UNSPECIFIED         0x0
#define CFETI_BASKET_ACTION_NEW_BASKET          0x1 /**< Market is first in a new basket */
#define CFETI_BASKET_ACTION_CANCEL_ON_EXECUTE   0x2
#define CFETI_BASKET_ACTION_RETAIN_PARTIAL_SIZE 0x3
#define CFETI_BASKET_ACTION_CANCEL_ALL          0x4 /**< All markets with the same basket id should be cancelled */

/*
 * Field data types 
 */
#define CFETI_FIELDTYPE_INT8                    0x1
#define CFETI_FIELDTYPE_BYTE                    0x2
#define CFETI_FIELDTYPE_INT16                   0x3
#define CFETI_FIELDTYPE_UINT16                  0x4
#define CFETI_FIELDTYPE_INT32                   0x5
#define CFETI_FIELDTYPE_UINT32                  0x6
#define CFETI_FIELDTYPE_DATETIME                0x7
#define CFETI_FIELDTYPE_STRING                  0x8
#define CFETI_FIELDTYPE_BUFFER                  0x9
#define CFETI_FIELDTYPE_DECIMAL                 0xa
#define CFETI_FIELDTYPE_BYTESTREAM              0xf
#define CFETI_FIELDTYPE_EMPTY                   0x10

/*
 * Query types 
 */
#define CFETQ_UNKNOWN                           0x0
#define CFETQ_INSTRUMENT                        0x1
#define CFETQ_TRADE                             0x2
#define CFETQ_USERLIST                          0x3
#define CFETQ_TRADE_LE                          0x4
#define CFETQ_COMMISSION_TABLE                  0x5
#define CFETQ_CALCULATION                       0x9

/*
 * Instrument Notification Message Types 
 */
#define CFETI_INSTRUMENT_NEW                    0x0
#define CFETI_INSTRUMENT_DELETE                 0x1
#define CFETI_INSTRUMENT_AUTO_ADD               0x2
#define CFETI_INSTRUMENT_AUTO_DELETE            0x3
#define CFETI_INSTRUMENT_ADVISORY               0x4
#define CFETI_INSTRUMENT_MKT_EXPIRE             0x5

/*
 * Maximum number of bytes of user data in markets/orders 
 */
#define CFETI_MAX_USER_DATA_SIZE                8

/*
 * Maximum length of short code in markets/orders 
 */
#define CFETI_MAX_SHORTCODE_LENGTH              10

/*
 * Maximum number of instruments in a list subscribe request 
 */
#define CFETI_MAX_SUBSCRIBE_LIST_INSTRUMENTS    64

/*
 * Maximum length of free text allocation information in the allocation field
 * 
 */
#define CFETI_MAX_ALLOCATION_INFO_SIZE          65

/*
 * Price tick rules 
 */
#define CFETI_FIXED                             0x1
#define CFETI_USREPO1                           0x2
#define CFETI_BRADY1                            0x3

/*
 * Trade control flags 
 */
#define CFETI_BID_DISABLED                      0x1
#define CFETI_ASK_DISABLED                      0x2
#define CFETI_BUY_DISABLED                      0x4
#define CFETI_SELL_DISABLED                     0x8
#define CFETI_BLOCK_TRADE_PRINCIPAL             0x10
#define CFETI_BLOCK_TRADE_AGENT                 0x20
#define CFETI_QUERY_DISABLED                    0x40
#define CFETI_SUBSCRIBE_DISABLED                0x80
#define CFETI_GIVEUP_ENABLED                    0x100
#define CFETI_DIRECT_DEALING_ENABLED            0x200
#define CFETI_PRICE_IMPROVEMENT_ENABLED         0x4000
#define CFETI_CHECKOUT_CLIENT_USER              0x10000 /**< User is a brokered user on the trading system and can checkout trades */
#define CFETI_ALLOW_SELECT_DERIVATION           0x20000 /**< User is allowed to stop derivation of his price */
#define CFETI_LINKED_MARKETS_ENABLED            0x40000 /**< Linked markets are supported by the trading system */
#define CFETI_TC_INSTRUMENT_DATA_ENABLED        0x200000 /**< The trading system will deliver instrument data in trade confirms */
#define CFETI_EXCHANGE_INTERFACE                0x40000000 /**< Trading Interface to an Exchange */

/*
 * Trading System Properties 
 */
#define CFETI_CHECKOUT_ALLOCATION_ENABLED       0x1 /**< Trading system supports inclusion of allocation instructions in checkout requsts. */
#define CFETI_ORDER_ALLOCATION_ENABLED          0x2 /**< Trading system supports inclusion of allocation instructions in market/order requests. */
#define CFETI_2WAY_ORDERS_ENABLED               0x4 /**< Trading system supports two-way orders. */
#define CFETI_PI_NEUTRAL_ENABLED                0x8 /**< PI Neutral is enabled for this trading session. */
#define CFETI_LOCKED_MARKETS_ENABLED            0x10 /**< Locked markets are enabled for this trading session */
#define CFETI_MARKET_MAKER_SESSION              0x20 /**< This trading session is designated to be a market-maker session */
#define CFETI_RESERVE_ENABLED                   0x80 /**< This trading session supports the reserve order type */
#define CFETI_TRACE_ROUTE_SUPPORTED             0x100 /**< Trading system supports Trace Route */

/*
 * Values for settlement type in CFETI_ORDER_DESC structure 
 */
#define CFETI_SETTLEMENT_MARKET_DEFAULT         0x0 /**< Market default settlement */
#define CFETI_SETTLEMENT_UNKNOWN                CFETI_SETTLEMENT_MARKET_DEFAULT /**< For backwards compatibility */
#define CFETI_SETTLEMENT_ONEDAY                 0x1
#define CFETI_SETTLEMENT_TWODAY                 0x2
#define CFETI_SETTLEMENT_THREEDAY               0x3
#define CFETI_SETTLEMENT_REPOCLEAR              0x4
#define CFETI_SETTLEMENT_RESTRICTED             0x5
#define CFETI_SETTLEMENT_CLEARNET               CFETI_SETTLEMENT_RESTRICTED /**< @depricated Use CFETI_SETTLEMENT_RESTRICTED instead */
#define CFETI_SETTLEMENT_REPORT_ONLY            0x6
#define CFETI_SETTLEMENT_REGULAR                0x7
#define CFETI_SETTLEMENT_CASH                   0x8
#define CFETI_SETTLEMENT_CORP                   0x9
#define CFETI_SETTLEMENT_SKIP                   0xa
#define CFETI_SETTLEMENT_WHEN_ISSUE             0xb
#define CFETI_SETTLEMENT_INTERBANK              0xc
#define CFETI_SETTLEMENT_VMAC                   0xd
#define CFETI_SETTLEMENT_NET                    0xe
#define CFETI_SETTLEMENT_CLS                    0xf
#define CFETI_SETTLEMENT_NOSTRO                 0x10
#define CFETI_SETTLEMENT_NYMEX                  0x11
#define CFETI_SETTLEMENT_CREST                  0x12
#define CFETI_SETTLEMENT_EUROCLEAR              0x13
#define CFETI_SETTLEMENT_CEDEL                  0x14
#define CFETI_SETTLEMENT_KASSENVEREIN           0x15
#define CFETI_SETTLEMENT_CLEARSTREAM            0x16
#define CFETI_SETTLEMENT_ITALIAN_DOMESTIC       0x17
#define CFETI_SETTLEMENT_BELGIAN_DOMESTIC       0x18
#define CFETI_SETTLEMENT_GREEK_DOMESTIC         0x19

/*
 * Values for instrument id type in CFETI_ORDER_DESC structure 
 */
#define CFETI_INSTRUMENT_ID_UNKNOWN             0
#define CFETI_INSTRUMENT_ID_CUSIP               1 /**< Committee on Uniform Securities Identification Procedures Number */
#define CFETI_INSTRUMENT_ID_ISIN                2 /**< International Securities Identification Number */
#define CFETI_INSTRUMENT_ID_CFID                3 /**< eSpeed Instrument Identifier */
#define CFETI_INSTRUMENT_ID_SYMBOL              4 /**< eSpeed Instrument Symbol */

/*
 * Values for trade info type component of CFETI_ORDER_DESC structure 
 */
#define CFETI_TRADEINFO_NONE                    0 /**< No additional trade info */
#define CFETI_TRADEINFO_USREPO                  1 /**< US Repo specific details */

/*
 * Values for order info type component of CFETI_ORDER_DESC structure 
 */
#define CFETI_ORDERINFO_NOT_SPECIFIED           0
#define CFETI_ORDERINFO_BLOCK_TRADE             1 /**< Block trade support */
#define CFETI_ORDERINFO_EXTENDED_PROPERTIES     4
#define CFETI_ORDERINFO_ENERGY_TRADE            5
#define CFETI_ORDERINFO_FX_TRADE                6
#define CFETI_ORDERINFO_TSWAP                   8
#define CFETI_ORDERINFO_PV01_LOCK               CFETI_ORDERINFO_TSWAP /**< @Indicates that the orderInfo pointer is of type CFETI_PV01_LOCK_DESC */
#define CFETI_ORDERINFO_MMTS                    9
#define CFETI_ORDERINFO_FX_OPTION_TRADE         10
#define CFETI_ORDERINFO_CANTOR_REPO             11
#define CFETI_ORDERINFO_IRD_TRADE               12
#define CFETI_ORDERINFO_ESPD_REPO_TRADE         13
#define CFETI_ORDERINFO_ESPD_REPO               14

/*
 * Values for ird trade leg of ird trade order info type 
 */
#define CFETI_IRD_PRODUCT_TYPE_IRS              1
#define CFETI_IRD_PRODUCT_TYPE_OIS              2
#define CFETI_IRD_PRODUCT_TYPE_FRA              3
#define CFETI_IRD_PRODUCT_TYPE_SWAPTION         4
#define CFETI_IRD_PRODUCT_TYPE_CAPSFLOORS       5

/*
 * Commission types 
 */
#define CFETI_COMMISSION_NOT_SPECIFIED          0
#define CFETI_COMMISSION_FLAT_FEE               1
#define CFETI_COMMISSION_ADJUSTED               2

/*
 * Commission table entry types 
 */
#define CFETI_COMMISSION_TYPE_NOT_SPECIFIED     0
#define CFETI_COMMISSION_TYPE_WHEN_ISSUE        1
#define CFETI_COMMISSION_TYPE_BILL              2

/*
 * Values for counterparty state CFETI_COUNTERPARTY_STATE 
 */
#define CFETI_COUNTERPARTY_STATE_NOT_APPLICABLE 0x0
#define CFETI_COUNTERPARTY_STATE_NO_CREDIT      0x1
#define CFETI_COUNTERPARTY_STATE_TRADABLE       0x2
#define CFETI_COUNTERPARTY_STATE_PRICE_NOT_AVAILABLE 0x3
#define CFETI_COUNTERPARTY_STATE_UNKNOWN        0x4

/*
 * Enumerated FX Product Type (FX trade confirm) 
 */
#define CFETI_FX_PRODUCT_TYPE_NOT_SPECIFIED     0 /**< Unspecified, represents no product type. */
#define CFETI_FX_PRODUCT_TYPE_OPTIONS           1 /**< FX Option OTC */
#define CFETI_FX_PRODUCT_TYPE_CASH              2 /**< FX Cash */
#define CFETI_FX_PRODUCT_TYPE_SPOT              3 /**< FX Spot */
#define CFETI_FX_PRODUCT_TYPE_FORWARD           4 /**< FX Forward */
#define CFETI_FX_PRODUCT_TYPE_SWAP              5 /**< FX Swap */
#define CFETI_FX_PRODUCT_TYPE_IOSWAP            6 /**< FX IO Swap */
#define CFETI_FX_PRODUCT_TYPE_FORWARD_NONDELIVERABLE 7 /**< FX Forward Non-deliverable */

/*
 * Enumerated FX Deal Type (FX trade confirm) 
 */
#define CFETI_FX_DEAL_TYPE_NOT_SPECIFIED        CFETI_FX_PRODUCT_TYPE_NOT_SPECIFIED /**< Unspecified, represents no product type. */
#define CFETI_FX_DEAL_TYPE_OPTIONS              CFETI_FX_PRODUCT_TYPE_OPTIONS
#define CFETI_FX_DEAL_TYPE_CASH                 CFETI_FX_PRODUCT_TYPE_CASH
#define CFETI_FX_DEAL_TYPE_SPOT                 CFETI_FX_PRODUCT_TYPE_SPOT
#define CFETI_FX_DEAL_TYPE_FORWARD              CFETI_FX_PRODUCT_TYPE_FORWARD
#define CFETI_FX_DEAL_TYPE_SWAP                 CFETI_FX_PRODUCT_TYPE_SWAP
#define CFETI_FX_DEAL_TYPE_IOSWAP               CFETI_FX_PRODUCT_TYPE_IOSWAP
#define CFETI_FX_DEAL_TYPE_FORWARD_NONDELIVERABLE CFETI_FX_PRODUCT_TYPE_FORWARD_NONDELIVERABLE

/*
 * Enumerated FX Option Strategy Type (FX option trade confirm) 
 */
#define CFETI_FX_STRATEGY_NOT_SPECIFIED         0 /**< Unspecified, represents no strategy. */
#define CFETI_FX_STRATEGY_BUTTERFLY_SPREAD      1 /**< Butterfly Spread for product FX Option. */
#define CFETI_FX_STRATEGY_COMBO                 2 /**< Combo for product FX Option. */
#define CFETI_FX_STRATEGY_CONDOR                3 /**< Condor for product FX Option. */
#define CFETI_FX_STRATEGY_OUTRIGHT              4 /**< Outright for product FX Option. */
#define CFETI_FX_STRATEGY_RISK_REVERSAL         5 /**< Risk Reversal For product FX Option. */
#define CFETI_FX_STRATEGY_SPREAD                6 /**< Spread. For product FX Option. */
#define CFETI_FX_STRATEGY_STRADDLE              7 /**< Straddle. For product FX Option. */
#define CFETI_FX_STRATEGY_STRANGLE              8 /**< Strangle. For product FX Option. */
#define CFETI_FX_STRATEGY_TIME_COMBO            9 /**< Time Combo. For product FX Option. */
#define CFETI_FX_STRATEGY_TIME_SPREAD           10 /**< Time Spread. For product FX Option. */
#define CFETI_FX_STRATEGY_VERTICAL_SPREAD       11 /**< Vertical Spread. For product FX Option. */

/*
 * Enumerated FX Option Class (FX option trade confirm) 
 */
#define CFETI_FX_OPTION_CLASS_NOT_SPECIFIED     0 /**< Unspecified, represents no exercise style. */
#define CFETI_FX_OPTION_CLASS_ACCRUAL_FLOATERS  1 /**< Option class accrual floeaters */
#define CFETI_FX_OPTION_CLASS_AVERAGE_OPTIONS   2 /**< Option class avregare options */
#define CFETI_FX_OPTION_CLASS_BARRIER           3 /**< Option class barrier */
#define CFETI_FX_OPTION_CLASS_BINARY            4 /**< Option class binary */
#define CFETI_FX_OPTION_CLASS_COMPOUND          5 /**< Option class compound */
#define CFETI_FX_OPTION_CLASS_EUROPEAN_DIGITAL  6 /**< Option class european digital */
#define CFETI_FX_OPTION_CLASS_FADER             7 /**< Option class fader */
#define CFETI_FX_OPTION_CLASS_LOOKBACK          8 /**< Option class lookback */
#define CFETI_FX_OPTION_CLASS_MULTI_PERIOD_BARRIERS 9 /**< Option class multi period barriers */
#define CFETI_FX_OPTION_CLASS_STRUCTURED_PRODUCTS 10 /**< Option class structured products */
#define CFETI_FX_OPTION_CLASS_VANILLA           11 /**< Option class vanilla */
#define CFETI_FX_OPTION_CLASS_WINDOW_BARRIER    12 /**< Option class window barrier */

/*
 * Enumerated FX Option Style (FX option trade confirm) 
 */
#define CFETI_FX_OPTION_STYLE_NOT_SPECIFIED     0 /**< Unspecified, represents no exercise style. */
#define CFETI_FX_OPTION_STYLE_AVERAGE_FORWARD   1 /**< FX Option style   */
#define CFETI_FX_OPTION_STYLE_AVERAGE_RATE      2 /**< FX Option style   */
#define CFETI_FX_OPTION_STYLE_AVERAGE_STRIKE    3 /**< FX Option style   */
#define CFETI_FX_OPTION_STYLE_BOX               4 /**< FX Option style   */
#define CFETI_FX_OPTION_STYLE_DOUBLE_AVERAGE    5 /**< FX Option style   */
#define CFETI_FX_OPTION_STYLE_DOUBLE_KNOCK_IN   6 /**< FX Option style   */
#define CFETI_FX_OPTION_STYLE_DOUBLE_KNOCK_OUT  7 /**< FX Option style   */
#define CFETI_FX_OPTION_STYLE_DOUBLE_NO_TOUCH   8 /**< FX Option style   */
#define CFETI_FX_OPTION_STYLE_DOUBLE_ONE_TOUCH  9 /**< FX Option style   */
#define CFETI_FX_OPTION_STYLE_FADE_IN_ONE_SIDED 10 /**< FX Option style   */
#define CFETI_FX_OPTION_STYLE_FADE_IN_RANGE     11 /**< FX Option style   */
#define CFETI_FX_OPTION_STYLE_FADE_OUT_ONE_SIDED 12 /**< FX Option style   */
#define CFETI_FX_OPTION_STYLE_FADE_OUT_RANGE    13 /**< FX Option style   */
#define CFETI_FX_OPTION_STYLE_FORWARD_EXTRA     14 /**< FX Option style   */
#define CFETI_FX_OPTION_STYLE_FORWARD_KNOCK_IN  15 /**< FX Option style   */
#define CFETI_FX_OPTION_STYLE_FORWARD_KNOCK_OUT 16 /**< FX Option style   */
#define CFETI_FX_OPTION_STYLE_IMPROVER          17 /**< FX Option style   */
#define CFETI_FX_OPTION_STYLE_KNOCK_IN          18 /**< FX Option style   */
#define CFETI_FX_OPTION_STYLE_KNOCK_IN_AND_KNOCK_OUT 19 /**< FX Option style   */
#define CFETI_FX_OPTION_STYLE_KNOCK_OUT         20 /**< FX Option style   */
#define CFETI_FX_OPTION_STYLE_NO_TOUCH          21 /**< FX Option style   */
#define CFETI_FX_OPTION_STYLE_NO_TOUCH_WITH_KNOCK_IN 22 /**< FX Option style   */
#define CFETI_FX_OPTION_STYLE_ONE_SIDED_ACCRUAL 23 /**< FX Option style   */
#define CFETI_FX_OPTION_STYLE_ONE_TOUCH         24 /**< FX Option style   */
#define CFETI_FX_OPTION_STYLE_ONE_TOUCH_WITH_KNOCK_OUT 25 /**< FX Option style   */
#define CFETI_FX_OPTION_STYLE_RANGE_ACCRUAL     26 /**< FX Option style   */
#define CFETI_FX_OPTION_STYLE_RANGE_BET         27 /**< FX Option style   */
#define CFETI_FX_OPTION_STYLE_REVERSE_KNOCK_IN  28 /**< FX Option style   */
#define CFETI_FX_OPTION_STYLE_REVERSE_KNOCK_OUT 29 /**< FX Option style   */
#define CFETI_FX_OPTION_STYLE_RR_KNOCKOUT       30 /**< FX Option style   */
#define CFETI_FX_OPTION_STYLE_STANDARD          31 /**< FX Option style   */
#define CFETI_FX_OPTION_STYLE_STEP_PREMIUM      32 /**< FX Option style   */

/*
 * Enumerated FX Leg Type (FX trade confirm) 
 */
#define CFETI_FX_LEG_TYPE_NOT_SPECIFIED         0 /**< Unspecified, represents no leg style. */
#define CFETI_FX_LEG_TYPE_FX_OPTIONS            1 /**< Type FX Options  */
#define CFETI_FX_LEG_TYPE_FX_FORWARD            2 /**< Type FX Forward */
#define CFETI_FX_LEG_TYPE_FX_SWAP               3 /**< Type FX Swap */
#define CFETI_FX_LEG_TYPE_NEAR                  4 /**< Type Near Leg */
#define CFETI_FX_LEG_TYPE_FAR                   5 /**< Type Far Leg */
#define CFETI_FX_LEG_TYPE_IN                    6 /**< Type In */
#define CFETI_FX_LEG_TYPE_OUT                   7 /**< Type Out */
#define CFETI_FX_LEG_TYPE_FX_SPOT               8 /**< Type FX Spot */

/*
 * Enumerated FX Exercise Style (FX trade confirm) 
 */
#define CFETI_FX_EXERCISE_NOT_SPECIFIED         0 /**< Unspecified, represents no exercise style. */
#define CFETI_FX_EXERCISE_BERMUDA               1 /**< Exercise style Bermuda */
#define CFETI_FX_EXERCISE_EUROPE                2 /**< Exercise style Europe */
#define CFETI_FX_EXERCISE_AMERICA               3 /**< Exercise style America */

/*
 * Enumerated FX Leg Transaction Type (FX trade confirm) 
 */
#define CFETI_FX_LEG_TRANSACTION_NOT_SPECIFIED  0 /**< Unspecified, represents no leg transaction. */
#define CFETI_FX_LEG_TRANSACTION_OPTION         1 /**< FX Option OTC */
#define CFETI_FX_LEG_TRANSACTION_CASH           2 /**< FX Cash */

/*
 * Enumerated FX Call Indicator (FX trade confirm) 
 */
#define CFETI_FX_CALL_INDICATOR_NOT_SPECIFIED   0 /**< Unspecified, represents no call indicator. */
#define CFETI_FX_CALL_INDICATOR_PUT             1
#define CFETI_FX_CALL_INDICATOR_CALL            2

/*
 * Enumerated FX Amount Method (FX trade confirm) 
 */
#define CFETI_FX_AMOUNT_METHOD_NOT_SPECIFIED    0 /**< Unspecified, represents no amount method. */
#define CFETI_FX_AMOUNT_METHOD_1_2              1 /**< Currency 1 per Currency 2 */
#define CFETI_FX_AMOUNT_METHOD_2_1              2 /**< Currency 2 per Currency 1 */
#define CFETI_FX_AMOUNT_METHOD_PERCENT_1        3 /**< Percent Currency 1 */
#define CFETI_FX_AMOUNT_METHOD_PERCENT_2        4 /**< Percent Currency 2 */

/*
 * Enumerated FX Delivery Terms (FX trade confirm) 
 */
#define CFETI_FX_DELIVERY_TERM_NOT_SPECIFIED    0 /**< Unspecified, represents no delivery term. */
#define CFETI_FX_DELIVERY_TERM_PHYSICAL         1 /**< Physical */
#define CFETI_FX_DELIVERY_TERM_CASH             2 /**< Cash */
#define CFETI_FX_DELIVERY_TERM_NON_DELIVERABLE  3 /**< Non Deliverable */

/*
 * Enumerated FX Option Hedge Types (FX Option trade confirm) 
 */
#define CFETI_FX_OPTION_HEDGE_NONE              0 /**< Unspecified, represents no delivery term. */
#define CFETI_FX_OPTION_HEDGE_SPOT              1 /**< Physical */
#define CFETI_FX_OPTION_HEDGE_FORWARD           2 /**< Cash */

/*
 * Values for market data field CFETF_PROPERTIES 
 */
#define CFETI_PROPERTY_IS_REISSUE               0x1
#define CFETI_PROPERTY_IS_WHEN_ISSUE            0x2
#define CFETI_PROPERTY_IS_GC_INSTRUMENT         0x4
#define CFETI_PROPERTY_IS_INQUIRE_DEPTH_INSTRUMENT 0x8
#define CFETI_PROPERTY_IS_OVERNIGHT_ISSUE       0x10
#define CFETI_PROPERTY_IS_ODD_LOT               0x20
#define CFETI_PROPERTY_IS_VIEW_ONLY             0x80
#define CFETI_PROPERTY_DISABLE_ALLOW_SELECT_DERIVATION 0x100
#define CFETI_PROPERTY_ALLOWS_MIDS              0x200

/*
 * Values for market data field CFETF_DISPLAY_PROPERTIES 
 */
#define CFETI_DISPLAY_PROPERTIES_IS_RESTRICTED_INSTRUMENT 0x1
#define CFETI_DISPLAY_PROPERTIES_IS_PORTFOLIO_EXCLUDED 0x2
#define CFETI_DISPLAY_PROPERTIES_HAS_CHAIN      0x4
#define CFETI_DISPLAY_PROPERTIES_IS_BENCHMARK   0x8
#define CFETI_DISPLAY_PROPERTIES_CHAIN_UNRESTRICTED 0x10

/*
 * Values for the field display rules 
 */
#define CFETI_DISPLAY_CHAIN_BELOW               0x1
#define CFETI_DISPLAY_CHAIN_ABOVE               0x2
#define CFETI_DISPLAY_DEFAULT_GRID              0x4
#define CFETI_DISPLAY_ALL_GRIDS                 0x8
#define CFETI_DISPLAY_FOCUS_GRID                0x10

/*
 * Values for market data field CFETF_IRS_INST_PROPERTIES 
 */
#define CFETI_IRS_PROPERTY_OUTRIGHT             0x1
#define CFETI_IRS_PROPERTY_TRACKING             0x2
#define CFETI_IRS_PROPERTY_CROSS                0x4
#define CFETI_IRS_PROPERTY_BASIS                0x8
#define CFETI_IRS_PROPERTY_YIELD_SPREAD         0x10
#define CFETI_IRS_PROPERTY_VS_1M_FLOATING       0x20 /**< vs. 1-month floating rate */
#define CFETI_IRS_PROPERTY_VS_3M_FLOATING       0x40 /**< vs. 3-month floating rate */
#define CFETI_IRS_PROPERTY_VS_6M_FLOATING       0x80 /**< vs. 6-month floating rate */
#define CFETI_IRS_PROPERTY_VS_OIS               0x100 /**< vs. OIS rate */
#define CFETI_IRS_PROPERTY_VS_BUND              0x200
#define CFETI_IRS_PROPERTY_VS_BOBL              0x400
#define CFETI_IRS_PROPERTY_VS_SCHATZ            0x800
#define CFETI_IRS_PROPERTY_DEFAULT_DERIVE       0x1000
#define CFETI_IRS_PROPERTY_DEFAULT_HEDGE        0x2000
#define CFETI_IRS_PROPERTY_BUTTERFLY            0x4000

/*
 * Values for market data field CFETF_DOLLAR_IRS_INST_PROPERTIES 
 */
#define CFETI_DOLLAR_IRS_PROPERTY_ANNUAL_MONEY  0x1
#define CFETI_DOLLAR_IRS_PROPERTY_SEMI_ANNUAL_30_360 0x2
#define CFETI_DOLLAR_IRS_PROPERTY_SEMI_ANNUAL_30_360_VS_UST 0x4
#define CFETI_DOLLAR_IRS_PROPERTY_TRACKING_VS_FUTURE 0x8
#define CFETI_DOLLAR_IRS_PROPERTY_CROSS_VS_FUTURE 0x10
#define CFETI_DOLLAR_IRS_PROPERTY_YIELD_SPREAD  0x20
#define CFETI_DOLLAR_IRS_PROPERTY_BUTTERFLY     0x40
#define CFETI_DOLLAR_IRS_PROPERTY_VS_1M_FLOATING 0x80 /**< vs. 1-month floating rate */
#define CFETI_DOLLAR_IRS_PROPERTY_VS_3M_FLOATING 0x100 /**< vs. 3-month floating rate */
#define CFETI_DOLLAR_IRS_PROPERTY_VS_6M_FLOATING 0x200 /**< vs. 6-month floating rate */
#define CFETI_DOLLAR_IRS_PROPERTY_VS_OIS_FLOATING 0x400 /**< vs. OIS rate */
#define CFETI_DOLLAR_IRS_PROPERTY_DEFAULT_DERIVE 0x800
#define CFETI_DOLLAR_IRS_PROPERTY_DEFAULT_HEDGE 0x1000
#define CFETI_DOLLAR_IRS_PROPERTY_DEFAULT_BOND_CROSS 0x2000
#define CFETI_DOLLAR_IRS_PROPERTY_SEMI_ANNUAL_30_360_VS_UST_INT 0x4000

/*
 * Values for market data field CFETF_STERLING_IRS_INST_PROPERTIES 
 */
#define CFETI_STERLING_IRS_PROPERTY_OUTRIGHT    0x1
#define CFETI_STERLING_IRS_PROPERTY_VS_GILT     0x2
#define CFETI_STERLING_IRS_PROPERTY_TRACKING    0x4
#define CFETI_STERLING_IRS_PROPERTY_CROSS       0x8
#define CFETI_STERLING_IRS_PROPERTY_YIELD_SPREAD 0x10
#define CFETI_STERLING_IRS_PROPERTY_BUTTERFLY   0x20
#define CFETI_STERLING_IRS_PROPERTY_VS_1M_FLOATING 0x40 /**< vs. 1-month floating rate */
#define CFETI_STERLING_IRS_PROPERTY_VS_3M_FLOATING 0x80 /**< vs. 3-month floating rate */
#define CFETI_STERLING_IRS_PROPERTY_VS_6M_FLOATING 0x100 /**< vs. 6-month floating rate */
#define CFETI_STERLING_IRS_PROPERTY_VS_SONIA_FLOATING 0x200 /**< vs. Sterling Overnight Interbank Average */
#define CFETI_STERLING_IRS_PROPERTY_DEFAULT_DERIVE 0x400
#define CFETI_STERLING_IRS_PROPERTY_DEFAULT_HEDGE 0x800
#define CFETI_STERLING_IRS_PROPERTY_DEFAULT_BOND_CROSS 0x1000

/*
 * Values for market data field CFETF_INST_SELECTION_PROPERTIES 
 */
#define CFETI_INST_SELECTION_NOT_SPECIFIED      0
#define CFETI_INST_SELECTION_OUTRIGHT           1 /**< Outright(Cash) instrument */
#define CFETI_INST_SELECTION_BASIS              2 /**< Basis instrument */
#define CFETI_INST_SELECTION_SPREAD             4 /**< Spread(switch) instrument */
#define CFETI_INST_SELECTION_SWITCH             CFETI_INST_SELECTION_SPREAD

/*
 * Values for market data field CFETF_ENERGY_CONTRACT_TYPE 
 */
#define CFETI_ENERGY_CONTRACT_NOT_SPECIFIED     0
#define CFETI_ENERGY_CONTRACT_FIRM              1
#define CFETI_ENERGY_CONTRACT_INTERRUPTIBLE     2

/*
 * Values for market data field CFETF_ENERGY_QUANTITY_UNIT 
 */
#define CFETI_ENERGY_QUANTITY_UNIT_NOT_SPECIFIED 0
#define CFETI_ENERGY_QUANTITY_UNIT_DTH          1 /**< Decatherm */
#define CFETI_ENERGY_QUANTITY_UNIT_GJ           2 /**< GigaJoules */
#define CFETI_ENERGY_QUANTITY_UNIT_MW           3 /**< MegaWatts */
#define CFETI_ENERGY_QUANTITY_UNIT_MWH          CFETI_ENERGY_QUANTITY_UNIT_MW
#define CFETI_ENERGY_QUANTITY_UNIT_MMBTU        4 /**< British Thermal Units (For Natural Gas and Physical Natural Gas trades) */
#define CFETI_ENERGY_QUANTITY_UNIT_BBL          5 /**< Barrel (For Natural Gas Liquid trades) */
#define CFETI_ENERGY_QUANTITY_UNIT_TONS         6 /**< Tons (For Coal and Emission trades) */

/*
 * Values for market data field CFETF_ENERGY_PRICE_TYPE 
 */
#define CFETI_ENERGY_PRICE_TYPE_NOT_SPECIFIED   0
#define CFETI_ENERGY_PRICE_TYPE_FIXED           1
#define CFETI_ENERGY_PRICE_TYPE_INDEXED         2
#define CFETI_ENERGY_PRICE_TYPE_SWINGSWAP       3
#define CFETI_ENERGY_PRICE_TYPE_BASISSWAP       4
#define CFETI_ENERGY_PRICE_TYPE_FIXEDSWAP       5
#define CFETI_ENERGY_PRICE_TYPE_OUTRIGHT_FORWARD 6

/*
 * Values for market data field CFETF_ENERGY_INDEX_TYPE 
 */
#define CFETI_ENERGY_INDEX_TYPE_NOT_SPECIFIED   0
#define CFETI_ENERGY_INDEX_TYPE_FXD             1 /**< Fixed */
#define CFETI_ENERGY_INDEX_TYPE_GDD             2 /**< Gas Daily Daily */
#define CFETI_ENERGY_INDEX_TYPE_GDM             3 /**< Gas Daily Monthly */
#define CFETI_ENERGY_INDEX_TYPE_NYM             4 /**< NYMEX */
#define CFETI_ENERGY_INDEX_TYPE_IFE             5 /**< Inside FERC */
#define CFETI_ENERGY_INDEX_TYPE_NGI             6 /**< Natural Gas Intelligence */
#define CFETI_ENERGY_INDEX_TYPE_IMO             7 /**< Independent Electricity Market Operator */
#define CFETI_ENERGY_INDEX_TYPE_PPA             8 /**< Power Pool Alberta */

/*
 * Values for market data field CFETF_ENERGY_PRODUCT_PERIOD 
 */
#define CFETI_ENERGY_PRODUCT_PERIOD_NOT_SPECIFIED 0
#define CFETI_ENERGY_PRODUCT_PEAK               1
#define CFETI_ENERGY_PRODUCT_OFFPEAK            2

/*
 * Values for market data field CFETF_ENERGY_SETTLEMENT_TYPE 
 */
#define CFETI_ENERGY_SETTLEMENT_TYPE_NOT_SPECIFIED 0
#define CFETI_ENERGY_SETTLEMENT_PHYSICAL        80
#define CFETI_ENERGY_SETTLEMENT_FINANCIAL       70

/*
 * Values to describe properties of a trade (bitmask) 
 */
#define CFETI_TRADE_IMPROVED_PRICE              0x1
#define CFETI_TRADE_DIRECT_DEAL                 0x2
#define CFETI_TRADE_RFQ                         CFETI_TRADE_DIRECT_DEAL
#define CFETI_TRADE_NON_ELECTRONIC              0x4 /**< Trade was not executed on the eSpeed platform */
#define CFETI_TRADE_NETTED                      0x8 /**< Indicating a netted trade */
#define CFETI_TRADE_ERROR                       0x20 /**< The trade is delivered in error */
#define CFETI_TRADE_MANAGED                     0x40 /**< The trade was managed by a broker */
#define CFETI_TRADE_IMPROVED_EXECUTION          0x80 /**< Indication that an improved execution was made */
#define CFETI_TRADE_NO_COMMISSION               0x100 /**< No commission will be applied to this trade */
#define CFETI_TRADE_DUPLICATE                   0x400 /**< Trade is a duplicate of a confirmation sent to another eSpeed user */

/*
 * Preferences and properties for Direct Dealing Requests and Quotes - can
 * be combined (|) 
 */
#define CFETI_DD_RETAIN_ON_EXECUTE              0x20
#define CFETI_DD_NO_EXPIRY                      0x40
#define CFETI_DD_IS_ANONYMOUS_CALLOUT           0x80 /**< The Direct Dealing Request is an anonymous call out */
#define CFETI_DD_IS_NAMED_CALLOUT               0x100 /**< The Direct Dealing Request is a named call out */
#define CFETI_DD_QUOTER_FAVOURS_BUY             0x400 /**< Provider of the quote expresses a preference to buy */
#define CFETI_DD_QUOTER_FAVORS_BUY              CFETI_DD_QUOTER_FAVOURS_BUY
#define CFETI_DD_QUOTER_FAVOURS_SELL            0x800 /**< Provider of the quote expresses a preference to sell */
#define CFETI_DD_QUOTER_FAVORS_SELL             CFETI_DD_QUOTER_FAVOURS_SELL
#define CFETI_DD_QUOTE_REVERSE_INQUIRY          0x1000 /**< Quote is a reverse inquiry */
#define CFETI_DD_QUOTE_USE_SURPLUS              0x2000 /**< Quote can be advertised as a reverse inquiry if there is no request to match */
#define CFETI_DD_IS_INDICATION_OF_INTEREST      0x4000 /**< The Direct Dealing Request is an indication of interest */

/*
 * Direct Dealing properties instruments 
 */
#define CFETI_DD_QUOTE_MODIFICATION_DISABLED    0x1
#define CFETI_DD_REQUESTER_MUST_QUOTE           0x4
#define CFETI_DD_INSTRUMENT_ENABLED             0x8

/*
 * Ownership types of an object releated to a specific user 
 */
#define CFETI_OWNER_TYPE_NONE                   0x0 /**< Not specified */
#define CFETI_OWNER_TYPE_THIS_USER              0x1 /**< The user is the owner */
#define CFETI_OWNER_TYPE_THIS_LEGAL_ENTITY      0x2 /**< The user is in the same legal entity as the owner */
#define CFETI_OWNER_TYPE_UNKNOWN                0x3 /**< The user is not in the same legal id as the owner */

/*
 * Text Notification Severity 
 */
#define CFETI_TEXT_SEVERITY_UNKNOWN             0
#define CFETI_TEXT_SEVERITY_LOW                 1
#define CFETI_TEXT_SEVERITY_MEDIUM              2
#define CFETI_TEXT_SEVERITY_HIGH                3
#define CFETI_TEXT_SEVERITY_URGENT              4

/*
 * Values for Extended Order Property types 
 */
#define CFETI_EXT_PROPERTY_ORDER_TYPE           0x1 /**< 0001 UINT32 */
#define CFETI_EXT_PROPERTY_ORDER_TIME_TYPE      0x2 /**< 0002 UINT32 */
#define CFETI_EXT_PROPERTY_ORDER_RESTRICTION    0x3 /**< 0003 UINT32 */
#define CFETI_EXT_PROPERTY_ORDER_OPENPOSITION   0x4 /**< 0004 UINT32 */
#define CFETI_EXT_PROPERTY_ORDER_EXPIRY         0x5 /**< 0005 UINT32 */
#define CFETI_EXT_PROPERTY_MINIMUM_VOLUME       0x6 /**< 0006 DECIMAL */
#define CFETI_EXT_PROPERTY_FHLB_MATURITY_DATE   0x9 /**< 0009 UINT32 */
#define CFETI_EXT_PROPERTY_ACCOUNT_CODE         0xa /**< 0010 STRING */
#define CFETI_EXT_PROPERTY_CLIENT_ID            0xb /**< 0011 STRING */
#define CFETI_EXT_PROPERTY_PRICE_FORMAT_CODE    0xe /**< 0014 UINT32 */
#define CFETI_EXT_PROPERTY_FHLB_REVERSE_INQUIRY 0x10 /**< 0016 UINT32 */
#define CFETI_EXT_PROPERTY_FHLB_SETTLEMENT_TYPE 0x1a /**< 0026 UINT32 */
#define CFETI_EXT_PROPERTY_FHLB_SETTLEMENT_DATE 0x1b /**< 0027 UINT32 */
#define CFETI_EXT_PROPERTY_FHLB_CUSTOMER_TYPE   0x1c /**< 0028 UINT32 */
#define CFETI_EXT_PROPERTY_FHLB_SWAP_INST_CODE  0x1d /**< 0029 STRING */
#define CFETI_EXT_PROPERTY_FHLB_SWAP_INST_DATE  0x1e /**< 0030 UINT32 */
#define CFETI_EXT_PROPERTY_FHLB_SWAP_BID_RATE   0x1f /**< 0031 DECIMAL */
#define CFETI_EXT_PROPERTY_LABEL                0x20 /**< 0032 STRING */
#define CFETI_EXT_PROPERTY_FHLB_NET_PROCEEDS    0x22 /**< 0034 DECIMAL */
#define CFETI_EXT_PROPERTY_FHLB_UNDERWRITTEN_OBLIGATION 0x23 /**< 0035 DECIMAL */
#define CFETI_EXT_PROPERTY_FHLB_ESPEED_FEE      0x24 /**< 0036 DECIMAL */
#define CFETI_EXT_PROPERTY_FHLB_CUSIP           0x25 /**< 0037 STRING */
#define CFETI_EXT_PROPERTY_FHLB_CHECKOUT_USER   0x26 /**< 0038 STRING */
#define CFETI_EXT_PROPERTY_CTI_CODE             0x27 /**< 0039 UINT32 */
#define CFETI_EXT_PROPERTY_ORDER_ORIGIN         0x28 /**< 0040 UINT32 */
#define CFETI_EXT_PROPERTY_CBOT_EXCHANGE_FEE    0x29 /**< 0041 UINT32 */
#define CFETI_EXT_PROPERTY_CUSTOMER_ACCOUNT     0x2a /**< 0042 STRING */
#define CFETI_EXT_PROPERTY_SPREAD_PLACEMENT     0x2b /**< 0043 UINT32 */
#define CFETI_EXT_PROPERTY_MEMBER_CLEARING_CODE 0x2c /**< 0044 STRING */
#define CFETI_EXT_PROPERTY_EXCHANGE_TRADE_REF   0x2d /**< 0045 STRING */
#define CFETI_EXT_PROPERTY_TRS_ORDER_ID         0x3a /**< 0058 STRING */
#define CFETI_EXT_PROPERTY_EXCHANGE_ACCOUNT_NAME 0x3b /**< 0059 STRING */
#define CFETI_EXT_PROPERTY_CME_ORDER_ORIGIN     0x3c /**< 0060 UINT32 */
#define CFETI_EXT_PROPERTY_ORDER_CLIP_RATIO     0x3d /**< 0061 UINT32 */

/*
 * Values for CFETI_EXT_PROPERTY_ORDER_TYPE order types 
 */
#define CFETI_EXT_PROPERTY_MARKETORDER          0x1
#define CFETI_EXT_PROPERTY_LIMITORDER           0x2
#define CFETI_EXT_PROPERTY_STOPORDER            0x3
#define CFETI_EXT_PROPERTY_MARKETONOPENORDER    0x4
#define CFETI_EXT_PROPERTY_STOPMARKETORDER      CFETI_EXT_PROPERTY_STOPORDER
#define CFETI_EXT_PROPERTY_STOPLIMITORDER       0x5
#define CFETI_EXT_PROPERTY_MARKETIFTOUCHEDORDER 0x6
#define CFETI_EXT_PROPERTY_MARKETLIMITORDER     0x7

/*
 * Values for CFETI_EXT_PROPERTY_ORDER_TIME_TYPE order types 
 */
#define CFETI_EXT_PROPERTY_ORDER_GOOD_TILL_DATE 0x1
#define CFETI_EXT_PROPERTY_ORDER_GOOD_FOR_DAY   0x2
#define CFETI_EXT_PROPERTY_ORDER_GOOD_TILL_CANCEL 0x3
#define CFETI_EXT_PROPERTY_ORDER_CLIP           0x4

/*
 * Values for CFETI_EXT_PROPERTY_ORDER_RESTRICTION order types. This is a bitmask
 * and values can be combined 
 */
#define CFETI_EXT_PROPERTY_ORDER_IMMEDIATE_OR_CANCEL 0x1
#define CFETI_EXT_PROPERTY_ORDER_COMPLETE_VOLUME 0x2
#define CFETI_EXT_PROPERTY_ORDER_FILL_OR_KILL   0x4

/*
 * Values for CFETI_EXT_PROPERTY_IS_CLIENT_ORDER client id types 
 */
#define CFETI_EXT_PROPERTY_NOT_CLIENT_ORDER     0x0
#define CFETI_EXT_PROPERTY_CLIENT_ORDER         0x1

/*
 * Values for CFETI_EXT_PROPERTY_CTI_CODE 
 */
#define CFETI_EXT_PROPERTY_CTI_CODE_UNKNOWN     0x0
#define CFETI_EXT_PROPERTY_CTI_CODE_OWN_ACCOUNT 0x1
#define CFETI_EXT_PROPERTY_CTI_CODE_HOUSE_PROPRIETARY 0x2
#define CFETI_EXT_PROPERTY_CTI_CODE_OTHER_MEMBER 0x3
#define CFETI_EXT_PROPERTY_CTI_CODE_CUSTOMER_ACCOUNT 0x4

/*
 * Values for CFETI_EXT_PROPERTY_ORDER_ORIGIN 
 */
#define CFETI_EXT_PROPERTY_ORDER_ORIGIN_UNKNOWN 0x0
#define CFETI_EXT_PROPERTY_ORDER_ORIGIN_CUSTOMER 0x1
#define CFETI_EXT_PROPERTY_ORDER_ORIGIN_NON_CUSTOMER 0x2
#define CFETI_EXT_PROPERTY_ORDER_ORIGIN_HOUSE_FLOOR_TRADER 0x3
#define CFETI_EXT_PROPERTY_ORDER_ORIGIN_FLOOR_TRADER 0x4

/*
 * Values for CFETI_EXT_PROPERTY_CME_ORDER_ORIGIN 
 */
#define CFETI_EXT_PROPERTY_CME_ORDER_ORIGIN_UNKNOWN 0x0
#define CFETI_EXT_PROPERTY_CME_ORDER_ORIGIN_CUSTOMER 0x1
#define CFETI_EXT_PROPERTY_CME_ORDER_ORIGIN_FIRM 0x2

/*
 * Values for CFETI_EXT_PROPERTY_CBOT_EXCHANGE_FEE 
 */
#define CFETI_EXT_PROPERTY_CBOT_EXCHANGE_FEE_UNKNOWN 0x0
#define CFETI_EXT_PROPERTY_CBOT_EXCHANGE_FEE_NON_MEMBER 0x1
#define CFETI_EXT_PROPERTY_CBOT_EXCHANGE_FEE_CBOT_MEMBER 0x2
#define CFETI_EXT_PROPERTY_CBOT_EXCHANGE_FEE_EUREX_MEMBER 0x3

/*
 * Values for CFETI_EXT_PROPERTY_SPREAD_PLACEMENT 
 */
#define CFETI_EXT_PROPERTY_SPREAD_PLACEMENT_SPREADS_BOOK 0x0
#define CFETI_EXT_PROPERTY_SPREAD_PLACEMENT_OUTRIGHTS_MARKET 0x1

/*
 * Values for FHLB specific market data field CFETF_SETTLEMENT_TYPES 
 */
#define CFETI_SETTLEMENT_FHLB_CASH              0x1
#define CFETI_SETTLEMENT_FHLB_REGULAR           0x2
#define CFETI_SETTLEMENT_FHLB_SKIP              0x3
#define CFETI_SETTLEMENT_FHLB_SKIP_PLUS         0x4

/*
 * Bitmask flags for Price Improvement properties CFETF_PI_PRICE_TICK_PROPERTIES
 * and CFETI_PRICE_IMPROVEMENT_INFO_DESC::priceTickProperties 
 */
#define CFETI_PI_PRICE_TICK_ALLOW_BEST          0x1
#define CFETI_PI_PRICE_TICK_NO_INCREMENTS       0x2
#define CFETI_PI_PRICE_TICK_DECIMAL             0x4
#define CFETI_PI_PRICE_TICK_DISABLED            0x8

/*
 * Enumerated constants to represent a number of PI ticks 
 */
#define CFETI_PI_BEST                           0x0
#define CFETI_PI_1                              0x1
#define CFETI_PI_2                              0x2
#define CFETI_PI_3                              0x3
#define CFETI_PI_MID_PRICE                      0xffffffff

/*
 * Bitmask flags for eSpeed features 
 */
#define CFETI_FEATURE_DIRECT_DEALING            0x1
#define CFETI_FEATURE_INDICATIVE_PRICING        0x2
#define CFETI_FEATURE_TRADING                   0x3
#define CFETI_FEATURE_ADMIN                     0x4
#define CFETI_FEATURE_SYSTEM                    0x5

/*
 * Bitmask flags for indicative pricing access control 
 */
#define CFETI_INDICATIVE_PRICE_CONTRIBUTOR      0x1

/*
 * Client Checkout permissions bitmask flags 
 */
#define CFETI_CLIENT_CHECKOUT_NONE              0x0
#define CFETI_CLIENT_CHECKOUT_ACCEPT            0x1 /**< User may issue checkout accept requests for this trade. This bit will be set for brokered clients. */
#define CFETI_CLIENT_CHECKOUT_REFUSE            0x2 /**< User may issue checkout refuse requests for this trade. For the first client checkout implementation electronic trade rejection will not be supported. This bit will therefore not be set in the bit-mask */
#define CFETI_CLIENT_CHECKOUT_AMMEND            0x4 /**< User may issue checkout amend requests for this trade. For the first client checkout implementation electronic trade amendment will not be supported. This bit will therefore not be set in the bit-mask */

/*
 * User status enumeration 
 */
#define CFETI_USER_STATUS_LOGGED_OUT            0x0 /**< User is logged out from the eSpeed system */
#define CFETI_USER_STATUS_LOGGED_IN             0x1 /**< User is logged into the eSpeed system */

/*
 * Direct Dealing features bitmask flags 
 */
#define CFETI_DD_FEATURES_NOT_SPECIFIED         0x0 /**< Features not specified */
#define CFETI_DD_ANONYMOUS_CALLOUT_ENABLED      0x1 /**< Supports Direct Dealing anonymous callout functionality (default -> no) */
#define CFETI_DD_NAME_GIVEUP_CALLOUT_ENABLED    0x2 /**< Supports Direct Dealing name give-up callout functionality (default -> no) */
#define CFETI_DD_2WAY_REQUEST_ENABLED           0x4 /**< Supports two-way Direct Dealing requests (default -> no) */
#define CFETI_DD_1WAY_REQUEST_ENABLED           0x8 /**< Supports one-way Direct Dealing requests (default -> yes) */
#define CFETI_DD_BOLS_ENABLED                   0x10 /**< Supports BOLS Direct Dealing requests (default -> no) */
#define CFETI_DD_ALL_OR_NONE_ENABLED            0x20 /**< Is all-or-none enabled (i.e. minimum allowable quantity is not supported, default -> no) */
#define CFETI_DD_ALLOW_RETAIN_ON_EXECUTE        0x80 /**< Allows an Direct Dealing request to be submitted with an option that it be retained on execution. (default -> no) */
#define CFETI_DD_ENFORCE_RETAIN_ON_EXECUTE      0x100 /**< Requires all Direct Dealing requests to be submitted with the option that it be retained on execution. (default -> no). If this flag is set the eSpeed API shall automatically introduce this flag to all submitted Direct Dealing requests. */
#define CFETI_DD_DISABLE_QUOTE_TIMEOUT          0x200 /**< Disable quote timeout feature. If this option is set the lifetime of a quote is the lifetime of the Direct Dealing Request. Otherwise it is the timeout specified in the quote */
#define CFETI_DD_D2D_ENABLED                    0x800 /**< Direct Dealing system supports dealer-to-dealer (D2D) functionality (default -> no) */
#define CFETI_DD_ORDER_ALLOCATION_ENABLED       0x1000 /**< Direct Dealing server supports inclusion of allocation instructions in direct dealing requests and quotes. */
#define CFETI_DD_INDICATION_OF_INTEREST_ENABLED 0x2000 /**< Direct Dealing server supports requests that are an indication of interest only. */

/*
 * Direct Dealing Repo Attributes 
 */
#define CFETI_DD_REPO_MONEY_FILL                0x1 /**< Money-Fill */
#define CFETI_DD_REPO_FACE_VALUE                0x2 /**< Face-Value */

/*
 * State of trading sessions 
 */
#define CFETI_TRADING_SESSION_STATE_NONE        0x0 /**< Trading session state not specified */
#define CFETI_TRADING_SESSION_STATE_OPEN        0x1 /**< Trading session state is open */
#define CFETI_TRADING_SESSION_STATE_CLOSED      0x2 /**< Trading session state is closed */

/*
 * eSpeed Instrument Query Sort Rule Types 
 */
#define CFETI_SORT_RULE_UNKNOWN                 0x0 /**< Unknown */
#define CFETI_SORT_RULE_MATURITY_COUPON         0x1 /**< Sort Instruments by increasing maturity and then by increasing coupon */
#define CFETI_SORT_RULE_MATURITY                0x2 /**< Sort Instruments by increasing maturity/term dates only */
#define CFETI_SORT_RULE_ALPHA                   0x3 /**< Sort Instruments alphabetically by eSpeed instrument name */
#define CFETI_SORT_RULE_REVERSE_ALPHA           0x4 /**< Sort Instruments in reverse alphabetical order by eSpeed instrument name */
#define CFETI_SORT_RULE_REPO_DATES              0x5 /**< Sort instruments by end date and then by start date */

/*
 * Trade confirmation operation 
 */
#define CFETI_TRADE_CONFIRM_OPERATION_NONE      0x0 /**< Unspecified price source */
#define CFETI_TRADE_CONFIRM_OPERATION_NEW_TRANSACTION 0x1 /**< New Transaction */
#define CFETI_TRADE_CONFIRM_OPERATION_AMEND_TRANSACTION 0x2 /**< Amend Transaction */
#define CFETI_TRADE_CONFIRM_OPERATION_CANCEL_TRANSACTION 0x3 /**< Cancel Transaction */

/*
 * Exchange Id 
 */
#define CFETI_EXCHANGE_ID_CBOT_ACE                    0 /**< Chicago Board of Trade (deprecated) */
#define CFETI_EXCHANGE_ID_ECBOT_V1                    1 /**< Chicago Board of Trade, eCBOT */
#define CFETI_EXCHANGE_ID_ECBOT_V2                    2 /**< Chicago Board of Trade, eCBOT (Version 2) */
#define CFETI_EXCHANGE_ID_EUREX_ACE                   3 /**< Eurex, A/C/E */
#define CFETI_EXCHANGE_ID_LIFFE                       4 /**< LIFFE */
#define CFETI_EXCHANGE_ID_EUREX_US                    5 /**< Eurex US */
#define CFETI_EXCHANGE_ID_CME                         6 /**< Chicago Mercantile Exchange */
#define CFETI_EXCHANGE_ID_MX                          7 /**< Montreal Exchange */

/*
 * Exchange Permissions 
 */
#define CFETI_EXCHANGE_ACCOUNT_ADMIN                  0x1 /**< Exhchange interface provides central account admin */

/*
 * Pricing methods 
 */
#define CFETI_PRICING_METHOD_NONE               0x0
#define CFETI_PRICING_METHOD_PRICE              0x1
#define CFETI_PRICING_METHOD_BASIS              0x2
#define CFETI_PRICING_METHOD_PRICE_SPREAD       0x3
#define CFETI_PRICING_METHOD_YIELD              0x4
#define CFETI_PRICING_METHOD_YIELD_SPREAD       0x5
#define CFETI_PRICING_METHOD_VOLATILITY         0x6

/*
 * Deal Structure 
 */
#define CFETI_DEAL_STRUCTURE_NONE                     0 /**< Deal structure not specified */
#define CFETI_DEAL_STRUCTURE_STANDARD                 1 /**< Standard */
#define CFETI_DEAL_STRUCTURE_SWITCH                   2 /**< Switch */
#define CFETI_DEAL_STRUCTURE_BASIS                    3 /**< Basis */
#define CFETI_DEAL_STRUCTURE_SWAP                     4 /**< Swap */
#define CFETI_DEAL_STRUCTURE_ROLL                     5 /**< Roll */
#define CFETI_DEAL_STRUCTURE_SPREAD                   6 /**< Spread */
#define CFETI_DEAL_STRUCTURE_FX_SPOT                  7 /**< FX Spot */
#define CFETI_DEAL_STRUCTURE_FX_SWAP                  8 /**< FX Swap */
#define CFETI_DEAL_STRUCTURE_FX_OPTION                9 /**< FX Option */
#define CFETI_DEAL_STRUCTURE_FX_FORWARD               10 /**< FX Forward */
#define CFETI_DEAL_STRUCTURE_FX_IOSWAP                11 /**< FX Inside-Outside Swap */
#define CFETI_DEAL_STRUCTURE_BUTTERFLY                12 /**< Butterfly */
#define CFETI_DEAL_STRUCTURE_REPO                     13 /**< Repo */
#define CFETI_DEAL_STRUCTURE_BOND_HEDGE               14 /**< Bond Hedge */
#define CFETI_DEAL_STRUCTURE_INTERPOLATED_BOND_HEDGE  15 /**< Interpolated Bond Hedge */

/*
 * Trade Type 
 */
#define CFETI_TRADE_TYPE_NONE                         0 /**< Trade type not specified */
#define CFETI_TRADE_TYPE_CASH                         1 /**< Cash */
#define CFETI_TRADE_TYPE_BASIS                        2 /**< Basis */
#define CFETI_TRADE_TYPE_SWITCH                       3 /**< Switch */
#define CFETI_TRADE_TYPE_FUTURE                       4 /**< Future */
#define CFETI_TRADE_TYPE_SWAP                         5 /**< Swap */
#define CFETI_TRADE_TYPE_ROLL                         6 /**< Roll */
#define CFETI_TRADE_TYPE_SPREAD                       7 /**< Spread */
#define CFETI_TRADE_TYPE_DISCOUNT                     8 /**< Discount */
#define CFETI_TRADE_TYPE_REPO                         9 /**< Repo */
#define CFETI_TRADE_TYPE_WI                           10 /**< WI */
#define CFETI_TRADE_TYPE_BUTTERFLY                    11 /**< Butterfly */

/*
 * Availability State 
 */
#define CFETI_TRADER_AVAILABILITY_NONE                0 /**< This section of the market is not available to anyone */
#define CFETI_TRADER_AVAILABILITY_ALL                 1 /**< This section of the market is available for anyone to trade for the entire market size */
#define CFETI_TRADER_AVAILABILITY_ANOTHER             2 /**< This section of the market is exclusively available to another user for the full market size */
#define CFETI_TRADER_AVAILABILITY_ANOTHER_PARTIAL     3 /**< This section of the market is exclusively available to another user for the size specified. The remaining size is not available to anyone */
#define CFETI_TRADER_AVAILABILITY_YOURS               4 /**< This section of the market is exclusively available to this user for the full market size */
#define CFETI_TRADER_AVAILABILITY_YOURS_PARTIAL       5 /**< This section of the market is exclusively available to this user for the size specified. The remaining size is not available to anyone */
#define CFETI_TRADER_AVAILABILITY_JOINT               6 /**< This section of the market is available to the user, and is also available to one other user for the full market size */
#define CFETI_TRADER_AVAILABILITY_JOINT_PARTIAL       7 /**< This section of the market is available to the user, and is also available to one other user for the size specified. The remaining size is not available to anyone */

/*
 * Availability State 
 */
#define CFETI_INDEX_MARKET_BID                        0 /**< Key to market bid cells in the availability matrix */
#define CFETI_INDEX_MARKET_ASK                        1 /**< Key to market ask cells in the availability matrix */
#define CFETI_INDEX_ORDER_SELL                        2 /**< Key to market sell cells in the availability matrix */
#define CFETI_INDEX_ORDER_BUY                         3 /**< Key to market buy cells in the availability matrix */

/*
 * Exchange Market Mode 
 */
#define CFETI_EXCHANGE_MARKET_MODE_UNDEFINED    0x0 /**< Undefined */
#define CFETI_EXCHANGE_MARKET_MODE_PREOPEN      0x1 /**< Pre-Open */
#define CFETI_EXCHANGE_MARKET_MODE_OPEN         0x2 /**< Open */
#define CFETI_EXCHANGE_MARKET_MODE_PRECLOSE     0x4 /**< Pre-Close */
#define CFETI_EXCHANGE_MARKET_MODE_CLOSED       0x8 /**< Closed */
#define CFETI_EXCHANGE_MARKET_MODE_ENABLED      0x10 /**< Enabled */
#define CFETI_EXCHANGE_MARKET_MODE_DISABLED     0x20 /**< Disabled */
#define CFETI_EXCHANGE_MARKET_MODE_PRICE_LIMITS_ENABLED 0x40 /**< Price Limits Enabled */
#define CFETI_EXCHANGE_MARKET_MODE_FAST         0x80 /**< Fast */
#define CFETI_EXCHANGE_MARKET_MODE_RESTRICTED_OPEN 0x100 /**< Restricted Open */
#define CFETI_EXCHANGE_MARKET_MODE_EXCLUSIVE_RANGE_LIMIT 0x200 /**< Exclusive Range Limit */
#define CFETI_EXCHANGE_MARKET_MODE_HALTED       0x400 /**< Halted */

/*
 * Exchange Instrument State 
 */
#define CFETI_EXCHANGE_INSTRUMENT_STATE_UNDEFINED 0x0 /**< Undefined */
#define CFETI_EXCHANGE_INSTRUMENT_STATE_ENABLED 0x1 /**< Enabled */
#define CFETI_EXCHANGE_INSTRUMENT_STATE_DISABLED 0x2 /**< Disabled */
#define CFETI_EXCHANGE_INSTRUMENT_STATE_PRICE_LIMITS_ENABLED 0x4 /**< Price Limits Enabled */
#define CFETI_EXCHANGE_INSTRUMENT_STATE_FAST    0x8 /**< Fast */
#define CFETI_EXCHANGE_INSTRUMENT_STATE_RESTRICTED_OPEN 0x10 /**< Restricted Open */
#define CFETI_EXCHANGE_INSTRUMENT_STATE_EXCLUSIVE_RANGE_LIMIT 0x20 /**< Exclusive Range Limit */
#define CFETI_EXCHANGE_INSTRUMENT_STATE_UNAVAILABLE 0x40 /**< Unavailable */
#define CFETI_EXCHANGE_INSTRUMENT_STATE_HALTED  0x80 /**< Halted */
#define CFETI_EXCHANGE_INSTRUMENT_STATE_NORMAL  0x100 /**< Normal */
#define CFETI_EXCHANGE_INSTRUMENT_STATE_FROZEN  0x200 /**< Frozen */
#define CFETI_EXCHANGE_INSTRUMENT_STATE_RESERVED 0x400 /**< Reserved */
#define CFETI_EXCHANGE_INSTRUMENT_STATE_SIMPLE_RESERVED 0x800 /**< Simple Reserved */
#define CFETI_EXCHANGE_INSTRUMENT_STATE_RESERVED_UPWARD 0x1000 /**< Reserved Upward */
#define CFETI_EXCHANGE_INSTRUMENT_STATE_RESERVED_DOWNWARD 0x2000 /**< Reserved Downward */
#define CFETI_EXCHANGE_INSTRUMENT_STATE_RESUME  0x4000 /**< Resume */

/*
 * Enumerated market retain rule 
 */
#define CFETI_RETAIN_DEFAULT                    0x0 /**< Trading system default setting applies */
#define CFETI_RETAIN_INVALID                    0x1 /**< Trading system does not support market retain rules */
#define CFETI_RETAIN_MARKET                     0x2 /**< Retain complete size provided that original order was submitted as a market in bid/offer state */
#define CFETI_RETAIN_ORDER                      0x3 /**< Retain complete size provided that original order was submitted as a market in trading state */
#define CFETI_RETAIN_ALL                        0x4 /**< Retain complete size */
#define CFETI_RETAIN_PARTIAL_MARKET             0x5 /**< Retain defalut minimum size if original order was submitted in bid/offer state */
#define CFETI_RETAIN_PARTIAL_ORDER              0x6 /**< Retain defalut minimum size if original order was submitted in trading state */
#define CFETI_RETAIN_PARTIAL_ALL                0x7 /**< Retain defalut minimum size */
#define CFETI_RETAIN_NONE                       0x8 /**< Cancel any remaining size after a trade */

/*
 * Enumerated market priority follow rule 
 */
#define CFETI_PRIORITY_FOLLOW_DEFAULT           0x0 /**< Trading system default setting applies */
#define CFETI_PRIORITY_FOLLOW_INVALID           0x1 /**< Priority follow rule is not supported by this trading system */
#define CFETI_PRIORITY_FOLLOW                   0x2 /**< Minimum priority market size to be returned at the conclusion of the trade when eligigble to receive clearing period priority */

/*
 * Enumerated 2-way order skew 
 */
#define CFETI_2WAY_ORDER_SKEW_UNDEFINED         0x0 /**< Propagated market skew is not defined */
#define CFETI_2WAY_ORDER_SKEW_BETTER_BUYER      0x1 /**< Propagate in favour of the buy side */
#define CFETI_2WAY_ORDER_SKEW_BETTER_SELLER     0x2 /**< Propagate in favour of the sell side */

/*
 * 2-way order propagation preference bit-mask 
 */
#define CFETI_2WAY_ORDER_PROPAGATE_NONE         0x0 /**< Propagate no bid or offer */
#define CFETI_2WAY_ORDER_PROPAGATE_BID          0x1 /**< Propagate a bid */
#define CFETI_2WAY_ORDER_PROPAGATE_ASK          0x2 /**< Propagate an offer */

/*
 * 2-way order attributes bit-mask 
 */
#define CFETI_2WAY_ORDER_PRICE_IMPROVEMENT      0x1 /**< Apply PI to propagated markets */

/*
 * Subscribe User attributes bit-mask 
 */
#define CFETI_USER_TRADE_FEED                   0x1 /**< User is a trade feed user */

/*
 * Values of IRD trade attributes (bitmask) 
 */
#define CFETI_IRD_ATTR_IMM_DATES                0x1 /**< Payments must be made on IMM dates */
#define CFETI_IRD_ATTR_BREAK_CLAUSE             0x2 /**< A break clause has been set up */
#define CFETI_IRD_ATTR_MONTH_END_ROLLS          0x4 /**< Whether or not EOM applies */
#define CFETI_IRD_ATTR_PAYER_FIXED_INTEREST     0x8 /**< Confirm is to payer of fixed interest */
#define CFETI_IRD_ATTR_RECEIVER_FIXED_INTEREST  0x10 /**< Confirm is to receiver of fixed interest */
#define CFETI_IRD_ATTR_PAYER_FLOATING_INTEREST  0x20 /**< Confirm is to floating of fixed interest */
#define CFETI_IRD_ATTR_RECEIVER_FLOATING_INTEREST 0x40 /**< Confirm is to receiver of floating interest */
#define CFETI_IRD_ATTR_ROLL_ADJUST_END          0x80 /**< Roll dates adjust end dates */
#define CFETI_IRD_ATTR_ROLL_ADJUST_TERM         0x100 /**< Roll dates adjust termination dates */
#define CFETI_IRD_ATTR_FIXED_STUB_SHORT         0x200 /**< Fixed stub is short */
#define CFETI_IRD_ATTR_FIXED_STUB_LONG          0x400 /**< Fixed stub is long */
#define CFETI_IRD_ATTR_FLOATING_STUB_SHORT      0x800 /**< Floating stub is short */
#define CFETI_IRD_ATTR_FLOATING_STUB_LONG       0x1000 /**< Floating stub is long */
#define CFETI_IRD_ATTR_STUB_AT_START            0x2000 /**< Interim period of non-standard length at beginning */
#define CFETI_IRD_ATTR_STUB_AT_END              0x4000 /**< Interim period of non-standard length at end */
#define CFETI_IRD_ATTR_FRA_DISCOUNTING          0x8000 /**< If set net payment is made on value date of the FRA, other wise on maturity */
#define CFETI_IRD_ATTR_LINEAR_INTERPOLATION     0x10000 /**< Must be set if 2nd index tenor applies */
#define CFETI_IRD_ATTR_STUB_LINEAR_INTERPOLATION 0x20000 /**< Must be set if IRS stub start and end date are applied */
#define CFETI_IRD_ATTR_WRITTEN_CONFIRM_REQUIRED 0x40000 /**< Written confirmation of trade is required */
#define CFETI_IRD_ATTR_BUY                      0x80000 /**< The option is to buy */
#define CFETI_IRD_ATTR_SELL                     0x100000 /**< The option is to sell */
#define CFETI_IRD_ATTR_AUTO_EXERCISE            0x200000 /**< Whether or not swaption is automatically exercised on expiry */

/*
 * Master Agreement 
 */
#define CFETI_MASTER_AGREEMENT_NOT_SPECIFIED    0x0
#define CFETI_MASTER_AGREEMENT_ISDA             0x1
#define CFETI_MASTER_AGREEMENT_AFB_FBF          0x2
#define CFETI_MASTER_AGREEMENT_DRV              0x3

/*
 * Compounding Method 
 */
#define CFETI_COMPOUNDING_METHOD_NOT_SPECIFIED  0x0
#define CFETI_COMPOUNDING_METHOD_NORMAL         0x1
#define CFETI_COMPOUNDING_METHOD_NORMAL_WITH_SPREAD 0x2
#define CFETI_COMPOUNDING_METHOD_FLAT_WITHOUT_SPREAD 0x3

/*
 * Compounding Frequency 
 */
#define CFETI_COMPOUNDING_FREQUENCY_NOT_SPECIFIED 0x0
#define CFETI_COMPOUNDING_FREQUENCY_ANNUAL      0x1
#define CFETI_COMPOUNDING_FREQUENCY_SEMI_ANNUAL 0x2
#define CFETI_COMPOUNDING_FREQUENCY_QUARTERLY   0x3
#define CFETI_COMPOUNDING_FREQUENCY_MONTHLY     0x4

/*
 * IRD Floating Rate Index 
 */
#define CFETI_IRD_FLOATING_RATE_INDEX_NOT_SPECIFIED 0x0
#define CFETI_IRD_FLOATING_RATE_INDEX_CHF_LIBOR_BBA 0x1
#define CFETI_IRD_FLOATING_RATE_INDEX_DDK_CIBOR_DKNA13 0x2
#define CFETI_IRD_FLOATING_RATE_INDEX_EUR_EURIBOR_TELERATE 0x3
#define CFETI_IRD_FLOATING_RATE_INDEX_EUR_LIBOR_BBA 0x4
#define CFETI_IRD_FLOATING_RATE_INDEX_GBP_LIBOR_BBA 0x5
#define CFETI_IRD_FLOATING_RATE_INDEX_JPY_LIBOR_BBA 0x6
#define CFETI_IRD_FLOATING_RATE_INDEX_NOK_NIBOR_NIBR 0x8
#define CFETI_IRD_FLOATING_RATE_INDEX_SEK_STIBOR_SIDE 0x9
#define CFETI_IRD_FLOATING_RATE_INDEX_USD_LIBOR_BBA 0xa
#define CFETI_IRD_FLOATING_RATE_INDEX_CAD_LIBOR_BBA 0xb
#define CFETI_IRD_FLOATING_RATE_INDEX_THB_SOR_TELERATE 0xc
#define CFETI_IRD_FLOATING_RATE_INDEX_THB_THBFIX_REUTERS 0xd
#define CFETI_IRD_FLOATING_RATE_INDEX_IDR_SOR_TELERATE 0xe
#define CFETI_IRD_FLOATING_RATE_INDEX_ZAR_JIBAR_SAFEX 0xf
#define CFETI_IRD_FLOATING_RATE_INDEX_HUF_BUBOR_REUTERS 0x10
#define CFETI_IRD_FLOATING_RATE_INDEX_CZK_PRIBOR_PRBO 0x11
#define CFETI_IRD_FLOATING_RATE_INDEX_PLZ_WIBOR_WIBO 0x12
#define CFETI_IRD_FLOATING_RATE_INDEX_AUD_LIBOR_BBA 0x13
#define CFETI_IRD_FLOATING_RATE_INDEX_AUD_BBR_BBSW 0x14
#define CFETI_IRD_FLOATING_RATE_INDEX_NZD_BBR_FRA 0x15
#define CFETI_IRD_FLOATING_RATE_INDEX_NZD_BBR_TELERATE 0x16
#define CFETI_IRD_FLOATING_RATE_INDEX_SGD_SOR_TELERATE 0x17
#define CFETI_IRD_FLOATING_RATE_INDEX_HKD_HIBOR_ISDC 0x18
#define CFETI_IRD_FLOATING_RATE_INDEX_KHD_HIBOR_HKAB 0x19
#define CFETI_IRD_FLOATING_RATE_INDEX_HKD_HIBOR_HIBOR 0x1a
#define CFETI_IRD_FLOATING_RATE_INDEX_KRW_CD_3220 0x1b
#define CFETI_IRD_FLOATING_RATE_INDEX_KRW_CD_KSDA_BLOOMBERG 0x1c
#define CFETI_IRD_FLOATING_RATE_INDEX_CAD_CORRA_OIS_COMPOUND 0x1d
#define CFETI_IRD_FLOATING_RATE_INDEX_CHF_TOIS_OIS_COMPOUND 0x1e
#define CFETI_IRD_FLOATING_RATE_INDEX_DKK_DKKOIS_OIS_COMPOUND 0x1f
#define CFETI_IRD_FLOATING_RATE_INDEX_GBP_WMBA_SONIA_COMPOUND 0x20
#define CFETI_IRD_FLOATING_RATE_INDEX_JPY_TONA_OIS_COMPOUND 0x21
#define CFETI_IRD_FLOATING_RATE_INDEX_SEK_SIOR_OIS_COMPOUND 0x22
#define CFETI_IRD_FLOATING_RATE_INDEX_USD_FEDERAL_FUNDS_H15_OIS_COMPOUND 0x23
#define CFETI_IRD_FLOATING_RATE_INDEX_CAD_BA_CDOR 0x24
#define CFETI_IRD_FLOATING_RATE_INDEX_JPY_TIBOR_ZTIBOR 0x27
#define CFETI_IRD_FLOATING_RATE_INDEX_SKK_BRIBOR_NBSK07 0x28
#define CFETI_IRD_FLOATING_RATE_INDEX_EUR_EONIA_OIS_COMPOUND 0x29
#define CFETI_IRD_FLOATING_RATE_INDEX_CAD_REPO_CORRA 0x2a
#define CFETI_IRD_FLOATING_RATE_INDEX_DKK_CIBOR2_DKNA13 0x2b
#define CFETI_IRD_FLOATING_RATE_INDEX_EUR_ANNUAL_SWAP_RATE_1000 0x2c
#define CFETI_IRD_FLOATING_RATE_INDEX_GBP_SEMI_ANNUAL_SWAP_RATE 0x2d
#define CFETI_IRD_FLOATING_RATE_INDEX_EUR_TEC10_CNO 0x2e
#define CFETI_IRD_FLOATING_RATE_INDEX_HKD_HIBOR_HKAB 0x2f
#define CFETI_IRD_FLOATING_RATE_INDEX_JPY_ISDA_SWAP_RATE_1500 0x30
#define CFETI_IRD_FLOATING_RATE_INDEX_JPY_TSR_TELERATE_1500 0x31
#define CFETI_IRD_FLOATING_RATE_INDEX_NZD_BBR_ISDC 0x32
#define CFETI_IRD_FLOATING_RATE_INDEX_NZD_REFERENCE_BANKS 0x33
#define CFETI_IRD_FLOATING_RATE_INDEX_NZD_NZIONA_OIS_COMPOUND 0x34
#define CFETI_IRD_FLOATING_RATE_INDEX_SKK_BRIBOR_BLOOMBERG 0x35
#define CFETI_IRD_FLOATING_RATE_INDEX_SKK_BRIBOR_BRBO 0x36
#define CFETI_IRD_FLOATING_RATE_INDEX_USD_BA_H_15 0x37
#define CFETI_IRD_FLOATING_RATE_INDEX_USD_CD_H_15 0x38
#define CFETI_IRD_FLOATING_RATE_INDEX_USD_CP_H_15 0x39
#define CFETI_IRD_FLOATING_RATE_INDEX_USD_CMS_TELERATE 0x3a
#define CFETI_IRD_FLOATING_RATE_INDEX_USD_CMT_T7051 0x3b
#define CFETI_IRD_FLOATING_RATE_INDEX_USD_ISDA_SWAP_RATE 0x3d
#define CFETI_IRD_FLOATING_RATE_INDEX_USD_PRIME_H15 0x3e
#define CFETI_IRD_FLOATING_RATE_INDEX_USD_TBILL_H15 0x3f
#define CFETI_IRD_FLOATING_RATE_INDEX_JPY_TIBOR_TIBM_10_BANKS 0x40
#define CFETI_IRD_FLOATING_RATE_INDEX_JPY_TIBOR_TIBM_5_BANKS 0x41
#define CFETI_IRD_FLOATING_RATE_INDEX_JPY_TONIA_OIS_COMPOUND 0x42
#define CFETI_IRD_FLOATING_RATE_INDEX_THB_SOR_REFERENCE_BANKS 0x43
#define CFETI_IRD_FLOATING_RATE_INDEX_GRD_ATHIMID_REUTERS 0x44
#define CFETI_IRD_FLOATING_RATE_INDEX_SKK_BRIBOR_REFERENCE_BANKS 0x45
#define CFETI_IRD_FLOATING_RATE_INDEX_TWD_TELERATE_6165 0x46
#define CFETI_IRD_FLOATING_RATE_INDEX_TWD_TWCPBA 0x47
#define CFETI_IRD_FLOATING_RATE_INDEX_TWD_REFERENCE_DEALERS 0x48
#define CFETI_IRD_FLOATING_RATE_INDEX_INR_MIBOR_OIS_COMPOUND 0x49
#define CFETI_IRD_FLOATING_RATE_INDEX_INR_MITOR_OIS_COMPOUND 0x4a
#define CFETI_IRD_FLOATING_RATE_INDEX_INR_MIFOR 0x4b
#define CFETI_IRD_FLOATING_RATE_INDEX_INR_MIOIS 0x4c
#define CFETI_IRD_FLOATING_RATE_INDEX_INR_BMK   0x4d
#define CFETI_IRD_FLOATING_RATE_INDEX_INR_INBMK_REUTERS 0x4e
#define CFETI_IRD_FLOATING_RATE_INDEX_INR_CMT   0x4f
#define CFETI_IRD_FLOATING_RATE_INDEX_INR_REFERENCE_BANKS 0x50
#define CFETI_IRD_FLOATING_RATE_INDEX_AUD_AONIA_OIS_COMPOUND 0x51
#define CFETI_IRD_FLOATING_RATE_INDEX_HKD_HONIX_OIS_COMPOUND 0x52
#define CFETI_IRD_FLOATING_RATE_INDEX_SGD_SOR_REFERENCE_BANKS 0x53
#define CFETI_IRD_FLOATING_RATE_INDEX_SGD_SIBOR_TELERATE 0x54
#define CFETI_IRD_FLOATING_RATE_INDEX_SGD_SIBOR_REFERENCE_BANKS 0x55
#define CFETI_IRD_FLOATING_RATE_INDEX_SGD_SONAR_OIS_COMPOUND 0x56
#define CFETI_IRD_FLOATING_RATE_INDEX_JPY_TIBOR_17096 0x57
#define CFETI_IRD_FLOATING_RATE_INDEX_JPY_TIBOR_17097 0x58
#define CFETI_IRD_FLOATING_RATE_INDEX_JPY_TIBOR_DTIBOR01 0x59
#define CFETI_IRD_FLOATING_RATE_INDEX_IDR_SOR_REFERENCE_BANKS 0x5a
#define CFETI_IRD_FLOATING_RATE_INDEX_MYR_KLIBOR_BNM 0x5b
#define CFETI_IRD_FLOATING_RATE_INDEX_MYR_KLIBOR_REFERENCE_BANKS 0x5c
#define CFETI_IRD_FLOATING_RATE_INDEX_CHF_LIBOR_CHF 0x5d

/*
 * Contract State 
 */
#define CFETI_CONTRACT_STATE_NOT_SPECIFIED      0x0
#define CFETI_CONTRACT_STATE_NEW_WITH_TRADER    0x1
#define CFETI_CONTRACT_STATE_WITHDRAWN_BY_BROKER 0x2
#define CFETI_CONTRACT_STATE_REJECTED_BY_TRADER 0x3
#define CFETI_CONTRACT_STATE_REVISION_REQUESTED_BY_TRADER 0x4

/*
 * Basis 
 */
#define CFETI_BASIS_NOT_SPECIFIED               0x0
#define CFETI_BASIS_1_1                         0x1
#define CFETI_BASIS_ACT_365_ISDA                0x2
#define CFETI_BASIS_ACT_ACT_ISMA                0x3
#define CFETI_BASIS_ACT_ACT_AFB                 0x4
#define CFETI_BASIS_ACT_365_FIXED               0x5
#define CFETI_BASIS_ACT_360                     0x6
#define CFETI_BASIS_30_360                      0x7
#define CFETI_BASIS_30E_360                     0x8

/*
 * Payment Convention 
 */
#define CFETI_PAYMENT_CONVENTION_NOT_SPECIFIED  0x0
#define CFETI_PAYMENT_CONVENTION_MODIFIED_FOLLOWING 0x1
#define CFETI_PAYMENT_CONVENTION_FOLLOWING      0x2
#define CFETI_PAYMENT_CONVENTION_PRECEDING      0x3

/*
 * Calculation Agent 
 */
#define CFETI_CALCULATION_AGENT_NOT_SPECIFIED   0x0
#define CFETI_CALCULATION_AGENT_BUYER           0x1
#define CFETI_CALCULATION_AGENT_SELLER          0x2
#define CFETI_CALCULATION_AGENT_JOINT           0x3

/*
 * IRD Sub Type 
 */
#define CFETI_IRD_SUB_TYPE_NOT_SPECIFIED        0x0
#define CFETI_IRD_SUB_TYPE_CAPS                 0x1
#define CFETI_IRD_SUB_TYPE_FLOORS               0x2
#define CFETI_IRD_SUB_TYPE_PAYERS               0x3
#define CFETI_IRD_SUB_TYPE_RECEIVERS            0x4
#define CFETI_IRD_SUB_TYPE_STRADDLE             0x5

/*
 * Cash method 
 */
#define CFETI_CASH_METHOD_NOT_SPECIFIED         0x0
#define CFETI_CASH_METHOD_CASH_PRICE            0x1
#define CFETI_CASH_METHOD_CASH_PRICE_ALTERNATE  0x2
#define CFETI_CASH_METHOD_PAR_YIELD_CURVE       0x3
#define CFETI_CASH_METHOD_UNADJUSTED            0x4
#define CFETI_CASH_METHOD_ZERO_COUPON_YIELD     0x5

/*
 * Quotation Rate 
 */
#define CFETI_QUOTATION_RATE_NOT_SPECIFIED      0x0
#define CFETI_QUOTATION_RATE_ASK                0x1
#define CFETI_QUOTATION_RATE_BID                0x2
#define CFETI_QUOTATION_RATE_MID                0x3

/*
 * Interval Unit 
 */
#define CFETI_INTERVAL_UNIT_NOT_SPECIFIED       0x0
#define CFETI_INTERVAL_UNIT_DAY                 0x1
#define CFETI_INTERVAL_UNIT_WEEK                0x2
#define CFETI_INTERVAL_UNIT_MONTH               0x3
#define CFETI_INTERVAL_UNIT_YEAR                0x4
#define CFETI_INTERVAL_UNIT_TERM                0x5

/*
 * Enumerated IRS Bond 
 */
#define CFETI_IRD_IRS_BOND_NOT_SPECIFIED        0x0
#define CFETI_IRD_IRS_BOND_USG_02Y              0x1
#define CFETI_IRD_IRS_BOND_USG_03Y              0x2
#define CFETI_IRD_IRS_BOND_USG_05Y              0x3
#define CFETI_IRD_IRS_BOND_USG_10Y              0x4
#define CFETI_IRD_IRS_BOND_USG_30Y              0x5

/*
 * Market Data contribution bit mask 
 */
#define CFETI_CONTRIBUTION_NONE                 0x0
#define CFETI_CONTRIBUTION_LAST_PRICES          0x1
#define CFETI_CONTRIBUTION_PV01_LOCK            0x2
#define CFETI_CONTRIBUTION_CLOSE                0x4
#define CFETI_CONTRIBUTION_ALL                  0x80000000

/*
 * Values for Extended Attributes field 
 */
#define CFETI_ATTR_NO_BOND_CROSS                0x1

/*
 * Credit type (enumeration) 
 */
#define CFETI_CREDIT_TYPE_UNKNOWN               0x0 /**< User relationship type is not defined */
#define CFETI_CREDIT_TYPE_FX                    0x1 /**< Credit type for eSpeed FX */

/*
 * REPO Instrument properties (bitmask) 
 */
#define CFETI_ESPD_REPO_PROPERTY_ON_THE_RUN     0x1 /**< Underlying bond is an on-the-run */
#define CFETI_ESPD_REPO_PROPERTY_OFF_THE_RUN    0x2 /**< Underlying bond is an off-the-run */
#define CFETI_ESPD_REPO_PROPERTY_BILL           0x4 /**< Underlying bond is a bill */
#define CFETI_ESPD_REPO_PROPERTY_AGENCY         0x8 /**< Underlying bond is an Agency bond */
#define CFETI_ESPD_REPO_PROPERTY_TERM_SPECIAL   0x10 /**< Any underlying bond */
#define CFETI_ESPD_REPO_PROPERTY_CASH           0x20 /**< Cash settlement */
#define CFETI_ESPD_REPO_PROPERTY_REG            0x40 /**< Regular settlement */
#define CFETI_ESPD_REPO_PROPERTY_OVERNIGHT      0x80 /**< Repurchase will be on the next business day */
#define CFETI_ESPD_REPO_PROPERTY_TERM           0x100 /**< Repurchace will be on a specific date */
#define CFETI_ESPD_REPO_PROPERTY_AON            0x200 /**< REPO is all-or-none */

#endif /* _CFETI_CONSTS_ */
