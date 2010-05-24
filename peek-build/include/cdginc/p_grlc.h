/*
+--------------------------------------------------------------------------+
| PROJECT : PROTOCOL STACK                                                 |
| FILE    : p_grlc.h                                                       |
| SOURCE  : "__out__\LNX_TI_cgt2_70\rel_n5_camera_micron\cdginc\condat\ms\doc\sap\grlc.pdf"                  |
| LastModified : "2003-06-06"                                              |
| IdAndVersion : "8010.126.02.009"                                         |
| SrcFileTime  : "Thu Nov 29 09:42:56 2007"                                |
| Generated by CCDGEN_2.5.5A on Wed Oct 15 12:22:27 2008                   |
|           !!DO NOT MODIFY!!DO NOT MODIFY!!DO NOT MODIFY!!                |
+--------------------------------------------------------------------------+
*/

/* PRAGMAS
 * PREFIX                 : GRLC
 * COMPATIBILITY_DEFINES  : NO
 * ALWAYS_ENUM_IN_VAL_FILE: NO
 * ENABLE_GROUP: NO
 * CAPITALIZE_TYPENAME: NO
 */


#ifndef P_GRLC_H
#define P_GRLC_H


#define CDG_ENTER__P_GRLC_H

#define CDG_ENTER__FILENAME _P_GRLC_H
#define CDG_ENTER__P_GRLC_H__FILE_TYPE CDGINC
#define CDG_ENTER__P_GRLC_H__LAST_MODIFIED _2003_06_06
#define CDG_ENTER__P_GRLC_H__ID_AND_VERSION _8010_126_02_009

#define CDG_ENTER__P_GRLC_H__SRC_FILE_TIME _Thu_Nov_29_09_42_56_2007

#include "CDG_ENTER.h"

#undef CDG_ENTER__P_GRLC_H

#undef CDG_ENTER__FILENAME


#include "p_grlc.val"

#ifndef __T_GRLC_grlc_qos__
#define __T_GRLC_grlc_qos__
/*
 * Quality of service
 * CCDGEN:WriteStruct_Count==1616
 */
typedef struct
{
  U8                        delay;                    /*<  0:  1> Delay class                                        */
  U8                        relclass;                 /*<  1:  1> Reliablility class                                 */
  U8                        peak;                     /*<  2:  1> Peak throughput                                    */
  U8                        preced;                   /*<  3:  1> Precedence class                                   */
  U8                        mean;                     /*<  4:  1> Main throughput                                    */
  U8                        reserved_1;               /*<  5:  1> Reserved                                           */
  U8                        reserved_2;               /*<  6:  1> Reserved                                           */
  U8                        reserved_3;               /*<  7:  1> Reserved                                           */
} T_GRLC_grlc_qos;
#endif


/*
 * End of substructure section, begin of primitive definition section
 */

#ifndef __T_GRLC_DATA_REQ__
#define __T_GRLC_DATA_REQ__
/*
 * 
 * CCDGEN:WriteStruct_Count==1624
 */
typedef struct
{
  U8                        sapi;                     /*<  0:  1> service access point identifier                    */
  U8                        _align0;                  /*<  1:  1> alignment                                          */
  U8                        _align1;                  /*<  2:  1> alignment                                          */
  U8                        _align2;                  /*<  3:  1> alignment                                          */
  U32                       tlli;                     /*<  4:  4> temporary logical link identifier                  */
  T_GRLC_grlc_qos           grlc_qos;                 /*<  8:  8> Quality of service                                 */
  U8                        radio_prio;               /*< 16:  1> Radio Priority                                     */
  U8                        cause;                    /*< 17:  1> Cause                                              */
// ELEM-FF: !REL99 
//   U8                        reserved_data_req[16];    /*<  0:  0> reserved octets for data request                   */
// ELEM-FF: REL99 
  U8                        pkt_flow_id[16];          /*< 18: 16> Packet flow identifier. Only first byte of the array is used for the packet flow identifier. */
  U8                        _align3;                  /*< 34:  1> alignment                                          */
  U8                        _align4;                  /*< 35:  1> alignment                                          */
  T_sdu                     sdu;                      /*< 36: ? > service data unit                                  */
} T_GRLC_DATA_REQ;
#endif

#ifndef __T_GRLC_DATA_IND__
#define __T_GRLC_DATA_IND__
/*
 * 
 * CCDGEN:WriteStruct_Count==1625
 */
typedef struct
{
  U32                       tlli;                     /*<  0:  4> temporary logical link identifier                  */
  T_desc_list               desc_list;                /*<  4: ? > list of generic data descriptors                   */
} T_GRLC_DATA_IND;
#endif

#ifndef __T_GRLC_DATA_IND_TEST__
#define __T_GRLC_DATA_IND_TEST__
/*
 * 
 * CCDGEN:WriteStruct_Count==1626
 */
typedef struct
{
  U32                       tlli;                     /*<  0:  4> temporary logical link identifier                  */
  T_sdu                     sdu;                      /*<  4: ? > service data unit                                  */
} T_GRLC_DATA_IND_TEST;
#endif

#ifndef __T_GRLC_UNITDATA_REQ__
#define __T_GRLC_UNITDATA_REQ__
/*
 * 
 * CCDGEN:WriteStruct_Count==1627
 */
typedef struct
{
  U8                        sapi;                     /*<  0:  1> service access point identifier                    */
  U8                        _align0;                  /*<  1:  1> alignment                                          */
  U8                        _align1;                  /*<  2:  1> alignment                                          */
  U8                        _align2;                  /*<  3:  1> alignment                                          */
  U32                       tlli;                     /*<  4:  4> temporary logical link identifier                  */
  T_GRLC_grlc_qos           grlc_qos;                 /*<  8:  8> Quality of service                                 */
  U8                        radio_prio;               /*< 16:  1> Radio Priority                                     */
  U8                        reserved_unitdata_req1;   /*< 17:  1> reserved octet                                     */
// ELEM-FF: !REL99 
//   U8                        reserved_unitdata_req2[16]; /*<  0:  0> reserved octets for unitdata request               */
// ELEM-FF: REL99 
  U8                        pkt_flow_id[16];          /*< 18: 16> Packet flow identifier. Only first byte of the array is used for the packet flow identifier. */
  U8                        _align3;                  /*< 34:  1> alignment                                          */
  U8                        _align4;                  /*< 35:  1> alignment                                          */
  T_sdu                     sdu;                      /*< 36: ? > service data unit                                  */
} T_GRLC_UNITDATA_REQ;
#endif

#ifndef __T_GRLC_UNITDATA_IND__
#define __T_GRLC_UNITDATA_IND__
/*
 * 
 * CCDGEN:WriteStruct_Count==1628
 */
typedef struct
{
  U32                       tlli;                     /*<  0:  4> temporary logical link identifier                  */
  T_desc_list               desc_list;                /*<  4: ? > list of generic data descriptors                   */
} T_GRLC_UNITDATA_IND;
#endif

#ifndef __T_GRLC_UNITDATA_IND_TEST__
#define __T_GRLC_UNITDATA_IND_TEST__
/*
 * 
 * CCDGEN:WriteStruct_Count==1629
 */
typedef struct
{
  U32                       tlli;                     /*<  0:  4> temporary logical link identifier                  */
  T_sdu                     sdu;                      /*<  4: ? > service data unit                                  */
} T_GRLC_UNITDATA_IND_TEST;
#endif

#ifndef __T_GRLC_READY_IND__
#define __T_GRLC_READY_IND__
/*
 * 
 * CCDGEN:WriteStruct_Count==1630
 */
typedef struct
{
  U8                        dummy;                    /*<  0:  1> no parameters                                      */
} T_GRLC_READY_IND;
#endif

#ifndef __T_GRLC_ACTIVATE_GMM_QUEUE_REQ__
#define __T_GRLC_ACTIVATE_GMM_QUEUE_REQ__
/*
 * 
 * CCDGEN:WriteStruct_Count==1631
 */
typedef struct
{
  U8                        dummy;                    /*<  0:  1> no parameters                                      */
} T_GRLC_ACTIVATE_GMM_QUEUE_REQ;
#endif

#ifndef __T_GRLC_SUSPEND_READY_IND__
#define __T_GRLC_SUSPEND_READY_IND__
/*
 * 
 * CCDGEN:WriteStruct_Count==1632
 */
typedef struct
{
  U8                        dummy;                    /*<  0:  1> no parameters                                      */
} T_GRLC_SUSPEND_READY_IND;
#endif

#ifndef __T_GRLC_MAC_DATA_IND__
#define __T_GRLC_MAC_DATA_IND__
/*
 * 
 * CCDGEN:WriteStruct_Count==1633
 */
typedef struct
{
  U8                        dummy;                    /*<  0:  1> no parameters                                      */
} T_GRLC_MAC_DATA_IND;
#endif

#ifndef __T_GRLC_MAC_READY_IND__
#define __T_GRLC_MAC_READY_IND__
/*
 * 
 * CCDGEN:WriteStruct_Count==1634
 */
typedef struct
{
  U8                        dummy;                    /*<  0:  1> no parameters                                      */
} T_GRLC_MAC_READY_IND;
#endif

#ifndef __T_GRLC_MAC_PWR_CTRL_IND__
#define __T_GRLC_MAC_PWR_CTRL_IND__
/*
 * 
 * CCDGEN:WriteStruct_Count==1635
 */
typedef struct
{
  U8                        dummy;                    /*<  0:  1> no parameters                                      */
} T_GRLC_MAC_PWR_CTRL_IND;
#endif

#ifndef __T_GRLC_FLUSH_DATA_REQ__
#define __T_GRLC_FLUSH_DATA_REQ__
/*
 * 
 * CCDGEN:WriteStruct_Count==1636
 */
typedef struct
{
  U8                        dummy;                    /*<  0:  1> no parameters                                      */
} T_GRLC_FLUSH_DATA_REQ;
#endif


#include "CDG_LEAVE.h"


#endif
