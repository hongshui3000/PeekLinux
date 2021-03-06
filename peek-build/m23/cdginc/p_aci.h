/*
+--------------------------------------------------------------------------+
| PROJECT : PROTOCOL STACK                                                 |
| FILE    : p_aci.h                                                        |
| SOURCE  : "__out__\LNX_TI_cgt2_70\rel_n5_camera_micron\cdginc\condat\ms\doc\sap\aci.pdf"                 |
| LastModified : "2004-06-01"                                              |
| IdAndVersion : "8411.105.00.205"                                         |
| SrcFileTime  : "Thu Nov 29 09:29:50 2007"                                |
| Generated by CCDGEN_2.5.5A on Wed Mar 11 09:58:58 2009                   |
|           !!DO NOT MODIFY!!DO NOT MODIFY!!DO NOT MODIFY!!                |
+--------------------------------------------------------------------------+
*/

/* PRAGMAS
 * PREFIX                 : NONE
 * COMPATIBILITY_DEFINES  : NO (require PREFIX)
 * ALWAYS_ENUM_IN_VAL_FILE: NO
 * ENABLE_GROUP: NO
 * CAPITALIZE_TYPENAME: NO
 */


#ifndef P_ACI_H
#define P_ACI_H


#define CDG_ENTER__P_ACI_H

#define CDG_ENTER__FILENAME _P_ACI_H
#define CDG_ENTER__P_ACI_H__FILE_TYPE CDGINC
#define CDG_ENTER__P_ACI_H__LAST_MODIFIED _2004_06_01
#define CDG_ENTER__P_ACI_H__ID_AND_VERSION _8411_105_00_205

#define CDG_ENTER__P_ACI_H__SRC_FILE_TIME _Thu_Nov_29_09_29_50_2007

#include "CDG_ENTER.h"

#undef CDG_ENTER__P_ACI_H

#undef CDG_ENTER__FILENAME


#include "p_aci.val"


/*
 * End of substructure section, begin of primitive definition section
 */

#ifndef __T_ACI_CMD_REQ__
#define __T_ACI_CMD_REQ__
/*
 * 
 * CCDGEN:WriteStruct_Count==1154
 */
typedef struct
{
  U8                        cmd_src;                  /*<  0:  1> Command source                                     */
  U8                        _align0;                  /*<  1:  1> alignment                                          */
  U16                       cmd_len;                  /*<  2:  2> Length of command                                  */
  U8                        cmd_seq[MAX_CMD_LEN];     /*<  4:380> Command sequence                                   */
} T_ACI_CMD_REQ;
#endif

#ifndef __T_ACI_CMD_CNF__
#define __T_ACI_CMD_CNF__
/*
 * 
 * CCDGEN:WriteStruct_Count==1155
 */
typedef struct
{
  U16                       cmd_len;                  /*<  0:  2> Length of command                                  */
  U8                        cmd_seq[MAX_CMD_LEN];     /*<  2:380> Command sequence                                   */
  U8                        _align0;                  /*<382:  1> alignment                                          */
  U8                        _align1;                  /*<383:  1> alignment                                          */
} T_ACI_CMD_CNF;
#endif

#ifndef __T_ACI_CMD_IND__
#define __T_ACI_CMD_IND__
/*
 * 
 * CCDGEN:WriteStruct_Count==1156
 */
typedef struct
{
  U16                       cmd_len;                  /*<  0:  2> Length of command                                  */
  U8                        cmd_seq[MAX_CMD_LEN];     /*<  2:380> Command sequence                                   */
  U8                        _align0;                  /*<382:  1> alignment                                          */
  U8                        _align1;                  /*<383:  1> alignment                                          */
} T_ACI_CMD_IND;
#endif

#ifndef __T_ACI_CMD_RES__
#define __T_ACI_CMD_RES__
/*
 * 
 * CCDGEN:WriteStruct_Count==1157
 */
typedef struct
{
  U8                        dummy;                    /*<  0:  1> no parameters                                      */
} T_ACI_CMD_RES;
#endif

#ifndef __T_ACI_ABORT_REQ__
#define __T_ACI_ABORT_REQ__
/*
 * 
 * CCDGEN:WriteStruct_Count==1158
 */
typedef struct
{
  U8                        cmd_src;                  /*<  0:  1> Command source                                     */
  U8                        cause;                    /*<  1:  1> Cause                                              */
  U8                        _align0;                  /*<  2:  1> alignment                                          */
  U8                        _align1;                  /*<  3:  1> alignment                                          */
} T_ACI_ABORT_REQ;
#endif

#ifndef __T_ACI_URT_STAT_IND__
#define __T_ACI_URT_STAT_IND__
/*
 * 
 * CCDGEN:WriteStruct_Count==1159
 */
typedef struct
{
  U8                        urt_stat;                 /*<  0:  1> URT status                                         */
  U8                        _align0;                  /*<  1:  1> alignment                                          */
  U8                        _align1;                  /*<  2:  1> alignment                                          */
  U8                        _align2;                  /*<  3:  1> alignment                                          */
} T_ACI_URT_STAT_IND;
#endif

#ifndef __T_ACI_INIT_IND__
#define __T_ACI_INIT_IND__
/*
 * 
 * CCDGEN:WriteStruct_Count==1160
 */
typedef struct
{
  U8                        dummy;                    /*<  0:  1> no parameters                                      */
} T_ACI_INIT_IND;
#endif

#ifndef __T_ACI_INIT_RES__
#define __T_ACI_INIT_RES__
/*
 * 
 * CCDGEN:WriteStruct_Count==1161
 */
typedef struct
{
  U8                        ackflg;                   /*<  0:  1> Acknowledge flag                                   */
  U8                        _align0;                  /*<  1:  1> alignment                                          */
  U8                        _align1;                  /*<  2:  1> alignment                                          */
  U8                        _align2;                  /*<  3:  1> alignment                                          */
} T_ACI_INIT_RES;
#endif

#ifndef __T_ACI_DEINIT_REQ__
#define __T_ACI_DEINIT_REQ__
/*
 * 
 * CCDGEN:WriteStruct_Count==1162
 */
typedef struct
{
  U8                        dummy;                    /*<  0:  1> no parameters                                      */
} T_ACI_DEINIT_REQ;
#endif

#ifndef __T_ACI_DEINIT_CNF__
#define __T_ACI_DEINIT_CNF__
/*
 * 
 * CCDGEN:WriteStruct_Count==1163
 */
typedef struct
{
  U8                        dummy;                    /*<  0:  1> no parameters                                      */
} T_ACI_DEINIT_CNF;
#endif

#ifndef __T_ACI_OPEN_PORT_REQ__
#define __T_ACI_OPEN_PORT_REQ__
/*
 * 
 * CCDGEN:WriteStruct_Count==1164
 */
typedef struct
{
  U32                       port_nb;                  /*<  0:  4> port pumber                                        */
} T_ACI_OPEN_PORT_REQ;
#endif

#ifndef __T_ACI_OPEN_PORT_CNF__
#define __T_ACI_OPEN_PORT_CNF__
/*
 * 
 * CCDGEN:WriteStruct_Count==1165
 */
typedef struct
{
  U32                       port_nb;                  /*<  0:  4> port pumber                                        */
  U8                        ackflg;                   /*<  4:  1> Acknowledge flag                                   */
  U8                        _align0;                  /*<  5:  1> alignment                                          */
  U8                        _align1;                  /*<  6:  1> alignment                                          */
  U8                        _align2;                  /*<  7:  1> alignment                                          */
} T_ACI_OPEN_PORT_CNF;
#endif

#ifndef __T_ACI_CLOSE_PORT_REQ__
#define __T_ACI_CLOSE_PORT_REQ__
/*
 * 
 * CCDGEN:WriteStruct_Count==1166
 */
typedef struct
{
  U32                       port_nb;                  /*<  0:  4> port pumber                                        */
} T_ACI_CLOSE_PORT_REQ;
#endif

#ifndef __T_ACI_CLOSE_PORT_CNF__
#define __T_ACI_CLOSE_PORT_CNF__
/*
 * 
 * CCDGEN:WriteStruct_Count==1167
 */
typedef struct
{
  U32                       port_nb;                  /*<  0:  4> port pumber                                        */
} T_ACI_CLOSE_PORT_CNF;
#endif

#ifndef __T_ACI_ABORT_CNF__
#define __T_ACI_ABORT_CNF__
/*
 * 
 * CCDGEN:WriteStruct_Count==1168
 */
typedef struct
{
  U16                       cmd_len;                  /*<  0:  2> Length of command                                  */
  U8                        cmd_seq[MAX_CMD_LEN];     /*<  2:380> Command sequence                                   */
  U8                        _align0;                  /*<382:  1> alignment                                          */
  U8                        _align1;                  /*<383:  1> alignment                                          */
} T_ACI_ABORT_CNF;
#endif

#ifndef __T_ACI_CMD_REQ_BT__
#define __T_ACI_CMD_REQ_BT__
/*
 * 
 * CCDGEN:WriteStruct_Count==1169
 */
typedef struct
{
  U8                        cmd_src;                  /*<  0:  1> Command source                                     */
  U8                        _align0;                  /*<  1:  1> alignment                                          */
  U16                       cmd_len;                  /*<  2:  2> Length of command                                  */
  U8                        cmd_seq[MAX_CMD_LEN];     /*<  4:380> Command sequence                                   */
  U8                        src_id;                   /*<384:  1> ACI source ID                                      */
  U8                        _align1;                  /*<385:  1> alignment                                          */
  U8                        _align2;                  /*<386:  1> alignment                                          */
  U8                        _align3;                  /*<387:  1> alignment                                          */
} T_ACI_CMD_REQ_BT;
#endif

#ifndef __T_ACI_CMD_CNF_BT__
#define __T_ACI_CMD_CNF_BT__
/*
 * 
 * CCDGEN:WriteStruct_Count==1170
 */
typedef struct
{
  U16                       cmd_len;                  /*<  0:  2> Length of command                                  */
  U8                        cmd_seq[MAX_CMD_LEN];     /*<  2:380> Command sequence                                   */
  U8                        src_id;                   /*<382:  1> ACI source ID                                      */
  U8                        _align0;                  /*<383:  1> alignment                                          */
} T_ACI_CMD_CNF_BT;
#endif

#ifndef __T_ACI_CMD_IND_BT__
#define __T_ACI_CMD_IND_BT__
/*
 * 
 * CCDGEN:WriteStruct_Count==1171
 */
typedef struct
{
  U16                       cmd_len;                  /*<  0:  2> Length of command                                  */
  U8                        cmd_seq[MAX_CMD_LEN];     /*<  2:380> Command sequence                                   */
  U8                        src_id;                   /*<382:  1> ACI source ID                                      */
  U8                        _align0;                  /*<383:  1> alignment                                          */
} T_ACI_CMD_IND_BT;
#endif

#ifndef __T_ACI_CMD_RES_BT__
#define __T_ACI_CMD_RES_BT__
/*
 * 
 * CCDGEN:WriteStruct_Count==1172
 */
typedef struct
{
  U8                        src_id;                   /*<  0:  1> ACI source ID                                      */
  U8                        _align0;                  /*<  1:  1> alignment                                          */
  U8                        _align1;                  /*<  2:  1> alignment                                          */
  U8                        _align2;                  /*<  3:  1> alignment                                          */
} T_ACI_CMD_RES_BT;
#endif

#ifndef __T_ACI_ABORT_REQ_BT__
#define __T_ACI_ABORT_REQ_BT__
/*
 * 
 * CCDGEN:WriteStruct_Count==1173
 */
typedef struct
{
  U8                        cmd_src;                  /*<  0:  1> Command source                                     */
  U8                        cause;                    /*<  1:  1> Cause                                              */
  U8                        src_id;                   /*<  2:  1> ACI source ID                                      */
  U8                        _align0;                  /*<  3:  1> alignment                                          */
} T_ACI_ABORT_REQ_BT;
#endif

#ifndef __T_ACI_ABORT_CNF_BT__
#define __T_ACI_ABORT_CNF_BT__
/*
 * 
 * CCDGEN:WriteStruct_Count==1174
 */
typedef struct
{
  U8                        src_id;                   /*<  0:  1> ACI source ID                                      */
  U8                        _align0;                  /*<  1:  1> alignment                                          */
  U8                        _align1;                  /*<  2:  1> alignment                                          */
  U8                        _align2;                  /*<  3:  1> alignment                                          */
} T_ACI_ABORT_CNF_BT;
#endif

#ifndef __T_ACI_TRC_IND__
#define __T_ACI_TRC_IND__
/*
 * 
 * CCDGEN:WriteStruct_Count==1175
 */
typedef struct
{
  U8                        cmd_src;                  /*<  0:  1> Command source                                     */
  U8                        _align0;                  /*<  1:  1> alignment                                          */
  U16                       trc_len;                  /*<  2:  2> Length of trace                                    */
  U8                        trc_buf[MAX_TRC_LEN];     /*<  4: 80> Trace buffer                                       */
} T_ACI_TRC_IND;
#endif

#ifndef __T_ACI_RIV_CMD_REQ__
#define __T_ACI_RIV_CMD_REQ__
/*
 * 
 * CCDGEN:WriteStruct_Count==1176
 */
typedef struct
{
  U32                       cmd_ptr;                  /*<  0:  4> Command index                                      */
} T_ACI_RIV_CMD_REQ;
#endif

#ifndef __T_ACI_EXT_IND__
#define __T_ACI_EXT_IND__
/*
 * 
 * CCDGEN:WriteStruct_Count==1177
 */
typedef struct
{
  U16                       signal_id;                /*<  0:  2> Signal Id                                          */
  U8                        _align0;                  /*<  2:  1> alignment                                          */
  U8                        _align1;                  /*<  3:  1> alignment                                          */
  U32                       data[MAX_DATA_LEN];       /*<  4:800> Signal data                                        */
} T_ACI_EXT_IND;
#endif


#include "CDG_LEAVE.h"


#endif
