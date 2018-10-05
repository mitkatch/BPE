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


#include <espd/espd.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

#if !defined(_WIN32)

#include <signal.h>
#include <unistd.h>
#include <pthread.h>

#define STRCASECMP strcasecmp

#else

#include <windows.h>

#define STRCASECMP stricmp

#endif

#include "io.h"

static char *primary = 0;
static char *secondary = 0;
static char *username = 0;
static char *password = 0;
static char *logfile = 0;
static char *connectionMode = 0;
static char *currentSession = 0;
static CFETI_TRADE_SESS_ID currentTradingSession = 0;

static const int spd = 60 * 60 * 24;

static const char *DELIMITERS = " \t";

static char *cmdBuf = 0;
static char *emptyString = (char*)"";

unsigned int GetNextRequestId()
{
	static unsigned int NextRequestId = 100;
	return ++NextRequestId;
}

unsigned int GetNextQuoteId()
{
	static unsigned int NextQuoteId = 2000;
	return ++NextQuoteId;
}

unsigned int GetNextQuoteResponseId()
{
	static unsigned int NextQuoteResponseId = 3000;
	return ++NextQuoteResponseId;
}

char *tokenizer(char *cmd)
{
	char *result;

	if (cmdBuf)
		free(cmdBuf);
	cmdBuf = strdup(cmd);

	result = strtok(cmd, DELIMITERS);
	if (!result)
		result = emptyString;

	return result;
}

char *next()
{
	char *result = strtok(0, DELIMITERS);
	if (!result)
		result = emptyString;
	return result;
}

void Shutdown()
{
	CloseScreen();
	CFETICloseSession();
}

#if !defined(_WIN32)
#ifdef __cplusplus
extern "C" {
#endif
void SigHandler(int sig)
{
	Shutdown();
	exit(0);
}
#ifdef __cplusplus
}
#endif
#endif


const char* AvailabilityStateToString(unsigned int availabilityState)
{
	switch(availabilityState)
	{
	case CFETI_TRADER_AVAILABILITY_NONE:
		return "None";
	case CFETI_TRADER_AVAILABILITY_ALL:
		return "All";
	case CFETI_TRADER_AVAILABILITY_ANOTHER:
		return "Another";
	case CFETI_TRADER_AVAILABILITY_ANOTHER_PARTIAL:
		return "Another Partial";
	case CFETI_TRADER_AVAILABILITY_YOURS:
		return "Yours";
	case CFETI_TRADER_AVAILABILITY_YOURS_PARTIAL:
		return "Yours Partial";
	case CFETI_TRADER_AVAILABILITY_JOINT:
		return "Joint";
	case CFETI_TRADER_AVAILABILITY_JOINT_PARTIAL:
		return "Joint Partial";
	default:
		break;
	}
	return "Unknown";
}


void OutputTierAvailability(const CFETI_TIER_AVAILABILITY_DESC& d)
{
	if (d.availability != CFETI_TRADER_AVAILABILITY_NONE)
	{
		if (d.size > 0)
			Output("\t[%s - %f]", AvailabilityStateToString(d.availability), d.size);
		else
			Output("\t[%s]", AvailabilityStateToString(d.availability));
	}
}


void OutputAvailabilityInfo(CFETI_SESSION_ID sessId, CFETI_TRADE_SESS_ID tsSession, CFETI_FIELD_DESC& field, const char* szInstName)
{
	CFETI_MARKET_AVAILABILITY_DESC* pAvailability = 0;

	CFETI_DECODE_DATA_DESC decodeData;
	::memset(&decodeData, 0, sizeof(decodeData));
	decodeData.field = &field;
	decodeData.instClass = 0;
	decodeData.instrumentName = szInstName;

	CFETI_RC rc = CFETIDecodeDataField(currentSession, 
									  currentTradingSession,
									  &decodeData,
									  (void **)&pAvailability);

	if (rc == CFETI_SUCCESS)
	{
		Output("Availability for instrument \"%s\"", 
				(pAvailability->instrumentName? pAvailability->instrumentName: "missing instrument name"));
		unsigned int tierIdx = 0;

		Output("Market Bid:");
		for (tierIdx = 0; tierIdx < 5; ++tierIdx)
			OutputTierAvailability(pAvailability->tier[CFETI_INDEX_MARKET_BID][tierIdx]);

		Output("Market Ask:");
		for (tierIdx = 0; tierIdx < 5; ++tierIdx)
			OutputTierAvailability(pAvailability->tier[CFETI_INDEX_MARKET_ASK][tierIdx]);

		Output("Order Buy:");
		for (tierIdx = 0; tierIdx < 5; ++tierIdx)
			OutputTierAvailability(pAvailability->tier[CFETI_INDEX_ORDER_BUY][tierIdx]);

		Output("Order Sell:");
		for (tierIdx = 0; tierIdx < 5; ++tierIdx)
			OutputTierAvailability(pAvailability->tier[CFETI_INDEX_ORDER_SELL][tierIdx]);
	}
	else
	{
		Output("Failed to calculate availability: \"%s\" 0x%x",
				(field.fieldValue.string? field.fieldValue.string: ""),
				rc);
	}
}



void OutputInstrumentData(const CFETI_INSTRUMENT_DATA_DESC* instData)
{
	Output("Market Data Update : %s", 
		   instData->instName? instData->instName: "Instrument name not specified");
	for (int idx = 0; idx < instData->numFields; idx++)
	{
		switch(instData->fieldTable[idx].fieldType)
		{
		case CFETI_FIELDTYPE_INT8:
			Output("Field 0x%4.4x : %d", instData->fieldTable[idx].fieldId, instData->fieldTable[idx].fieldValue.int8);
			break;
		case CFETI_FIELDTYPE_BYTE:
			Output("Field 0x%4.4x : %u", instData->fieldTable[idx].fieldId, instData->fieldTable[idx].fieldValue.byte);
			break;
		case CFETI_FIELDTYPE_INT16:
			Output("Field 0x%4.4x : %d", instData->fieldTable[idx].fieldId, instData->fieldTable[idx].fieldValue.int16);
			break;
		case CFETI_FIELDTYPE_UINT16:
			Output("Field 0x%4.4x : %u", instData->fieldTable[idx].fieldId, instData->fieldTable[idx].fieldValue.uint16);
			break;
		case CFETI_FIELDTYPE_INT32:
			Output("Field 0x%4.4x : %d", instData->fieldTable[idx].fieldId, instData->fieldTable[idx].fieldValue.int32);
			break;
		case CFETI_FIELDTYPE_UINT32:
			Output("Field 0x%4.4x : %u", instData->fieldTable[idx].fieldId, instData->fieldTable[idx].fieldValue.uint32);
			break;
		case CFETI_FIELDTYPE_DATETIME:
			Output("Field 0x%4.4x : %24s", instData->fieldTable[idx].fieldId, ctime(&instData->fieldTable[idx].fieldValue.dateTime));
			break;
		case CFETI_FIELDTYPE_STRING:
		{
			switch (instData->fieldTable[idx].fieldId)
			{
			case CFETF_MARKET_AVAILABILITY_NOTIFICATION:
				OutputAvailabilityInfo(currentSession, currentTradingSession, instData->fieldTable[idx], instData->instName);
				break;
			default:
				Output("Field 0x%4.4x : %s", instData->fieldTable[idx].fieldId, instData->fieldTable[idx].fieldValue.string);
			}
		}
		break;
		case CFETI_FIELDTYPE_BUFFER:
		{
			switch (instData->fieldTable[idx].fieldId)
			{
			case CFETF_BID_LIST_1:
			case CFETF_BID_LIST_2:
			case CFETF_BID_LIST_3:
			case CFETF_BID_LIST_4:
			case CFETF_BID_LIST_5:
			case CFETF_BID_LIST_6:
			case CFETF_BID_LIST_7:
			case CFETF_BID_LIST_8:
			case CFETF_BID_LIST_9:
			case CFETF_BID_LIST_10:
			case CFETF_ASK_LIST_1:
			case CFETF_ASK_LIST_2:
			case CFETF_ASK_LIST_3:
			case CFETF_ASK_LIST_4:
			case CFETF_ASK_LIST_5:
			case CFETF_ASK_LIST_6:
			case CFETF_ASK_LIST_7:
			case CFETF_ASK_LIST_8:
			case CFETF_ASK_LIST_9:
			case CFETF_ASK_LIST_10:
			case CFETF_BUY_LIST_1:
			case CFETF_BUY_LIST_2:
			case CFETF_BUY_LIST_3:
			case CFETF_BUY_LIST_4:
			case CFETF_BUY_LIST_5:
			case CFETF_BUY_LIST_6:
			case CFETF_BUY_LIST_7:
			case CFETF_BUY_LIST_8:
			case CFETF_BUY_LIST_9:
			case CFETF_BUY_LIST_10:
			case CFETF_SELL_LIST_1:
			case CFETF_SELL_LIST_2:
			case CFETF_SELL_LIST_3:
			case CFETF_SELL_LIST_4:
			case CFETF_SELL_LIST_5:
			case CFETF_SELL_LIST_6:
			case CFETF_SELL_LIST_7:
			case CFETF_SELL_LIST_8:
			case CFETF_SELL_LIST_9:
			case CFETF_SELL_LIST_10:
			{
				Output("Field 0x%4.4x : (Buffer, length %d)", instData->fieldTable[idx].fieldId, instData->fieldTable[idx].fieldValue.buffer.len);
				CFETI_PARTICIPANT_LIST pList;

				CFETI_DECODE_DATA_DESC decodeData;
				::memset(&decodeData, 0, sizeof(decodeData));
				decodeData.field = &instData->fieldTable[idx];
				decodeData.instClass = 0;
				decodeData.instrumentName = instData->instName;

				CFETI_RC rc = CFETIDecodeDataField(currentSession, 
												currentTradingSession,
												&decodeData,
												(void **)&pList);

				if (rc == CFETI_SUCCESS)
				{
					for (unsigned int jdx = 0; jdx < pList->numParticipants; jdx++)
						Output("\t%d: %u %f %u", jdx, pList->participant[jdx].tsId, pList->participant[jdx].size, pList->participant[jdx].code);
				}
				else
				{
					Output("\tError %d", rc);
				}
			}
			break;
			default:
				/* Buffer field type not handled */
				Output("Field 0x%4.4x : (unknown buffer type)", instData->fieldTable[idx].fieldId);
				break;
			}
		}
		break;
		case CFETI_FIELDTYPE_DECIMAL:
			Output("Field 0x%4.4x : %f", instData->fieldTable[idx].fieldId, instData->fieldTable[idx].fieldValue.decimal);
			break;
		case CFETI_FIELDTYPE_BYTESTREAM:
			{
				switch (instData->fieldTable[idx].fieldId)
				{
				case CFETF_SESSION_BOUNDARY_24H:
				case CFETF_SESSION_BOUNDARY_ASIA:
				case CFETF_SESSION_BOUNDARY_EUROPE:
				case CFETF_SESSION_BOUNDARY_US_CASH:
				case CFETF_SESSION_BOUNDARY_US_FUTURES:
				{
					CFETI_PRICE_BOUNDARY_DESC* sessionInfo = 0;

					CFETI_DECODE_DATA_DESC decodeData;
					::memset(&decodeData, 0, sizeof(decodeData));
					decodeData.field = &instData->fieldTable[idx];
					decodeData.instClass = 0;
					decodeData.instrumentName = instData->instName;

					CFETI_RC rc = CFETIDecodeDataField(currentSession, 
													currentTradingSession,
													&decodeData,
													(void **)&sessionInfo);

					if (rc == CFETI_SUCCESS)
					{
						const char* sessionState = "unknown";
						switch(sessionInfo->uiSessionState)
						{
						case CFETI_TRADING_SESSION_STATE_OPEN:
							sessionState = "Open";
							break;
						case CFETI_TRADING_SESSION_STATE_CLOSED:
							sessionState = "Closed";
							break;
						case CFETI_TRADING_SESSION_STATE_NONE:
						default:
							sessionState = "None";
							break;
						}
					    Output("Session Info : High[%f][%u] yield[%f] | Low[%f][%u] yield[%f] | Open[%f] yield[%f] | Close[%f] yield[%f] | State[%s]", 
						    sessionInfo->dHigh,
						    (unsigned int)sessionInfo->highIndicator,
                            sessionInfo->dHighYield,
						    sessionInfo->dLow,
						    (unsigned int)sessionInfo->lowIndicator,
                            sessionInfo->dLowYield,
						    sessionInfo->dOpen,
                            sessionInfo->dOpenYield,
						    sessionInfo->dClose,
                            sessionInfo->dCloseYield,
							sessionState);
					}
					else
					{
						Output("\tError %d", rc);
					}
				}
                break;
		        case CFETF_SESSION_BOUNDARY_NET_24H:
		        case CFETF_SESSION_BOUNDARY_NET_ASIA:
		        case CFETF_SESSION_BOUNDARY_NET_EUROPE:
		        case CFETF_SESSION_BOUNDARY_NET_US_CASH:
		        case CFETF_SESSION_BOUNDARY_NET_US_FUTURES:
				{
					CFETI_PRICE_NET_CHANGE_DESC* sessionInfo = 0;
                    CFETI_DECODE_DATA_DESC decodeData;
                    memset(&decodeData, 0, sizeof(decodeData));
                    decodeData.field = &instData->fieldTable[idx];

					CFETI_RC rc = CFETIDecodeDataField(currentSession, currentTradingSession, &decodeData, (void **)&sessionInfo);
					if (rc == CFETI_SUCCESS)
					{
                        Output("Session Info : Net change: [%f] yield[%f]", 
                            sessionInfo->dNetChange,
                            sessionInfo->dNetChangeYield);
					}
					else
					{
						Output("\tError %d : %s", rc, CFETIGetLastError());
					}
				}

				break;
				default:
					Output("Field 0x%4.4x : Fields %d Length %d", instData->fieldTable[idx].fieldId, instData->fieldTable[idx].fieldValue.bytestream.fields, instData->fieldTable[idx].fieldValue.bytestream.fields);
					break;
				}
			}
			break;
		default:
			Output("Field 0x%4.4x : (unknown data type)", instData->fieldTable[idx].fieldId);
			break;
		}
	}
	return;
}

#if defined(CFETI_PRIORITY_NOTIFICATION)
void OutputTraderPriority(CFETI_TRADER_PRIORITY_DESC* n)
{
	Output("Trader Priority Notification");
	if (!n)
	{
		Output("\nMissing command data");
		return;
	}

	const char* szPriorityState = "<unspecified>";
	const char* szTradeSide = "<unspecified>";
	const char* szInstrumentName = n->instrumentName? n->instrumentName: "<unspecified>";

	switch(n->priorityState)
	{
	case CFETI_TRADER_PRIORITY_CLEAR:
		szPriorityState = "clear";
		break;
	case CFETI_TRADER_PRIORITY_UNCLEAR:
		szPriorityState = "another";
		break;
	case CFETI_TRADER_PRIORITY_FIRST:
		szPriorityState = "first";
		break;
	case CFETI_TRADER_PRIORITY_JOINT_FIRST:
		szPriorityState = "joint first";
		break;
	default:
		break;
	}
  
//	switch(n->tradeSide)
//	{
//	case CFETI_ORDER_BUY:
//		szTradeSide = "buy";
//		break;
//	case CFETI_ORDER_SELL:
//		szTradeSide = "sell";
//		break;
//	}

	Output("\tInstrument\t%s", szInstrumentName);
	Output("\tPriority State\t%s", szPriorityState);
	Output("\tTrade side:\t%s", szTradeSide);
	Output("");
}
#endif

void OutputData(const char* title, unsigned char* dp, int dlen)
{
	char buf[(CFETI_MAX_USER_DATA_SIZE * 2) + 1];

	memset(buf, 0, sizeof(buf));
	for (int i = 0; i < dlen; i++)
		sprintf(&buf[2 * i], "%02x", dp[i]);

	Output("%s: (%d bytes) 0x%s", title, dlen, buf);
}

void OutputTSwapDetails(CFETI_TSWAP pTSwap)
{
	Output("Selected TSWAP Ratio:        [%f]", pTSwap->dTSwapRatio);
	Output("Selected TSWAP Lock Price:   [%f]", pTSwap->dLockPrice);
	Output("Prevailing TSWAP Ratio:      [%f]", pTSwap->dCurrentTSwapRatio);
	Output("Prevailing TSWAP Lock Price: [%f]", pTSwap->dCurrentLockPrice);
}

void OutputCantorRepoDetails(CFETI_CANTOR_REPO_DESC* pRepo)
{
	Output("Start date:                  [%u]", pRepo->startDate);
	Output("End date:                    [%u]", pRepo->endDate);
	Output("Rights of Substitution:      [%u]", pRepo->rightsOfSubstitution);
	Output("Annotation                   [%s]", pRepo->annotation ? pRepo->annotation : "<not specified>");
	Output("Attributes                   [%u]", pRepo->attributes);
}

void OutputMMTSDetails(CFETI_MMTS_ORDER pMMTS)
{
	Output("All-in-price\t\t\t[%f]", pMMTS->allInPrice);
	Output("Accrued Interest\t\t[%f]", pMMTS->accruedInterest);
	Output("Repo Price Adjustment\t[%f]", pMMTS->repoPriceAdjustment);
}

void OutputFXTradeDetails(CFETI_FX_TRADE_DESC* pFX)
{
	Output("Product Type\t[%d]", pFX->productType);
	Output("Deal Type\t[%d]", pFX->dealType);
	Output("Contract Date\t[%d]", pFX->contractDate);
	Output("Delivery Date\t[%d]", pFX->deliveryDate);
	Output("Leg Type\t[%d]", pFX->legType);
	Output("Cutoff Time\t[%d]", pFX->cutoffTime);
	Output("Cutoff Time Region\t[%s]", (pFX->cutoffTimeRegion? pFX->cutoffTimeRegion: "<not specified>"));
	Output("Expiry Date\t[%d]", pFX->expiryDate);
	Output("Reference Price\t[%f]", pFX->referencePrice);
	Output("Receiving Agent\t[%s]", (pFX->receivingAgent? pFX->receivingAgent: "<not specified>"));
	Output("Beneficiary\t[%s]", (pFX->beneficiary? pFX->beneficiary: "<not specified>"));
	Output("Beneficiary Account\t[%s]", (pFX->beneficiaryAccount? pFX->beneficiaryAccount: "<not specified>"));
	Output("Buy Currency\t[%s]", (pFX->buyCurrency? pFX->buyCurrency: "<not specified>"));
	Output("Buy Amount\t[%f]", pFX->buyAmount);
	Output("Sell Currency\t[%s]", (pFX->sellCurrency? pFX->sellCurrency: "<not specified>"));
	Output("Sell Amount\t[%f]",  pFX->sellAmount);
	Output("Currency 1 Buyer\t[%s]", (pFX->currency1Buyer? pFX->currency1Buyer: "<not specified>"));
	Output("Currency 1 Seller\t[%s]", (pFX->currency1Seller? pFX->currency1Seller: "<not specified>"));
	Output("Forward Rate\t[%f]", pFX->forwardRate);
	Output("Currency Settlement\t[%s]", (pFX->currencySettlement? pFX->currencySettlement: "<not specified>"));
	Output("Fixing Date\t[%d]", pFX->fixingDate);
	Output("Fixing Source\t[%s]", (pFX->fixingSource? pFX->fixingSource: "<not specified>"));
	Output("Hedge Type\t[%d]",  pFX->hedgeType);
}

void OutputFXOptionTradeDetails(CFETI_FX_OPTION_TRADE_DESC* pFXOption)
{
	OutputFXTradeDetails(&pFXOption->fxTrade);
	Output("Exercise Style\t[%d]", pFXOption->exerciseStyle);
	Output("Option Class\t[%d]", pFXOption->optionClass);
	Output("Option Style\t[%d]", pFXOption->optionStyle);
	Output("Option Strategy\t[%d]", pFXOption->optionStrategy);
	Output("Delivery Terms\t[%d]",  pFXOption->deliveryTerms);
	Output("Low Trigger\t[%f]", pFXOption->lowTrigger);
	Output("High Trigger\t[%f]", pFXOption->highTrigger);
	Output("Low Trigger Basis\t[%u]", pFXOption->lowTriggerBasis);
	Output("High Trigger Basis\t[%u]", pFXOption->highTriggerBasis);
	Output("Trigger Agent\t[%s]", (pFXOption->triggerAgent? pFXOption->triggerAgent: "<not specified>"));
	Output("Put Call Indicator\t[%d]", pFXOption->putCallIndicator);
	Output("Strike Prices\t[%f]", pFXOption->strikePrice);
	Output("Strike Basis\t[%d]", pFXOption->strikeBasis);
	Output("Premium Currency\t[%d]", (pFXOption->premiumCurrency? pFXOption->premiumCurrency: "<not specified>"));
	Output("Premium Quote\t[%f]", pFXOption->premiumQuote);
	Output("Premium Quote Basis\t[%d]", pFXOption->premiumQuoteBasis);
	Output("Premium Amount\t[%f]", pFXOption->premiumAmount);
	Output("Volatility\t[%f]", pFXOption->volatility);
	Output("Spot Rate\t[%f]", pFXOption->spotRate);
	Output("Swap Points\t[%f]", pFXOption->swapPoints);
	Output("Deposit Rate Currency 1\t[%f]", pFXOption->depositRateCurrency1);
	Output("Deposit Rate Currency 2\t[%f]", pFXOption->depositRateCurrency2);
	Output("Delta\t[%f]", pFXOption->delta);
}

void OutputMarket(CFETI_MARKET market)
{
	Output("Instrument\t[%s]\n Trading system\t[%d]\n Price\t\t[%f]\n Size\t\t[%f]\n Pref\t\t[%d:%d]\n Side\t\t[%s]\n Id, Req Id\t[%u, %u]", 
		   (market->tradeInstrument ? market->tradeInstrument : "not specified"), 
		   market->tsId, 
		   market->price, 
		   market->size, 
		   market->preferences, 
		   market->preferences2, 
		   market->side == CFETI_MARKET_BID ? "Bid" : (market->side == CFETI_MARKET_ASK ? "Ask" : "???"), 
		   market->id, market->requestId);

	if (market->preferences2 & CFETI_USE_RESERVE_SIZE)
		Output("Reserve Size Min [%f] Max [%f] Initial [%f]", market->reserveMinSize, market->reserveMaxSize, market->reserveInitialSize);
	if (market->tradeId)
		Output("Trade Id\t[%s]\n Size\t\t[%f]\n Time\t\t[%.24s]", market->tradeId, market->tradeSize, ctime(&market->tradeTime));
	if (market->userDataSize > 0)
		Output("User Data\t[%.*s]", market->userDataSize, (unsigned char*)market->userData);
	if (market->appUserDataSize > 0)
		Output("App User Data\t[%.*s]", market->appUserDataSize, (unsigned char*)market->appUserData);
	if (market->shortCode)
		Output("Short code\t[%s]", market->shortCode);
	if (market->userName)
		Output("User name\t[%s]", market->userName);

	switch (market->orderInfoType)
	{
	case CFETI_ORDERINFO_NOT_SPECIFIED:
		break;
	case CFETI_ORDERINFO_TSWAP:
		Output("\t" "OrderInfoType\t[%d]", market->orderInfoType);		
		OutputTSwapDetails((CFETI_TSWAP)market->orderInfo);
		break;
	case CFETI_ORDERINFO_MMTS:
		Output("\t" "OrderInfoType\t[%d]", market->orderInfoType);		
		OutputMMTSDetails((CFETI_MMTS_ORDER)market->orderInfo);
		break;
	default:
		break;
	}
}

void OutputOrder(CFETI_ORDER order)
{
	char buf1[32], buf2[32];
	Output("Instrument\t[%s]\n Trading system\t[%d]\n Price\t\t[%f]\n Size\t\t[%f]\n Pref\t\t[%d:%d]\n Side\t\t[%s]\n Id, Req Id\t[%u %u]",
		   (order->tradeInstrument ? order->tradeInstrument : "not specified"),
		   order->tsId,
		   order->price,
		   order->size,
		   order->preferences,
		   order->preferences2,
		   (order->indicator == CFETI_ORDER_HIT || order->indicator == CFETI_ORDER_SELL)  ? "Sell" : (order->indicator == CFETI_ORDER_BUY ? "Buy" : "???"),
		   order->id,
		   order->requestId);
	if (order->yield != 0.0)
		Output(" Yield\t\t%f", order->yield);
	if (order->preferences2 & CFETI_USE_RESERVE_SIZE)
		Output("Reserve Size Min [%f] Max [%f] Initial [%f]", order->reserveMinSize, order->reserveMaxSize, order->reserveInitialSize);
	if (order->tradeComments)
		Output("Trade comments\t[%s]", order->tradeComments);
	if (order->toPrice)
		Output("To Price\t\t[%f]", order->toPrice);
	if (order->tradeId)
		Output("Trade Id\t[%s]\n Size\t\t[%f]\n Time\t\t[%.24s]", order->tradeId, order->tradeSize, ctime(&order->tradeTime));
	if (order->userName)
		Output("Trader\t\t[%s]", order->userName);
	if (order->userDataSize > 0)
	{
		Output("User Data\t[%.*s]", order->userDataSize, (unsigned char*)order->userData);
	}
	if (order->appUserDataSize > 0)
		Output("App User Data\t[%.*s]", order->appUserDataSize, (unsigned char*)order->appUserData);
	if (order->shortCode)
		Output("Short code\t\t[%s]", order->shortCode);
	if (order->counterpartyName)
		Output("Counterparty name\t[%s]", order->counterpartyName);
	if (order->settlementMethod)
		Output("Settlement Method\t[%s]", order->settlementMethod);
	if (order->settlementMethod)
		Output("Brokerage\t\t[%f]", order->brokerage);
	if (order->instProperties)
		Output("Properties\t\t[0x%x]", order->instProperties);

	sprintf(buf1, "%f", order->tradePrice);
	if (order->tradeSettlement != 0)
		sprintf(buf2, "%s", ctime(&order->tradeSettlement));
	else
		buf2[0] = '\0';
	switch(order->tradeSide)
	{
	case CFETI_TRADE_PASSIVE:
		Output("Trade\t\t[PASSIVE, %s %.24s]", buf1, buf2);
		break;
	case CFETI_TRADE_ACTIVE:
		Output("Trade\t\t[ACTIVE, %s %.24s]", buf1, buf2);
		break;
	default:
		break;
	}

	if (order->pPIBenefit)
	{
		Output("PI Benefit Straight\t\t[%f]", order->pPIBenefit->straight);
		Output("PI Benefit Held\t\t[%f]", order->pPIBenefit->held);
		Output("PI Benefit Better Fill\t\t[%f]", order->pPIBenefit->betterFilled);
	}

	switch (order->orderInfoType)
	{
	case CFETI_ORDERINFO_NOT_SPECIFIED:
		break;
	case CFETI_ORDERINFO_TSWAP:
		Output("\t" "OrderInfoType\t[%d]", order->orderInfoType);		
		OutputTSwapDetails((CFETI_TSWAP)order->orderInfo);
		break;
	case CFETI_ORDERINFO_MMTS:
		Output("\t" "OrderInfoType\t[%d]", order->orderInfoType);		
		OutputMMTSDetails((CFETI_MMTS_ORDER)order->orderInfo);
		break;
	case CFETI_ORDERINFO_FX_TRADE:
		Output("\t" "OrderInfoType\t[%d]", order->orderInfoType);		
		OutputFXTradeDetails((CFETI_FX_TRADE_DESC*)order->orderInfo);
		break;
	case CFETI_ORDERINFO_FX_OPTION_TRADE:
		Output("\t" "OrderInfoType\t[%d]", order->orderInfoType);		
		OutputFXOptionTradeDetails((CFETI_FX_OPTION_TRADE_DESC*)order->orderInfo);
		break;
	default:
		break;
	}

	if (order->instrumentData)
		OutputInstrumentData(order->instrumentData);
}

const char* OutputString(const char* str)
{
	return str ? str : "unknown";
}

const char* DDCommandToString(unsigned int uiCmd)
{
	switch(uiCmd)
	{
	case CFETI_DD_REQUEST_SUBMIT_ACCEPTED:
		return "Direct Dealing Submit Request Accepted";
	case CFETI_DD_REQUEST_SUBMIT_REJECTED:
		return "Direct Dealing Submit Request Rejected";
	case CFETI_DD_REQUEST_CANCEL_ACCEPTED:
		return "Direct Dealing Cancel Request Accepted";
	case CFETI_DD_REQUEST_CANCEL_REJECTED:
		return "Direct Dealing Cancel Request Rejected";
	case CFETI_DD_REQUEST_MODIFY_ACCEPTED:
		return "Direct Dealing Modify Request Accepted";
	case CFETI_DD_REQUEST_MODIFY_REJECTED:
		return "Direct Dealing Modify Request Rejected";
	case CFETI_DD_REQUEST_MODIFIED:
		return "Direct Dealing Request Modified";
	case CFETI_DD_REQUEST_CANCELLED:
		return "Direct Dealing Request Cancelled";
	case CFETI_DD_REQUEST_EXECUTED:
		return "Direct Dealing Request Executed";
	case CFETI_DD_REQUEST_CREATED:
		return "Direct Dealing Request Created";
	case CFETI_DD_REQUEST_TRADED:
		return "Direct Dealing Request Traded";
	case CFETI_DD_QUOTE_SUBMIT_ACCEPTED:
		return "Quote Submit Accepted";
	case CFETI_DD_QUOTE_SUBMIT_REJECTED:
		return "Quote Submit Rejected";
	case CFETI_DD_QUOTE_CANCEL_ACCEPTED:
		return "Quote Cancel Accepted";
	case CFETI_DD_QUOTE_CANCEL_REJECTED:
		return "Quote Cancel Rejected";
	case CFETI_DD_QUOTE_MODIFY_ACCEPTED:
		return "Quote Modify Accepted";
	case CFETI_DD_QUOTE_MODIFY_REJECTED:
		return "Quote Modify Rejected";
	case CFETI_DD_QUOTE_CANCELLED:
		return "Quote Cancelled";
	case CFETI_DD_QUOTE_EXECUTED:
		return "Quote Executed";
	case CFETI_DD_QUOTE_ACCEPTED:
		return "Quote Accepted";
	case CFETI_DD_QUOTE_REJECTED:
		return "Quote Rejected";
	case CFETI_DD_ACCEPT_QUOTE_ACCEPTED:
		return "Accept Quote Accepted";
	case CFETI_DD_ACCEPT_QUOTE_REJECTED:
		return "Accept Quote Rejected";
	case CFETI_DD_REJECT_QUOTE_ACCEPTED:
		return "Reject Quote Accepted";
	case CFETI_DD_REJECT_QUOTE_REJECTED:
		return "Reject Quote Rejected";
	case CFETI_DD_SUBMIT_TO_MARKET_ACCEPTED:
		return "Submit to Market Accepted";
	case CFETI_DD_SUBMIT_TO_MARKET_REJECTED:
		return "Submit to Market Rejected";
	default:
		break;
	}

	return "unknown";
}

void OutputDDQuoteRankInfo(unsigned int cmd, void* cmdData)
{
	if (cmdData)
	{
		CFETI_DD_RANK_DESC& ranks = *((CFETI_DD_RANK_DESC*)cmdData);
		Output("%s DD name [%s]\n", DDCommandToString(cmd), OutputString(ranks.szInstName));
		Output("\tid\t[%u]\n", (unsigned long)ranks.uiRFQId);
		Output("\tmatched ask\t[%u]\n", (unsigned long)ranks.uiBOLSMatchedAsk);
		Output("\tmatched bid\t[%u]\n", (unsigned long)ranks.uiBOLSMatchedBid);
		Output("\tcountdown\t[%u]\n", (unsigned long)ranks.uiCountdownTime); 
		Output("\tquotes [%u]\n", (unsigned long)ranks.uiNumQuoteRanks);
		
		for (unsigned int index=0; index < ranks.uiNumQuoteRanks; ++index)
		{
			CFETI_DD_QUOTE_RANK_DESC& qr = ranks.pQuoteRanks[index];
			Output("\t  Quote id [%u] Ranks : sort=%u  bid=%u  ask=%u  \n",
				(unsigned long)qr.uiQuoteId,
				(unsigned long)qr.uiSortRank,
				(unsigned long)qr.uiBidRank,
				(unsigned long)qr.uiAskRank);
		} 
	}
	else 
	{
		Output("%s : Error, missing command data CFETI_DD_QUOTE_RANK_DESC", DDCommandToString(cmd));
	}
}

void OutputDDRequest(CFETI_DD_REQUEST r)
{
	unsigned int idx;

	if (!r)
	{
		Output("\t" "No DD Request details");
		return;
	}
	Output("\t" "DD Request Id\t[%d]", r->id);
	Output("\t" "Request Id\t[%d]", r->requestId);
	Output("\t" "Instrument\t\t[%s]", r->tradeInstrument?(const char*)r->tradeInstrument:"");
	Output("\t" "DD Preferences\t[0x%-8.8x]", r->preferences);
	Output("\t" "Size\t\t[%f]", r->size);
	Output("\t" "Minimum Size\t[%f]", r->minSize);
	Output("\t" "Price Ask\t[%f]", r->askPrice);
	Output("\t" "Price Bid\t[%f]", r->bidPrice);
	Output("\t" "Reserve Price Ask\t[%f]", r->askReservePrice);
	Output("\t" "Reserve Price Bid\t[%f]", r->bidReservePrice);
	Output("\t" "Minimum number of prices:\t[%d]", r->minPricesCount);
	Output("\t" "Time Limit (seconds)\t[%d]", r->timeLimit);
	Output("\t" "Action Type\t[%u]", r->action);
	Output("\t" "Classification\t[%s]", OutputString(r->szClassification));
	Output("\t" "Owner\t\t[%s]", OutputString(r->szFullUsername));
	Output("\t" "Legal Entity\t[%s]", OutputString(r->szLegalEntityName));
	Output("\t" "BOLS Spread\t[%f]", r->bolsSpread);
	for (idx = 0; idx < r->uiNumCounterparties; idx++)
		Output("\t" "Counterparty\t[%u]", r->pCounterpartyList[idx]);
	if (r->pMessage)
	{
		Output("\t" "Sender\t[%s]", OutputString(r->pMessage->sender));
		Output("\t" "Send Time\t[%24s]", ctime(&r->pMessage->sendTime));
		Output("\t" "Severity\t[%u]", r->pMessage->severity);
		for (idx = 0; idx < (unsigned int)r->pMessage->textLines; idx++)
			Output("\t" "Message\t[%s]", OutputString(r->pMessage->text[idx]));
	}
	switch (r->orderInfoType)
	{
	case CFETI_ORDERINFO_NOT_SPECIFIED:
		break;
	case CFETI_ORDERINFO_TSWAP:
		Output("\t" "OrderInfoType\t[%d]", r->orderInfoType);
		OutputTSwapDetails((CFETI_TSWAP)r->orderInfo);
		break;
	case CFETI_ORDERINFO_CANTOR_REPO:
		Output("\t" "OrderInfoType\t[%d]", r->orderInfoType);
		OutputCantorRepoDetails((CFETI_CANTOR_REPO_DESC*)r->orderInfo);
		break;
	default:
		break;
	}
}

void OutputQuote(CFETI_DD_QUOTE q)
{
	if (!q)
	{
		Output("\t" "No Quote details");
		return;
	}
	Output("\t" "Quote Id\t[%d]", q->id);
	Output("\t" "Request Id\t[%d]", q->requestId);
	Output("\t" "Instrument\t\t[%s]", (q->tradeInstrument?(const char*)q->tradeInstrument:""));
	Output("\t" "DD Preferences\t[%u]", q->preferences);
	Output("\t" "Size\t\t[%f]", q->size);
	Output("\t" "Price Ask\t[%f]", q->askPrice);
	Output("\t" "Price Bid\t[%f]", q->bidPrice);
	Output("\t" "Time Limit (seconds)\t[%d]", q->timeLimit);
	Output("\t" "Action Type\t[%u]", q->action);
	Output("\t" "DD Request Id\t\t[%d]", q->RFQId);
	Output("\t" "Owner\t\t[%s]", OutputString(q->szFullUsername));
	Output("\t" "Legal Entity\t[%s]", OutputString(q->szLegalEntityName));
	switch (q->orderInfoType)
	{
	case CFETI_ORDERINFO_NOT_SPECIFIED:
		break;
	case CFETI_ORDERINFO_TSWAP:
		Output("\t" "OrderInfoType\t[%d]", q->orderInfoType);		
		OutputTSwapDetails((CFETI_TSWAP)q->orderInfo);
		break;
	case CFETI_ORDERINFO_CANTOR_REPO:
		Output("\t" "OrderInfoType\t[%d]", q->orderInfoType);
		OutputCantorRepoDetails((CFETI_CANTOR_REPO_DESC*)q->orderInfo);
	default:
		break;
	}
}

#ifdef  __cplusplus
extern "C" {
#endif
void ConnectCallback(CFETI_CMD cmd, CFETI_CMD_STATUS cmdStatus, CFETI_CMDDATA cmdData, CFETI_UD /* userData */)
{
	CFETI_CONNECT_INFO *c;

	/*
	 * Output status text if there is any. Exclude if cmd is CFETI_STATUS
	 * since these may have been reported in the system callback
	 */
	if (cmd != CFETI_STATUS && cmdStatus && cmdStatus->statusText)
	{
		Output(cmdStatus->statusText);
	}

	switch(cmd)
	{
	case CFETI_MKT_ACCEPTED:
		Output("Market accepted");
		OutputMarket((CFETI_MARKET)cmdData);
		break;
	case CFETI_MKT_REJECTED:
		Output("Market rejected");
		OutputMarket((CFETI_MARKET)cmdData);
		break;
	case CFETI_MKT_EXECUTED:
		Output("Market executed");
		OutputMarket((CFETI_MARKET)cmdData);
		break;
	case CFETI_MKT_EXECUTING:
		Output("Market executing");
		OutputMarket((CFETI_MARKET)cmdData);
		break;
	case CFETI_MKT_CANCELLED:
		Output("Market cancelled");
		OutputMarket((CFETI_MARKET)cmdData);
		break;
	case CFETI_MKT_CANCEL_QUEUED:
		Output("Market cancel queued");
		OutputMarket((CFETI_MARKET)cmdData);
		break;
	case CFETI_CANCEL_MARKET_ACCEPTED:
		Output("Cancel Market accepted");
		OutputMarket((CFETI_MARKET)cmdData);
		break;
	case CFETI_CANCEL_MARKET_REJECTED:
		Output("Cancel Market rejected");
		OutputMarket((CFETI_MARKET)cmdData);
		break;
	case CFETI_MKT_REFRESH:
		Output("Refresh Market");
		OutputMarket((CFETI_MARKET)cmdData);
		break;
	case CFETI_MKT_CREATED:
		Output("Market Created");
		OutputMarket((CFETI_MARKET)cmdData);
		break;
	case CFETI_MKT_MOVED:
		Output("Market Moved");
		OutputMarket((CFETI_MARKET)cmdData);
		break;
	case CFETI_ORDER_QUEUED:
		Output("Order queued");
		OutputOrder((CFETI_ORDER)cmdData);
		break;
	case CFETI_ORDER_REJECTED:
		Output("Order rejected");
		OutputOrder((CFETI_ORDER)cmdData);
		break;
	case CFETI_ORDER_CANCELLED:
		Output("Order Cancelled");
		OutputOrder((CFETI_ORDER)cmdData);
		break;
	case CFETI_ORDER_CANCEL_QUEUED:
		Output("Order Cancel Queued");
		OutputOrder((CFETI_ORDER)cmdData);
		break;
	case CFETI_ORDER_EXECUTED:
		Output("Order executed");
		OutputOrder((CFETI_ORDER)cmdData);
		break;
	case CFETI_ORDER_EXECUTING:
		Output("Order executing");
		OutputOrder((CFETI_ORDER)cmdData);
		break;
	case CFETI_ORDER_BROKER_HELD:
		Output("Order broker held");
		OutputOrder((CFETI_ORDER) cmdData);
		break;
	case CFETI_ORDER_HELD:
		Output("Order held");
		OutputOrder((CFETI_ORDER) cmdData);
		break;
	case CFETI_ORDER_UNHELD:
		Output("Order unheld");
		OutputOrder((CFETI_ORDER) cmdData);
		break;
	case CFETI_TRADE_CHECKEDOUT:
		Output("Trade checkedout");
		OutputOrder((CFETI_ORDER) cmdData);
		break;
	case CFETI_TRADE_CHECKOUT_ACCEPTED:
		Output("Trade checkout request accepted");
		OutputOrder((CFETI_ORDER) cmdData);
		break;
	case CFETI_TRADE_CHECKOUT_REJECTED:
		Output("Trade checkout request rejected");
		OutputOrder((CFETI_ORDER) cmdData);
		break;
	case CFETI_TRADE_REFUSE_ACCEPTED:
		Output("Trade refuse request accepted");
		OutputOrder((CFETI_ORDER) cmdData);
		break;
	case CFETI_TRADE_REFUSE_REJECTED:
		Output("Trade refuse request rejected");
		OutputOrder((CFETI_ORDER) cmdData);
		break;
	case CFETI_TRADE_CONFIRM:
		Output("Trade Confirm");
		OutputOrder((CFETI_ORDER)cmdData);
		break;
	case CFETI_CANCEL_ORDER_ACCEPTED:
		Output("Cancel Order Accepted");
		OutputOrder((CFETI_ORDER)cmdData);
		break;
	case CFETI_CANCEL_ORDER_REJECTED:
		Output("Cancel Order Rejected");
		OutputOrder((CFETI_ORDER)cmdData);
		break;
	case CFETI_ORDER_REFRESH:
		Output("Refresh Order");
		OutputOrder((CFETI_ORDER)cmdData);
		break;
	case CFETI_QUERY_ACCEPTED:
		Output("Query Accepted");
		{
			unsigned int kdx;
			CFETI_QUERY_RESULT qr = (CFETI_QUERY_RESULT)cmdData;
			switch(qr->query.queryType)
			{
			case CFETQ_TRADE:
				{
					Output("Trade query results ");
					for (kdx = 0; kdx < qr->result.trade.numOrders; kdx++)
						OutputOrder(&qr->result.trade.orderList[kdx]);
				}
				break;
			case CFETQ_INSTRUMENT:
				{
					Output("Instrument query results ");
					for (kdx = 0; kdx < qr->result.instrument.numInstruments; kdx++)
					{
						Output("\t%s : %s", qr->result.instrument.instrumentList[kdx].name, qr->result.instrument.instrumentList[kdx].description ? qr->result.instrument.instrumentList[kdx].description : "(no description)");
					}
				}
				break;
			default:
				Output("Unknown query type");
				break;
			}
		}
		break;
	case CFETI_QUERY_REJECTED:
		if (cmdStatus)
			Output("Query Rejected -> %d (%s)", cmdStatus->cmdStatus, (cmdStatus->statusText ? cmdStatus->statusText : "unknown"));
		else
			Output("Query Rejected");

		break;
	case CFETI_SUBSCRIBE_ACCEPTED:
		Output("Subscribe Accepted : %s", ((CFETI_INSTRUMENT_DATA)cmdData)->instName);
		OutputInstrumentData((CFETI_INSTRUMENT_DATA)cmdData);
		break;
	case CFETI_SUBSCRIBE_REJECTED:
		Output("Subscribe Rejected : \"%s\"", (CFETI_INSTRUMENT)cmdData);
		break;
	case CFETI_SUBSCRIBE_STATUS:
		if (cmdStatus)
		{
			switch(cmdStatus->cmdStatus)
			{
			case CFETI_INSTRUMENT_LOST:
				Output("Subscribe Status - Instrument Lost : \"%s\"", cmdData ? (CFETI_INSTRUMENT)cmdData : "(instrument not specified)");
				break;
			case CFETI_INSTRUMENT_RESTORED:
				Output("Subscribe Status - Instrument Restored : \"%s\"", cmdData ? (CFETI_INSTRUMENT)cmdData : "(instrument not specified)");
				break;
			case CFETI_MARKET_FEED_DOWN:
				Output("Subscribe Status - Market Data Feed Down (Trading system %d)", (CFETI_TRADING_SYSTEM)cmdData);
				break;
			case CFETI_MARKET_FEED_RESTORED:
				Output("Subscribe Status - Market Data Feed Restored (Trading system %d)", (CFETI_TRADING_SYSTEM)cmdData);
				break;
			default:
				Output("Subscribe Status %d", cmdStatus->cmdStatus);
				break;
			}
		}
		else
		{
			Output("Subscribe Status (unknown)");
		}
		break;
	case CFETI_UNSUBSCRIBE_ACCEPTED:
		Output("Unsubscribe Accepted : \"%s\"", (CFETI_INSTRUMENT)cmdData);
		break;
	case CFETI_UNSUBSCRIBE_REJECTED:
		Output("Unsubscribe Rejected : \"%s\"", (CFETI_INSTRUMENT)cmdData);
		break;
	case CFETI_UPDATE:
		Output("Market Data Update");
		OutputInstrumentData((CFETI_INSTRUMENT_DATA)cmdData);
		break;
	case CFETI_CONNECTION_ACCEPTED:
		{
			c = (CFETI_CONNECT_INFO *)cmdData;
			Output("Connection Accepted - trading session id %d", c->sessionId);
			if (currentTradingSession == 0)
				currentTradingSession = c->sessionId;

			//If this connection is Direct Dealing enabled, send an Direct Dealing Registration Message
			if(c->tradeFlags & CFETI_DIRECT_DEALING_ENABLED)
			{
				CFETI_RC result = CFETIPostMessage(currentSession, currentTradingSession, CFETC_DD_REGISTER, 0, 0);
				if(result != CFETI_SUCCESS)
				{
					Output("Direct Dealing Registration Failed");
				}
			}
		}

		break;
	case CFETI_CONNECTION_REJECTED:
		Output("Connection Rejected for session %s", (char *)cmdData);
		break;
	case CFETI_CONNECTION_TERMINATED:
		c = (CFETI_CONNECT_INFO *)cmdData;
		Output("Connection to %d terminated - trading session id %d", c->tradingSystem.tsId, c->sessionId);
		break;
	case CFETI_DISCONNECT_ACCEPTED:
		c = (CFETI_CONNECT_INFO *)cmdData;
		Output("Disconnect from %d accepted - trading session id %d", c->tradingSystem.tsId, c->sessionId);
		break;
	case CFETI_DISCONNECT_REJECTED:
		c = (CFETI_CONNECT_INFO *)cmdData;
		Output("Disconnect from %d rejected - trading session id %d", c->tradingSystem.tsId, c->sessionId);
		break;
	case CFETI_REFRESH_COMPLETE:
		if (cmdStatus)
			Output("Refresh Complete -> %d (%s)", cmdStatus->cmdStatus, (cmdStatus->statusText ? cmdStatus->statusText : "no message"));
		else
			Output("Refresh Complete");
		break;
	case CFETI_INST_QUERY_COMPLETE:
		if (cmdStatus)
			Output("Instrument Query Complete -> %d (%s)", cmdStatus->cmdStatus, 
				(cmdStatus->statusText ? cmdStatus->statusText : "no message"));
		else
			Output("Instrument Query Complete");
		break;
	case CFETI_TRADE_QUERY_COMPLETE:
		if (cmdStatus)
			Output("Trade Query Complete -> %d (%s)", cmdStatus->cmdStatus, 
				(cmdStatus->statusText ? cmdStatus->statusText : "no message"));
		else
			Output("Trade Query Complete");
		break;
#if defined(CFETI_PRIORITY_NOTIFICATION)
	case CFETI_PRIORITY_NOTIFICATION:
		OutputTraderPriority((CFETI_TRADER_PRIORITY_DESC*)cmdData);		
		break;
#endif

	case CFETI_DD_REQUEST_SUBMIT_ACCEPTED:
	case CFETI_DD_REQUEST_SUBMIT_REJECTED:
	case CFETI_DD_REQUEST_CANCEL_ACCEPTED:
	case CFETI_DD_REQUEST_CANCEL_REJECTED:
	case CFETI_DD_REQUEST_MODIFY_ACCEPTED:
	case CFETI_DD_REQUEST_MODIFY_REJECTED:
	case CFETI_DD_REQUEST_MODIFIED:
	case CFETI_DD_REQUEST_CANCELLED:
	case CFETI_DD_REQUEST_EXECUTED:
	case CFETI_DD_REQUEST_CREATED:
	case CFETI_DD_REQUEST_TRADED:
	case CFETI_DD_SYSTEM_MESSAGE:
		OutputDDRequest((CFETI_DD_REQUEST) cmdData);
		if (cmdStatus)
		{
			Output("%s -> %d (%s)",
				   DDCommandToString(cmd),
				   cmdStatus->cmdStatus, 
				   (cmdStatus->statusText ? cmdStatus->statusText : "no message"));
					
		}
		else
			Output("%s Complete", DDCommandToString(cmd));
		break;
				
	case CFETI_DD_QUOTE_SUBMIT_ACCEPTED:
	case CFETI_DD_QUOTE_SUBMIT_REJECTED:
	case CFETI_DD_QUOTE_CANCEL_ACCEPTED:
	case CFETI_DD_QUOTE_CANCEL_REJECTED:
	case CFETI_DD_QUOTE_MODIFY_ACCEPTED:
	case CFETI_DD_QUOTE_MODIFY_REJECTED:
	case CFETI_DD_QUOTE_CANCELLED:
	case CFETI_DD_QUOTE_ACCEPTED:
	case CFETI_DD_QUOTE_REJECTED:
	case CFETI_DD_QUOTE_CREATED:
	case CFETI_DD_QUOTE_MODIFIED:
	case CFETI_DD_QUOTE_DELETED:
	case CFETI_DD_ACCEPT_QUOTE_ACCEPTED:
	case CFETI_DD_ACCEPT_QUOTE_REJECTED:
	case CFETI_DD_REJECT_QUOTE_ACCEPTED:
	case CFETI_DD_REJECT_QUOTE_REJECTED:
	case CFETI_DD_SUBMIT_TO_MARKET_ACCEPTED:
	case CFETI_DD_SUBMIT_TO_MARKET_REJECTED:
		OutputQuote((CFETI_DD_QUOTE) cmdData);
		if (cmdStatus)
		{
			Output("%s -> %d (%s)",
				   DDCommandToString(cmd),
				   cmdStatus->cmdStatus, 
				   (cmdStatus->statusText ? cmdStatus->statusText : "no message"));
					
		}
		else
			Output("%s Complete", DDCommandToString(cmd));
		break;
	case CFETI_DD_UPDATE_QUOTE_RANK:
		OutputDDQuoteRankInfo(cmd, cmdData);
		break;
	case CFETI_DD_SET_LEGAL_ENTITY_LIST:
		{
			Output("Legal Entity List");
			CFETI_LEGAL_ENTITY_LIST pList = (CFETI_LEGAL_ENTITY_LIST)cmdData;
			for (unsigned int idx = 0; idx < pList->uiNumEntities; idx++)
			{
				if (pList->pEntity[idx].szLEShortName)
					Output("\t%-8d\t%s [%s]", pList->pEntity[idx].uiLegalEntityId, pList->pEntity[idx].szLegalEntityName, pList->pEntity[idx].szLEShortName);
				else
					Output("\t%-8d\t%s", pList->pEntity[idx].uiLegalEntityId, pList->pEntity[idx].szLegalEntityName);
			}
		}
		break;
	case CFETI_CREDIT_STATUS_UPDATE:
		{
			CFETI_CREDIT_STATUS_DESC* credit = (CFETI_CREDIT_STATUS_DESC*)cmdData;
			Output("Credit: Initial[%f] Current[%f] Low[%f]", credit->initialCredit, credit->currentCredit, credit->lowCredit);
		}
		break;
	default:
		break;
	}
}
#ifdef  __cplusplus
}
#endif

#ifdef  __cplusplus
extern "C" {
#endif
void SystemCallback(CFETI_CMD cmd, CFETI_CMD_STATUS cmdStatus, CFETI_CMDDATA cmdData, CFETI_UD /* userData */)
{
	if (cmdStatus && cmdStatus->statusText)
		Output(cmdStatus->statusText);

	switch(cmd)
	{
	case CFETI_LOGIN_ACCEPTED:
		{
			CFETI_LOGIN_INFO *l = (CFETI_LOGIN_INFO *)cmdData;
			Output("Login accepted - session id %s connection mode %s", l->sessionId, l->szConnectionMode);
			for (CFETI_TRADING_SYS *ts = l->ts; ts && *ts; ts++)
				Output("\tTrading System : %d %s", (*ts)->tsId, ((*ts)->tsDescription ? (*ts)->tsDescription : "(no description)"));
			if (!currentSession)
				currentSession = strdup(l->sessionId);
		}
		break;
	case CFETI_LOGIN_REJECTED:
		Output("Login rejected : user %s", cmdData ? (char *)cmdData : "unknown");
		break;
	case CFETI_LOGIN_TERMINATED:
		Output("Login terminated : session %s", cmdData ? (char *)cmdData : "unknown");
		break;
	case CFETI_STATUS:
		if (cmdStatus)
			Output("Status %d", cmdStatus->cmdStatus);
		else
			Output("Status");
		break;
	case CFETI_LOGOUT_ACCEPTED:
		Output("Logout accepted : session %s", cmdData ? (char *)cmdData : "unknown");
		break;
	case CFETI_LOGOUT_REJECTED:
		Output("Logout rejected : session %s", cmdData ? (char *)cmdData : "unknown");
		break;
	case CFETI_SET_PASSWORD_ACCEPTED:
		Output("Set password accepted: session %s", cmdData ? (char *)cmdData : "unknown");
		break;
	case CFETI_SET_PASSWORD_REJECTED:
		Output("Set password rejected: session %s", cmdData ? (char *)cmdData : "unknown");
	    break;
	default:
		break;
	}
}
#ifdef  __cplusplus
}
#endif

unsigned int String2ActionType(const char* str)
{
	if (!str)
		return 0;

	if (STRCASECMP(str, "buy") == 0)
		return CFETI_DD_BUY;

	if (STRCASECMP(str, "sell") == 0)
		return CFETI_DD_SELL;

	if (STRCASECMP(str, "bid") == 0)
		return CFETI_DD_BID;

	if (STRCASECMP(str, "ask") == 0)
		return CFETI_DD_ASK;

	if (STRCASECMP(str, "2way") == 0)
		return CFETI_DD_2WAY;

	if (STRCASECMP(str, "bols") == 0)
		return CFETI_DD_BOLS_2WAY;

	if (STRCASECMP(str, "2wayQuote") == 0)
		return CFETI_DD_2WAY_QUOTE;

	if (STRCASECMP(str, "bolsQuote") == 0)
		return CFETI_DD_BOLS_2WAY_QUOTE;

	return 0;
}

void DoBidAskBuySell(char *cmd, int cancel)
{
	unsigned char flag;
	CFETI_CMD command;
	CFETI_PREF pref;
	char* code;
	char* app;
	char* usr;

	flag = 0;
	command = 0;
	pref = 0;
	code = 0;
	app = 0;
	usr = 0;

	if (!currentSession)
	{
		Output("Expect this to fail - you haven't set the current session!");
	}

	if (STRCASECMP(cmd, "bid") == 0)
	{
		command = cancel ? CFETC_CANCEL_MARKET : CFETC_POST_MARKET;
		flag = CFETI_MARKET_BID;
	}
	else if (STRCASECMP(cmd, "ask") == 0)
	{
		command = cancel ? CFETC_CANCEL_MARKET : CFETC_POST_MARKET;
		flag = CFETI_MARKET_ASK;
	}
	else if (STRCASECMP(cmd, "buy") == 0)
	{
		command = cancel ? CFETC_CANCEL_ORDER : CFETC_SUBMIT_ORDER;
		flag = CFETI_ORDER_TAK;
	}
	else if (STRCASECMP(cmd, "sell") == 0)
	{
		command = cancel ? CFETC_CANCEL_ORDER : CFETC_SUBMIT_ORDER;
		flag = CFETI_ORDER_HIT;
	}
	else if (STRCASECMP(cmd, "checkout") == 0)
	{
		command = CFETC_TRADE_CHECKOUT_REQUEST;
		flag = CFETI_ORDER_BUY_SELL;
	}
	else if (STRCASECMP(cmd, "refuse") == 0)
	{
		command = CFETC_TRADE_REFUSE_REQUEST;
		flag = CFETI_ORDER_BUY_SELL;
	}
	else
	{
		return;
	}

	switch(command)
	{
	case CFETC_POST_MARKET:
	case CFETC_CANCEL_MARKET:
		{
			CFETI_MARKET_DESC market;
			memset((char*)&market, 0, sizeof(market));

			market.tradeInstrument = next();
			market.price = atof(next());
			market.size = atof(next());
			market.side = flag;
			pref = atoi(next());
			market.id = atoi(next());
			market.requestId = atoi(next());
			market.tradeId = 0;

			usr = next();
			if (STRCASECMP(usr, "NULL") == 0)
			{
				market.userData = 0;
				market.userDataSize = 0;			
			}
			else
			{
				market.userData = (void*)usr;
				market.userDataSize = static_cast<unsigned short>(strlen((char*)market.userData));
			}

			app = next(); 
			if (STRCASECMP(app, "NULL") == 0)
			{
				market.appUserData = 0;
				market.appUserDataSize = 0;
			}
			else
			{
				market.appUserData = (void*)app;
				market.appUserDataSize = static_cast<unsigned short>(strlen((char*)market.appUserData));
			}
		 
			code = next();
			if ((STRCASECMP(code, "NULL") == 0) || (*code == '\0'))
				market.shortCode = 0;
			else	
				market.shortCode = code;

			int result = CFETIPostMessage(currentSession, currentTradingSession, command, (CFETI_CMDDATA)&market, pref);

			switch(result)
			{
			case CFETI_SUCCESS:
				break;
			default:
				Output("CFETIPostMessage (%s) Failed : Error %d (%s)", (command == CFETC_POST_MARKET) ? "POST MARKET" : "CANCEL MARKET", result, CFETIGetLastError());
				break;
			}
		}
		break;
	case CFETC_SUBMIT_ORDER:
	case CFETC_CANCEL_ORDER:
		{
			CFETI_ORDER_DESC order;
			memset((char*)&order, 0, sizeof(order));

			order.tradeInstrument = next();
			order.price = atof(next());
			order.size = atof(next());
			order.indicator = flag;
			order.userName = 0;
			pref = atoi(next());
			order.id = atoi(next());
			order.requestId = atoi(next());
			order.tradeId = 0;
			
			usr = next();
			if (STRCASECMP(usr, "NULL") == 0)
			{
				order.userData = 0;
				order.userDataSize = 0;			
			}
			else
			{
				order.userData = (void*)usr;
				order.userDataSize = static_cast<unsigned short>(strlen((char*)order.userData));
			}

			app = next(); 
			if (STRCASECMP(app, "NULL") == 0)
			{
				order.appUserData = 0;
				order.appUserDataSize = 0;
			}
			else
			{
				order.appUserData = (void*)app;
				order.appUserDataSize = static_cast<unsigned short>(strlen((char*)order.appUserData));
			}

		 
			code = next();
			if ((STRCASECMP(code, "NULL") == 0) || (*code == '\0'))
				order.shortCode = 0;
			else	
				order.shortCode = code;

			order.toPrice = atof(next());

			int result = CFETIPostMessage(currentSession, currentTradingSession, command, (CFETI_CMDDATA)&order, pref);

			switch(result)
			{
			case CFETI_SUCCESS:
				break;
			default:
				Output("CFETIPostMessage (%s) Failed : Error %d (%s)", (command == CFETC_POST_MARKET) ? "POST MARKET" : "CANCEL MARKET", result, CFETIGetLastError());
				break;
			}
		}
		break;
	case CFETC_TRADE_CHECKOUT_REQUEST:
	case CFETC_TRADE_REFUSE_REQUEST:
		{
			CFETI_ORDER_DESC order;
			memset((char*)&order, 0, sizeof(order));

			order.userName = order.requestorId = next();
			order.originatorId = next();
			order.tradeInstrument = next();
			char* tmp = next();
			order.tradeId = tmp;
			order.tradeReference = tmp;

			char* indicator = next();
			order.indicator = (STRCASECMP(indicator, "sell") == 0) ? CFETI_ORDER_SELL : CFETI_ORDER_BUY;

			order.tradeSide = (STRCASECMP(indicator, "sell") == 0) ? CFETI_TRADE_ACTIVE : CFETI_TRADE_PASSIVE;
			order.tradePrice = atof(next());
			order.tradeSize = atof(next());

			// typically set to CFETI_MARKET_SIZE_IS_TOTAL | CFETI_MANAGED_ORDER;
			pref = atoi(next());

			order.requestId = atoi(next());
			
			// client checkout flags
			order.checkoutPermissions = CFETI_CLIENT_CHECKOUT_ACCEPT | CFETI_CLIENT_CHECKOUT_REFUSE;

			int result = CFETIPostMessage(currentSession, currentTradingSession, command, (CFETI_CMDDATA) &order, pref);
			if (CFETI_SUCCESS != result)
			{
				Output("CFETIPostMessage (%s) Failed : Error %d (%s)",
					   (command == CFETC_TRADE_CHECKOUT_REQUEST) ? "CFETC_TRADE_CHECKOUT_REQUEST" : "CFETC_TRADE_REFUSE_REQUEST",
					   result,
					   CFETIGetLastError());
			}
		}
		break;
	default:
		break;
	}
}

void ShowHelp()
{
	Output("");
	Output("Login user password [preferences]          : Session login");
	Output("Logout session [preferences]               : Session logout");
	Output("Connect session tsId [tsPref] [password]   : Trd Sys connect");
	Output("Disconnect session tsSession [preferences] : Trd Sys disconnect");
	Output("Select session tsSession                   : Set session info");
	Output("Subscribe|Unsubscribe instrument           : Subscribe to inst *");
	Output("Query Trade [tradeId]                      : Trades (no id = today *)");
	Output("Query Inst field-id(int) type(int) value   : List instruments *");
	Output("Query Inst all                             : List all instruments");
	Output("[cancel] bid|ask|buy|sell                  : inst price size [pref] [id] [reqId] -");
	Output("    [data]+ [appData]+ [code]+ [toPrice]   : [cxl] bid/ask/buy/sell *");
	Output("Refresh                                    : Refresh markets/orders *");
	Output("");
	Output("checkout|refuse                            : trader broker inst tradeRef buy|sell price size pref reqId");
	Output("");
	Output("DDR submit buy|sell|2way|bols inst askPrice bidPrice askReservePrice bidReservePrice size minSize minPrices timeLimit [voice][auto][bolsSpread]: DD Request");
	Output("DDR modify buy|sell|2way|bols inst DDid askPrice bidPrice askReservePrice bidReservePrice size minSize minPrices timeLimit [voice][guarantee][auto][bolsSpread]: DD Request");
	Output("DDR cancel inst DDid                       : DD Request");
	Output("");
	Output("DDQ submit bid|ask inst DDid   askPrice bidPrice size timeLimit           : DD Quote");
	Output("DDQ modify bid|ask inst QuoteId askPrice bidPrice size timeLimit [inactive]: DD Quote");
	Output("DDQ cancel inst QuoteId                                                    : DD Quote");
	Output("");
	Output("DQR accept buy|sell inst QuoteId DDid size askPrice bidPrice  : DD Quote Response - accept quote");
	Output("DQR reject buy|sell inst QuoteId DDid size askPrice bidPrice  : DD Quote Response - reject quote");
	Output("");
   	Output("SetPw session password newPw confirmedPw   : Set new password for the user");
	Output("Help                                       : List commands");
	Output("Bye|Exit|Quit                              : Close client");
	Output("* = Select session first  + = Enter \"NULL\" if not required");

	return;
}

int DoCommand(char *cmdp)
{
	int carryOn = 1;
	char *cmd = strdup(cmdp);
	char *token = tokenizer(cmd);

	if (!token && !*token)
	{
		free(cmd);
		return carryOn;
	}

	Output(cmdp);

	if (STRCASECMP(token, "login") == 0)
	{
		char *userTok = next();
		char *user = !*userTok ? 0 : userTok;
		char *passTok = next();
		char *pass = !*passTok ? 0 : passTok;
		int pref = atoi(next());

		int result = CFETILogin(user, pass, pref, SystemCallback, 0);
		switch(result)
		{
		case CFETI_SUCCESS:
			break;
		default:
			Output("CFETILogin Failed : Error %d (%s)", result, CFETIGetLastError());
			break;
		}
	}
	else if (STRCASECMP(token, "logout") == 0)
	{
		char *sessTok = next();
		char *sessId = !*sessTok ? 0 : sessTok;
		int pref = atoi(next());

		int result = CFETILogout(sessId, pref);
		switch(result)
		{
		case CFETI_SUCCESS:
			break;
		default:
			Output("CFETILogout Failed : Error %d (%s)", result, CFETIGetLastError());
			break;
		}
	}
	else if (STRCASECMP(token, "connect") == 0)
	{
		char *sessTok = next();
		char *sessId = !*sessTok ? 0 : sessTok;
		int tsId = atoi(next());
		int pref = atoi(next());
		char *passTok = next();
		char *pass = !*passTok ? 0 : passTok;

		CFETI_TRADE_SETTINGS_DESC tradeSettings;
		::memset(&tradeSettings, 0, sizeof(tradeSettings));
		tradeSettings.retainRule = CFETI_RETAIN_DEFAULT;
		tradeSettings.priorityFollowRule = CFETI_PRIORITY_FOLLOW_DEFAULT;

		int result = CFETIConnect(sessId, pass, tsId, pref, ConnectCallback, 0, &tradeSettings);
		switch(result)
		{
		case CFETI_SUCCESS:
			break;
		default:
			Output("CFETIConnect Failed : Error %d (%s)", result, CFETIGetLastError());
			break;
		}
	}
	else if (STRCASECMP(token, "disconnect") == 0)
	{
		char *sessTok = next();
		char *sessId = !*sessTok ? 0 : sessTok;
		int tsSessId = atoi(next());
		int pref = atoi(next());

		int result = CFETIDisconnect(sessId, tsSessId, pref);
		switch(result)
		{
		case CFETI_SUCCESS:
			break;
		default:
			Output("CFETIDisconnect Failed : Error %d (%s)", result, CFETIGetLastError());
			break;
		}
	}
	else if (STRCASECMP(token, "select") == 0)
	{
		if (currentSession)
			free(currentSession);
		currentSession = next();
		if (*currentSession)
			currentSession = strdup(currentSession);
		currentTradingSession = atoi(next());
		Output("Current Session is now %s", (const char *)currentSession);
		Output("Current Trading System Session is now %d", (const char *)currentTradingSession);
	}
	else if (STRCASECMP(token, "cancel") == 0)
	{
		token = next();
		if ((STRCASECMP(token, "bid") == 0) || (STRCASECMP(token, "ask") == 0) || (STRCASECMP(token, "buy") == 0) || (STRCASECMP(token, "sell") == 0))
		{
			DoBidAskBuySell(token, 1);
		}
		else
		{
			Output("Unrecognised trading command");
		}
	}
	else if ((STRCASECMP(token, "bid") == 0) ||
               (STRCASECMP(token, "ask") == 0) ||
               (STRCASECMP(token, "buy") == 0) ||
               (STRCASECMP(token, "sell") == 0) ||
               (STRCASECMP(token, "checkout") == 0) ||
               (STRCASECMP(token, "refuse") == 0))
	{
			DoBidAskBuySell(token, 0);
	}
	else if (STRCASECMP(token, "refresh") == 0)
	{
		CFETI_CMD cmd = CFETC_REFRESH;

		int result = CFETIPostMessage(currentSession, currentTradingSession, cmd, 0, 0);

		switch(result)
		{
		case CFETI_SUCCESS:
			break;
		default:
			Output("CFETIPostMessage (refresh) Failed : Error %d (%s)", result, CFETIGetLastError());
			break;
		}
	}
	else if ((STRCASECMP(token, "subscribe") == 0) || (STRCASECMP(token, "unsubscribe") == 0))
	{
		CFETI_CMD cmd = (STRCASECMP(token, "subscribe") == 0) ? CFETC_SUBSCRIBE : CFETC_UNSUBSCRIBE;

		char *instTok = next();
		char *inst = !*instTok ? 0 : instTok;

		int result = CFETIPostMessage(currentSession, currentTradingSession, cmd, (CFETI_CMDDATA)inst, 0);

		switch(result)
		{
		case CFETI_SUCCESS:
			break;
		default:
			Output("CFETIPostMessage (%s) Failed : Error %d (%s)", (cmd == CFETC_SUBSCRIBE) ? "SUBSCRIBE" : "UNSUBSCRIBE", result, CFETIGetLastError());
			break;
		}
	}
	else if ((STRCASECMP(token, "bye") == 0) || (STRCASECMP(token, "exit") == 0) || (STRCASECMP(token, "quit") == 0))
	{
		carryOn = 0;
	}
	else if (STRCASECMP(token, "query") == 0)
	{
		CFETI_QUERY_DESC query;

		char *qname = next();
		if (STRCASECMP(qname, "trade") == 0)
		{
			query.queryType = CFETQ_TRADE;
			char *idTok = next();
			char *id = !*idTok ? 0 : idTok;

			if (!id)
			{
				// This code is correct only if local timezone is GMT.
				// For other time zones, convert the local time to GMT
				time_t now = time(0);
				query.query.trade.startTime = ((now / spd) * spd);
				query.query.trade.endTime = now;
				query.query.trade.tradeId = 0;
			}
			else
			{
				query.query.trade.startTime = 0;
				query.query.trade.endTime = 0;
				query.query.trade.tradeId = id;
			}

			int result = CFETIPostMessage(currentSession, currentTradingSession, CFETC_QUERY, (CFETI_CMDDATA)&query, 0);

			switch(result)
			{
			case CFETI_SUCCESS:
				break;
			default:
				Output("CFETIPostMessage (QUERY) Failed : Error %d (%s)", result, CFETIGetLastError());
				break;
			}
		}
		else if (STRCASECMP(qname, "inst") == 0)
		{
			CFETI_FIELD_DESC field;
			query.queryType = CFETQ_INSTRUMENT;

			char* szWhat = next();
			unsigned short fid = 0;
			int type = atoi(next());
			char *value = next();
		   
			if (szWhat && STRCASECMP(szWhat, "all") == 0)
			{
				fid = 57;
				type = 8;
				value = (char*) "*";
			}
			else
			{
				fid = (unsigned short) atoi(szWhat);
			}
			
			if (fid <= 0)
			{
				Output("Field id %d out of range", fid);
			}
			else if (!*value)
			{
				Output("Must specify field id, type and value");
			}
			else if (type <= 0 || type >= 0x0B)
			{
				Output("Field type %d out of range", type);
			}
			else
			{
				field.fieldId = fid;
				field.fieldType = (unsigned char)type;
				switch(type)
				{
				case CFETI_FIELDTYPE_INT8:
					field.fieldValue.int8 = (char)(atoi((const char *)value));
					break;
				case CFETI_FIELDTYPE_BYTE:
					field.fieldValue.byte = (unsigned char)(atoi((const char *)value));
					break;
				case CFETI_FIELDTYPE_INT16:
					field.fieldValue.int16 = (short)(atoi((const char *)value));
					break;
				case CFETI_FIELDTYPE_UINT16:
					field.fieldValue.uint16 = (unsigned short)(atoi((const char *)value));
					break;
				case CFETI_FIELDTYPE_INT32:
					field.fieldValue.int32 = (int)(atoi((const char *)value));
					break;
				case CFETI_FIELDTYPE_UINT32:
					field.fieldValue.uint32 = (unsigned int)(atoi((const char *)value));
					break;
				case CFETI_FIELDTYPE_DECIMAL:
					field.fieldValue.decimal = atof((const char *)value);
					break;
				case CFETI_FIELDTYPE_DATETIME:
					field.fieldValue.dateTime = (time_t)(atoi((const char *)value));
					break;
				case CFETI_FIELDTYPE_BUFFER:
					field.fieldValue.buffer.bp = (char*)((const char *)value);
					field.fieldValue.buffer.len = static_cast<unsigned short>(strlen(field.fieldValue.buffer.bp));
					break;
				case CFETI_FIELDTYPE_STRING:
					field.fieldValue.string = (char*)((const char *)value);
					break;
				default:
					break;
				}

				query.query.instrument.numIndicativeFields = 1;
				query.query.instrument.indicativeFields = &field;
				query.query.instrument.trdType = 0;

				int result = CFETIPostMessage((char *)((const char *)currentSession), currentTradingSession, CFETC_QUERY, (CFETI_CMDDATA)&query, 0);

				switch(result)
				{
				case CFETI_SUCCESS:
					break;
				default:
					Output("CFETIPostMessage (QUERY) Failed : Error %d (%s)", result, CFETIGetLastError());
					break;
				}
			}
		}
		else
		{
			Output("Unrecognised query type : %s", (const char *)qname);
		}
	}
	else if (STRCASECMP(token, "DDR") == 0)
	{
		char* nTok = next();
		char* cmdStr =    *nTok? nTok: 0;

            nTok = next();

		if (!cmdStr)
		{
			Output("Direct Dealing: Command must be specified");
		}
		else 
		{
			CFETI_DD_REQUEST_DESC ddRequest;
			memset(&ddRequest, 0, sizeof(CFETI_DD_REQUEST_DESC));
			CFETI_CMD cmd = 0;

			if (STRCASECMP(cmdStr, "submit") == 0)
			{
				char* actionStr = *nTok? nTok: 0; nTok = next();
				char* instStr = *nTok? nTok: 0; nTok = next();
				char* askPriceStr =  *nTok? nTok: 0; nTok = next();
				char* bidPriceStr =  *nTok? nTok: 0; nTok = next();
				char* askReserveStr = *nTok? nTok: 0; nTok = next();
				char* bidReserveStr = *nTok? nTok: 0; nTok = next();
				char* sizeStr =   *nTok? nTok: 0; nTok = next();
				char* minSizeStr =   *nTok? nTok: 0; nTok = next();
				char* minPricesStr = *nTok? nTok: 0; nTok = next();
				char* timeLimitStr = *nTok? nTok: 0; nTok = next();
//				char* prefStr1 =      *nTok? nTok: 0;nTok = next();
//				char* prefStr2 =      *nTok? nTok: 0;nTok = next();
				char* bolsSpread =	  *nTok? nTok: 0; 

				ddRequest.action = String2ActionType(actionStr);
				ddRequest.requestId = GetNextRequestId();
				ddRequest.tradeInstrument = instStr;
				ddRequest.askPrice = askPriceStr? atof(askPriceStr): 0;
				ddRequest.bidPrice = bidPriceStr? atof(bidPriceStr): 0;
				ddRequest.askReservePrice = askReserveStr? atof(askReserveStr): 0;
				ddRequest.bidReservePrice = bidReserveStr? atof(bidReserveStr): 0;
				ddRequest.size = sizeStr? atof(sizeStr): 0;
				ddRequest.minSize = minSizeStr? atoi(minSizeStr): 0;
				ddRequest.minPricesCount = minPricesStr? atoi(minPricesStr): 0;
				ddRequest.timeLimit = timeLimitStr? atoi(timeLimitStr): 0;
				ddRequest.bolsSpread = bolsSpread? atof(bolsSpread): 0;

				// Test validity of cmd data
				if (!ddRequest.action || !ddRequest.tradeInstrument)
					Output("DD: DD Action and trade instrument must be specified.");
				else
					cmd = CFETC_DD_REQUEST_SUBMIT;
			}
			else if (STRCASECMP(cmdStr, "modify") == 0)
			{
				char* actionStr = *nTok? nTok: 0; nTok = next();
				char* instStr = *nTok? nTok: 0; nTok = next();
				char* idStr =  *nTok? nTok: 0; nTok = next();
				char* askPriceStr =  *nTok? nTok: 0; nTok = next();
				char* bidPriceStr =  *nTok? nTok: 0; nTok = next();
				char* askReserveStr = *nTok? nTok: 0; nTok = next();
				char* bidReserveStr = *nTok? nTok: 0; nTok = next();
				char* sizeStr =   *nTok? nTok: 0; nTok = next();
				char* minSizeStr =   *nTok? nTok: 0; nTok = next();
				char* minPricesStr = *nTok? nTok: 0; nTok = next();
				char* timeLimitStr = *nTok? nTok: 0; nTok = next();
//				char* prefStr1 =      *nTok? nTok: 0;nTok = next();
//				char* prefStr2 =      *nTok? nTok: 0;nTok = next();
//				char* prefStr3 =      *nTok? nTok: 0;nTok = next();
				char* bolsSpread =	  *nTok? nTok: 0; 

				ddRequest.action = String2ActionType(actionStr);
				ddRequest.tradeInstrument = instStr;
				ddRequest.id = idStr? atoi(idStr): 0;
				ddRequest.askPrice = askPriceStr? atof(askPriceStr): 0;
				ddRequest.bidPrice = bidPriceStr? atof(bidPriceStr): 0;
				ddRequest.askReservePrice = askReserveStr? atof(askReserveStr): 0;
				ddRequest.bidReservePrice = bidReserveStr? atof(bidReserveStr): 0;
				ddRequest.size = sizeStr? atof(sizeStr): 0;
				ddRequest.minSize = minSizeStr? atof(minSizeStr): 0;
				ddRequest.minPricesCount = minPricesStr? atoi(minPricesStr): 0;
				ddRequest.timeLimit = timeLimitStr? atoi(timeLimitStr): 0;
				ddRequest.bolsSpread = bolsSpread? atof(bolsSpread): 0;

				// Test validity of cmd data
				if (!ddRequest.action || !ddRequest.id)
					Output("DD: DD Action and id must be specified.");
				else
					cmd = CFETC_DD_REQUEST_MODIFY;
			}
			else if (STRCASECMP(cmdStr, "cancel") == 0)
			{
				char* instStr = *nTok? nTok: 0; nTok = next();
				char* idStr =  *nTok? nTok: 0;

				ddRequest.tradeInstrument = instStr;
				ddRequest.id = idStr? atoi(idStr): 0;

				// Test validity of cmd data
				if (!ddRequest.id)
					Output("DD: DD id must be specified.");
				else
					cmd = CFETC_DD_REQUEST_CANCEL;
			}
			else
			{
				Output("DD Request: Command not supported");
			}
				
			int result = 0;
			if (cmd)
				result = CFETIPostMessage((char *)((const char *)currentSession), currentTradingSession, cmd, (CFETI_CMDDATA)&ddRequest, 0);
			switch (result)
			{
			case CFETI_SUCCESS:
				break;
			default:
				Output("CFETIPostMessage (%d) Failed : Error %d (%s)", cmd, result, CFETIGetLastError());
				break;
			}

		}
	}
	else if (STRCASECMP(token, "DDQ") == 0)
	{
		char *nTok = next();
		char *cmdStr =    *nTok? nTok: 0; nTok = next();
			
		if (!cmdStr)
		{
			Output("Quote: Command must be specified");
		}
		else 
		{
			CFETI_DD_QUOTE_DESC quote;
			memset(&quote, 0, sizeof(CFETI_DD_QUOTE_DESC));
			CFETI_CMD cmd = 0;

			if (STRCASECMP(cmdStr, "submit") == 0)
			{
				char* actionStr =   *nTok? nTok: 0; nTok = next();
				char* instStr =   *nTok? nTok: 0; nTok = next();
				char* idStr =     *nTok? nTok: 0; nTok = next();
				char* askPriceStr =  *nTok? nTok: 0; nTok = next();
				char* bidPriceStr =  *nTok? nTok: 0; nTok = next();
				char* sizeStr =   *nTok? nTok: 0; nTok = next();
				char* timeLimitStr = *nTok? nTok: 0;

				quote.action = String2ActionType(actionStr);
				quote.tradeInstrument = instStr;				
				quote.RFQId = idStr? atoi(idStr): 0;
				quote.askPrice = askPriceStr? atol(askPriceStr): 0;
				quote.bidPrice = bidPriceStr? atol(bidPriceStr): 0;
				quote.size = sizeStr? atoi(sizeStr): 0;
				quote.timeLimit = timeLimitStr? atoi(timeLimitStr): 0;

				quote.preferences = 0;
				quote.requestId = GetNextQuoteId();

				// Test validity of cmd data
				if (!quote.RFQId || !quote.action || !quote.tradeInstrument)
					Output("Quote: DD request id  and trade instrument must be specified.");
				else
					cmd = CFETC_DD_QUOTE_SUBMIT;
			}
			else if (STRCASECMP(cmdStr, "modify") == 0)
			{
				char* actionStr =   *nTok? nTok: 0; nTok = next();
				char* instStr =   *nTok? nTok: 0; nTok = next();
				char* idStr =     *nTok? nTok: 0; nTok = next();
				char* askPriceStr =  *nTok? nTok: 0; nTok = next();
				char* bidPriceStr =  *nTok? nTok: 0; nTok = next();
				char* sizeStr =   *nTok? nTok: 0; nTok = next();
				char* timeLimitStr = *nTok? nTok: 0; nTok = next();
//				char* prefStr1 =      *nTok? nTok: 0;

				quote.action = String2ActionType(actionStr);
				quote.tradeInstrument = instStr;
				quote.id = idStr? atoi(idStr): 0;
				quote.askPrice = askPriceStr? atol(askPriceStr): 0;
				quote.bidPrice = bidPriceStr? atol(bidPriceStr): 0;
				quote.size = sizeStr? atoi(sizeStr): 0;
				quote.timeLimit = timeLimitStr? atoi(timeLimitStr): 0;

				quote.requestId = 0;

				// Test validity of cmd data
				if (!quote.tradeInstrument || !quote.id)
					Output("Quote: DD Action, trade instrument and quote id must be specified.");
				else
					cmd = CFETC_DD_QUOTE_MODIFY;
			}
			else if (STRCASECMP(cmdStr, "cancel") == 0)
			{
				char* instStr =   *nTok? nTok: 0; nTok = next();
				char* idStr =     *nTok? nTok: 0;

				quote.tradeInstrument = instStr;
				quote.id = idStr? atoi(idStr): 0; // This string is the id with cancel

				// Test validity of cmd data
				if (!quote.id)
					Output("Quote: DD Request id must be specified.");
				else
					cmd = CFETC_DD_QUOTE_CANCEL;
			}
			else
			{
				Output("DD Quote: Command not supported");
			}

			int result = 0;
			if (cmd)
				result = CFETIPostMessage((char *)((const char *)currentSession), currentTradingSession, cmd, (CFETI_CMDDATA)&quote, 0);
			switch (result)
			{
			case CFETI_SUCCESS:
				break;
			default:
				Output("CFETIPostMessage (%d) Failed : Error %d (%s)", cmd, result, CFETIGetLastError());
				break;
			}				
		}
	}
	else if (STRCASECMP(token, "DDQR") == 0)
	{
		char* nTok = next();
		char* cmdStr =    *nTok? nTok: 0; nTok = next();
		char* actionStr = *nTok? nTok: 0; nTok = next();
		char* instStr = *nTok? nTok: 0; nTok = next();
		char* quoteIdStr = *nTok? nTok: 0; nTok = next();
		char* RFQIdStr =  *nTok? nTok: 0; nTok = next();
		char* sizeStr =   *nTok? nTok: 0; nTok = next();
		char* askPriceStr =   *nTok? nTok: 0; nTok = next();
		char* bidPriceStr =   *nTok? nTok: 0; nTok = next();
			
		if (!cmdStr)
		{
			Output("Quote Response: Command must be specified");
		}
		else 
		{
			CFETI_DD_QUOTE_DESC quote;
			memset(&quote, 0, sizeof(CFETI_DD_QUOTE_DESC));
			CFETI_CMD cmd = 0;

			quote.action = String2ActionType(actionStr);
			quote.id = quoteIdStr? atoi(quoteIdStr): 0;
			quote.RFQId = RFQIdStr? atoi(RFQIdStr): 0;
			quote.tradeInstrument = instStr;
			quote.size = sizeStr? atoi(sizeStr): 0;
			quote.askPrice = askPriceStr? atol(askPriceStr): 0;
			quote.bidPrice = bidPriceStr? atol(bidPriceStr): 0;
			quote.requestId = GetNextQuoteResponseId();

			// Test validity of cmd data
			if (!quote.id || !quote.RFQId || !quote.tradeInstrument)
			{
				Output("Quote Response: Quote id, DD Request id and trade instrument must be specified.");
			}
			else if (STRCASECMP(cmdStr, "accept") == 0)
			{
				cmd = CFETC_DD_QUOTE_ACCEPT;
			}
			else if (STRCASECMP(cmdStr, "reject") == 0)
			{
				cmd = CFETC_DD_QUOTE_REJECT;
			}
			else
			{
				Output("DD Quote Response: Command not supported");
			}

			int result = 0;
			if (cmd)
				result = CFETIPostMessage((char *)((const char *)currentSession), currentTradingSession, cmd, (CFETI_CMDDATA)&quote, 0);
			switch (result)
			{
			case CFETI_SUCCESS:
				break;
			default:
				Output("CFETIPostMessage (%d) Failed : Error %d (%s)", cmd, result, CFETIGetLastError());
				break;
			}
		}
	}
   	else if (STRCASECMP(token, "setpw") == 0)
	{
		char* tok = 0;
		char* sessId = 0;
		char* setPassword = 0;
		char* newPassword = 0;
		char* confirmedPassword = 0;

		tok = next();
		sessId = !*tok ? 0 : tok;
		tok = next();
		setPassword = !*tok ? 0 : tok;
		tok = next();
		newPassword = !*tok ? 0 : tok;
		tok = next();
		confirmedPassword = !*tok ? 0 : tok;

		int result = CFETISetPassword(sessId, setPassword, newPassword, confirmedPassword);
		switch(result)
		{
		case CFETI_SUCCESS:
			break;
		default:
			Output("CFETISetPassword Failed : Error %d (%s)", result, CFETIGetLastError());
			break;
		}
	}
	else if (STRCASECMP(token, "help") == 0)
	{
		ShowHelp();
	}
	else if (strlen(token) > 0)
	{
		Output("Unrecognised command : %s", token);
	}

	free(cmd);
	return carryOn;
}


#if !defined(_WIN32)
#ifdef __cplusplus
extern "C" {
#endif
void* MessageLoop(void *arg)
{
	CFETIMessageLoop();
	Output("Message loop terminated");
	return 0;
}
#ifdef  __cplusplus
}
#endif
#else
DWORD MessageLoop(LPVOID /* param */)
{
	CFETIMessageLoop();
	Output("Message loop terminated");
	return 0;
}
#endif

int SampleMain(int argc, char **argv)
{
	int showUsageFlag = 0;
	int showModesFlag = 0;
	int idx = 1;
	int useGraphics = 1;

	while (idx < argc)
	{
		if (!strcmp(argv[idx], "-p"))
		{
			if ((++idx) < argc)
				primary = strdup(argv[idx]);
			else
				showUsageFlag = 1;
		}
		else if (!strcmp(argv[idx], "-s"))
		{
			if ((++idx) < argc)
				secondary = strdup(argv[idx]);
			else
				showUsageFlag = 1;
		}
		else if (!strcmp(argv[idx], "-u"))
		{
			if ((++idx) < argc)
				username = strdup(argv[idx]);
			else
				showUsageFlag = 1;
		}
		else if (!strcmp(argv[idx], "-x"))
		{
			if ((++idx) < argc)
				password = strdup(argv[idx]);
			else
				showUsageFlag = 1;
		}
		else if (!strcmp(argv[idx], "-f"))
		{
			if ((++idx) < argc)
				logfile = strdup(argv[idx]);
			else
				showUsageFlag = 1;
		}
		else if (!strcmp(argv[idx], "-b"))
		{
			useGraphics = 0;
		}
		else if (!strcmp(argv[idx], "-h"))
		{
			showUsageFlag = 1;
		}
		else if (!strcmp(argv[idx], "-es"))
		{
			if ((++idx) < argc)
				connectionMode = strdup(argv[idx]);
			else
				showUsageFlag = 1;
		}
		else if (!strcmp(argv[idx], "-el"))
		{
			showModesFlag = 1;
		} 
        else
        {
            printf("Unknown option %s\n", argv[idx]);
            showUsageFlag = 1;
        }
        
		idx++;
	}

	if (connectionMode)
	{
		CFETI_RC result;
		result = CFETISelectConnectionMode(connectionMode);
		switch (result)
		{
		case CFETI_SUCCESS:
			break;
		case CFETI_CONNECTION_MODE_NOT_VALID:
			(void)fprintf(stderr, "%s: connection mode %s is not valid\n", argv[0], connectionMode);
			showModesFlag = 1;
			break;
		case CFETI_CONNECTION_MODE_SELECT_FAILED:
			// This error occurs when for instance a communication session  to the session manager already is established
			(void)fprintf(stderr, "%s: connection mode %s could not be selected because session already established\n", argv[0], connectionMode);
			break;
		default:
			(void)fprintf(stderr, "%s: selecting connection mode %s caused an unknown internal error: code=%d\n", argv[0], connectionMode, result);
			showModesFlag = 1;
			showUsageFlag = 1;
			break;
		}
	}

    if (showModesFlag)
	{
		const CFETI_API_CONNECTION_INFO info = CFETIGetAPIConnectionInfo();
		unsigned int index;

		(void)fprintf(stderr, "%s: %d connection modes available:\n", argv[0], info->numConnectionModes);
		for (index=0; index < info->numConnectionModes; ++index)
		{
			(void)fprintf(stderr, "  %14s: %s\n", info->connectionModeNames[index], info->connectionModeDescs[index]);
		}
		(void)fprintf(stderr, "\n");

		if (!showUsageFlag) 
			return 0;
	}

	if (showUsageFlag || !primary)
	{
		(void)fprintf(stderr, "usage:\n\t%s -h | -el\n", argv[0]);
		(void)fprintf(stderr, "\t%s -p primary [-s secondary] [-u user -x password] [-f log] [-b] [-es connectionMode]\n", argv[0]);
		return 0;
	}

	Screen("eSpeed Electronic Trading Interface", useGraphics);
	Output("%s", CFETIVersion());
	Output("");
	Output("Primary %s, Secondary %s, Connection Mode: %s", 
		   primary, 
		   (secondary ? secondary : "(not set)"), 
		   (connectionMode? connectionMode: "(not set)"));
	SetLog(logfile);

	ShowHelp();

#if !defined(_WIN32)
	pthread_t tid;
	pthread_create(&tid, NULL, MessageLoop, NULL);
	signal(SIGINT, SigHandler);
#else
	{
		DWORD tid;
		HANDLE hThread = 0;
		hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MessageLoop, NULL, 0, &tid);	
	}
#endif

	CFETI_IDENTIFICATION_DESC Ident;
	::memset(&Ident, 0, sizeof(Ident));
	Ident.Company = const_cast<char*>("eSpeed");
	Ident.Application = const_cast<char*>("sample");
	Ident.Version = const_cast<char*>("1.0.0");

	int result = 0;
	result = CFETIOpenSession(primary, secondary, &Ident);
	switch(result)
	{
	case CFETI_SUCCESS:
		break;
	default:
		Output("CFETIOpenSession Failed : Error %d (%s)", result, CFETIGetLastError());
		break;
	}

	if (result == CFETI_SUCCESS && username && password)
	{
		int result = 0;
		result = CFETILogin(username, password, 0, SystemCallback, 0);
		switch(result)
		{
		case CFETI_SUCCESS:
			break;
		default:
			Output("CFETILogin Failed : Error %d (%s)", result, CFETIGetLastError());
			break;
		}
	}

	int carryOn = 1;
	while (carryOn)
	{
		char buf[256];
		Input(buf);
		carryOn = DoCommand(buf);
	}

	Shutdown();

	return 0;
}
