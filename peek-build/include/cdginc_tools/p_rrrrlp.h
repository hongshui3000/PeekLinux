/*
+--------------------------------------------------------------------------+
| PROJECT : PROTOCOL STACK                                                 |
| FILE    : p_rrrrlp.h                                                     |
| SOURCE  : "__out__\LNX_TI_cgt2_70\rel_n5_camera_micron\cdginc\condat\ms\doc\sap\rrrrlp.pdf"                    |
| LastModified : "2002-09-10"                                              |
| IdAndVersion : "0000.000.00.002"                                         |
| SrcFileTime  : "Thu Nov 29 09:53:22 2007"                                |
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


#ifndef P_RRRRLP_H
#define P_RRRRLP_H


#define CDG_ENTER__P_RRRRLP_H

#define CDG_ENTER__FILENAME _P_RRRRLP_H
#define CDG_ENTER__P_RRRRLP_H__FILE_TYPE CDGINC
#define CDG_ENTER__P_RRRRLP_H__LAST_MODIFIED _2002_09_10
#define CDG_ENTER__P_RRRRLP_H__ID_AND_VERSION _0000_000_00_002

#define CDG_ENTER__P_RRRRLP_H__SRC_FILE_TIME _Thu_Nov_29_09_53_22_2007

#include "CDG_ENTER.h"

#undef CDG_ENTER__P_RRRRLP_H

#undef CDG_ENTER__FILENAME


#include "p_rrrrlp.val"


/*
 * End of substructure section, begin of primitive definition section
 */

#ifndef __T_RRRRLP_DATA_IND__
#define __T_RRRRLP_DATA_IND__
/*
 * 
 * CCDGEN:WriteStruct_Count==3005
 */
typedef struct
{
  U8                        cr;                       /*<  0:  1> Command or Response                                */
  U8                        _align0;                  /*<  1:  1> alignment                                          */
  U8                        _align1;                  /*<  2:  1> alignment                                          */
  U8                        _align2;                  /*<  3:  1> alignment                                          */
  T_sdu                     sdu;                      /*<  4: ? > Service Data Unit                                  */
} T_RRRRLP_DATA_IND;
#endif

#ifndef __T_RRRRLP_DATA_REQ__
#define __T_RRRRLP_DATA_REQ__
/*
 * 
 * CCDGEN:WriteStruct_Count==3006
 */
typedef struct
{
  U8                        cr;                       /*<  0:  1> Command or Response                                */
  U8                        _align0;                  /*<  1:  1> alignment                                          */
  U8                        _align1;                  /*<  2:  1> alignment                                          */
  U8                        _align2;                  /*<  3:  1> alignment                                          */
  T_sdu                     sdu;                      /*<  4: ? > Service Data Unit                                  */
} T_RRRRLP_DATA_REQ;
#endif

#ifndef __T_RRRRLP_DATA_CNF__
#define __T_RRRRLP_DATA_CNF__
/*
 * 
 * CCDGEN:WriteStruct_Count==3007
 */
typedef struct
{
  U8                        dummy;                    /*<  0:  1> Dummy                                              */
  U8                        _align0;                  /*<  1:  1> alignment                                          */
  U8                        _align1;                  /*<  2:  1> alignment                                          */
  U8                        _align2;                  /*<  3:  1> alignment                                          */
} T_RRRRLP_DATA_CNF;
#endif

#ifndef __T_RRRRLP_ERROR_IND__
#define __T_RRRRLP_ERROR_IND__
/*
 * 
 * CCDGEN:WriteStruct_Count==3008
 */
typedef struct
{
  U8                        dummy;                    /*<  0:  1> Dummy                                              */
  U8                        _align0;                  /*<  1:  1> alignment                                          */
  U8                        _align1;                  /*<  2:  1> alignment                                          */
  U8                        _align2;                  /*<  3:  1> alignment                                          */
} T_RRRRLP_ERROR_IND;
#endif


#include "CDG_LEAVE.h"


#endif
