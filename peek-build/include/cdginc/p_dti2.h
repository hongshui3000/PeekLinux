/*
+--------------------------------------------------------------------------+
| PROJECT : PROTOCOL STACK                                                 |
| FILE    : p_dti2.h                                                       |
| SOURCE  : "__out__\LNX_TI_cgt2_70\rel_n5_camera_micron\cdginc\condat\ms\doc\sap\dti2.pdf"                  |
| LastModified : "2001-09-19"                                              |
| IdAndVersion : "8411.111.01.002"                                         |
| SrcFileTime  : "Thu Nov 29 09:40:38 2007"                                |
| Generated by CCDGEN_2.5.5A on Wed Oct 15 12:22:27 2008                   |
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


#ifndef P_DTI2_H
#define P_DTI2_H


#define CDG_ENTER__P_DTI2_H

#define CDG_ENTER__FILENAME _P_DTI2_H
#define CDG_ENTER__P_DTI2_H__FILE_TYPE CDGINC
#define CDG_ENTER__P_DTI2_H__LAST_MODIFIED _2001_09_19
#define CDG_ENTER__P_DTI2_H__ID_AND_VERSION _8411_111_01_002

#define CDG_ENTER__P_DTI2_H__SRC_FILE_TIME _Thu_Nov_29_09_40_38_2007

#include "CDG_ENTER.h"

#undef CDG_ENTER__P_DTI2_H

#undef CDG_ENTER__FILENAME


#include "p_dti2.val"

#ifndef __T_desc_list2__
#define __T_desc_list2__
/*
 * List of generic data descriptors
 * CCDGEN:WriteStruct_Count==1461
 */
typedef struct
{
  U16                       list_len;                 /*<  0:  2> length in octets of whole data                     */
  U8                        _align0;                  /*<  2:  1> alignment                                          */
  U8                        _align1;                  /*<  3:  1> alignment                                          */
  U32                       first;                    /*<  4:  4> pointer to first generic data descriptor           */
} T_desc_list2;
#endif

#ifndef __T_desc2__
#define __T_desc2__
/*
 * generic data descriptor
 * CCDGEN:WriteStruct_Count==1462
 */
typedef struct
{
  U32                       next;                     /*<  0:  4> next generic data descriptor                       */
  U16                       offset;                   /*<  4:  2> offset in octets                                   */
  U16                       len;                      /*<  6:  2> length of content in octets                        */
  U16                       size;                     /*<  8:  2> size of buffer in octets                           */
  U8                        buffer[1];                /*< 10:  1> buffer content                                     */
  U8                        _align0;                  /*< 11:  1> alignment                                          */
} T_desc2;
#endif

#ifndef __T_st_lines__
#define __T_st_lines__
/*
 * Serial line states
 * CCDGEN:WriteStruct_Count==1464
 */
typedef struct
{
  U8                        st_flow;                  /*<  0:  1> flow control state                                 */
  U8                        st_line_sa;               /*<  1:  1> line state sa                                      */
  U8                        st_line_sb;               /*<  2:  1> line state sb                                      */
  U8                        _align0;                  /*<  3:  1> alignment                                          */
  U16                       st_break_len;             /*<  4:  2> break length                                       */
  U8                        _align1;                  /*<  6:  1> alignment                                          */
  U8                        _align2;                  /*<  7:  1> alignment                                          */
} T_st_lines;
#endif

#ifndef __T_parameters__
#define __T_parameters__
/*
 * Additional parameters
 * CCDGEN:WriteStruct_Count==1465
 */
typedef struct
{
  U8                        p_id;                     /*<  0:  1> protocol identifier                                */
  U8                        _align0;                  /*<  1:  1> alignment                                          */
  U8                        _align1;                  /*<  2:  1> alignment                                          */
  U8                        _align2;                  /*<  3:  1> alignment                                          */
  T_st_lines                st_lines;                 /*<  4:  8> Serial line states                                 */
} T_parameters;
#endif


/*
 * End of substructure section, begin of primitive definition section
 */

#ifndef __T_DTI2_CONNECT_REQ__
#define __T_DTI2_CONNECT_REQ__
/*
 * 
 * CCDGEN:WriteStruct_Count==1468
 */
typedef struct
{
  U32                       link_id;                  /*<  0:  4> link identifier                                    */
  U32                       version;                  /*<  4:  4> Version of DTI                                     */
} T_DTI2_CONNECT_REQ;
#endif

#ifndef __T_DTI2_CONNECT_IND__
#define __T_DTI2_CONNECT_IND__
/*
 * 
 * CCDGEN:WriteStruct_Count==1469
 */
typedef struct
{
  U32                       link_id;                  /*<  0:  4> link identifier                                    */
  U32                       version;                  /*<  4:  4> Version of DTI                                     */
} T_DTI2_CONNECT_IND;
#endif

#ifndef __T_DTI2_CONNECT_CNF__
#define __T_DTI2_CONNECT_CNF__
/*
 * 
 * CCDGEN:WriteStruct_Count==1470
 */
typedef struct
{
  U32                       link_id;                  /*<  0:  4> link identifier                                    */
  U32                       version;                  /*<  4:  4> Version of DTI                                     */
} T_DTI2_CONNECT_CNF;
#endif

#ifndef __T_DTI2_CONNECT_RES__
#define __T_DTI2_CONNECT_RES__
/*
 * 
 * CCDGEN:WriteStruct_Count==1471
 */
typedef struct
{
  U32                       link_id;                  /*<  0:  4> link identifier                                    */
  U32                       version;                  /*<  4:  4> Version of DTI                                     */
} T_DTI2_CONNECT_RES;
#endif

#ifndef __T_DTI2_DISCONNECT_REQ__
#define __T_DTI2_DISCONNECT_REQ__
/*
 * 
 * CCDGEN:WriteStruct_Count==1472
 */
typedef struct
{
  U32                       link_id;                  /*<  0:  4> link identifier                                    */
  U8                        cause;                    /*<  4:  1> cause of disconnection                             */
  U8                        _align0;                  /*<  5:  1> alignment                                          */
  U8                        _align1;                  /*<  6:  1> alignment                                          */
  U8                        _align2;                  /*<  7:  1> alignment                                          */
} T_DTI2_DISCONNECT_REQ;
#endif

#ifndef __T_DTI2_DISCONNECT_IND__
#define __T_DTI2_DISCONNECT_IND__
/*
 * 
 * CCDGEN:WriteStruct_Count==1473
 */
typedef struct
{
  U32                       link_id;                  /*<  0:  4> link identifier                                    */
  U8                        cause;                    /*<  4:  1> cause of disconnection                             */
  U8                        _align0;                  /*<  5:  1> alignment                                          */
  U8                        _align1;                  /*<  6:  1> alignment                                          */
  U8                        _align2;                  /*<  7:  1> alignment                                          */
} T_DTI2_DISCONNECT_IND;
#endif

#ifndef __T_DTI2_GETDATA_REQ__
#define __T_DTI2_GETDATA_REQ__
/*
 * 
 * CCDGEN:WriteStruct_Count==1474
 */
typedef struct
{
  U32                       link_id;                  /*<  0:  4> link identifier                                    */
} T_DTI2_GETDATA_REQ;
#endif

#ifndef __T_DTI2_READY_IND__
#define __T_DTI2_READY_IND__
/*
 * 
 * CCDGEN:WriteStruct_Count==1475
 */
typedef struct
{
  U32                       link_id;                  /*<  0:  4> link identifier                                    */
} T_DTI2_READY_IND;
#endif

#ifndef __T_DTI2_DATA_IND__
#define __T_DTI2_DATA_IND__
/*
 * 
 * CCDGEN:WriteStruct_Count==1476
 */
typedef struct
{
  U32                       link_id;                  /*<  0:  4> link identifier                                    */
  T_parameters              parameters;               /*<  4: 12> Additional parameters                              */
  T_desc_list2              desc_list2;               /*< 16:  8> List of generic data descriptors                   */
} T_DTI2_DATA_IND;
#endif

#ifndef __T_DTI2_DATA_REQ__
#define __T_DTI2_DATA_REQ__
/*
 * 
 * CCDGEN:WriteStruct_Count==1477
 */
typedef struct
{
  U32                       link_id;                  /*<  0:  4> link identifier                                    */
  T_parameters              parameters;               /*<  4: 12> Additional parameters                              */
  T_desc_list2              desc_list2;               /*< 16:  8> List of generic data descriptors                   */
} T_DTI2_DATA_REQ;
#endif

#ifndef __T_DTI2_DATA_TEST_IND__
#define __T_DTI2_DATA_TEST_IND__
/*
 * 
 * CCDGEN:WriteStruct_Count==1478
 */
typedef struct
{
  U32                       link_id;                  /*<  0:  4> link identifier                                    */
  T_parameters              parameters;               /*<  4: 12> Additional parameters                              */
  T_sdu                     sdu;                      /*< 16: ? > test data                                          */
} T_DTI2_DATA_TEST_IND;
#endif

#ifndef __T_DTI2_DATA_TEST_REQ__
#define __T_DTI2_DATA_TEST_REQ__
/*
 * 
 * CCDGEN:WriteStruct_Count==1479
 */
typedef struct
{
  U32                       link_id;                  /*<  0:  4> link identifier                                    */
  T_parameters              parameters;               /*<  4: 12> Additional parameters                              */
  T_sdu                     sdu;                      /*< 16: ? > test data                                          */
} T_DTI2_DATA_TEST_REQ;
#endif

#ifndef __T_DTI2_DUMMY_REQ__
#define __T_DTI2_DUMMY_REQ__
/*
 * 
 * CCDGEN:WriteStruct_Count==1480
 */
typedef struct
{
  T_desc2                   desc2;                    /*<  0: 12> generic data descriptor                            */
} T_DTI2_DUMMY_REQ;
#endif


#include "CDG_LEAVE.h"


#endif
