#ifndef __L2CAP_I_H
#define __L2CAP_I_H
/***************************************************************************
 *
 * File:
 *     $Workfile:l2cap_i.h$ for iAnywhere Blue SDK, Version 2.1.2
 *     $Revision:89$
 *
 * Description:
 *     This file contains the defines, typedefs and function
 *     prototypes for functions internal to the L2CAP protocol.
 *
 * Created:
 *     July 15, 1999
 *
 * Copyright 1999-2005 Extended Systems, Inc.
 * Portions copyright 2005-2006 iAnywhere Solutions, Inc.
 * All rights reserved. All unpublished rights reserved.
 *
 * Unpublished Confidential Information of iAnywhere Solutions, Inc.  
 * Do Not Disclose.
 *
 * No part of this work may be used or reproduced in any form or by any 
 * means, or stored in a database or retrieval system, without prior written 
 * permission of iAnywhere Solutions, Inc.
 * 
 * Use of this work is governed by a license granted by iAnywhere Solutions, 
 * Inc.  This work contains confidential and proprietary information of 
 * iAnywhere Solutions, Inc. which is protected by copyright, trade secret, 
 * trademark and other intellectual property rights.
 *
 ****************************************************************************/

#include "l2cap.h"
#include "me.h"
#include "sec.h"
#include "sys/evmxp.h"
#include "utils.h"
#include "sys/hci.h"

/* -- L2SIG_DATA_LEN ---------------------------------------------------------
 *
 * This size is used to allocate the data area for L2CAP signalling packets. 
 * Both the signal packet data area and spare BtPacket header area (8 bytes)
 * are used when building signalling commands and responses. So the effective
 * size is 8 bytes larger than these defines.
 */
#if L2CAP_FLOW_CONTROL == XA_ENABLED
/* Maximum: Configure Req(8) + MTU(4) + Flow Control(11) */
#define L2SIG_DATA_LEN    15
#else
/* Maximum: Connect Rsp(12) */
#define L2SIG_DATA_LEN    4
#endif

/* -- L2C_WORKSPACE_LEN ------------------------------------------------------
 *
 * This size is used to allocate a channel workspace used to store rejected 
 * configuration options. Only one option at a time is rejected so the size
 * reflects the largest 'acceptable' option value we may respond with.
 */
#if L2CAP_FLOW_CONTROL == XA_ENABLED
/* Maximum: Flow Control Modes(11) Option */
#define L2C_WORKSPACE_LEN       11
#else
/* Maximum: MTU(4) or FlushTimeout(4) Options */
#define L2C_WORKSPACE_LEN       4
#endif

/* -- SignalPacket ----------------------------------------------------------
 *
 * Used by L2CAP for sending signalling packets. A combination of a BtPacket
 * and the workspace necessary for building signalling commands.
 */
typedef struct _SignalPacket {
    BtPacket    packet;
    U8          workspace[L2SIG_DATA_LEN];
} SignalPacket;


/* -- L2CAP_Channel ----------------------------------------------------------
 *
 * One per protocol binding between two devices. For example when 2 ACL links are
 * established two channels will be established for each protocol and therefore 
 * there will be local 2 CID's. The CID values are dynamically allocated in the 
 * range from 0x0040 to 0xFFFF. There is no Channel for the signalling channel(s).
 */
typedef struct _L2CAP_Channel {
    U16               flags;
    U8                state;        /* L2CAP Channel Connection State */
    U8                localIdent;   /* Identifier sent in signalling request */

    BtRemoteDevice   *link;         /* remote device info and data queues */
    EvmTimer          timer;        /* RTX/ERTX */

    U16               outMtu;       /* Maximum sized Tx packet (SDU) for channel */
    U16               inUse;        /* Number of signal + data packets outstanding on channel */

    U16               remotePsm;    /* Remote PSM value */

    /* Channel oriented stuff */
    const L2capPsm   *psmInfo;      /* PSM hosting this channel */
    L2capChannelId    localCid;     /* Local Channel Id (>= 0x0040) */
    L2capChannelId    remoteCid;    /* Remote devices channel id */
    U8                remoteIdent;  /* Identifier recv'd in signalling req */
    U8                workspace[L2C_WORKSPACE_LEN]; /* Holder for rejected config options. */
    U8                wsLen;        /* Length of options in workspace */
    U16               result;       /* Configure/Connect response "result" */
#if BT_SECURITY == XA_ENABLED
    BtSecurityToken   authorization;/* Passed to ME for access requests. */
#endif
    U16               localMtu;

#if L2CAP_FLOW_CONTROL == XA_ENABLED
    U16               flags2;

    /* Sender variables */
    U8                outLinkMode;  /* Uses LINK_MODE_XXX defines. */
    U8                txWindow;
    U8                sendSeq;      /* Next sequence number to send: V(S) */
    U8                ackSeq;       /* Next segment to be acknowledged by receiver: V(A) */
    U8                transmitCount;/* Number of times current unacked segment was sent */
    U8                transmitMax;    /* From configuration option */
    U16               retransTimeout; /* From configuration option */
    U16               monitorTimeout; /* From configuration option */
    U16               outMpu;       /* Maximum sized tx-sdu-segment for channel */

    /* Receiver variables */
    U8                inLinkMode;   /* Uses LINK_MODE_XXX defines. */
    U8                recvSeq;      /* Next expected sequence number: V(R) */
    U8                buffSeq;      /* Next segment to Ack for sender: V(B) */
    U8                rxWindow;     /* Window advertised to sending peer */
    U8                sar;          /* Last received SAR state */
    U16               segStart;     /* Starting sduBuffer offset for current segment */
    U16               segLength;    /* Received length of current segment */
    U8               *sduBuffer;    /* SDU reassembly buffer provided by L2CAP user */
    U16               sduLength;    /* Total length of SDU (from SDU header) */
    U16               localMpu;     /* Maximum sized rx-sdu-segment for channel */
    U16               buffered;     /* Data passed to application */

#endif /* L2CAP_FLOW_CONTROL == XA_ENABLED */
} L2CAP_Channel;


/* -- L2CAP Channel Flags ----------------------------------------------------
 * 
 * Flags used in the 'flags' field of the L2CAP_Channel structure.
 */
#define L2CHF_SEC_PENDING     0x0001  /* SEC_AccessRequest() in progress */
#define L2CHF_CONNECTED       0x0002  /* Channel is connected */
#define L2CHF_RTX_ACTIVE      0x0004
#define L2CHF_ERTX_ACTIVE     0x0008
#define L2CHF_OUT_CONFIG_OK   0x0010  /* Local Config Request was accepted */
#define L2CHF_IN_CONFIG_OK    0x0020  /* Remote Config Request was accepted */
#define L2CHF_CONFIG_REQ_SENT 0x0040  /* Config Req Sent */
#define L2CHF_MORE_CONFIG     0x0080  /* Received Config Req had More bit set */
#define L2CHF_CONFIG_IND_RCVD 0x0100  /* Config Req Received */
#define L2CHF_CONN_RSP_SENT   0x0200  /* Connect Response sent or requested */
#define L2CHF_NOT_VISIBLE     0x0400  /* App does not know of channel yet */
#define L2CHF_AUTO_CONFIG     0x0800  /* Handle Config Internally */
#define L2CHF_SEND_REQUEST    0x1000  /* Need to send a request (when a packet is available) */
#define L2CHF_SEND_RESPONSE   0x2000  /* Need to send a response (when a packet is available) */

/*
 * Flags used in the 'flags2' field of the L2CAP_Channel structure. 
 */
#if L2CAP_FLOW_CONTROL == XA_ENABLED
#define L2CHF2_MONITOR_ACTIVE   0x0001 /* Monitor timer active */
#define L2CHF2_RETRANS_ACTIVE   0x0002 /* Retransmit timer active */
#define L2CHF2_NO_RETRANS_OUT   0x0004 /* Remote side disabled retransmissions */
#define L2CHF2_NO_RETRANS_IN    0x0008 /* Local user disabled retransmissions */
#define L2CHF2_REJECT_ACTIVE    0x0010 /* Sent a REJECT frame */
#define L2CHF2_SEND_RR          0x0020 /* Send a Receiver Ready S-frame */
#define L2CHF2_SEND_REJECT      0x0040 /* Send a Reject S-frame */
#define L2CHF2_FEATURES_REQ     0x0080 /* Requesting remote features */
#define L2CHF2_FOUND_FC_OPTION  0x0100 /* Found Flow Control Option in config req */
#endif /* L2CAP_FLOW_CONTROL == XA_ENABLED */

/* -- L2CAP States -----------------------------------------------------------
 * 
 * The following states are used by the L2CAP_Channel 'state' field.
 */
/* CLOSED: No channel associated with this CID */
#define L2C_CLOSED         0

/* CLOSING: The channel is releasing all resources before it indicates
 * a disconnect to the upper layer.
 */
#define L2C_CLOSING        1

/* W4_CONN_CNF: An L2CAP_ConnectReq has been sent, waiting for a L2CAP_ConnectRsp. */
#define L2C_W4_CONN_CNF    2

/* W4_CONN_RSP: An L2CA_ConnectInd has been indicated to the upper layer.
 *              Waiting for the upper layer to send the response.
 */
#define L2C_W4_CONN_RSP    3

/* CONFIG: Both sides are negotiating the channel configuration parameters. */
#define L2C_CONFIG         4

/* OPEN: The connection is established and configured, and data may flow. */
#define L2C_OPEN           5

/* W4_DISC_CNF: An L2CAP_DisconnectReq has been sent, waiting for a L2CAP_DisconnectRsp. */
#define L2C_W4_DISC_CNF    6

/* W4_DISC_RSP: An L2CA_DisconnectInd has been indicated to the upper layer.
 *              waiting for the upper layer to send the response.
 */
#define L2C_W4_DISC_RSP    7

/* W4_ECHO_CNF: Special state for echo client, waiting for L2CAP_EchoRsp. */
#define L2C_W4_ECHO_CNF    8

/* W4_INFO_CNF: Special state for info client, waiting for L2CAP_InfoRsp. */
#define L2C_W4_INFO_CNF    9

/* -- General Defines --------------------------------------------------------
 * 
 */
#define L2CAP_HEADER_MIN         4  /* Size in Bytes of minimum L2CAP Header */
#define SIGNAL_HEADER_LEN        4  /* Size in Bytes of a Signal Header */
#define L2CAP_CONS_HEADER_LEN    4  /* Size of L2CAP Connection Oriented header. */

/*---------------------------------------------------------------------------
 * L2CAP_DEFAULT_FLUSH_TIMEOUT constant
 *
 *     Defines the maximum time (in milliseconds) that the Bluetooth
 *     hardware will attempt to send L2CAP data before giving up. Data
 *     packets that cannot be transmitted within this time period are
 *     "flushed". The default value of 0xFFFF indicates that the packets
 *     should be retransmitted indefinitely and never be flushed.
 */
#define L2CAP_DEFAULT_FLUSH_TIMEOUT 0xFFFF

/* -- L2CAP_MAXIMUM_MTU_V12 --------------------------------------------------
 * 
 * Maximum MTU available to BT1.2 channels. The six bytes cover Control, FCS
 * and SDU-length field octets.
 */
#define L2CAP_MAXIMUM_MTU_V12   (0xFFFF - 6)        /* 65,529 */

/*---------------------------------------------------------------------------
 * L2CAP_SIGNAL_DEPTH constant
 *
 *     Number of non-channel based signalling requests that can be
 *     simultaneously handled. If the remote device exceeds this depth, 
 *     non-channel requests may be silently discarded. Non-channel requests
 *     include echo, info and connect reject responses. This parameter is
 *     per-device with a default of 2.
 */
#ifndef L2CAP_SIGNAL_DEPTH
#define L2CAP_SIGNAL_DEPTH      2
#endif

/* -- GetNextIdent Macro -----------------------------------------------------
 * 
 * This macro returns the next available signalling identifier.
 */
#define GetNextIdent()   (L2C(nextIdent) == 0 ? L2C(nextIdent)=2,1 : L2C(nextIdent)++)


/* -- L2CAP Channel Id Macros ------------------------------------------------
 * 
 * These macros and defines are used to convert from ChannelId's to
 * L2CAP_Channel structures. Similar macros are also used for L2capGroups.
 */
#define BASE_DYNAMIC_CID         0x0040
#define LAST_DYNAMIC_CID         (BASE_DYNAMIC_CID + L2CAP_NUM_CHANNELS - 1)
#define CID_TO_INDEX(_CID)       ((_CID) - BASE_DYNAMIC_CID)

#define BASE_GROUP_CID           ((LAST_DYNAMIC_CID+16) & 0xFFF0)
#define LAST_GROUP_CID           (BASE_GROUP_CID + L2CAP_NUM_GROUPS - 1)
#define GROUP_CID_TO_INDEX(_CID) ((_CID) - BASE_GROUP_CID)


/* -- L2CAP Context Structure ------------------------------------------------
 * 
 * This structure provides the context and state information for the entire
 * L2CAP module. The structure is global to all of the files in L2CAP.
 */
typedef struct _BtL2CapContext {
    const L2capPsm      *protocols[L2CAP_NUM_PROTOCOLS];
    L2CAP_Channel        channels[L2CAP_NUM_CHANNELS];   /* Indexed by CID - 0x0040 */
#if L2CAP_NUM_GROUPS > 0
    L2capGroup          *groups[L2CAP_NUM_GROUPS];  /* Indexed by CID - BASE_GROUP_CID */
#endif
    BtRemoteDevice       broadcastLink;
    ListEntry            signalPackets;          /* Queue of packets used for signalling */
    U8                   flags;                  /* L2CAP Module flags */
    U8                   nextIdent;              /* Used for Signal Command Identification */
    U16                  currRxDataLen;          /* length of data pointed to by L2C(currRxData) */
    U8                  *currRxData;             /* pointer to current location in Link RxPacket */
    L2capCallbackParms   callbackParms;          /* Callback information */
    /* Packet pool RAM */
    SignalPacket         _sigPacket[L2CAP_NUM_SIGNAL_PACKETS + L2CAP_SIGNAL_DEPTH];
} BtL2CapContext;


/* -- L2CAP Context Flags ----------------------------------------------------
 *
 * Flags global to the L2CAP Context.
 *
 */
#define L2CCF_BROADCASTS_ENABLED  0x01
#define L2CCF_NEED_SYSPACKET      0x02
#define L2CCF_PROCESS_CHANNELS    0x04

/* -- L2CAP Signal Codes -----------------------------------------------------
 *
 * The following defines are used as events to the L2CAP state machine. The 
 * codes 0x01 through 0x0B match the opcodes of the respective L2CAP signalling
 * commands. The remainder of the defines are for events used internally.
 */
#define LLC_COMMAND_REJ     0x01
#define LLC_CONN_REQ        0x02
#define LLC_CONN_RSP        0x03
#define LLC_CONFIG_REQ      0x04
#define LLC_CONFIG_RSP      0x05
#define LLC_DISC_REQ        0x06
#define LLC_DISC_RSP        0x07
#define LLC_ECHO_REQ        0x08
#define LLC_ECHO_RSP        0x09
#define LLC_INFO_REQ        0x0A
#define LLC_INFO_RSP        0x0B

/* The following events are only used internally */
#define LLC_RTX_EXPIRED     0x10    /* RTX timer expired */
#define LLC_ERTX_EXPIRED    0x11    /* Extended RTX timer expired */
#define LLC_TX_DONE         0x12    /* Transmit done */
#define LLC_ACCESS_RSP      0x13    /* + response to SEC_AccessRequest() */
#define LLC_ACCESS_REJ      0x14    /* - response to SEC_AccessRequest() */
#define LLC_DISCONNECT_IND  0x15    /* ACL Link disconnected */
#define LLC_QOS_VIOL        0x16    /* QoS Violation occurred */
#define LLC_SEND_PACKET     0x17    /* Indicates that a transmit packet is available. */
#define LLC_RETRANS_EXPIRED 0x18    /* Segment retransmit timer expired */
#define LLC_MONITOR_EXPIRED 0x19    /* Monitor timer expired */
#define LLC_LAST_VALUE      0x19

/*
 * MTU used before Configuration starts (it equals the default signalling MTU).
 */
#define L2CAP_MIN_MTU           48

/*
 * L2CAP Configure Packet Response values not included in l2cap.h
 */
#define L2CFG_RESULT_SUCCESS    0x0000   /* Requested values have been accepted */

#define CONFIG_OPTION_HINT      0x80

/* -- L2CAP Link Modes -------------------------------------------------------
 * 
 * The link mode settings used during configuration negotiation. Note that
 * these values must correlate with the L2capLinkMode values used by the API
 * as follows: L2MODE_XXX = (1 << LINK_MODE_XXX)
 */
#define LINK_MODE_BASIC             0
#define LINK_MODE_RETRANSMISSION    1
#define LINK_MODE_FLOW_CONTROL      2


/* -- L2CAP Channel Rx Error Types -------------------------------------------
 * 
 */
#define L2ERR_INVALID_PDU       1   /* Segment type and framing errors */
#define L2ERR_INVALID_MPU       2   /* Length errors, causes channel closure. */
#define L2ERR_INVALID_NR        3   /* N(R) */
#define L2ERR_INVALID_NS        4   /* N(S) */
#define L2ERR_INVALID_FCS       5   /* FCS */
#define L2ERR_INVALID_SAR       6   /* SAR field inappropriate */

/* -- L2MODE_UNKNOWN ---------------------------------------------------------
 *
 * This define is used with BtRemoteDevice->linkModes to describe the condition
 * when an Info request has not been sent to retrieve the available link modes.
 * Typically, an Info Request is not sent until a PSM that uses BT 1.2 extended
 * features (Flow Control/Retransmissions) attempts to establish a connection.
 */
#define L2MODE_UNKNOWN              0x00

/* -- Minimum timeouts -------------------------------------------------------
 *
 * Minimum timeouts are used to protect the local device from having its 
 * processor consumed by L2CAP. This is prudent because the timeout values
 * are specified by the remote device.
 */
#ifndef MINIMUM_MONITOR_TIMEOUT
#define MINIMUM_MONITOR_TIMEOUT     200     /* 200 milliseconds */
#endif

#ifndef MINIMUM_RETRANS_TIMEOUT
#define MINIMUM_RETRANS_TIMEOUT     100     /* 100 milliseconds */
#endif

/* -- WriteSignalHeader Macro  -----------------------------------------------
 *
 * Initializes the Signal header workspace and writes the 4 byte command
 * header. It assumes that the command length is less than 255 bytes. (It
 * only writes the LSB of the length). It can only be used in the  L2Cap_
 * ProcessReq() function because it accesses variables which are local to
 * that function (ws & wsLen).
 */
void WriteSignalHeader(U8 Command, U8 Identifier, U16 Length);

#if TI_CHANGES == XA_ENABLED
#define WriteSignalHeader(_CMD, _ID, _LEN)                          \
      do { Assert((_LEN) <= 0xFF); OS_MemSet(ws, 0, L2SIG_DATA_LEN+BT_PACKET_HEADER_LEN-4); \
           ws[0] = (_CMD); ws[1] = (_ID); ws[2] = (U8)(_LEN);       \
           wsLen = (U16)(4+(_LEN)); } while(0)
#else
#define WriteSignalHeader(_CMD, _ID, _LEN)                          \
      do { Assert((_LEN) <= 0xFF); OS_MemSet(ws, 0, L2SIG_DATA_LEN+BT_PACKET_HEADER_LEN-4); \
           ws[0] = (_CMD); ws[1] = (_ID); ws[2] = (U8)(_LEN);       \
           wsLen = 4+(_LEN); } while(0)
#endif /* TI_CHANGES == XA_ENABLED */


/* -- WriteConfigOption16 Macro  ---------------------------------------------
 *
 * Writes a two byte format confguration option into the specified buffer.
 */
#define WriteConfigOption16(_BUFF, _OPTN, _VAL16) \
           (_BUFF)[0] = (_OPTN);(_BUFF)[1] = 2;StoreLE16((_BUFF)+2, (_VAL16))


/* -- ReturnSysPacket Macro  -------------------------------------------------
 *
 * Returns a system packet, allocated via L2Cap_GetSysPkt() to the system pool.
 */
#define ReturnSysPkt(_PKT)  do {InsertTailList(&L2C(signalPackets), &(_PKT)->node); \
                                XASTAT_PeakDec(l2cap_num_signal_packets, 1);} while (0)

/****************************************************************************
 *
 * Internal function prototypes
 *
 ****************************************************************************/
/* -- From l2cap_utl.c ----------------------------------------------------- */
L2CAP_Channel *L2Cap_GetValidChannel(L2capChannelId Cid);
L2CAP_Channel *L2Cap_FindChannelByLocalIdent(U8 Ident);
L2CAP_Channel *L2Cap_CheckForDupilicateRequest(U8 Ident, BtRemoteDevice *Link);
L2CAP_Channel *L2Cap_AllocLinkChannel(const L2capPsm *Psm, BtRemoteDevice *Device, BtStatus *Error);
L2CAP_Channel *L2Cap_AllocPsmChannel(U16 Psm, U16 *Reason);
BtPacket      *L2Cap_GetSysPkt(BOOL Priority);
#if L2CAP_NUM_GROUPS > 0
L2capGroup    *L2Cap_GetValidGroup(L2capChannelId Cid);
#endif
void     L2Cap_FreeChannel(L2CAP_Channel *);
void     L2Cap_StartRtx(L2CAP_Channel *Channel);
void     L2Cap_StartErtx(L2CAP_Channel *Channel);
void     L2Cap_StopTimer(L2CAP_Channel *Channel);
U16      L2Cap_GetMtu(U8 *Options, U16 OptionLen);
BOOL     L2Cap_ValidateCommand(const U8 *RxData, U16 RxLen);
void     L2Cap_NotifyPsm(L2CAP_Channel *Channel, L2capEventType Event, U16 Status);
#if (XA_ERROR_CHECK == XA_ENABLED) || (XA_DEBUG == XA_ENABLED)
BOOL     L2Cap_IsValidProtocol(const L2capPsm *Protocol);
#endif
void     L2Cap_IssueHciRequestToSend(BtRemoteDevice *Link);
void     L2Cap_FlushChannelTransmits(L2CAP_Channel *Channel);
void     L2Cap_NotifyUserAndCloseChannel(L2CAP_Channel *Channel, U16 Status);

#if L2CAP_FLOW_CONTROL == XA_ENABLED
BOOL L2Cap_ProcessNr(L2CAP_Channel *Channel, U8 Control);
BOOL L2Cap_ProcessNs(L2CAP_Channel *Channel, U8 Control);
void L2Cap_ProcessRbit(L2CAP_Channel *Channel, U8 Control);
void L2Cap_SendSFrame(L2CAP_Channel *Channel);
void L2Cap_StartMonitorTimer(L2CAP_Channel *Channel);
void L2Cap_StartRetransmitTimer(L2CAP_Channel *Channel);
BOOL L2Cap_GetNextSegment(BtPacket *Segment, BtPacket *Sdu);
void L2Cap_BuildIFrameHeader(BtPacket *Segment);
BtPacket *L2Cap_BuildSFrame(L2CAP_Channel *Channel, BtPacket *Packet);
BOOL L2Cap_IsTxWindowAvailable(L2CAP_Channel *Channel);
void L2Cap_ReturnSentPackets(L2CAP_Channel *Channel, BtStatus Status);
void L2Cap_ChannelRxError(L2CAP_Channel *Channel, U8 Error);
U16  L2Cap_ComputeCrc(const U8 *Buffer, U16 Length, U16 Crc);
U16  L2Cap_SetFlowControlOptions(L2CAP_Channel *Channel, const U8 *Options, U16 OptionsLen);
BOOL L2Cap_UpdateRxWindow(L2CAP_Channel *Channel);
void L2Cap_InitFlowControlSettings(L2CAP_Channel *Channel);

/* No need to use InitCrc() since value is always zero. */
#define L2Cap_InitCrc(_VAL)    (0)

#define L2Cap_IsTxWindowAvailable(_CHANNEL) \
        (((((_CHANNEL)->sendSeq - (_CHANNEL)->ackSeq) & 0x3f) < (_CHANNEL)->txWindow) ? TRUE : FALSE)
#endif

/* -- From l2cap.c -------------------------------------------------------- */
BtStatus L2Cap_Send(L2CAP_Channel *Channel, BtPacket *Packet, U16 RemoteCid);
#if ((TI_CHANGES == XA_ENABLED) && (L2CAP_PACKET_PRIORITIZATION == XA_ENABLED))
void     L2Cap_QueueTxPacket(BtRemoteDevice *Link, BtPacket *Packet, HciPriority priority);
#else
void     L2Cap_QueueTxPacket(BtRemoteDevice *Link, BtPacket *Packet);
#endif
BtStatus L2Cap_SendConnRspInternal(L2CAP_Channel *Channel);
#if L2CAP_NUM_GROUPS > 0
BtStatus L2Cap_GroupSend(L2capGroup *Group, BtPacket *Packet);
#endif
BtStatus L2Cap_SendConnReqInternal(L2CAP_Channel *Channel);
BtStatus L2Cap_SendDiscReqInternal(L2CAP_Channel *Channel);
BtStatus L2Cap_SendDiscRspInternal(L2CAP_Channel *Channel);
BtStatus L2Cap_SendConfigReqInternal(L2CAP_Channel *Channel, BtPacket *Req,
                                     L2capConfigFlags Flags);
BtStatus L2Cap_SendConfigRspInternal(L2CAP_Channel *Channel, BtPacket *Rsp,
                                     U16 Result, L2capConfigFlags Flags);

/* -- From l2cap_sm.c ----------------------------------------------------- */
void     L2Cap_StateMachine(L2CAP_Channel *Channel, I8 Event);
void     L2Cap_ProcessReq(U8 *RxData, U16 RxDataLen, BtRemoteDevice *Link);
void     L2Cap_ProcessRsp(U8 *RxData, U16 RxDataLen);

/* -- From l2cap_if.c ----------------------------------------------------- */

/* -- From l2cap tester --------------------------------------------------- */
#if JETTEST == XA_ENABLED
extern void (*Tester_HciReceive)(const HciBuffer *RxBuff, HciHandle Source);
extern void (*Tester_HciTxDone)(BtPacket *Packet, BtStatus Status);
extern void (*Tester_LinkDisconnect)(BtRemoteDevice *Link);
#endif /* JETTEST == XA_ENABLED */

#endif /* __L2CAP_I_H */
