/*
 * eSpeed, Inc.
 * 
 * This system and/or its use is protected under one or more of the following
 * United States patents:  5,905,974; 6,560,580.  All rights reserved.  Other
 * patents pending.
 * 
 * cfeti_fields.h generated 02/21/2006 18:09:55 - do not edit directly
 */

#if !defined(_CFETI_FIELDS_)
#define _CFETI_FIELDS_

/*
 * eSpeed Market Data Fields. Valid field identifiers are in the range 0x0001
 * to 0xff00 (1 to 65280). 
 */
#define CFETF_BID_1                             0x1
#define CFETF_BID_2                             0x2
#define CFETF_BID_3                             0x3
#define CFETF_BID_4                             0x4
#define CFETF_BID_5                             0x5
#define CFETF_BID                               CFETF_BID_1
#define CFETF_BID_SIZE_1                        0x6
#define CFETF_BID_SIZE_2                        0x7
#define CFETF_BID_SIZE_3                        0x8
#define CFETF_BID_SIZE_4                        0x9
#define CFETF_BID_SIZE_5                        0xa
#define CFETF_BID_SIZE                          CFETF_BID_SIZE_1
#define CFETF_BID_LIST_1                        0xb
#define CFETF_BID_LIST_2                        0xc
#define CFETF_BID_LIST_3                        0xd
#define CFETF_BID_LIST_4                        0xe
#define CFETF_BID_LIST_5                        0xf
#define CFETF_BID_LIST                          CFETF_BID_LIST_1
#define CFETF_ASK_1                             0x10
#define CFETF_ASK_2                             0x11
#define CFETF_ASK_3                             0x12
#define CFETF_ASK_4                             0x13
#define CFETF_ASK_5                             0x14
#define CFETF_ASK                               CFETF_ASK_1
#define CFETF_ASK_SIZE_1                        0x15
#define CFETF_ASK_SIZE_2                        0x16
#define CFETF_ASK_SIZE_3                        0x17
#define CFETF_ASK_SIZE_4                        0x18
#define CFETF_ASK_SIZE_5                        0x19
#define CFETF_ASK_SIZE                          CFETF_ASK_SIZE_1
#define CFETF_ASK_LIST_1                        0x1a
#define CFETF_ASK_LIST_2                        0x1b
#define CFETF_ASK_LIST_3                        0x1c
#define CFETF_ASK_LIST_4                        0x1d
#define CFETF_ASK_LIST_5                        0x1e
#define CFETF_ASK_LIST                          CFETF_ASK_LIST_1
#define CFETF_STATE                             0x1f
#define CFETF_LAST_1                            0x20
#define CFETF_LAST_2                            0x21
#define CFETF_LAST_3                            0x22
#define CFETF_LAST_4                            0x154
#define CFETF_LAST_5                            0x155
#define CFETF_LAST                              CFETF_LAST_1
#define CFETF_LAST_SIZE_1                       0x23
#define CFETF_LAST_SIZE_2                       0x24
#define CFETF_LAST_SIZE_3                       0x25
#define CFETF_LAST_SIZE_4                       0x156
#define CFETF_LAST_SIZE_5                       0x157
#define CFETF_LAST_SIZE                         CFETF_LAST_SIZE_1
#define CFETF_LAST_YIELD                        0x26
#define CFETF_LAST_TIME                         0x27
#define CFETF_VOLUME                            0x28
#define CFETF_OPEN                              0x29
#define CFETF_HIGH                              0x2a
#define CFETF_LOW                               0x2b
#define CFETF_CLOSE                             0x2c
#define CFETF_TICK                              0x2d
#define CFETF_BID_YIELD                         0x2e
#define CFETF_ASK_YIELD                         0x2f
#define CFETF_COUPON                            0x30
#define CFETF_MAT_DATE                          0x31
#define CFETF_ISSUE_DATE                        0x32
#define CFETF_SETTLEMENT_DATE                   0x33
#define CFETF_SETTLEMENT                        0x34
#define CFETF_FIRST_COUPON                      0x35
#define CFETF_CANTORID                          0x36
#define CFETF_CUSIP                             0x37
#define CFETF_ISIN                              0x38
#define CFETF_DESCRIPTION                       0x39
#define CFETF_STATUS                            0x3a
#define CFETF_PRICE_TICK                        0x3b
#define CFETF_SIZE_MULTIPLIER                   0x3c
#define CFETF_TRADE_PRICE                       0x3d
#define CFETF_TRADE_SIZE                        0x3e
#define CFETF_BUY_SIZE                          0x3f
#define CFETF_BUY_LIST                          0x40
#define CFETF_SELL_SIZE                         0x41
#define CFETF_SELL_LIST                         0x42
#define CFETF_TYPE                              0x43 /**< Reserved for future use */
#define CFETF_SUBTYPE                           0x44 /**< Reserved for future use */
#define CFETF_SUBSYSTEM                         0x45
#define CFETF_AUCTION_DATE                      0x46
#define CFETF_ANNOUNCEMENT_DATE                 0x47
#define CFETF_ALT_INST_1                        0x48
#define CFETF_ALT_INST                          CFETF_ALT_INST_1
#define CFETF_PRICETYPE                         0x49
#define CFETF_MINIMUM_SIZE                      0x53
#define CFETF_SIZE_TICK                         0x54
#define CFETF_LABEL                             0x55
#define CFETF_GROUP                             0x56
#define CFETF_SUBGROUP                          0x57
#define CFETF_INSTGROUP                         0x58
#define CFETF_PRICE_SEPARATOR                   0x5b
#define CFETF_COLLATERAL_ALL_IN_PRICE           0x5d
#define CFETF_LAST_BID                          0x5e
#define CFETF_LAST_ASK                          0x5f
#define CFETF_END_DATE                          0x60
#define CFETF_REPO_END_DATE                     CFETF_END_DATE
#define CFETF_PRICE_TICK_RULES                  0x61
#define CFETF_COLLATERAL                        0x62
#define CFETF_SETTLEMENT_PRICE_TYPE             0x63
#define CFETF_PRICE_DEC_PLACES                  0x64
#define CFETF_PRICE_MIN_DEC_PLACES              0x65
#define CFETF_SETTLEMENT_PRICE_DEC_PLACES       0x66
#define CFETF_SETTLEMENT_PRICE_MIN_DEC_PLACES   0x67
#define CFETF_BID_CASH_PRICE                    0x68
#define CFETF_ASK_CASH_PRICE                    0x69
#define CFETF_BID_CORP_PRICE                    0x6a
#define CFETF_ASK_CORP_PRICE                    0x6b
#define CFETF_BID_SKIP_PRICE                    0x6c
#define CFETF_ASK_SKIP_PRICE                    0x6d
#define CFETF_ACCRUED_INTEREST                  0x6e
#define CFETF_NUM_COUPONS                       0x6f
#define CFETF_FIRST_PAYMENT                     0x70
#define CFETF_SHORT_COUPON                      0x71
#define CFETF_YEARS_TO_MATURITY                 0x72
#define CFETF_CASH_ACCRUED_INTEREST             0x73
#define CFETF_CASH_NUM_COUPONS                  0x74
#define CFETF_CASH_FIRST_PAYMENT                0x75
#define CFETF_CASH_SHORT_COUPON                 0x76
#define CFETF_CASH_YEARS_TO_MATURITY            0x77
#define CFETF_SKIP_ACCRUED_INTEREST             0x78
#define CFETF_SKIP_NUM_COUPONS                  0x79
#define CFETF_SKIP_FIRST_PAYMENT                0x7a
#define CFETF_SKIP_SHORT_COUPON                 0x7b
#define CFETF_SKIP_YEARS_TO_MATURITY            0x7c
#define CFETF_CORP_ACCRUED_INTEREST             0x7d
#define CFETF_CORP_NUM_COUPONS                  0x7e
#define CFETF_CORP_FIRST_PAYMENT                0x7f
#define CFETF_CORP_SHORT_COUPON                 0x80
#define CFETF_CORP_YEARS_TO_MATURITY            0x81
#define CFETF_ALT_DESCRIPTION_1                 0x82
#define CFETF_SHORT_DESCRIPTION                 CFETF_ALT_DESCRIPTION_1
#define CFETF_HEDGE_RATIO                       0x83
#define CFETF_CONVERSION_FACTOR                 0x84 /**< The conversion factor for a basis instrument */
#define CFETF_TSWAP_RATIO                       CFETF_CONVERSION_FACTOR /**< For US Treasury Swaps */
#define CFETF_PV01_RATIO                        CFETF_CONVERSION_FACTOR /**< For IRS vs Futures */
#define CFETF_INSTRUMENT_SETTLEMENT_DATE        0x85
#define CFETF_BID_LIST_6                        0x86
#define CFETF_BID_LIST_7                        0x87
#define CFETF_BID_LIST_8                        0x88
#define CFETF_BID_LIST_9                        0x89
#define CFETF_BID_LIST_10                       0x8a
#define CFETF_ASK_LIST_6                        0x8b
#define CFETF_ASK_LIST_7                        0x8c
#define CFETF_ASK_LIST_8                        0x8d
#define CFETF_ASK_LIST_9                        0x8e
#define CFETF_ASK_LIST_10                       0x8f
#define CFETF_BID_6                             0x90
#define CFETF_BID_7                             0x91
#define CFETF_BID_8                             0x92
#define CFETF_BID_9                             0x93
#define CFETF_BID_10                            0x94
#define CFETF_ASK_6                             0x95
#define CFETF_ASK_7                             0x96
#define CFETF_ASK_8                             0x97
#define CFETF_ASK_9                             0x98
#define CFETF_ASK_10                            0x99
#define CFETF_BID_SIZE_6                        0x9a
#define CFETF_BID_SIZE_7                        0x9b
#define CFETF_BID_SIZE_8                        0x9c
#define CFETF_BID_SIZE_9                        0x9d
#define CFETF_BID_SIZE_10                       0x9e
#define CFETF_ASK_SIZE_6                        0x9f
#define CFETF_ASK_SIZE_7                        0xa0
#define CFETF_ASK_SIZE_8                        0xa1
#define CFETF_ASK_SIZE_9                        0xa2
#define CFETF_ASK_SIZE_10                       0xa3
#define CFETF_BUY_LIST_1                        CFETF_BUY_LIST
#define CFETF_BUY_LIST_2                        0xa4
#define CFETF_BUY_LIST_3                        0xa5
#define CFETF_BUY_LIST_4                        0xa6
#define CFETF_BUY_LIST_5                        0xa7
#define CFETF_BUY_LIST_6                        0xa8
#define CFETF_BUY_LIST_7                        0xa9
#define CFETF_BUY_LIST_8                        0xaa
#define CFETF_BUY_LIST_9                        0xab
#define CFETF_BUY_LIST_10                       0xac
#define CFETF_SELL_LIST_1                       CFETF_SELL_LIST
#define CFETF_SELL_LIST_2                       0xad
#define CFETF_SELL_LIST_3                       0xae
#define CFETF_SELL_LIST_4                       0xaf
#define CFETF_SELL_LIST_5                       0xb0
#define CFETF_SELL_LIST_6                       0xb1
#define CFETF_SELL_LIST_7                       0xb2
#define CFETF_SELL_LIST_8                       0xb3
#define CFETF_SELL_LIST_9                       0xb4
#define CFETF_SELL_LIST_10                      0xb5
#define CFETF_BUY_SIZE_1                        CFETF_BUY_SIZE
#define CFETF_BUY_SIZE_2                        0xb6
#define CFETF_BUY_SIZE_3                        0xb7
#define CFETF_BUY_SIZE_4                        0xb8
#define CFETF_BUY_SIZE_5                        0xb9
#define CFETF_BUY_SIZE_6                        0xba
#define CFETF_BUY_SIZE_7                        0xbb
#define CFETF_BUY_SIZE_8                        0xbc
#define CFETF_BUY_SIZE_9                        0xbd
#define CFETF_BUY_SIZE_10                       0xbe
#define CFETF_SELL_SIZE_1                       CFETF_SELL_SIZE
#define CFETF_SELL_SIZE_2                       0xbf
#define CFETF_SELL_SIZE_3                       0xc0
#define CFETF_SELL_SIZE_4                       0xc1
#define CFETF_SELL_SIZE_5                       0xc2
#define CFETF_SELL_SIZE_6                       0xc3
#define CFETF_SELL_SIZE_7                       0xc4
#define CFETF_SELL_SIZE_8                       0xc5
#define CFETF_SELL_SIZE_9                       0xc6
#define CFETF_SELL_SIZE_10                      0xc7
#define CFETF_TRADE_PRICE_1                     CFETF_TRADE_PRICE
#define CFETF_TRADE_PRICE_2                     0xc8
#define CFETF_TRADE_PRICE_3                     0xc9
#define CFETF_TRADE_PRICE_4                     0xca
#define CFETF_TRADE_PRICE_5                     0xcb
#define CFETF_TRADE_PRICE_6                     0xcc
#define CFETF_TRADE_PRICE_7                     0xcd
#define CFETF_TRADE_PRICE_8                     0xce
#define CFETF_TRADE_PRICE_9                     0xcf
#define CFETF_TRADE_PRICE_10                    0xd0
#define CFETF_TRADE_SIZE_1                      CFETF_TRADE_SIZE
#define CFETF_TRADE_SIZE_2                      0xd1
#define CFETF_TRADE_SIZE_3                      0xd2
#define CFETF_TRADE_SIZE_4                      0xd3
#define CFETF_TRADE_SIZE_5                      0xd4
#define CFETF_TRADE_SIZE_6                      0xd5
#define CFETF_TRADE_SIZE_7                      0xd6
#define CFETF_TRADE_SIZE_8                      0xd7
#define CFETF_TRADE_SIZE_9                      0xd8
#define CFETF_TRADE_SIZE_10                     0xd9
#define CFETF_STATE_1                           CFETF_STATE
#define CFETF_STATE_2                           0xda
#define CFETF_STATE_3                           0xdb
#define CFETF_STATE_4                           0xdc
#define CFETF_STATE_5                           0xdd
#define CFETF_STATE_6                           0xde
#define CFETF_STATE_7                           0xdf
#define CFETF_STATE_8                           0xe0
#define CFETF_STATE_9                           0xe1
#define CFETF_STATE_10                          0xe2
#define CFETF_SWAP_DURATION                     0xe3
#define CFETF_NET_CHANGE                        0xe4
#define CFETF_PCT_NET_CHANGE                    0xe5
#define CFETF_PROPERTIES                        0xe6 /**< Instrument properties bit mask field */
#define CFETF_INSTRUMENT_CLASSIFICATION         0xe7 /**< Used for giveup */
#define CFETF_ENERGY_INDEX_TYPE                 0xe8
#define CFETF_FUTURE_CROSS_PRICE                0xe9 /**< Futures Cross price at which the trade is executing */
#define CFETF_CUTOFF_TIME                       0xeb
#define CFETF_CUTOFF_REGION                     0xec
#define CFETF_CONTRACT_DATE                     0xed
#define CFETF_EXPIRY_DATE                       0xee
#define CFETF_DELIVERY_DATE                     0xef
#define CFETF_INST_CLASSIFICATION               0xf0 /**< Classification string */
#define CFETF_PRICING_METHOD                    0xf1
#define CFETF_ALT_DESCRIPTION_2                 0x11a
#define CFETF_START_DATE                        0x11b
#define CFETF_PAYMENT_DATE                      0x11c
#define CFETF_BREAK_CLAUSE                      0x11d
#define CFETF_TRADE_FLAGS                       0x11e
#define CFETF_ACCUMULATED_LAST_SIZE             0x122
#define CFETF_DD_INST_PROPERTY                  0x12e
#define CFETF_DD_SYSTEM_EXPIRY                  0x12f
#define CFETF_ENERGY_START_CALENDAR_DATE        0x130
#define CFETF_ENERGY_END_CALENDAR_DATE          0x131
#define CFETF_ENERGY_CONTRACT_TYPE              0x132
#define CFETF_ENERGY_POINT_NAME                 0x133
#define CFETF_ENERGY_POINT_NUMBER               0x134
#define CFETF_ENERGY_QUANTITY_UNIT              0x135
#define CFETF_ENERGY_PRICE_TYPE                 0x136
#define CFETF_ENERGY_INDEX_NAME                 0x137
#define CFETF_ENERGY_LOAD_PROFILE               0x138
#define CFETF_ENERGY_PRODUCT_PERIOD             0x139
#define CFETF_ENERGY_SETTLEMENT_TYPE            0x13a
#define CFETF_ENERGY_SHORT_LOCATION             0x13b
#define CFETF_ENERGY_SHORT_INDEX_NAME           0x13c
#define CFETF_ENERGY_FIRST_TRADE_DATE           0x13d
#define CFETF_ENERGY_LAST_TRADE_DATE            0x13e
#define CFETF_ENERGY_FIRST_TRADE_TIME           0x13f
#define CFETF_ENERGY_LAST_TRADE_TIME            0x140
#define CFETF_ENERGY_PRICE_MECHANISM            0x141
#define CFETF_COMMODITY                         0x142
#define CFETF_TIME_ZONE                         0x143
#define CFETF_CURRENCY                          0x144
#define CFETF_INDICATIVE_BID                    0x145 /**< Indicative bid price */
#define CFETF_INDICATIVE_OFFER                  0x146 /**< Indicative offer price */
#define CFETF_MID_PRICE                         0x147 /**< Mid-price */
#define CFETF_INDICATIVE_STATE                  0x148 /**< Indicative market state */
#define CFETF_CONTRIBUTOR                       0x149 /**< Contributor name */
#define CFETF_NAME                              0x152 /**< eSpeed instrument name */
#define CFETF_DAILY_LAST_TRADED_PRICE           0x153 /**< Intra-day last traded price */
#define CFETF_LOCK_PRICE                        0x158 /**< Lock price of a benchmark instrument */
#define CFETF_MAX_PRICE_DELTA                   0x159 /**< Maximum delta that a trader can aggress away from the touch price where trading through the stack is supported */
#define CFETF_PI_PRICE_TICK_DENOMINATOR         0x15a /**< Minimum price increment for price improvement will be the reciprocal of this value */
#define CFETF_PI_PRICE_TICK_COUNT_MIN           0x15b /**< Lowest multiple reciprocal of tick denominator */
#define CFETF_PI_PRICE_TICK_COUNT_MAX           0x15c /**< Highest multiple of reciprocal of tick denominator */
#define CFETF_PI_PRICE_TICK_COUNT_STEP          0x15d /**< Step between tick count increments */
#define CFETF_PI_PRICE_TICK_PROPERTIES          0x15e /**< Bitmask field to describe the properties of price improvement price tick behaviour.  */
#define CFETF_TRADE_PREF                        0x15f /**< Populated by the trading system with the bit-wise combination of supported market and order preferences for the instrument */
#define CFETF_PI_PRICE_TICK_DENOMINATOR_SCALAR  0x162 /**< Value to modify the published denominator */
#define CFETF_BID_YIELD_SPREAD                  0x163 /**< Yield spread corresponding to the best bid price */
#define CFETF_ASK_YIELD_SPREAD                  0x164 /**< Yield spread corresponding to the best ask price */
#define CFETF_YIELD_PRICE_TYPE                  0x165 /**< Format to be used to display values in the yield and yield spread fields */
#define CFETF_LOCK_YIELD                        0x166 /**< The yield value corresponding to the lock price */
#define CFETF_TOTAL_TRADE_SIZE                  0x167 /**< Total size traded during a trade */
#define CFETF_STATE_EXT                         0x168 /**< Tier 1 extended state */
#define CFETF_STATE_EXT_1                       CFETF_STATE_EXT /**< Synonym for CFETF_STATE_EXT */
#define CFETF_STATE_EXT_2                       0x169 /**< Tier 2 extended state */
#define CFETF_STATE_EXT_3                       0x16a /**< Tier 3 extended state */
#define CFETF_STATE_EXT_4                       0x16b /**< Tier 4 extended state */
#define CFETF_STATE_EXT_5                       0x16c /**< Tier 5 extended state */
#define CFETF_STATE_EXT_6                       0x16d /**< Tier 6 extended state */
#define CFETF_STATE_EXT_7                       0x16e /**< Tier 7 extended state */
#define CFETF_STATE_EXT_8                       0x16f /**< Tier 8 extended state */
#define CFETF_STATE_EXT_9                       0x170 /**< Tier 9 extended state */
#define CFETF_STATE_EXT_10                      0x171 /**< Tier 10 extended state */
#define CFETF_MID_YIELD                         0x173 /**< Mid-price yield */
#define CFETF_MARKET_DURATION                   0x174 /**< Text description of the date duration or effective start date of an energy security */
#define CFETF_COMPOUND_INST_LIST                0x175 /**< Compound instrument list [bytestream] */
#define CFETF_SIZE_FORMAT                       0x176 /**< Pricetype to use when displaying sizes (default CFETI_PRICETYPE_DECIMAL_D0) [uint32] */
#define CFETF_SIZE_FORMAT_DEC_PLACES            0x177 /**< Number of decimal places to show when SIZE_FORMAT is CFETI_PRICETYPE_DECIMAL_DVAR [uint32] */
#define CFETF_SIZE_FORMAT_MIN_DEC_PLACES        0x178 /**< Minimum number of decimal places to show when SIZE_FORMAT is CFETI_PRICETYPE_DECIMAL_DVAR [uint32] */
#define CFETF_CURRENCY_ORDINAL                  0x179 /**< FX currency ordinal number [uint32] */
#define CFETF_BASE_IS_DENOMINATOR               0x17e /**< Flag to indicate whether Price=Term Amount / Base Amount (1) or instead Price=Base Amount / Term Amount (0) */
#define CFETF_DD_REQUEST_DURATION               0x17f /**< Default number of seconds that an Direct Deal Request should be valid for */
#define CFETF_ENERGY_INDEX_TYPE_ID              0x180 /**< Enumerated energy index type */
#define CFETF_ENERGY_INDEX_TYPE_ID_2            0x181 /**< Enumerated energy index type (swap instruments only) */
#define CFETF_ENERGY_INDEX_TYPE_2               0x182 /**< Energy index type (swap instruments only) */
#define CFETF_ENERGY_INDEX_NAME_2               0x183 /**< Energy index name (swap instruments only) */
#define CFETF_ENERGY_SHORT_INDEX_NAME_2         0x184 /**< Energy index name description (swap instruments only) */
#define CFETF_TRADE_THROUGH_STACK_PRICE_TICKS   0x185 /**< Maximum number of price ticks to which trade through stack is permitted */
#define CFETF_DD_SYSTEM_ATTRIBUTES              0x186 /**< Direct Dealing system attributes for the instrument - overrides connection-wide settings */
#define CFETF_DD_LOCKOUT_DURATION               0x187 /**< Amount of time before Direct Deal Request expires during which no more quotes will be accepted */
#define CFETF_DD_MIN_QUANTITY                   0x188 /**< Default minimum quantity for Direct Deal Requests */
#define CFETF_DD_MINIMUM_SIZE                   CFETF_DD_MIN_QUANTITY /**< Minimum size allowable in a direct deal request or quote on this instrument */
#define CFETF_DD_QUOTE_SPREAD                   0x189 /**< Default maximum quote spread for the 2-Way and BOLS quotes */
#define CFETF_DISPLAY_PROPERTIES                0x196 /**< Instrument display properties bit mask field */
#define CFETF_INSTRUMENT_CHAIN                  0x197 /**< Instrument chain for root item */
#define CFETF_CHAIN_PARENT_LIST                 0x198 /**< List of all chain root instruments */
#define CFETF_SQ_INST_LABEL                     0x199 /**< Super-Quad instrument label */
#define CFETF_BENCHMARK_INST_SPEC               0x19c /**< Underlying benchmark instrument */
#define CFETF_SQ_BENCHMARK_KEY_LABEL            0x1a4 /**< Super-Quad benchmark key label */
#define CFETF_SQ_EGB_KEY_LABEL                  0x1b0 /**< Super-Quad benchmark key label */
#define CFETF_SQ_SPECIFIC_KEY_LABEL             0x1b1 /**< Super-Quad benchmark key label */
#define CFETF_OUTRIGHT_BID                      0x1b3 /**< Outright price corresponding to current price in field CFETF_BID */
#define CFETF_OUTRIGHT_ASK                      0x1b4 /**< Outright price corresponding to current price in field CFETF_ASK */
#define CFETF_OUTRIGHT_PRICETYPE                0x1b5 /**< Enumerated price type for outright bid and ask */
#define CFETF_OUTRIGHT_DEC_PLACES               0x1b6 /**< Decimal places to use when outright pricetype is CFETI_PRICETYPE_DECIMAL_DVAR */
#define CFETF_OUTRIGHT_MIN_DEC_PLACES           0x1b7 /**< Minimum number of decimal places to use when outright pricetype is CFETI_PRICETYPE_DECIMAL_DVAR */
#define CFETF_DISPLAY_PRICETYPE                 0x1bb /**< Price format to be used for display prices where the display price format is different from the input price format */
#define CFETF_DISPLAY_PRICETYPE_DEC_PLACES      0x1bc /**< Number of decimal places to display when the display price type is CFETI_PRICETYPE_DECIMAL_DVAR */
#define CFETF_DISPLAY_PRICETYPE_MIN_DEC_PLACES  0x1bd /**< Minimum number of decimal places to display when the display price type is CFETI_PRICETYPE_DECIMAL_DVAR */
#define CFETF_MINIMUM_MID_SIZE                  0x1be /**< The minimum size that must be specified on a mid-price order */
#define CFETF_MID_TRADE_SIZE                    0x1bf /**< The currently trading mid size */
#define CFETF_MARKET_AVAILABILITY_NOTIFICATION  0x1f2 /**< Notification of a market availability update */
#define CFETF_MARKET_SECTOR                     0x1f8 /**< Market sector to which the instrument belongs */
#define CFETF_SESSION_BOUNDARY_24H              0x1f9 /**< Contains the fields describing the boundary prices for the 24H session */
#define CFETF_SESSION_BOUNDARY_ASIA             0x1fa /**< Contains the fields describing the boundary prices for the Asia session */
#define CFETF_SESSION_BOUNDARY_EUROPE           0x1fb /**< Contains the fields describing the boundary prices for the Europe session */
#define CFETF_SESSION_BOUNDARY_US_CASH          0x1fc /**< Contains the fields describing the boundary prices for the US Cash session */
#define CFETF_SESSION_BOUNDARY_US_FUTURES       0x1fd /**< Contains the fields describing the boundary prices for the US Futures session */
#define CFETF_HIGH_INDICATOR                    0x1fe /**< The trade event that led to the latest high price */
#define CFETF_LOW_INDICATOR                     0x1ff /**< The trade event that led to the latest low price */
#define CFETF_LAST_FUTURE_CROSS_PRICE           0x200 /**< Futures Cross price at which the previous trade was executed */
#define CFETF_EXCHANGE_MARKET_MODE              0x206 /**< Bit-mask to describe exchange market mode */
#define CFETF_EXCHANGE_INSTRUMENT_STATE         0x207 /**< Bit-mask to describe exchange instrument state */
#define CFETF_FIRST_SETTLEMENT_DATE             0x20d /**< First settlement date (uint32, CCYYMMDD) */
#define CFETF_DIRTY_PRICE                       0x20e /**< Dirty price */
#define CFETF_COLLATERAL_PRICE                  0x20f /**< Collateral price */
#define CFETF_TICKER_SYMBOL                     0x210 /**< Ticker symbol for the instrument. Links related instruments to the issuer */
#define CFETF_CALL_DATE                         0x211 /**< The next date, prior to maturity, on which a callable bond may be redeemed */
#define CFETF_MIN_UNCLEAR_TICK_SPREAD           0x212 /**< Price increment within which a market must be of a contra market to be eleigible for clearing */
#define CFETF_MIN_MARKET_CLEARING_SIZE          0x213 /**< Minimum size that a market participant must have at the time a contra-market is received to be eleigible for clearing */
#define CFETF_MIN_PRIORITY_TRADING_SIZE         0x214 /**< Size that last worker of trade must have executed or the sum of this amount and the untraded amount prior to the conclusion of the trade */
#define CFETF_MINIMUM_2WAY_SIZE                 0x215 /**< Minimum size that can be used for propagated 2-way markets (if different from CFETF_MINIMUM_SIZE) */
#define CFETF_MARKET_AVAILABILITY_MULTI_USER    0x219 /**< Notification of a market availability update */
#define CFETF_PAGE_GROUPING                     0x21a /**< Page grouping */
#define CFETF_IRS_INST_PROPERTIES               0x21b /**< Interest Rate Swap specific instrument properties bitmask */
#define CFETF_AT_MARKET_BID_1                   0x21c /**< Un-rounded bid price at tier 1 */
#define CFETF_AT_MARKET_BID_2                   0x21d /**< Un-rounded bid price at tier 2 */
#define CFETF_AT_MARKET_BID_3                   0x21e /**< Un-rounded bid price at tier 3 */
#define CFETF_AT_MARKET_BID_4                   0x21f /**< Un-rounded bid price at tier 4 */
#define CFETF_AT_MARKET_BID_5                   0x220 /**< Un-rounded bid price at tier 5 */
#define CFETF_AT_MARKET_BID_6                   0x221 /**< Un-rounded bid price at tier 6 */
#define CFETF_AT_MARKET_BID_7                   0x222 /**< Un-rounded bid price at tier 7 */
#define CFETF_AT_MARKET_BID_8                   0x223 /**< Un-rounded bid price at tier 8 */
#define CFETF_AT_MARKET_BID_9                   0x224 /**< Un-rounded bid price at tier 9 */
#define CFETF_AT_MARKET_BID_10                  0x225 /**< Un-rounded bid price at tier 10 */
#define CFETF_AT_MARKET_ASK_1                   0x226 /**< Un-rounded ask price at tier 1 */
#define CFETF_AT_MARKET_ASK_2                   0x227 /**< Un-rounded ask price at tier 2 */
#define CFETF_AT_MARKET_ASK_3                   0x228 /**< Un-rounded ask price at tier 3 */
#define CFETF_AT_MARKET_ASK_4                   0x229 /**< Un-rounded ask price at tier 4 */
#define CFETF_AT_MARKET_ASK_5                   0x22a /**< Un-rounded ask price at tier 5 */
#define CFETF_AT_MARKET_ASK_6                   0x22b /**< Un-rounded ask price at tier 6 */
#define CFETF_AT_MARKET_ASK_7                   0x22c /**< Un-rounded ask price at tier 7 */
#define CFETF_AT_MARKET_ASK_8                   0x22d /**< Un-rounded ask price at tier 8 */
#define CFETF_AT_MARKET_ASK_9                   0x22e /**< Un-rounded ask price at tier 9 */
#define CFETF_AT_MARKET_ASK_10                  0x22f /**< Un-rounded ask price at tier 10 */
#define CFETF_BID_LOCK_1                        0x230 /**< Selected lock price at tier 1 */
#define CFETF_BID_LOCK_2                        0x231 /**< Selected lock price at tier 2 */
#define CFETF_BID_LOCK_3                        0x232 /**< Selected lock price at tier 3 */
#define CFETF_BID_LOCK_4                        0x233 /**< Selected lock price at tier 4 */
#define CFETF_BID_LOCK_5                        0x234 /**< Selected lock price at tier 5 */
#define CFETF_BID_LOCK_6                        0x235 /**< Selected lock price at tier 6 */
#define CFETF_BID_LOCK_7                        0x236 /**< Selected lock price at tier 7 */
#define CFETF_BID_LOCK_8                        0x237 /**< Selected lock price at tier 8 */
#define CFETF_BID_LOCK_9                        0x238 /**< Selected lock price at tier 9 */
#define CFETF_BID_LOCK_10                       0x239 /**< Selected lock price at tier 10 */
#define CFETF_ASK_LOCK_1                        0x23a /**< Selected lock price at tier 1 */
#define CFETF_ASK_LOCK_2                        0x23b /**< Selected lock price at tier 2 */
#define CFETF_ASK_LOCK_3                        0x23c /**< Selected lock price at tier 3 */
#define CFETF_ASK_LOCK_4                        0x23d /**< Selected lock price at tier 4 */
#define CFETF_ASK_LOCK_5                        0x23e /**< Selected lock price at tier 5 */
#define CFETF_ASK_LOCK_6                        0x23f /**< Selected lock price at tier 6 */
#define CFETF_ASK_LOCK_7                        0x240 /**< Selected lock price at tier 7 */
#define CFETF_ASK_LOCK_8                        0x241 /**< Selected lock price at tier 8 */
#define CFETF_ASK_LOCK_9                        0x242 /**< Selected lock price at tier 9 */
#define CFETF_ASK_LOCK_10                       0x243 /**< Selected lock price at tier 10 */
#define CFETF_OPEN_YIELD                        0x244 /**< Open yield for this session */
#define CFETF_CLOSE_YIELD                       0x245 /**< Close yield for this session */
#define CFETF_HIGH_YIELD                        0x246 /**< High yield for this session */
#define CFETF_LOW_YIELD                         0x247 /**< Low yield for this session */
#define CFETF_SESSION_BOUNDARY_NET_24H          0x248 /**< Session 24H prices */
#define CFETF_SESSION_BOUNDARY_NET_ASIA         0x249 /**< Session Asia prices */
#define CFETF_SESSION_BOUNDARY_NET_EUROPE       0x24a /**< Session Europe prices */
#define CFETF_SESSION_BOUNDARY_NET_US_CASH      0x24b /**< Session US Cash prices */
#define CFETF_SESSION_BOUNDARY_NET_US_FUTURES   0x24c /**< Session US Futures prices */
#define CFETF_NET_CHANGE_YIELD                  0x24d /**< Net Yield change */
#define CFETF_BID_CHANGE                        0x280 /**< Change on bid over closing price */
#define CFETF_ASK_CHANGE                        0x281 /**< Change on offer over closing price */
#define CFETF_ASSET_SWAP_BID_1                  0x282 /**< The bid asset swap level corresponding to the best bid */
#define CFETF_ASSET_SWAP_ASK_1                  0x283 /**< The offer asset swap level corresponding to the best offer */
#define CFETF_ASSET_SWAP_TRADE                  0x284 /**< The asset swap level corresponding to the current trade price */
#define CFETF_YIELD_PRICETYPE                   CFETF_YIELD_PRICE_TYPE /**< Synonym for CFETF_YIELD_PRICE_TYPE */
#define CFETF_YIELD_MIN_DEC_PLACES              0x285 /**< Minimum of decimal places to display if the yield price type is CFETI_PRICETYPE_DECIMAL_DVAR */
#define CFETF_YIELD_DEC_PLACES                  0x286 /**< Number of decimal places to display if the yield price type is CFETI_PRICETYPE_DECIMAL_DVAR */
#define CFETF_ALT_INST_2                        0x287 /**< Alternate instrument name - e.g. identifies underlying benchmark in a treasury swap lock instrument */
#define CFETF_TRADE_PREF2                       0x288 /**< Populated by the trading system with the bit-wise combination of supported market and order preferences2 field for the instrument */
#define CFETF_INST_SELECTION_PROPERTIES         0x28c /**< Bit-mask to identify instrument selection properties */
#define CFETF_ESPD_REPO_INST_PROPERTY           0x28f /**< Bit-mask of attributes that identifies the characteristics of each eSpeed REPO instrument */
#define CFETF_WEIGHTED_CUMULATIVE_AVERAGE       0x290 /**< Sum of ((volume of each trade) * rate) / (aggregate volume) */
#define CFETF_VWA_SESSION_BOUNDARY_24H          0x291 /**< Contains the fields describing the vol weighted avg boundary prices for the 24H session */
#define CFETF_VWA_SESSION_BOUNDARY_ASIA         0x292 /**< Contains the fields describing the vol weighted avg boundary prices for the Asia session */
#define CFETF_VWA_SESSION_BOUNDARY_EUROPE       0x293 /**< Contains the fields describing the vol weighted avg boundary prices for the Europe session */
#define CFETF_VWA_SESSION_BOUNDARY_US_CASH      0x294 /**< Contains the fields describing the vol weighted avg boundary prices for the US Cash session */
#define CFETF_VWA_SESSION_BOUNDARY_US_FUTURES   0x295 /**< Contains the fields describing the vol weighted avg boundary prices for the US Futures session */
#define CFETF_VWAY                              0x296 /**< Volume Weighted Average Yield */
#define CFETF_NET_CHANGE_VWAP                   0x297 /**< Volume Weighted Average Price Net Change */
#define CFETF_NET_CHANGE_VWAY                   0x298 /**< Volume Weighted Average Yield Net Change */
#define CFETF_DOLLAR_IRS_INST_PROPERTIES        0x299 /**< Instrument attributes specific to $IRS. This field shall be populated in all published $IRS instruments */
#define CFETF_STERLING_IRS_INST_PROPERTIES      0x29a /**< Instrument attributes specific to £IRS. This field shall be populated in all published £IRS instruments */
#define CFETF_DEAL_STRUCTURE                    0x2a6 /**< Enumerated deal structure */
#define CFETF_TRADE_TYPE                        0x2a7 /**< Enumerated trade type */
#define CFETF_LAST_ASSET_SWAP_LEVEL             0x2a8 /**< The last asset swap level */

/*
 * Last field id 
 */
#define CFETI_LAST_FIELD                        0x2b2 /**< Value of last field ID */

/*
 * Field id 0 is invalid 
 */
#define CFETF_UNDEFINED                         0x0 /**< Undefined field ID */

/*
 * Values for trade state field CFETF_STATE 
 */
#define CFETI_STATE_NO_MARKET                   0x0
#define CFETI_STATE_BID                         0x1
#define CFETI_STATE_UNCLEAR_BID                 0x2
#define CFETI_STATE_UU_SELL                     0x4
#define CFETI_STATE_SELL                        0x8
#define CFETI_STATE_OFFER                       0x10
#define CFETI_STATE_ASK                         CFETI_STATE_OFFER
#define CFETI_STATE_UNCLEAR_OFFER               0x20
#define CFETI_STATE_UNCLEAR_ASK                 CFETI_STATE_UNCLEAR_OFFER
#define CFETI_STATE_UU_BUY                      0x40
#define CFETI_STATE_BUY                         0x80
#define CFETI_STATE_CHECK_CREDIT                0x100
#define CFETI_STATE_INDICATIVE_BID              0x200 /**< Bid is indicative */
#define CFETI_STATE_INDICATIVE_OFFER            0x400 /**< Offer is indicative */
#define CFETI_STATE_TRADE_ENDING                0x800 /**< Trade is ending */
#define CFETI_STATE_NON_TRADABLE                0x4000
#define CFETI_STATE_ERROR                       0x8000

/*
 * Values for trade state field CFETF_STATE_EXT 
 */
#define CFETI_STATE_EXT_PI_BID                  0x1 /**< Price improvement is present on the bid side */
#define CFETI_STATE_EXT_PI_BUY                  0x2 /**< Price improvement is present on the buy side */
#define CFETI_STATE_EXT_PI_ASK                  0x4 /**< Price improvement is present on the offer side */
#define CFETI_STATE_EXT_PI_SELL                 0x8 /**< Price improvement is present on the sell side */
#define CFETI_STATE_EXT_WHEN_TAKE               0x80 /**< The instrument is in when take state */
#define CFETI_STATE_EXT_WHEN_HIT                0x100 /**< The instrument is in when hit state */
#define CFETI_STATE_EXT_MID_PRICE               0x10000 /**< Mid Price */
#define CFETI_STATE_EXT_REACHABLE_BUY           0x20000 /**< Reachable Buy */
#define CFETI_STATE_EXT_REACHABLE_SELL          0x40000 /**< Reachable Sell */
#define CFETI_STATE_EXT_BID_NO_MORE_SIZE        0x100000 /**< Trade through stack attempt on the bid size is not completely pre-executed */
#define CFETI_STATE_EXT_ASK_NO_MORE_SIZE        0x200000 /**< Trade through stack attempt on the ask size is not completely pre-executed */
#define CFETI_STATE_EXT_LAST_BID                0x400000 /**< Last bid */
#define CFETI_STATE_EXT_LAST_OFFER              0x800000 /**< Lastoffer */

/*
 * Price code definitions (for price codes in decoded bid/offer lists) 
 */
#define CFETI_PRICE_GOOD_TILL_CANCEL            0x1
#define CFETI_PRICE_ALL_OR_NONE                 0x2
#define CFETI_PRICE_LIMIT                       0x4
#define CFETI_PRICE_CHECK_CREDIT                0x8
#define CFETI_PRICE_OPTION_1                    0x10
#define CFETI_PRICE_OPTION_2                    0x20
#define CFETI_PRICE_OPTION_3                    0x40
#define CFETI_PRICE_OPTION_BLIND                0x40000000
#define CFETI_PRICE_OPTION_EXCLUDE              0x80000000

/*
 * Synonyms for price options (Mid Price) 
 */
#define CFETI_PRICE_MID                         CFETI_PRICE_OPTION_3 /**< This entry in the participant list is a MID_PRICE */

/*
 * Synonyms for price options (European Repos) 
 */
#define CFETI_PRICE_CLEARING_LCH                CFETI_PRICE_OPTION_1
#define CFETI_PRICE_CLEARING_RESTRICTED         CFETI_PRICE_OPTION_2
#define CFETI_PRICE_CLEARING_CLEARNET           CFETI_PRICE_OPTION_2 /**< @depricated Use CFETI_PRICE_CLEARING_RESTRICTED instead */
#define CFETI_PRICE_CLEARING_INTERBANK          CFETI_PRICE_OPTION_3

/*
 * Synonyms for price options (BGC T-Swap and T-Basis) 
 */
#define CFETI_PRICE_OPTION_RESTRICTED           CFETI_PRICE_OPTION_1

/*
 * Synonyms for price options (Exchanges) 
 */
#define CFETI_PRICE_EXCHANGE_DERIVED            CFETI_PRICE_OPTION_1
#define CFETI_PRICE_DERIVED                     CFETI_PRICE_EXCHANGE_DERIVED /**< deprecated */

/*
 * Synonyms for price options (IRS). LCH and INTERBANK options are retained
 * from European REPOs. OPTION_2 is redefined as DERIVED_SPREAD 
 */
#define CFETI_PRICE_DERIVED_SPREAD              CFETI_PRICE_OPTION_2

/*
 * Synonyms for price options (eSpeed FX) 
 */
#define CFETI_PRICE_OPTION_MME                  CFETI_PRICE_OPTION_1 /**< This entry in the participant list is a non-dominant market-maker price */

/*
 * Price type definitions (for field CFETF_PRICETYPE) 
 */
#define CFETI_PRICETYPE_32ND                    0x1
#define CFETI_PRICETYPE_DECIMAL                 0x2 /**< NNN.NNNN */
#define CFETI_PRICETYPE_DECIMAL_D0              0x3 /**< Integer prices */
#define CFETI_PRICETYPE_DECIMAL_D1              0x4 /**< NNN.N */
#define CFETI_PRICETYPE_DECIMAL_D2              0x5 /**< NNN.NN */
#define CFETI_PRICETYPE_DECIMAL_D2_PLUS         0x6 /**< NNN.NN[+] (opt + = 0.005) */
#define CFETI_PRICETYPE_DECIMAL_D3              0x7 /**< NNN.NNN */
#define CFETI_PRICETYPE_DECIMAL_D4              CFETI_PRICETYPE_DECIMAL /**< NNN.NNNN */
#define CFETI_PRICETYPE_DECIMAL_F4              0x8 /**< NNN.[246] (1/8ths) */
#define CFETI_PRICETYPE_DECIMAL_F8              0x9 /**< NNN.[1-7] (1/8ths) */
#define CFETI_PRICETYPE_DECIMAL_F16             0xa /**< NNN.NN (00-16) */
#define CFETI_PRICETYPE_DECIMAL_F32             0xb /**< NNN.NN (00-32) */
#define CFETI_PRICETYPE_DECIMAL_F64             0xc /**< NNN.NN[+] (00-32) */
#define CFETI_PRICETYPE_DECIMAL_F128            0xd /**< NNN.NN[2+6] */
#define CFETI_PRICETYPE_MMTS_F256               0xe /**< NNN.NN[123+567] where decimal places denote NN whole ticks and [123+567] partial ticks */
#define CFETI_PRICETYPE_DECIMAL_F256            CFETI_PRICETYPE_MMTS_F256 /**< @deprecated Use CFETI_PRICETYPE_MMTS_F256 instead */
#define CFETI_PRICETYPE_DECIMAL_D4_PLUS         0xf /**< NNN.NNNN[+] (opt + = 0.00005) */
#define CFETI_PRICETYPE_DECIMAL_DVAR            0x10 /**< Variable - see documentation */
#define CFETI_PRICETYPE_DECIMAL_D2_BILL         0x11 /**< NNN.NN[1,2(+),3] */
#define CFETI_PRICETYPE_DECIMAL_D2_WI           0x12 /**< NNN.NN[2,4(+),6] */
#define CFETI_PRICETYPE_CX_SPREAD               0x13 /**< NNN.N up to 3 sf and 1 dp */
#define CFETI_PRICETYPE_CBOT_32ND               0x14 /**< NN-nn e.g 80-16 is 80 16/32 (Price tick 1/32) */
#define CFETI_PRICETYPE_CBOT_HALF_32ND          0x15 /**< NN-nn e.g 80-16 is 80 16/32, 80-165 is 80 16.5/32 (Price tick 1/64) */
#define CFETI_PRICETYPE_CBOT_QTR_32ND           0x16 /**< NN-nn e.g 80-16 is 80 16/32, 80-162 is 80 16.25/32, 80-165 is 80 16.5/32, 80-167 is 80 16.75/32 (Price tick 1/128) */
#define CFETI_PRICETYPE_CBOT_64TH               0x17 /**< NN-nn e.g 80-32 is 80 32/64 (Price tick 1/64) */
#define CFETI_PRICETYPE_CBOT_HALF_64TH          0x18 /**< NN-nn e.g 80-32 is 80 32/64, 80-325 is 80 32.5/64 (Price tick 1/128) */
#define CFETI_PRICETYPE_CBOT_3D_HALVES          0x19 /**< NN.NNN    3rd dec place is either 5 or not present */
#define CFETI_PRICETYPE_CBOT_HALVES             0x1a /**< NNN.NNN (if price between -1 and 1 then leading zero not displayed) */
#define CFETI_PRICETYPE_CBOT_FOURTHS            0x1b
#define CFETI_PRICETYPE_CBOT_MOD_FOURTHS        0x1c
#define CFETI_PRICETYPE_CBOT_EIGHTHS            0x1d
#define CFETI_PRICETYPE_CBOT_TENTHS             0x1e
#define CFETI_PRICETYPE_CBOT_TWENTIETHS         0x1f
#define CFETI_PRICETYPE_D3_SHORT                0x20 /**< NNN.NNN (if price between -1 and 1 then leading zero not displayed) */
#define CFETI_PRICETYPE_CBOT_MOD_FORTIETHS      0x21
#define CFETI_PRICETYPE_CBOT_HUNDREDTHS         0x22
#define CFETI_PRICETYPE_UST_BASIS               0x23
#define CFETI_PRICETYPE_DECIMAL_F256_8TH_32ND   0x24 /**< NNN.NN[123+567] */
#define CFETI_PRICETYPE_DECIMAL_D2_WI_256       0x25
#define CFETI_PRICETYPE_DECIMAL_D2_BILL_256     0x26
#define CFETI_PRICETYPE_CME                     0x27
#define CFETI_PRICETYPE_CBOT_HALF_HUNDREDTHS    0x28

#endif /* _CFETI_FIELDS_ */
