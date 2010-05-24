/*
+--------------------------------------------------------------------------+
| PROJECT : PROTOCOL STACK                                                 |
| FILE    : p_8010_128_sm_sap.h                                            |
| SOURCE  : "__out__\LNX_TI_cgt2_70\rel_n5_camera_micron\cdginc\condat\ms\doc\sap\8010_128_sm_sap.pdf"                             |
| LastModified : "2004-06-08"                                              |
| IdAndVersion : "8010.128.02.009"                                         |
| SrcFileTime  : "Thu Nov 29 09:26:48 2007"                                |
| Generated by CCDGEN_2.5.5A on Wed Oct 15 12:22:27 2008                   |
|           !!DO NOT MODIFY!!DO NOT MODIFY!!DO NOT MODIFY!!                |
+--------------------------------------------------------------------------+
*/

/* PRAGMAS
 * PREFIX                 : SM
 * COMPATIBILITY_DEFINES  : YES
 * ALWAYS_ENUM_IN_VAL_FILE: YES
 * ENABLE_GROUP: NO
 * CAPITALIZE_TYPENAME: NO
 */


#ifndef P_8010_128_SM_SAP_H
#define P_8010_128_SM_SAP_H


#define CDG_ENTER__P_8010_128_SM_SAP_H

#define CDG_ENTER__FILENAME _P_8010_128_SM_SAP_H
#define CDG_ENTER__P_8010_128_SM_SAP_H__FILE_TYPE CDGINC
#define CDG_ENTER__P_8010_128_SM_SAP_H__LAST_MODIFIED _2004_06_08
#define CDG_ENTER__P_8010_128_SM_SAP_H__ID_AND_VERSION _8010_128_02_009

#define CDG_ENTER__P_8010_128_SM_SAP_H__SRC_FILE_TIME _Thu_Nov_29_09_26_48_2007

#include "CDG_ENTER.h"

#undef CDG_ENTER__P_8010_128_SM_SAP_H

#undef CDG_ENTER__FILENAME


#include "p_8010_128_sm_sap.val"

#include "p_8010_137_nas_include.h"

#include "p_8010_152_ps_include.h"

#include "p_8010_153_cause_include.h"


/*
 * End of substructure section, begin of primitive definition section
 */

#ifndef __T_SM_ACTIVATE_STARTED_IND__
#define __T_SM_ACTIVATE_STARTED_IND__
/*
 * 
 * CCDGEN:WriteStruct_Count==1238
 */
typedef struct
{
  U8                        nsapi;                    /*<  0:  1> T_NAS_nsapi,  network layer service access point identifier */
  U8                        rat;                      /*<  1:  1> T_PS_rat,  Radio access technology                 */
  U8                        _align0;                  /*<  2:  1> alignment                                          */
  U8                        _align1;                  /*<  3:  1> alignment                                          */
} T_SM_ACTIVATE_STARTED_IND;
#endif

#ifndef __T_SM_ACTIVATE_IND__
#define __T_SM_ACTIVATE_IND__
/*
 * 
 * CCDGEN:WriteStruct_Count==1239
 */
typedef struct
{
  U8                        nsapi;                    /*<  0:  1> T_NAS_nsapi,  network layer service access point identifier */
  U8                        sapi;                     /*<  1:  1> T_PS_sapi,  Service Access Point Identifier        */
  U8                        radio_prio;               /*<  2:  1> T_PS_radio_prio,  Radio priority                   */
  U8                        _align0;                  /*<  3:  1> alignment                                          */
  T_PS_ctrl_qos             ctrl_qos;                 /*<  4:  4> (enum=32bit) controller for union                  */
  T_PS_qos                  qos;                      /*<  8:  0> quality of service (type defined in "p_8010_152_ps_include.h") */
  U16                       pkt_flow_id;              /*<  0:  2> T_PS_pkt_flow_id,  packet flow identifier          */
  U8                        _align1;                  /*<  0:  1> alignment                                          */
  U8                        _align2;                  /*<  1:  1> alignment                                          */
  U8                        _align3;                  /*<  2:  1> alignment                                          */
  U8                        v_comp_params;            /*<  3:  1> valid-flag                                         */
  T_NAS_comp_params         comp_params;              /*<  4:  4> Header compression parameters (type defined in "p_8010_137_nas_include.h") */
} T_SM_ACTIVATE_IND;
#endif

#ifndef __T_SM_ACTIVATE_RES__
#define __T_SM_ACTIVATE_RES__
/*
 * 
 * CCDGEN:WriteStruct_Count==1240
 */
typedef struct
{
  U8                        nsapi;                    /*<  0:  1> T_NAS_nsapi,  network layer service access point identifier */
  U8                        _align0;                  /*<  1:  1> alignment                                          */
  U8                        _align1;                  /*<  2:  1> alignment                                          */
  U8                        v_comp_params;            /*<  3:  1> valid-flag                                         */
  T_NAS_comp_params         comp_params;              /*<  4:  4> Header compression parameters (type defined in "p_8010_137_nas_include.h") */
} T_SM_ACTIVATE_RES;
#endif

#ifndef __T_SM_DEACTIVATE_IND__
#define __T_SM_DEACTIVATE_IND__
/*
 * 
 * CCDGEN:WriteStruct_Count==1241
 */
typedef struct
{
  U16                       nsapi_set;                /*<  0:  2> set of network layer service access point identifiers */
  U8                        rel_ind;                  /*<  2:  1> T_PS_rel_ind,  Local Release Indicator             */
  U8                        _align0;                  /*<  3:  1> alignment                                          */
} T_SM_DEACTIVATE_IND;
#endif

#ifndef __T_SM_DEACTIVATE_RES__
#define __T_SM_DEACTIVATE_RES__
/*
 * 
 * CCDGEN:WriteStruct_Count==1242
 */
typedef struct
{
  U16                       nsapi_set;                /*<  0:  2> set of network layer service access point identifiers */
  U8                        _align0;                  /*<  2:  1> alignment                                          */
  U8                        _align1;                  /*<  3:  1> alignment                                          */
} T_SM_DEACTIVATE_RES;
#endif

#ifndef __T_SM_MODIFY_IND__
#define __T_SM_MODIFY_IND__
/*
 * 
 * CCDGEN:WriteStruct_Count==1243
 */
typedef struct
{
  U8                        nsapi;                    /*<  0:  1> T_NAS_nsapi,  network layer service access point identifier */
  U8                        sapi;                     /*<  1:  1> T_PS_sapi,  Service Access Point Identifier        */
  U8                        radio_prio;               /*<  2:  1> T_PS_radio_prio,  Radio priority                   */
  U8                        _align0;                  /*<  3:  1> alignment                                          */
  U16                       pkt_flow_id;              /*<  4:  2> T_PS_pkt_flow_id,  packet flow identifier          */
  U8                        _align1;                  /*<  6:  1> alignment                                          */
  U8                        _align2;                  /*<  7:  1> alignment                                          */
  T_PS_ctrl_qos             ctrl_qos;                 /*<  8:  4> (enum=32bit) controller for union                  */
  T_PS_qos                  qos;                      /*< 12:  0> quality of service (type defined in "p_8010_152_ps_include.h") */
} T_SM_MODIFY_IND;
#endif

#ifndef __T_SM_MODIFY_RES__
#define __T_SM_MODIFY_RES__
/*
 * 
 * CCDGEN:WriteStruct_Count==1244
 */
typedef struct
{
  U8                        nsapi;                    /*<  0:  1> T_NAS_nsapi,  network layer service access point identifier */
  U8                        _align0;                  /*<  1:  1> alignment                                          */
  U8                        _align1;                  /*<  2:  1> alignment                                          */
  U8                        _align2;                  /*<  3:  1> alignment                                          */
} T_SM_MODIFY_RES;
#endif

#ifndef __T_SM_STATUS_REQ__
#define __T_SM_STATUS_REQ__
/*
 * 
 * CCDGEN:WriteStruct_Count==1245
 */
typedef struct
{
  U16                       nsapi_set;                /*<  0:  2> set of network layer service access point identifiers */
  U8                        sapi;                     /*<  2:  1> T_PS_sapi,  Service Access Point Identifier        */
  U8                        _align0;                  /*<  3:  1> alignment                                          */
  T_CAUSE_ps_cause          ps_cause;                 /*<  4:  4> Cause element containing result of operation (type defined in "p_8010_153_cause_include.h") */
} T_SM_STATUS_REQ;
#endif

#ifndef __T_SM_COMPRESSION_REQ__
#define __T_SM_COMPRESSION_REQ__
/*
 * 
 * CCDGEN:WriteStruct_Count==1246
 */
typedef struct
{
  U16                       nsapi_set;                /*<  0:  2> set of network layer service access point identifiers */
  U8                        _align0;                  /*<  2:  1> alignment                                          */
  U8                        _align1;                  /*<  3:  1> alignment                                          */
  T_NAS_comp_params         comp_params;              /*<  4:  4> Header compression parameters (type defined in "p_8010_137_nas_include.h") */
} T_SM_COMPRESSION_REQ;
#endif


#include "CDG_LEAVE.h"


#endif
