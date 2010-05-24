/*
+--------------------------------------------------------------------------+
| PROJECT : PROTOCOL STACK                                                 |
| FILE    : p_dio.h                                                        |
| SOURCE  : "__out__\LNX_TI_cgt2_70\rel_n5_camera_micron\cdginc\condat\ms\doc\sap\dio.pdf"                 |
| LastModified : "2004-03-19"                                              |
| IdAndVersion : "..."                                                     |
| SrcFileTime  : "Thu Nov 29 09:39:48 2007"                                |
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


#ifndef P_DIO_H
#define P_DIO_H


#define CDG_ENTER__P_DIO_H

#define CDG_ENTER__FILENAME _P_DIO_H
#define CDG_ENTER__P_DIO_H__FILE_TYPE CDGINC
#define CDG_ENTER__P_DIO_H__LAST_MODIFIED _2004_03_19
#define CDG_ENTER__P_DIO_H__ID_AND_VERSION _

#define CDG_ENTER__P_DIO_H__SRC_FILE_TIME _Thu_Nov_29_09_39_48_2007

#include "CDG_ENTER.h"

#undef CDG_ENTER__P_DIO_H

#undef CDG_ENTER__FILENAME


#include "p_dio.val"

#ifndef __T_DIO_DCB_SER__
#define __T_DIO_DCB_SER__
/*
 * Structured Element
 * CCDGEN:WriteStruct_Count==1225
 */
typedef struct
{
  U32                       device_type;              /*<  0:  4> device identifier with general capability information */
  U8                        sleep_mode;               /*<  4:  1> enter sleep mode is possible or not                */
  U8                        _align0;                  /*<  5:  1> alignment                                          */
  U8                        _align1;                  /*<  6:  1> alignment                                          */
  U8                        _align2;                  /*<  7:  1> alignment                                          */
  U32                       baudrate;                 /*<  8:  4> user set baudrate                                  */
  U32                       char_frame;               /*< 12:  4> supported character framing                        */
  U32                       flow_control;             /*< 16:  4> supported modes of flow control                    */
  U8                        xon;                      /*< 20:  1> set/reset XON for flow control                     */
  U8                        xoff;                     /*< 21:  1> set/reset XOFF for flow control                    */
  U8                        esc_char;                 /*< 22:  1> ASCII character which is used in an escape sequence */
  U8                        _align3;                  /*< 23:  1> alignment                                          */
  U16                       guard_period;             /*< 24:  2> duration value for escape sequence                 */
  U8                        _align4;                  /*< 26:  1> alignment                                          */
  U8                        _align5;                  /*< 27:  1> alignment                                          */
} T_DIO_DCB_SER;
#endif

#ifndef __T_DIO_DCB_SER_MUX__
#define __T_DIO_DCB_SER_MUX__
/*
 * Structured Element
 * CCDGEN:WriteStruct_Count==1226
 */
typedef struct
{
  U32                       device_type;              /*<  0:  4> device identifier with general capability information */
  U8                        sleep_mode;               /*<  4:  1> enter sleep mode is possible or not                */
  U8                        _align0;                  /*<  5:  1> alignment                                          */
  U8                        _align1;                  /*<  6:  1> alignment                                          */
  U8                        _align2;                  /*<  7:  1> alignment                                          */
  U32                       baudrate;                 /*<  8:  4> user set baudrate                                  */
  U32                       char_frame;               /*< 12:  4> supported character framing                        */
  U32                       flow_control;             /*< 16:  4> supported modes of flow control                    */
  U8                        xon;                      /*< 20:  1> set/reset XON for flow control                     */
  U8                        xoff;                     /*< 21:  1> set/reset XOFF for flow control                    */
  U8                        esc_char;                 /*< 22:  1> ASCII character which is used in an escape sequence */
  U8                        _align3;                  /*< 23:  1> alignment                                          */
  U16                       guard_period;             /*< 24:  2> duration value for escape sequence                 */
  U8                        _align4;                  /*< 26:  1> alignment                                          */
  U8                        _align5;                  /*< 27:  1> alignment                                          */
  U32                       device_mode;              /*< 28:  4> work mode of device                                */
  U32                       mux_mode;                 /*< 32:  4> supported multiplexer modes                        */
  U16                       n1;                       /*< 36:  2> max frame size of mux frame                        */
  U8                        n2;                       /*< 38:  1> max number of retransmissions                      */
  U8                        t1;                       /*< 39:  1> acknowledgement timer                              */
  U8                        t2;                       /*< 40:  1> response timer                                     */
  U8                        t3;                       /*< 41:  1> wake up response timer                             */
  U8                        k;                        /*< 42:  1> windows size                                       */
  U8                        _align6;                  /*< 43:  1> alignment                                          */
} T_DIO_DCB_SER_MUX;
#endif

#ifndef __T_DIO_DCB_PKT__
#define __T_DIO_DCB_PKT__
/*
 * Structured Element
 * CCDGEN:WriteStruct_Count==1227
 */
typedef struct
{
  U32                       device_type;              /*<  0:  4> device identifier with general capability information */
  U8                        sleep_mode;               /*<  4:  1> enter sleep mode is possible or not                */
  U8                        _align0;                  /*<  5:  1> alignment                                          */
  U8                        _align1;                  /*<  6:  1> alignment                                          */
  U8                        _align2;                  /*<  7:  1> alignment                                          */
} T_DIO_DCB_PKT;
#endif

#ifndef __T_DIO_CAP_SER__
#define __T_DIO_CAP_SER__
/*
 * serial device capabiliity structures
 * CCDGEN:WriteStruct_Count==1228
 */
typedef struct
{
  U32                       device_type;              /*<  0:  4> device identifier with general capability information */
  U32                       device_flags;             /*<  4:  4> driver features like power saving state            */
  U16                       mtu_control;              /*<  8:  2> max length of copied control information bytes by the driver */
  U16                       mtu_data;                 /*< 10:  2> max size of packet data which the driver can transfer */
  U32                       driver_name;              /*< 12:  4> pointer to driver name                             */
  U32                       baudrate_auto;            /*< 16:  4> automatically detected driver baudrates            */
  U32                       baudrate_fixed;           /*< 20:  4> driver baudrates which cannot be detected automatically */
  U32                       char_frame;               /*< 24:  4> supported character framing                        */
  U32                       flow_control;             /*< 28:  4> supported modes of flow control                    */
  U32                       ser_flags;                /*< 32:  4> additional supported features of serial/mux devices */
} T_DIO_CAP_SER;
#endif

#ifndef __T_DIO_CAP_SER_MUX__
#define __T_DIO_CAP_SER_MUX__
/*
 * Structured Element
 * CCDGEN:WriteStruct_Count==1229
 */
typedef struct
{
  U32                       device_type;              /*<  0:  4> device identifier with general capability information */
  U32                       device_flags;             /*<  4:  4> driver features like power saving state            */
  U16                       mtu_control;              /*<  8:  2> max length of copied control information bytes by the driver */
  U16                       mtu_data;                 /*< 10:  2> max size of packet data which the driver can transfer */
  U32                       driver_name;              /*< 12:  4> pointer to driver name                             */
  U32                       baudrate_auto;            /*< 16:  4> automatically detected driver baudrates            */
  U32                       baudrate_fixed;           /*< 20:  4> driver baudrates which cannot be detected automatically */
  U32                       char_frame;               /*< 24:  4> supported character framing                        */
  U32                       flow_control;             /*< 28:  4> supported modes of flow control                    */
  U32                       ser_flags;                /*< 32:  4> additional supported features of serial/mux devices */
  U32                       mux_mode;                 /*< 36:  4> supported multiplexer modes                        */
} T_DIO_CAP_SER_MUX;
#endif

#ifndef __T_DIO_CAP_PKT__
#define __T_DIO_CAP_PKT__
/*
 * Structured Element
 * CCDGEN:WriteStruct_Count==1230
 */
typedef struct
{
  U32                       device_type;              /*<  0:  4> device identifier with general capability information */
  U32                       device_flags;             /*<  4:  4> driver features like power saving state            */
  U16                       mtu_control;              /*<  8:  2> max length of copied control information bytes by the driver */
  U16                       mtu_data;                 /*< 10:  2> max size of packet data which the driver can transfer */
  U32                       driver_name;              /*< 12:  4> pointer to driver name                             */
} T_DIO_CAP_PKT;
#endif

#ifndef __T_DIO_CAP__
#define __T_DIO_CAP__
/*
 * device capabiliity structures
 * CCDGEN:WriteStruct_Count==1231
 */
typedef struct
{
  U32                       device_type;              /*<  0:  4> device identifier with general capability information */
  U32                       device_flags;             /*<  4:  4> driver features like power saving state            */
  U16                       mtu_control;              /*<  8:  2> max length of copied control information bytes by the driver */
  U16                       mtu_data;                 /*< 10:  2> max size of packet data which the driver can transfer */
  U32                       driver_name;              /*< 12:  4> pointer to driver name                             */
} T_DIO_CAP;
#endif

#ifndef __T_DIO_DCB__
#define __T_DIO_DCB__
/*
 * all elements of driver configuration parameter 
 * CCDGEN:WriteStruct_Count==1232
 */
typedef struct
{
  U32                       device_type;              /*<  0:  4> device identifier with general capability information */
  U8                        sleep_mode;               /*<  4:  1> enter sleep mode is possible or not                */
  U8                        _align0;                  /*<  5:  1> alignment                                          */
  U8                        _align1;                  /*<  6:  1> alignment                                          */
  U8                        _align2;                  /*<  7:  1> alignment                                          */
} T_DIO_DCB;
#endif

#ifndef __T_dio_segment__
#define __T_dio_segment__
/*
 * Structured Element
 * CCDGEN:WriteStruct_Count==1233
 */
typedef struct
{
  U8                        _align0;                  /*<  0:  1> alignment                                          */
  U8                        _align1;                  /*<  1:  1> alignment                                          */
  U16                       c_data;                   /*<  2:  2> counter                                            */
  U8                        *ptr_data;                /*<  4:  4> pointer to  pointer to the first byte of the data buffer segment */
} T_dio_segment;
#endif

#ifndef __T_dio_buffer__
#define __T_dio_buffer__
/*
 * data buffer
 * CCDGEN:WriteStruct_Count==1234
 */
typedef struct
{
  U16                       length;                   /*<  0:  2> len of dio_ctrl                                    */
  U8                        _align0;                  /*<  2:  1> alignment                                          */
  U8                        c_dio_segment;            /*<  3:  1> counter                                            */
  T_dio_segment             *ptr_dio_segment;         /*<  4:  4> pointer to Structured Element                      */
} T_dio_buffer;
#endif

#ifndef __T_DIO_CTRL__
#define __T_DIO_CTRL__
/*
 * Structured Element
 * CCDGEN:WriteStruct_Count==1235
 */
typedef struct
{
  U16                       control_type;             /*<  0:  2> data types                                         */
  U16                       length;                   /*<  2:  2> len of dio_ctrl                                    */
} T_DIO_CTRL;
#endif

#ifndef __T_DIO_CTRL_LINES__
#define __T_DIO_CTRL_LINES__
/*
 * Structured Element
 * CCDGEN:WriteStruct_Count==1236
 */
typedef struct
{
  U16                       control_type;             /*<  0:  2> data types                                         */
  U16                       length;                   /*<  2:  2> len of dio_ctrl                                    */
  U32                       state;                    /*<  4:  4> line states                                        */
} T_DIO_CTRL_LINES;
#endif

#ifndef __T_DIO_CTRL_MUX__
#define __T_DIO_CTRL_MUX__
/*
 * Structured Element
 * CCDGEN:WriteStruct_Count==1237
 */
typedef struct
{
  U16                       control_type;             /*<  0:  2> data types                                         */
  U16                       length;                   /*<  2:  2> len of dio_ctrl                                    */
  U32                       state;                    /*<  4:  4> line states                                        */
} T_DIO_CTRL_MUX;
#endif


/*
 * End of substructure section, begin of primitive definition section
 */


#include "CDG_LEAVE.h"


#endif
