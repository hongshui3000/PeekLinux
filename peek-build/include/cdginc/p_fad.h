/*
+--------------------------------------------------------------------------+
| PROJECT : PROTOCOL STACK                                                 |
| FILE    : p_fad.h                                                        |
| SOURCE  : "__out__\LNX_TI_cgt2_70\rel_n5_camera_micron\cdginc\condat\ms\doc\sap\fad.pdf"                 |
| LastModified : "2002-04-22"                                              |
| IdAndVersion : "8411.103.00.305"                                         |
| SrcFileTime  : "Thu Nov 29 09:41:22 2007"                                |
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


#ifndef P_FAD_H
#define P_FAD_H


#define CDG_ENTER__P_FAD_H

#define CDG_ENTER__FILENAME _P_FAD_H
#define CDG_ENTER__P_FAD_H__FILE_TYPE CDGINC
#define CDG_ENTER__P_FAD_H__LAST_MODIFIED _2002_04_22
#define CDG_ENTER__P_FAD_H__ID_AND_VERSION _8411_103_00_305

#define CDG_ENTER__P_FAD_H__SRC_FILE_TIME _Thu_Nov_29_09_41_22_2007

#include "CDG_ENTER.h"

#undef CDG_ENTER__P_FAD_H

#undef CDG_ENTER__FILENAME


#include "p_fad.val"


/*
 * End of substructure section, begin of primitive definition section
 */

#ifndef __T_FAD_ACTIVATE_REQ__
#define __T_FAD_ACTIVATE_REQ__
/*
 * 
 * CCDGEN:WriteStruct_Count==1549
 */
typedef struct
{
  U16                       trans_rate;               /*<  0:  2> transmission rate                                  */
  U16                       frames_per_prim;          /*<  2:  2> no. of frames per primitive                        */
} T_FAD_ACTIVATE_REQ;
#endif

#ifndef __T_FAD_DEACTIVATE_REQ__
#define __T_FAD_DEACTIVATE_REQ__
/*
 * 
 * CCDGEN:WriteStruct_Count==1550
 */
typedef struct
{
  U8                        dummy;                    /*<  0:  1> no parameters                                      */
} T_FAD_DEACTIVATE_REQ;
#endif

#ifndef __T_FAD_MODIFY_REQ__
#define __T_FAD_MODIFY_REQ__
/*
 * 
 * CCDGEN:WriteStruct_Count==1551
 */
typedef struct
{
  U16                       trans_rate;               /*<  0:  2> transmission rate                                  */
  U8                        _align0;                  /*<  2:  1> alignment                                          */
  U8                        _align1;                  /*<  3:  1> alignment                                          */
} T_FAD_MODIFY_REQ;
#endif

#ifndef __T_FAD_DATA_REQ__
#define __T_FAD_DATA_REQ__
/*
 * 
 * CCDGEN:WriteStruct_Count==1552
 */
typedef struct
{
  U8                        final;                    /*<  0:  1> final flag                                         */
  U8                        _align0;                  /*<  1:  1> alignment                                          */
  U16                       trans_rate;               /*<  2:  2> transmission rate                                  */
  T_sdu                     sdu;                      /*<  4: ? > Service Data Unit                                  */
} T_FAD_DATA_REQ;
#endif

#ifndef __T_FAD_DATA_IND__
#define __T_FAD_DATA_IND__
/*
 * 
 * CCDGEN:WriteStruct_Count==1553
 */
typedef struct
{
  U8                        final;                    /*<  0:  1> final flag                                         */
  U8                        _align0;                  /*<  1:  1> alignment                                          */
  U16                       reserved;                 /*<  2:  2> Reserved                                           */
  T_sdu                     sdu;                      /*<  4: ? > Service Data Unit                                  */
} T_FAD_DATA_IND;
#endif

#ifndef __T_FAD_DATA_CNF__
#define __T_FAD_DATA_CNF__
/*
 * 
 * CCDGEN:WriteStruct_Count==1554
 */
typedef struct
{
  U8                        dummy;                    /*<  0:  1> no parameters                                      */
} T_FAD_DATA_CNF;
#endif

#ifndef __T_FAD_SND_TCF_REQ__
#define __T_FAD_SND_TCF_REQ__
/*
 * 
 * CCDGEN:WriteStruct_Count==1555
 */
typedef struct
{
  U16                       trans_rate;               /*<  0:  2> transmission rate                                  */
  U8                        _align0;                  /*<  2:  1> alignment                                          */
  U8                        _align1;                  /*<  3:  1> alignment                                          */
} T_FAD_SND_TCF_REQ;
#endif

#ifndef __T_FAD_SND_TCF_CNF__
#define __T_FAD_SND_TCF_CNF__
/*
 * 
 * CCDGEN:WriteStruct_Count==1556
 */
typedef struct
{
  U8                        dummy;                    /*<  0:  1> no parameters                                      */
} T_FAD_SND_TCF_CNF;
#endif

#ifndef __T_FAD_RCV_TCF_REQ__
#define __T_FAD_RCV_TCF_REQ__
/*
 * 
 * CCDGEN:WriteStruct_Count==1557
 */
typedef struct
{
  U16                       trans_rate;               /*<  0:  2> transmission rate                                  */
  U8                        _align0;                  /*<  2:  1> alignment                                          */
  U8                        _align1;                  /*<  3:  1> alignment                                          */
} T_FAD_RCV_TCF_REQ;
#endif

#ifndef __T_FAD_RCV_TCF_CNF__
#define __T_FAD_RCV_TCF_CNF__
/*
 * 
 * CCDGEN:WriteStruct_Count==1558
 */
typedef struct
{
  U8                        ratio;                    /*<  0:  1> percentage of correct TCF received                 */
  U8                        _align0;                  /*<  1:  1> alignment                                          */
  U8                        _align1;                  /*<  2:  1> alignment                                          */
  U8                        _align2;                  /*<  3:  1> alignment                                          */
} T_FAD_RCV_TCF_CNF;
#endif

#ifndef __T_FAD_READY_REQ__
#define __T_FAD_READY_REQ__
/*
 * 
 * CCDGEN:WriteStruct_Count==1559
 */
typedef struct
{
  U8                        dummy;                    /*<  0:  1> no parameters                                      */
} T_FAD_READY_REQ;
#endif

#ifndef __T_FAD_READY_IND__
#define __T_FAD_READY_IND__
/*
 * 
 * CCDGEN:WriteStruct_Count==1560
 */
typedef struct
{
  U8                        dummy;                    /*<  0:  1> no parameters                                      */
} T_FAD_READY_IND;
#endif

#ifndef __T_FAD_IGNORE_REQ__
#define __T_FAD_IGNORE_REQ__
/*
 * 
 * CCDGEN:WriteStruct_Count==1561
 */
typedef struct
{
  U8                        dummy;                    /*<  0:  1> no parameters                                      */
} T_FAD_IGNORE_REQ;
#endif

#ifndef __T_FAD_MUX_IND__
#define __T_FAD_MUX_IND__
/*
 * 
 * CCDGEN:WriteStruct_Count==1562
 */
typedef struct
{
  U8                        mode;                     /*<  0:  1> receive mode                                       */
  U8                        _align0;                  /*<  1:  1> alignment                                          */
  U8                        _align1;                  /*<  2:  1> alignment                                          */
  U8                        _align2;                  /*<  3:  1> alignment                                          */
} T_FAD_MUX_IND;
#endif

#ifndef __T_FAD_ERROR_IND__
#define __T_FAD_ERROR_IND__
/*
 * 
 * CCDGEN:WriteStruct_Count==1563
 */
typedef struct
{
  U16                       cause;                    /*<  0:  2> error cause                                        */
  U8                        _align0;                  /*<  2:  1> alignment                                          */
  U8                        _align1;                  /*<  3:  1> alignment                                          */
} T_FAD_ERROR_IND;
#endif

#ifndef __T_FAD_DEACTIVATE_CNF__
#define __T_FAD_DEACTIVATE_CNF__
/*
 * 
 * CCDGEN:WriteStruct_Count==1564
 */
typedef struct
{
  U8                        dummy;                    /*<  0:  1> no parameters                                      */
} T_FAD_DEACTIVATE_CNF;
#endif

#ifndef __T_FAD_ACTIVATE_CNF__
#define __T_FAD_ACTIVATE_CNF__
/*
 * 
 * CCDGEN:WriteStruct_Count==1565
 */
typedef struct
{
  U16                       buf_size_rx;              /*<  0:  2> buffer size RX                                     */
  U16                       buf_size_tx;              /*<  2:  2> buffer size TX                                     */
} T_FAD_ACTIVATE_CNF;
#endif


#include "CDG_LEAVE.h"


#endif
