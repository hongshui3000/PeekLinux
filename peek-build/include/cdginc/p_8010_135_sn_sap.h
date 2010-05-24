/*
+--------------------------------------------------------------------------+
| PROJECT : PROTOCOL STACK                                                 |
| FILE    : p_8010_135_sn_sap.h                                            |
| SOURCE  : "__out__\LNX_TI_cgt2_70\rel_n5_camera_micron\cdginc\condat\ms\doc\sap\8010_135_sn_sap.pdf"                             |
| LastModified : "2004-06-08"                                              |
| IdAndVersion : "8010.135.04.012"                                         |
| SrcFileTime  : "Thu Nov 29 09:27:14 2007"                                |
| Generated by CCDGEN_2.5.5A on Wed Oct 15 12:22:27 2008                   |
|           !!DO NOT MODIFY!!DO NOT MODIFY!!DO NOT MODIFY!!                |
+--------------------------------------------------------------------------+
*/

/* PRAGMAS
 * PREFIX                 : SN
 * COMPATIBILITY_DEFINES  : NO
 * ALWAYS_ENUM_IN_VAL_FILE: YES
 * ENABLE_GROUP: NO
 * CAPITALIZE_TYPENAME: NO
 */


#ifndef P_8010_135_SN_SAP_H
#define P_8010_135_SN_SAP_H


#define CDG_ENTER__P_8010_135_SN_SAP_H

#define CDG_ENTER__FILENAME _P_8010_135_SN_SAP_H
#define CDG_ENTER__P_8010_135_SN_SAP_H__FILE_TYPE CDGINC
#define CDG_ENTER__P_8010_135_SN_SAP_H__LAST_MODIFIED _2004_06_08
#define CDG_ENTER__P_8010_135_SN_SAP_H__ID_AND_VERSION _8010_135_04_012

#define CDG_ENTER__P_8010_135_SN_SAP_H__SRC_FILE_TIME _Thu_Nov_29_09_27_14_2007

#include "CDG_ENTER.h"

#undef CDG_ENTER__P_8010_135_SN_SAP_H

#undef CDG_ENTER__FILENAME


#include "p_8010_135_sn_sap.val"

#include "p_8010_137_nas_include.h"

#include "p_8010_152_ps_include.h"

#include "p_8010_153_cause_include.h"


/*
 * typedef between var and valtab enums
 */
#ifndef __T_SN_establish__
#define __T_SN_establish__
typedef T_SN_VAL_establish T_SN_establish;
#endif

#ifndef __T_SN_desc_list2__
#define __T_SN_desc_list2__
/*
 * DTI parameters
 * CCDGEN:WriteStruct_Count==1265
 */
typedef struct
{
  U16                       list_len;                 /*<  0:  2> Octet count of data in all descriptors             */
  U8                        _align0;                  /*<  2:  1> alignment                                          */
  U8                        _align1;                  /*<  3:  1> alignment                                          */
  U32                       first;                    /*<  4:  4> Pointer to first descriptor in list                */
} T_SN_desc_list2;
#endif

#ifndef __T_SN_ul_pdus__
#define __T_SN_ul_pdus__
/*
 * Sequence number and descriptor list handles pr. NSAPI
 * CCDGEN:WriteStruct_Count==1266
 */
typedef struct
{
  U8                        nsapi;                    /*<  0:  1> T_NAS_nsapi,  network layer service access point identifier */
  U8                        dl_sequence_number;       /*<  1:  1> Sequence number of last unacknowledged downlink PDU (used by the network) */
  U8                        _align0;                  /*<  2:  1> alignment                                          */
  U8                        c_desc_list2;             /*<  3:  1> counter                                            */
  T_SN_desc_list2           *ptr_desc_list2;          /*<  4:  4> pointer to DTI parameters                          */
} T_SN_ul_pdus;
#endif

#ifndef __T_SN_receive_no_list__
#define __T_SN_receive_no_list__
/*
 * Ssequence numbers and MEM  handles pr. NSAPI
 * CCDGEN:WriteStruct_Count==1267
 */
typedef struct
{
  U8                        nsapi;                    /*<  0:  1> T_NAS_nsapi,  network layer service access point identifier */
  U8                        receive_no;               /*<  1:  1> Receive NPDU number                                */
  U8                        _align0;                  /*<  2:  1> alignment                                          */
  U8                        _align1;                  /*<  3:  1> alignment                                          */
} T_SN_receive_no_list;
#endif


/*
 * End of substructure section, begin of primitive definition section
 */

#ifndef __T_SN_ACTIVATE_CNF__
#define __T_SN_ACTIVATE_CNF__
/*
 * 
 * CCDGEN:WriteStruct_Count==1268
 */
typedef struct
{
  U8                        nsapi;                    /*<  0:  1> T_NAS_nsapi,  network layer service access point identifier */
  U8                        _align0;                  /*<  1:  1> alignment                                          */
  U8                        _align1;                  /*<  2:  1> alignment                                          */
  U8                        _align2;                  /*<  3:  1> alignment                                          */
  T_NAS_comp_params         comp_params;              /*<  4:  4> Header compression parameters (type defined in "p_8010_137_nas_include.h") */
} T_SN_ACTIVATE_CNF;
#endif

#ifndef __T_SN_ACTIVATE_REQ__
#define __T_SN_ACTIVATE_REQ__
/*
 * 
 * CCDGEN:WriteStruct_Count==1269
 */
typedef struct
{
  U8                        nsapi;                    /*<  0:  1> T_NAS_nsapi,  network layer service access point identifier */
  U8                        sapi;                     /*<  1:  1> T_PS_sapi,  Service Access Point Identifier        */
  U8                        radio_prio;               /*<  2:  1> T_PS_radio_prio,  Radio priority                   */
  U8                        _align0;                  /*<  3:  1> alignment                                          */
  T_PS_qos_r97              snsm_qos;                 /*<  4:  8> quality of service R97 (type defined in "p_8010_152_ps_include.h") */
  U8                        establish;                /*< 12:  1> T_SN_establish,                                    */
  U8                        _align1;                  /*< 13:  1> alignment                                          */
  U8                        _align2;                  /*< 14:  1> alignment                                          */
  U8                        _align3;                  /*< 15:  1> alignment                                          */
  T_NAS_comp_params         comp_params;              /*< 16:  4> Header compression parameters (type defined in "p_8010_137_nas_include.h") */
  U16                       pkt_flow_id;              /*< 20:  2> T_PS_pkt_flow_id,  packet flow identifier          */
  U8                        _align4;                  /*< 22:  1> alignment                                          */
  U8                        _align5;                  /*< 23:  1> alignment                                          */
} T_SN_ACTIVATE_REQ;
#endif

#ifndef __T_SN_DEACTIVATE_CNF__
#define __T_SN_DEACTIVATE_CNF__
/*
 * 
 * CCDGEN:WriteStruct_Count==1270
 */
typedef struct
{
  U8                        nsapi;                    /*<  0:  1> T_NAS_nsapi,  network layer service access point identifier */
  U8                        _align0;                  /*<  1:  1> alignment                                          */
  U8                        _align1;                  /*<  2:  1> alignment                                          */
  U8                        _align2;                  /*<  3:  1> alignment                                          */
} T_SN_DEACTIVATE_CNF;
#endif

#ifndef __T_SN_DEACTIVATE_REQ__
#define __T_SN_DEACTIVATE_REQ__
/*
 * 
 * CCDGEN:WriteStruct_Count==1271
 */
typedef struct
{
  U8                        nsapi;                    /*<  0:  1> T_NAS_nsapi,  network layer service access point identifier */
  U8                        rel_ind;                  /*<  1:  1> T_PS_rel_ind,  Local Release Indicator             */
  U8                        _align0;                  /*<  2:  1> alignment                                          */
  U8                        _align1;                  /*<  3:  1> alignment                                          */
} T_SN_DEACTIVATE_REQ;
#endif

#ifndef __T_SN_GET_PENDING_PDU_CNF__
#define __T_SN_GET_PENDING_PDU_CNF__
/*
 * 
 * CCDGEN:WriteStruct_Count==1272
 */
typedef struct
{
  U8                        _align0;                  /*<  0:  1> alignment                                          */
  U8                        _align1;                  /*<  1:  1> alignment                                          */
  U8                        _align2;                  /*<  2:  1> alignment                                          */
  U8                        c_ul_pdus;                /*<  3:  1> counter                                            */
  T_SN_ul_pdus              ul_pdus[NAS_SIZE_NSAPI];  /*<  4:128> Sequence number and descriptor list handles pr. NSAPI */
} T_SN_GET_PENDING_PDU_CNF;
#endif

#ifndef __T_SN_GET_PENDING_PDU_REQ__
#define __T_SN_GET_PENDING_PDU_REQ__
/*
 * 
 * CCDGEN:WriteStruct_Count==1273
 */
typedef struct
{
  U8                        dummy;                    /*<  0:  1> no parameters                                      */
} T_SN_GET_PENDING_PDU_REQ;
#endif

#ifndef __T_SN_MODIFY_CNF__
#define __T_SN_MODIFY_CNF__
/*
 * 
 * CCDGEN:WriteStruct_Count==1274
 */
typedef struct
{
  U8                        nsapi;                    /*<  0:  1> T_NAS_nsapi,  network layer service access point identifier */
  U8                        _align0;                  /*<  1:  1> alignment                                          */
  U8                        _align1;                  /*<  2:  1> alignment                                          */
  U8                        _align2;                  /*<  3:  1> alignment                                          */
} T_SN_MODIFY_CNF;
#endif

#ifndef __T_SN_MODIFY_REQ__
#define __T_SN_MODIFY_REQ__
/*
 * 
 * CCDGEN:WriteStruct_Count==1275
 */
typedef struct
{
  U8                        nsapi;                    /*<  0:  1> T_NAS_nsapi,  network layer service access point identifier */
  U8                        sapi;                     /*<  1:  1> T_PS_sapi,  Service Access Point Identifier        */
  U8                        radio_prio;               /*<  2:  1> T_PS_radio_prio,  Radio priority                   */
  U8                        _align0;                  /*<  3:  1> alignment                                          */
  T_PS_qos_r97              snsm_qos;                 /*<  4:  8> quality of service R97 (type defined in "p_8010_152_ps_include.h") */
  U16                       pkt_flow_id;              /*< 12:  2> T_PS_pkt_flow_id,  packet flow identifier          */
  U8                        _align1;                  /*< 14:  1> alignment                                          */
  U8                        _align2;                  /*< 15:  1> alignment                                          */
} T_SN_MODIFY_REQ;
#endif

#ifndef __T_SN_STATUS_IND__
#define __T_SN_STATUS_IND__
/*
 * 
 * CCDGEN:WriteStruct_Count==1276
 */
typedef struct
{
  U16                       nsapi_set;                /*<  0:  2> set of network layer service access point identifiers */
  U8                        sapi;                     /*<  2:  1> T_PS_sapi,  Service Access Point Identifier        */
  U8                        _align0;                  /*<  3:  1> alignment                                          */
  T_CAUSE_ps_cause          ps_cause;                 /*<  4:  4> Cause element containing result of operation (type defined in "p_8010_153_cause_include.h") */
} T_SN_STATUS_IND;
#endif

#ifndef __T_SN_SEQUENCE_REQ__
#define __T_SN_SEQUENCE_REQ__
/*
 * 
 * CCDGEN:WriteStruct_Count==1277
 */
typedef struct
{
  U8                        _align0;                  /*<  0:  1> alignment                                          */
  U8                        _align1;                  /*<  1:  1> alignment                                          */
  U8                        _align2;                  /*<  2:  1> alignment                                          */
  U8                        c_receive_no_list;        /*<  3:  1> counter                                            */
  T_SN_receive_no_list      receive_no_list[NAS_SIZE_NSAPI]; /*<  4: 64> Ssequence numbers and MEM  handles pr. NSAPI       */
} T_SN_SEQUENCE_REQ;
#endif

#ifndef __T_SN_SEQUENCE_CNF__
#define __T_SN_SEQUENCE_CNF__
/*
 * 
 * CCDGEN:WriteStruct_Count==1278
 */
typedef struct
{
  U8                        _align0;                  /*<  0:  1> alignment                                          */
  U8                        _align1;                  /*<  1:  1> alignment                                          */
  U8                        _align2;                  /*<  2:  1> alignment                                          */
  U8                        c_receive_no_list;        /*<  3:  1> counter                                            */
  T_SN_receive_no_list      receive_no_list[NAS_SIZE_NSAPI]; /*<  4: 64> Ssequence numbers and MEM  handles pr. NSAPI       */
} T_SN_SEQUENCE_CNF;
#endif

#ifndef __T_SN_COUNT_REQ__
#define __T_SN_COUNT_REQ__
/*
 * 
 * CCDGEN:WriteStruct_Count==1279
 */
typedef struct
{
  U8                        nsapi;                    /*<  0:  1> T_NAS_nsapi,  network layer service access point identifier */
  U8                        reset;                    /*<  1:  1> T_NAS_reset,  reset flag                           */
  U8                        _align0;                  /*<  2:  1> alignment                                          */
  U8                        _align1;                  /*<  3:  1> alignment                                          */
} T_SN_COUNT_REQ;
#endif

#ifndef __T_SN_COUNT_CNF__
#define __T_SN_COUNT_CNF__
/*
 * 
 * CCDGEN:WriteStruct_Count==1280
 */
typedef struct
{
  U8                        nsapi;                    /*<  0:  1> T_NAS_nsapi,  network layer service access point identifier */
  U8                        _align0;                  /*<  1:  1> alignment                                          */
  U8                        _align1;                  /*<  2:  1> alignment                                          */
  U8                        _align2;                  /*<  3:  1> alignment                                          */
  U32                       octets_uplink;            /*<  4:  4> octets uplink                                      */
  U32                       octets_downlink;          /*<  8:  4> octets downlink                                    */
  U32                       packets_uplink;           /*< 12:  4> packets uplink                                     */
  U32                       packets_downlink;         /*< 16:  4> packets downlink                                   */
} T_SN_COUNT_CNF;
#endif

#ifndef __T_SN_DTI_REQ__
#define __T_SN_DTI_REQ__
/*
 * 
 * CCDGEN:WriteStruct_Count==1281
 */
typedef struct
{
  U8                        nsapi;                    /*<  0:  1> T_NAS_nsapi,  network layer service access point identifier */
  U8                        dti_conn;                 /*<  1:  1> T_NAS_dti_conn,  DTI connect                       */
  U8                        _align0;                  /*<  2:  1> alignment                                          */
  U8                        _align1;                  /*<  3:  1> alignment                                          */
  U32                       dti_linkid;               /*<  4:  4> DTI link identifier                                */
  U32                       dti_neighbor;             /*<  8:  4> DTI neighbor entity name.                          */
  U8                        dti_direction;            /*< 12:  1> T_NAS_dti_direction,  direction of DTI connection  */
  U8                        _align2;                  /*< 13:  1> alignment                                          */
  U8                        _align3;                  /*< 14:  1> alignment                                          */
  U8                        _align4;                  /*< 15:  1> alignment                                          */
} T_SN_DTI_REQ;
#endif

#ifndef __T_SN_DTI_CNF__
#define __T_SN_DTI_CNF__
/*
 * 
 * CCDGEN:WriteStruct_Count==1282
 */
typedef struct
{
  U32                       dti_linkid;               /*<  0:  4> DTI link identifier                                */
  U8                        dti_conn;                 /*<  4:  1> T_NAS_dti_conn,  DTI connect                       */
  U8                        _align0;                  /*<  5:  1> alignment                                          */
  U8                        _align1;                  /*<  6:  1> alignment                                          */
  U8                        _align2;                  /*<  7:  1> alignment                                          */
} T_SN_DTI_CNF;
#endif


#include "CDG_LEAVE.h"


#endif
