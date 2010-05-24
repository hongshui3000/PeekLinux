/*
+--------------------------------------------------------------------------+
| PROJECT : PROTOCOL STACK                                                 |
| FILE    : p_cci.h                                                        |
| SOURCE  : "__out__\LNX_TI_cgt2_70\rel_n5_camera_micron\cdginc\condat\ms\doc\sap\cci.pdf"                 |
| LastModified : "2003-01-13"                                              |
| IdAndVersion : "8441.118.00.017"                                         |
| SrcFileTime  : "Thu Nov 29 09:37:42 2007"                                |
| Generated by CCDGEN_2.5.5A on Wed Oct 15 12:22:31 2008                   |
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


#ifndef P_CCI_H
#define P_CCI_H


#define CDG_ENTER__P_CCI_H

#define CDG_ENTER__FILENAME _P_CCI_H
#define CDG_ENTER__P_CCI_H__FILE_TYPE CDGINC
#define CDG_ENTER__P_CCI_H__LAST_MODIFIED _2003_01_13
#define CDG_ENTER__P_CCI_H__ID_AND_VERSION _8441_118_00_017

#define CDG_ENTER__P_CCI_H__SRC_FILE_TIME _Thu_Nov_29_09_37_42_2007

#include "CDG_ENTER.h"

#undef CDG_ENTER__P_CCI_H

#undef CDG_ENTER__FILENAME


#include "p_cci.val"


/*
 * End of substructure section, begin of primitive definition section
 */

#ifndef __T_CCI_INIT_REQ__
#define __T_CCI_INIT_REQ__
/*
 * 
 * CCDGEN:WriteStruct_Count==1220
 */
typedef struct
{
  U8                        algo_type;                /*<  0:  1> algorithm type                                     */
  U8                        comp_inst;                /*<  1:  1> compressor instance                                */
  U8                        _align0;                  /*<  2:  1> alignment                                          */
  U8                        _align1;                  /*<  3:  1> alignment                                          */
} T_CCI_INIT_REQ;
#endif


#include "CDG_LEAVE.h"


#endif
