/*
+--------------------------------------------------------------------------+
| PROJECT : PROTOCOL STACK                                                 |
| FILE    : p_t30.h                                                        |
| SOURCE  : "__out__\LNX_TI_cgt2_70\rel_n5_camera_micron\cdginc\condat\ms\doc\sap\t30.pdf"                 |
| LastModified : "2002-04-23"                                              |
| IdAndVersion : "8411.104.98.312"                                         |
| SrcFileTime  : "Thu Nov 29 09:54:28 2007"                                |
| Generated by CCDGEN_2.5.5A on Wed Oct 15 12:22:29 2008                   |
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


#ifndef P_T30_H
#define P_T30_H


#define CDG_ENTER__P_T30_H

#define CDG_ENTER__FILENAME _P_T30_H
#define CDG_ENTER__P_T30_H__FILE_TYPE CDGINC
#define CDG_ENTER__P_T30_H__LAST_MODIFIED _2002_04_23
#define CDG_ENTER__P_T30_H__ID_AND_VERSION _8411_104_98_312

#define CDG_ENTER__P_T30_H__SRC_FILE_TIME _Thu_Nov_29_09_54_28_2007

#include "CDG_ENTER.h"

#undef CDG_ENTER__P_T30_H

#undef CDG_ENTER__FILENAME


#include "p_t30.val"

#ifndef __T_dis__
#define __T_dis__
/*
 * digital identification signal
 * CCDGEN:WriteStruct_Count==3075
 */
typedef struct
{
  U8                        v8;                       /*<  0:  1> V.8 capabilities                                   */
  U8                        n_byte;                   /*<  1:  1> no. of octets preferred                            */
  U8                        ready_tx_fax;             /*<  2:  1> ready to transmit a facsimile document (polling)   */
  U8                        rec_fax_op;               /*<  3:  1> receiver fax operation                             */
  U8                        data_sig_rate;            /*<  4:  1> data signalling rate                               */
  U8                        R8_lines_pels;            /*<  5:  1> R8*7.7 lines/mm and/or 200 x 200 pels/25.4 mm      */
  U8                        two_dim_coding;           /*<  6:  1> two-dimensional coding                             */
  U8                        rec_width;                /*<  7:  1> recording-width                                    */
  U8                        max_rec_len;              /*<  8:  1> maximum recording length                           */
  U8                        min_scan_time;            /*<  9:  1> minimum scan line time at the receiver             */
  U8                        uncomp_mode;              /*< 10:  1> uncompressed mode                                  */
  U8                        err_corr_mode;            /*< 11:  1> error correction mode                              */
  U8                        frame_size;               /*< 12:  1> frame size                                         */
  U8                        t6_coding;                /*< 13:  1> T.6 coding                                         */
  U8                        R8_lines;                 /*< 14:  1> R8*15.4 lines/mm                                   */
  U8                        r_300_pels;               /*< 15:  1> 300*300 pels/25.4 mm                               */
  U8                        R16_lines_pels;           /*< 16:  1> R16*15.4 lines/mm and/or 400*400 pels/25.4 mm      */
  U8                        resolution_type;          /*< 17:  1> resolution type selection                          */
  U8                        i_res_pref;               /*< 18:  1> inch based resolution preferred                    */
  U8                        m_res_pref;               /*< 19:  1> metric-based resolution preferred                  */
  U8                        min_scan_time_hr;         /*< 20:  1> minimum scan line time for higher resolutions      */
  U8                        sel_polling;              /*< 21:  1> selective polling                                  */
  U8                        subaddr;                  /*< 22:  1> subaddressing                                      */
  U8                        password;                 /*< 23:  1> password                                           */
  U8                        ready_tx_doc;             /*< 24:  1> ready to transmit a data file (polling)            */
  U8                        bft;                      /*< 25:  1> Binary File Transfer (BFT)                         */
  U8                        dtm;                      /*< 26:  1> Document Transfer Mode (DTM)                       */
  U8                        edi;                      /*< 27:  1> Electronic Data Interchange (EDI)                  */
  U8                        btm;                      /*< 28:  1> Basic Transfer Mode (BTM)                          */
  U8                        ready_tx_mixed;           /*< 29:  1> ready to transmit a character or mixed mode document (polling) */
  U8                        char_mode;                /*< 30:  1> character mode                                     */
  U8                        mixed_mode;               /*< 31:  1> mixed mode (Annex E/T.4)                           */
  U8                        proc_mode_26;             /*< 32:  1> processable mode 26)                               */
  U8                        dig_network_cap;          /*< 33:  1> digital network capability                         */
  U8                        duplex;                   /*< 34:  1> duplex and half duplex capabilities                */
  U8                        jpeg;                     /*< 35:  1> JPEG coding                                        */
  U8                        full_colour;              /*< 36:  1> full colour mode                                   */
  U8                        huffman_tables;           /*< 37:  1> preferred Huffman tables                           */
  U8                        r_12_bits_pel_comp;       /*< 38:  1> 12 bits/pel component                              */
  U8                        no_subsamp;               /*< 39:  1> no subsampling                                     */
  U8                        cust_illum;               /*< 40:  1> custom illuminant                                  */
  U8                        cust_gamut;               /*< 41:  1> custom gamut range                                 */
  U8                        na_letter;                /*< 42:  1> North American Letter (215.9*279.4 mm)             */
  U8                        na_legal;                 /*< 43:  1> North American Legal (215.9*355.6 mm)              */
  U8                        sing_prog_seq_coding_basic; /*< 44:  1> single-progression sequential coding basic         */
  U8                        sing_prog_seq_coding_L0;  /*< 45:  1> single-progression sequential coding optional L0   */
  U8                        _align0;                  /*< 46:  1> alignment                                          */
  U8                        _align1;                  /*< 47:  1> alignment                                          */
} T_dis;
#endif

/*
 * digital transmit command
 * CCDGEN:WriteStruct_Count==3076
 */
#ifndef __T_dtc__
#define __T_dtc__
typedef T_dis T_dtc;
#endif
/*
 * digital command signal
 * CCDGEN:WriteStruct_Count==3077
 */
#ifndef __T_dcs__
#define __T_dcs__
typedef T_dis T_dcs;
#endif
#ifndef __T_hdlc_info__
#define __T_hdlc_info__
/*
 * HDLC information field
 * CCDGEN:WriteStruct_Count==3078
 */
typedef struct
{
  U8                        crp;                      /*<  0:  1> CRP flag                                           */
  U8                        c_pwd;                    /*<  1:  1> length of PWD                                      */
  U8                        pwd[PWD_LEN];             /*<  2: 20> password                                           */
  U8                        c_sub;                    /*< 22:  1> length of SUB                                      */
  U8                        sub[SUB_LEN];             /*< 23: 20> subaddress                                         */
  U8                        c_sep;                    /*< 43:  1> length of SEP                                      */
  U8                        sep[SEP_LEN];             /*< 44: 20> selective polling                                  */
  U8                        c_nsc;                    /*< 64:  1> length of NSC                                      */
  U8                        nsc[NSC_LEN];             /*< 65: 90> non-standard facilities command                    */
  U8                        c_nsf;                    /*<155:  1> length of NSF                                      */
  U8                        nsf[NSF_LEN];             /*<156: 90> non-standard facilities                            */
  U8                        c_tsi;                    /*<246:  1> length of TSI                                      */
  U8                        tsi[TSI_LEN];             /*<247: 20> transmitting subscriber identification             */
  U8                        c_cig;                    /*<267:  1> length of CIG                                      */
  U8                        cig[CIG_LEN];             /*<268: 20> calling subscriber identification                  */
  U8                        c_csi;                    /*<288:  1> length of CSI                                      */
  U8                        csi[CSI_LEN];             /*<289: 20> calling subscriber identification                  */
  U8                        c_nss;                    /*<309:  1> length of NSS                                      */
  U8                        nss[NSS_LEN];             /*<310: 90> non-standard set-up                                */
  U8                        v_dis;                    /*<400:  1> DIS flag                                           */
  U8                        _align0;                  /*<401:  1> alignment                                          */
  U8                        _align1;                  /*<402:  1> alignment                                          */
  U8                        _align2;                  /*<403:  1> alignment                                          */
  T_dis                     dis;                      /*<404: 48> digital identification signal                      */
  U8                        v_dcs;                    /*<452:  1> DCS flag                                           */
  U8                        _align3;                  /*<453:  1> alignment                                          */
  U8                        _align4;                  /*<454:  1> alignment                                          */
  U8                        _align5;                  /*<455:  1> alignment                                          */
  T_dcs                     dcs;                      /*<456: 48> digital command signal                             */
  U8                        v_dtc;                    /*<504:  1> DTC flag                                           */
  U8                        _align6;                  /*<505:  1> alignment                                          */
  U8                        _align7;                  /*<506:  1> alignment                                          */
  U8                        _align8;                  /*<507:  1> alignment                                          */
  T_dtc                     dtc;                      /*<508: 48> digital transmit command                           */
} T_hdlc_info;
#endif


/*
 * End of substructure section, begin of primitive definition section
 */

#ifndef __T_T30_ACTIVATE_REQ__
#define __T_T30_ACTIVATE_REQ__
/*
 * 
 * CCDGEN:WriteStruct_Count==3081
 */
typedef struct
{
  U16                       trans_rate;               /*<  0:  2> transmission rate                                  */
  U8                        half_rate;                /*<  2:  1> half-rate flag                                     */
  U8                        threshold;                /*<  3:  1> tcf threshold                                      */
  U16                       frames_per_prim;          /*<  4:  2> no. of frames per primitive                        */
  U8                        bitorder;                 /*<  6:  1> Bitorder                                           */
  U8                        _align0;                  /*<  7:  1> alignment                                          */
} T_T30_ACTIVATE_REQ;
#endif

#ifndef __T_T30_CONFIG_REQ__
#define __T_T30_CONFIG_REQ__
/*
 * 
 * CCDGEN:WriteStruct_Count==3082
 */
typedef struct
{
  U8                        hdlc_report;              /*<  0:  1> HDLC report mode                                   */
  U8                        _align0;                  /*<  1:  1> alignment                                          */
  U16                       test_mode;                /*<  2:  2> Test mode                                          */
} T_T30_CONFIG_REQ;
#endif

#ifndef __T_T30_CAP_IND__
#define __T_T30_CAP_IND__
/*
 * 
 * CCDGEN:WriteStruct_Count==3083
 */
typedef struct
{
  T_hdlc_info               hdlc_info;                /*<  0:556> HDLC information field                             */
} T_T30_CAP_IND;
#endif

#ifndef __T_T30_CAP_REQ__
#define __T_T30_CAP_REQ__
/*
 * 
 * CCDGEN:WriteStruct_Count==3084
 */
typedef struct
{
  T_hdlc_info               hdlc_info;                /*<  0:556> HDLC information field                             */
} T_T30_CAP_REQ;
#endif

#ifndef __T_T30_DTI_REQ__
#define __T_T30_DTI_REQ__
/*
 * 
 * CCDGEN:WriteStruct_Count==3085
 */
typedef struct
{
  U8                        dti_conn;                 /*<  0:  1> DTI connect                                        */
  U8                        entity_name[T30_ENTITY_NAME_LEN]; /*<  1:  6> name of an neighbour entity                        */
  U8                        _align0;                  /*<  7:  1> alignment                                          */
  U32                       link_id;                  /*<  8:  4> Link identifier                                    */
  U8                        dti_direction;            /*< 12:  1> DTI direction                                      */
  U8                        _align1;                  /*< 13:  1> alignment                                          */
  U8                        _align2;                  /*< 14:  1> alignment                                          */
  U8                        _align3;                  /*< 15:  1> alignment                                          */
} T_T30_DTI_REQ;
#endif

#ifndef __T_T30_DTI_CNF__
#define __T_T30_DTI_CNF__
/*
 * 
 * CCDGEN:WriteStruct_Count==3086
 */
typedef struct
{
  U8                        dti_conn;                 /*<  0:  1> DTI connect                                        */
  U8                        _align0;                  /*<  1:  1> alignment                                          */
  U8                        _align1;                  /*<  2:  1> alignment                                          */
  U8                        _align2;                  /*<  3:  1> alignment                                          */
} T_T30_DTI_CNF;
#endif

#ifndef __T_T30_DTI_IND__
#define __T_T30_DTI_IND__
/*
 * 
 * CCDGEN:WriteStruct_Count==3087
 */
typedef struct
{
  U8                        dummy;                    /*<  0:  1> no parameters                                      */
} T_T30_DTI_IND;
#endif

#ifndef __T_T30_PHASE_IND__
#define __T_T30_PHASE_IND__
/*
 * 
 * CCDGEN:WriteStruct_Count==3088
 */
typedef struct
{
  U8                        phase;                    /*<  0:  1> T.30 phase                                         */
  U8                        _align0;                  /*<  1:  1> alignment                                          */
  U8                        _align1;                  /*<  2:  1> alignment                                          */
  U8                        _align2;                  /*<  3:  1> alignment                                          */
} T_T30_PHASE_IND;
#endif

#ifndef __T_T30_SGN_IND__
#define __T_T30_SGN_IND__
/*
 * 
 * CCDGEN:WriteStruct_Count==3089
 */
typedef struct
{
  U8                        sgn;                      /*<  0:  1> binary coded signal                                */
  U8                        _align0;                  /*<  1:  1> alignment                                          */
  U8                        _align1;                  /*<  2:  1> alignment                                          */
  U8                        _align2;                  /*<  3:  1> alignment                                          */
} T_T30_SGN_IND;
#endif

#ifndef __T_T30_SGN_REQ__
#define __T_T30_SGN_REQ__
/*
 * 
 * CCDGEN:WriteStruct_Count==3090
 */
typedef struct
{
  U8                        sgn;                      /*<  0:  1> binary coded signal                                */
  U8                        _align0;                  /*<  1:  1> alignment                                          */
  U8                        _align1;                  /*<  2:  1> alignment                                          */
  U8                        _align2;                  /*<  3:  1> alignment                                          */
} T_T30_SGN_REQ;
#endif

#ifndef __T_T30_CMPL_IND__
#define __T_T30_CMPL_IND__
/*
 * 
 * CCDGEN:WriteStruct_Count==3091
 */
typedef struct
{
  U8                        cmpl;                     /*<  0:  1> completion                                         */
  U8                        _align0;                  /*<  1:  1> alignment                                          */
  U8                        _align1;                  /*<  2:  1> alignment                                          */
  U8                        _align2;                  /*<  3:  1> alignment                                          */
} T_T30_CMPL_IND;
#endif

#ifndef __T_T30_REPORT_IND__
#define __T_T30_REPORT_IND__
/*
 * 
 * CCDGEN:WriteStruct_Count==3092
 */
typedef struct
{
  U8                        dir;                      /*<  0:  1> direction of call                                  */
  U8                        _align0;                  /*<  1:  1> alignment                                          */
  U8                        _align1;                  /*<  2:  1> alignment                                          */
  U8                        _align2;                  /*<  3:  1> alignment                                          */
  T_sdu                     sdu;                      /*<  4: ? > Service Data Unit                                  */
} T_T30_REPORT_IND;
#endif

#ifndef __T_T30_MODIFY_REQ__
#define __T_T30_MODIFY_REQ__
/*
 * 
 * CCDGEN:WriteStruct_Count==3093
 */
typedef struct
{
  U16                       trans_rate;               /*<  0:  2> transmission rate                                  */
  U8                        half_rate;                /*<  2:  1> half-rate flag                                     */
  U8                        _align0;                  /*<  3:  1> alignment                                          */
} T_T30_MODIFY_REQ;
#endif

#ifndef __T_T30_ERROR_IND__
#define __T_T30_ERROR_IND__
/*
 * 
 * CCDGEN:WriteStruct_Count==3094
 */
typedef struct
{
  U16                       cause;                    /*<  0:  2> error cause                                        */
  U8                        _align0;                  /*<  2:  1> alignment                                          */
  U8                        _align1;                  /*<  3:  1> alignment                                          */
} T_T30_ERROR_IND;
#endif

#ifndef __T_T30_EOL_IND__
#define __T_T30_EOL_IND__
/*
 * 
 * CCDGEN:WriteStruct_Count==3095
 */
typedef struct
{
  U16                       eol;                      /*<  0:  2> end of line count                                  */
  U8                        _align0;                  /*<  2:  1> alignment                                          */
  U8                        _align1;                  /*<  3:  1> alignment                                          */
} T_T30_EOL_IND;
#endif

#ifndef __T_T30_DEACTIVATE_REQ__
#define __T_T30_DEACTIVATE_REQ__
/*
 * 
 * CCDGEN:WriteStruct_Count==3096
 */
typedef struct
{
  U8                        dummy;                    /*<  0:  1> no parameters                                      */
} T_T30_DEACTIVATE_REQ;
#endif

#ifndef __T_T30_DEACTIVATE_CNF__
#define __T_T30_DEACTIVATE_CNF__
/*
 * 
 * CCDGEN:WriteStruct_Count==3097
 */
typedef struct
{
  U8                        dummy;                    /*<  0:  1> no parameters                                      */
} T_T30_DEACTIVATE_CNF;
#endif

#ifndef __T_T30_ACTIVATE_CNF__
#define __T_T30_ACTIVATE_CNF__
/*
 * 
 * CCDGEN:WriteStruct_Count==3098
 */
typedef struct
{
  U16                       buf_size_rx;              /*<  0:  2> buffer size RX                                     */
  U16                       buf_size_tx;              /*<  2:  2> buffer size TX                                     */
} T_T30_ACTIVATE_CNF;
#endif

#ifndef __T_T30_PREAMBLE_IND__
#define __T_T30_PREAMBLE_IND__
/*
 * 
 * CCDGEN:WriteStruct_Count==3099
 */
typedef struct
{
  U8                        dummy;                    /*<  0:  1> no parameters                                      */
} T_T30_PREAMBLE_IND;
#endif


#include "CDG_LEAVE.h"


#endif
