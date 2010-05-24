/*
+--------------------------------------------------------------------------+
| PROJECT : PROTOCOL STACK                                                 |
| FILE    : p_l2r.h                                                        |
| SOURCE  : "__out__\LNX_TI_cgt2_70\rel_n5_camera_micron\cdginc\condat\ms\doc\sap\l2r.pdf"                 |
| LastModified : "2001-10-31"                                              |
| IdAndVersion : "8411.102.01.124"                                         |
| SrcFileTime  : "Thu Nov 29 09:44:22 2007"                                |
| Generated by CCDGEN_2.5.5A on Wed Oct 15 12:22:32 2008                   |
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


#ifndef P_L2R_H
#define P_L2R_H


#define CDG_ENTER__P_L2R_H

#define CDG_ENTER__FILENAME _P_L2R_H
#define CDG_ENTER__P_L2R_H__FILE_TYPE CDGINC
#define CDG_ENTER__P_L2R_H__LAST_MODIFIED _2001_10_31
#define CDG_ENTER__P_L2R_H__ID_AND_VERSION _8411_102_01_124

#define CDG_ENTER__P_L2R_H__SRC_FILE_TIME _Thu_Nov_29_09_44_22_2007

#include "CDG_ENTER.h"

#undef CDG_ENTER__P_L2R_H

#undef CDG_ENTER__FILENAME


#include "p_l2r.val"


/*
 * End of substructure section, begin of primitive definition section
 */

#ifndef __T_L2R_ACTIVATE_REQ__
#define __T_L2R_ACTIVATE_REQ__
/*
 * 
 * CCDGEN:WriteStruct_Count==1708
 */
typedef struct
{
  U16                       k_ms_iwf;                 /*<  0:  2> window size ms -> iwf                              */
  U16                       k_iwf_ms;                 /*<  2:  2> window size iwf -> ms                              */
  U8                        t1;                       /*<  4:  1> acknowledge timer                                  */
  U8                        t2;                       /*<  5:  1> reply delay                                        */
  U8                        n2;                       /*<  6:  1> maximum number of retransmission attempts          */
  U8                        pt;                       /*<  7:  1> type of data compression                           */
  U8                        p0;                       /*<  8:  1> v.42bis data compression direction                 */
  U8                        _align0;                  /*<  9:  1> alignment                                          */
  U16                       p1;                       /*< 10:  2> v.42bis number of possible codewords               */
  U8                        p2;                       /*< 12:  1> v.42bis maximum encodable string length            */
  U8                        uil2p;                    /*< 13:  1> user information layer 2 protocol                  */
  U16                       bytes_per_prim;           /*< 14:  2> bytes per primitive                                */
  U16                       buffer_size;              /*< 16:  2> buffer size in bytes                               */
  U8                        rate;                     /*< 18:  1> transmission rate                                  */
  U8                        _align1;                  /*< 19:  1> alignment                                          */
} T_L2R_ACTIVATE_REQ;
#endif

#ifndef __T_L2R_ACTIVATE_CNF__
#define __T_L2R_ACTIVATE_CNF__
/*
 * 
 * CCDGEN:WriteStruct_Count==1709
 */
typedef struct
{
  U8                        ack_flg;                  /*<  0:  1> acknowledge flag                                   */
  U8                        _align0;                  /*<  1:  1> alignment                                          */
  U8                        _align1;                  /*<  2:  1> alignment                                          */
  U8                        _align2;                  /*<  3:  1> alignment                                          */
} T_L2R_ACTIVATE_CNF;
#endif

#ifndef __T_L2R_DEACTIVATE_REQ__
#define __T_L2R_DEACTIVATE_REQ__
/*
 * 
 * CCDGEN:WriteStruct_Count==1710
 */
typedef struct
{
  U8                        dummy;                    /*<  0:  1> no parameters                                      */
} T_L2R_DEACTIVATE_REQ;
#endif

#ifndef __T_L2R_DEACTIVATE_CNF__
#define __T_L2R_DEACTIVATE_CNF__
/*
 * 
 * CCDGEN:WriteStruct_Count==1711
 */
typedef struct
{
  U8                        dummy;                    /*<  0:  1> no parameters                                      */
} T_L2R_DEACTIVATE_CNF;
#endif

#ifndef __T_L2R_CONNECT_REQ__
#define __T_L2R_CONNECT_REQ__
/*
 * 
 * CCDGEN:WriteStruct_Count==1712
 */
typedef struct
{
  U8                        dummy;                    /*<  0:  1> no parameters                                      */
} T_L2R_CONNECT_REQ;
#endif

#ifndef __T_L2R_CONNECT_CNF__
#define __T_L2R_CONNECT_CNF__
/*
 * 
 * CCDGEN:WriteStruct_Count==1713
 */
typedef struct
{
  U8                        ack_flg;                  /*<  0:  1> acknowledge flag                                   */
  U8                        _align0;                  /*<  1:  1> alignment                                          */
  U8                        _align1;                  /*<  2:  1> alignment                                          */
  U8                        _align2;                  /*<  3:  1> alignment                                          */
} T_L2R_CONNECT_CNF;
#endif

#ifndef __T_L2R_CONNECT_IND__
#define __T_L2R_CONNECT_IND__
/*
 * 
 * CCDGEN:WriteStruct_Count==1714
 */
typedef struct
{
  U8                        dummy;                    /*<  0:  1> no parameters                                      */
} T_L2R_CONNECT_IND;
#endif

#ifndef __T_L2R_DISC_REQ__
#define __T_L2R_DISC_REQ__
/*
 * 
 * CCDGEN:WriteStruct_Count==1715
 */
typedef struct
{
  U8                        dummy;                    /*<  0:  1> no parameters                                      */
} T_L2R_DISC_REQ;
#endif

#ifndef __T_L2R_DISC_CNF__
#define __T_L2R_DISC_CNF__
/*
 * 
 * CCDGEN:WriteStruct_Count==1716
 */
typedef struct
{
  U8                        dummy;                    /*<  0:  1> no parameters                                      */
} T_L2R_DISC_CNF;
#endif

#ifndef __T_L2R_DISC_IND__
#define __T_L2R_DISC_IND__
/*
 * 
 * CCDGEN:WriteStruct_Count==1717
 */
typedef struct
{
  U8                        dummy;                    /*<  0:  1> no parameters                                      */
} T_L2R_DISC_IND;
#endif

#ifndef __T_L2R_XID_IND__
#define __T_L2R_XID_IND__
/*
 * 
 * CCDGEN:WriteStruct_Count==1718
 */
typedef struct
{
  U8                        rlp_vers;                 /*<  0:  1> rlp version number                                 */
  U8                        _align0;                  /*<  1:  1> alignment                                          */
  U16                       k_ms_iwf;                 /*<  2:  2> window size ms -> iwf                              */
  U16                       k_iwf_ms;                 /*<  4:  2> window size iwf -> ms                              */
  U8                        t1;                       /*<  6:  1> acknowledge timer                                  */
  U8                        t2;                       /*<  7:  1> reply delay                                        */
  U8                        n2;                       /*<  8:  1> maximum number of retransmission attempts          */
  U8                        pt;                       /*<  9:  1> type of data compression                           */
  U8                        p0;                       /*< 10:  1> v.42bis data compression direction                 */
  U8                        _align1;                  /*< 11:  1> alignment                                          */
  U16                       p1;                       /*< 12:  2> v.42bis number of possible codewords               */
  U8                        p2;                       /*< 14:  1> v.42bis maximum encodable string length            */
  U8                        _align2;                  /*< 15:  1> alignment                                          */
} T_L2R_XID_IND;
#endif

#ifndef __T_L2R_ERROR_IND__
#define __T_L2R_ERROR_IND__
/*
 * 
 * CCDGEN:WriteStruct_Count==1719
 */
typedef struct
{
  U16                       cause;                    /*<  0:  2> error cause                                        */
  U8                        _align0;                  /*<  2:  1> alignment                                          */
  U8                        _align1;                  /*<  3:  1> alignment                                          */
} T_L2R_ERROR_IND;
#endif

#ifndef __T_L2R_RESET_IND__
#define __T_L2R_RESET_IND__
/*
 * 
 * CCDGEN:WriteStruct_Count==1720
 */
typedef struct
{
  U8                        dummy;                    /*<  0:  1> no parameters                                      */
} T_L2R_RESET_IND;
#endif

#ifndef __T_L2R_STATISTIC_IND__
#define __T_L2R_STATISTIC_IND__
/*
 * 
 * CCDGEN:WriteStruct_Count==1721
 */
typedef struct
{
  U32                       error_rate;               /*<  0:  4> error_rate                                         */
} T_L2R_STATISTIC_IND;
#endif

#ifndef __T_L2R_DTI_REQ__
#define __T_L2R_DTI_REQ__
/*
 * 
 * CCDGEN:WriteStruct_Count==1722
 */
typedef struct
{
  U8                        dti_conn;                 /*<  0:  1> DTI connect                                        */
  U8                        entity_name[L2R_ENTITY_NAME_LEN]; /*<  1:  6> name of an neighbour entity                        */
  U8                        _align0;                  /*<  7:  1> alignment                                          */
  U32                       link_id;                  /*<  8:  4> Link identifier                                    */
  U8                        dti_direction;            /*< 12:  1> DTI direction                                      */
  U8                        _align1;                  /*< 13:  1> alignment                                          */
  U8                        _align2;                  /*< 14:  1> alignment                                          */
  U8                        _align3;                  /*< 15:  1> alignment                                          */
} T_L2R_DTI_REQ;
#endif

#ifndef __T_L2R_DTI_CNF__
#define __T_L2R_DTI_CNF__
/*
 * 
 * CCDGEN:WriteStruct_Count==1723
 */
typedef struct
{
  U8                        dti_conn;                 /*<  0:  1> DTI connect                                        */
  U8                        _align0;                  /*<  1:  1> alignment                                          */
  U8                        _align1;                  /*<  2:  1> alignment                                          */
  U8                        _align2;                  /*<  3:  1> alignment                                          */
  U32                       link_id;                  /*<  4:  4> Link identifier                                    */
} T_L2R_DTI_CNF;
#endif

#ifndef __T_L2R_DTI_IND__
#define __T_L2R_DTI_IND__
/*
 * 
 * CCDGEN:WriteStruct_Count==1724
 */
typedef struct
{
  U32                       link_id;                  /*<  0:  4> Link identifier                                    */
} T_L2R_DTI_IND;
#endif

#ifndef __T_L2R_CHANGE_RATE_REQ__
#define __T_L2R_CHANGE_RATE_REQ__
/*
 * 
 * CCDGEN:WriteStruct_Count==1725
 */
typedef struct
{
  U8                        rate;                     /*<  0:  1> transmission rate                                  */
  U8                        _align0;                  /*<  1:  1> alignment                                          */
  U8                        _align1;                  /*<  2:  1> alignment                                          */
  U8                        _align2;                  /*<  3:  1> alignment                                          */
} T_L2R_CHANGE_RATE_REQ;
#endif


#include "CDG_LEAVE.h"


#endif
