/*******************************************************************************\
##                                                                           	*
##  TEXAS INSTRUMENTS ISRAEL PROPRIETARY AND CONFIDENTIAL INFORMATION			*
##																				*
##  LEGAL NOTE: THE TRANSFER OF THE TECHNICAL INFORMATION IS BEING MADE			*
##	UNDER AN EXPORT LICENSE ISSUED BY THE ISRAELI GOVERNMENT AND THAT THE		*
##	APPLICABLE EXPORT LICENSE DOES NOT ALLOW THE TECHNICAL INFORMATION TO		*
##	BE USED FOR THE MODIFICATION OF THE BT ENCRYPTION OR THE DEVELOPMENT		*
##	OF ANY NEW ENCRYPTION. THE INFORMATION CAN BE USED FOR THE INTERNAL			*
##	DESIGN AND MANUFACTURE OF TI PRODUCTS THAT WILL CONTAIN THE BT IC.			*
##																				*
\*******************************************************************************/
/*******************************************************************************\
*
*   FILE NAME:      lineparser.h
*
*   BRIEF:          This file defines the API of the line parser.
*
*   AUTHOR:         Udi Ron
*
\*******************************************************************************/
#ifndef __LINE_PARSER_H_
#define __LINE_PARSER_H_

#include "bthal_types.h"
#include "btl_commoni.h"

#define LINE_PARSER_MAX_NUM_OF_ARGUMENTS	10
#define LINE_PARSER_MAX_LINE_LEN			200
#define LINE_PARSER_MAX_MODULE_NAME_LEN		10
#define LINE_PARSER_MAX_STR_LEN			    80

#define LINE_PARSER_MODULE_NAME_BMG		    "BMG"	
#define LINE_PARSER_MODULE_NAME_SPP		    "SPP"
#define LINE_PARSER_MODULE_NAME_OPPC		"OPPC"
#define LINE_PARSER_MODULE_NAME_OPPS		"OPPS"	
#define LINE_PARSER_MODULE_NAME_BPPSND		"BPPSND"
#define LINE_PARSER_MODULE_NAME_PBAPS		"PBAPS"
#define LINE_PARSER_MODULE_NAME_PAN		    "PAN"
#define LINE_PARSER_MODULE_NAME_AVRCPTG		"AVRCPTG"
#define LINE_PARSER_MODULE_NAME_FTPS		"FTPS"
#define LINE_PARSER_MODULE_NAME_FTPC		"FTPC"
#define LINE_PARSER_MODULE_NAME_VG		    "VG"
#define LINE_PARSER_MODULE_NAME_AG		    "AG"
#define LINE_PARSER_MODULE_NAME_RFCOMM		"RFCOMM"
#define LINE_PARSER_MODULE_NAME_A2DP		"A2DP"		
#define LINE_PARSER_MODULE_NAME_HID		    "HID"
#define LINE_PARSER_MODULE_NAME_MDG		    "MDG"		
#define LINE_PARSER_MODULE_NAME_BIPINT		"BIPINT"
#define LINE_PARSER_MODULE_NAME_BIPRSP		"BIPRSP"
#define LINE_PARSER_MODULE_NAME_SAPS		"SAPS"
#define LINE_PARSER_MODULE_NAME_MAIN		"MAIN"
#define LINE_PARSER_MODULE_NAME_BSC		    "BSC"
#define LINE_PARSER_MODULE_NAME_L2CAP		"L2CAP"
#define LINE_PARSER_MODULE_NAME_L2CAP_THROUGHPUT	"L2CAP_TP"
#define LINE_PARSER_MODULE_NAME_FMRADIO		"FMRADIO"



typedef enum
{
	LINE_PARSER_STATUS_SUCCESS,
	LINE_PARSER_STATUS_FAILED,
	LINE_PARSER_STATUS_ARGUMENT_TOO_LONG,
	LINE_PARSER_STATUS_NO_MORE_ARGUMENTS
} LINE_PARSER_STATUS;

LINE_PARSER_STATUS LINE_PARSER_ParseLine(U8 *line, const char* delimiters);
U32 LINE_PARSER_GetNumOfArgs(void);
BOOL LINE_PARSER_AreThereMoreArgs(void);

void LINE_PARSER_ToLower(U8 *str);

LINE_PARSER_STATUS LINE_PARSER_GetNextModuleType(BtlModuleType *moduleType);
LINE_PARSER_STATUS LINE_PARSER_GetNextChar(U8 *c);
LINE_PARSER_STATUS LINE_PARSER_GetNextStr(U8 *str, U8 len);
LINE_PARSER_STATUS LINE_PARSER_GetNextU8(U8 *value, BOOL hex);
LINE_PARSER_STATUS LINE_PARSER_GetNextU16(U16 *value, BOOL hex);
LINE_PARSER_STATUS LINE_PARSER_GetNextU32(U32 *value, BOOL hex);
LINE_PARSER_STATUS LINE_PARSER_GetNextS8(S8 *value);
LINE_PARSER_STATUS LINE_PARSER_GetNextS16(S16 *value);
LINE_PARSER_STATUS LINE_PARSER_GetNextS32(S32 *value);
LINE_PARSER_STATUS LINE_PARSER_GetNextBool(BOOL *value);

#endif	/* __LINE_PARSER_H_ */

