

#ifndef BALERRS_H
#define BALERRS_H


/*------------------------------------------------------------------------
*  The following definitions are fault ids for MonFault routine.
*-----------------------------------------------------------------------*/

typedef enum 
{
   BAL_MSG_ID_ERR = 0,
   BAL_KEY_ID_ERR,
   BAL_STATE_ERR,
   BAL_STARTUP_ERR,
   BAL_DATABASES_NOT_PROG_ERR = 4,
   BAL_STARTUP_WITHOUT_DSPV_ERR,
   
   /* voice memo */
   BAL_ERR_VMEM_BUFFER_FULL = 0x10,
   BAL_ERR_VMEM_CANNOT_DO_WRONG_STATE,
   BAL_ERR_VMEM_RINGER_ACTIVE,
   BAL_ERR_VMEM_BUFFER_EMPTY,
   BAL_ERR_VMEM_ERR_MAKE_DIR,
   BAL_ERR_VMEM_ERR_OPEN_FILE,
   BAL_ERR_VMEM_ERR_WRITE_FILE,
   BAL_ERR_VMEM_ERR_READ_FILE,
   BAL_ERR_VMEM_GET_SPACE_ERR,
   BAL_ERR_VMEM_DEVICE_FULL_ERR,
   BAL_ERR_VMEM_INVALID_NUM_PKTS,
   
   /* voice recognition */
   BAL_ERR_VREC_INVALID_STATE = 0x20,
   BAL_ERR_VREC_INVALID_MSG_FOR_STATE,
   BAL_ERR_VREC_OPERATION_TIMEOUT,
   BAL_ERR_VREC_SPURIOUS_TIMEOUT,
   BAL_ERR_VREC_SECTOR_FULL,
   BAL_ERR_VREC_VOCABULARY_FULL,
   BAL_ERR_VREC_ENGINE_ERR,
   BAL_ERR_VREC_COMPRESSED_BUFFER_FULL,
   BAL_ERR_VREC_TEMPLATE_BUFFER_FULL,
   BAL_ERR_VREC_VOCAB_EMPTY,
   BAL_ERR_VREC_NAMETAG_NOT_FOUND,
   BAL_ERR_VREC_TRAIN_BUFFER_TOO_SMALL,
   
   /* Audio errors */
   BAL_ERR_AUDIO_NOT_INITIALIZED = 0x35,
   BAL_ERR_AUDIO_WRONG_VOLUME_MODE,
   BAL_ERR_AUDIO_WRONG_TONE_TYPE,
   BAL_ERR_AUDIO_NO_DATA,
   BAL_ERR_AUDIO_FILE_READ_FAIL,
   BAL_ERR_AUDIO_WRONG_DEVICE_MODE,
   BAL_ERR_AUDIO_MUSIC_MALLOC_FAIL,
   BAL_ERR_AUDIO_MUSIC_NOT_ENOUGH_MEM,

   /* FSI errors */
   BAL_ERR_FSI_INIT_FAILED = 0x45,
   BAL_ERR_FSI_DOS_FS_INIT_FAIL,
   BAL_ERR_FSI_DOS_FS_EXIT_FAIL,
   
   /* SMS errors */
   BAL_ERR_SMS_NOT_READY_ERR   = 0x70,
   BAL_ERR_SMS_FILE_CREATE_ERR,
   BAL_ERR_SMS_FILE_READ_ERR,
   BAL_ERR_SMS_FILE_WRITE_ERR,

   /* Display errors */
   BAL_ERR_DISP_UNEXPECT_CALLBACK_ERR = 0x80,
   BAL_ERR_DISP_EVENT_ALREADY_ACTIVE,
   BAL_ERR_DISP_IMAGE_WIDTH_INVALID,
   BAL_ERR_DISP_FILE_READ_ERR,
   BAL_ERR_DISP_FILE_WRITE_ERR,
   BAL_ERR_DISP_IMAGE_TYPE_INVALID,
   BAL_ERR_DISP_FILE_HANDLE_INVALID,
   BAL_ERR_DISP_MALLOC_FAIL,
   BAL_ERR_DISP_NOT_ENOUGH_MEM,
   BAL_ERR_DISP_IMAGE_NOT_STORED,

   /* Last entry place holder */
   BAL_ERR_MAX_NUM_ERR_CODES
} BalErrsT;


#endif
