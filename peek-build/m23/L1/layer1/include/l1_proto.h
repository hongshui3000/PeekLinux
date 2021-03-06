/************* Revision Controle System Header *************
 *                  GSM Layer 1 software
 * L1_PROTO.H
 *
 *        Filename l1_proto.h
 *  Copyright 2003 (C) Texas Instruments
 *
 ************* Revision Controle System Header *************/

/**************************************/
/* prototypes of L1_SYNC.C  functions */
/**************************************/
void  hisr                        (void);

void  frit_task                   (UWORD32 argc, void *argv);
void  l1s_task                    (UWORD32 argc, void *argv);
#if FF_L1_IT_DSP_USF
void  usf_task                    (UWORD32 argc, void *argv);
#endif
void  l1s_synch                   (void);

void  l1s_task_scheduler_process  (void);
void  l1s_execute_frame           (void);
void  l1s_meas_manager            (void);
void  l1s_end_manager             (void);
void  l1s_schedule_tasks          (WORD32 *pending_task);
void  l1s_merge_manager           (WORD32 dl_pending_task);

void  l1s_dedicated_mode_manager  (void);
void  l1s_keep_mftab_hist         (void);

/**************************************/
/* prototypes of L1_PWMGR.C functions */
/**************************************/
void  l1s_sleep_manager                    (void);
void  l1s_wakeup                           (void);
void  l1s_wakeup_adjust                    (void);
void  GAUGING_Handler                      (void);
UWORD8 l1s_recover_Os                      (void);
UWORD8 l1s_check_System                    (void);
void  l1s_recover_Frame                    (void);
void l1s_recover_HWTimers                  (void);
BOOL l1s_compute_wakeup_ticks              (void);
void l1s_gauging_task                      (void);
void  l1s_gauging_task_end                 (void);
UWORD32 l1s_get_next_gauging_in_Packet_Idle(void);
void    l1s_adapt_traffic_controller       (void);

/**************************************/
/* prototypes of L1_MFMGR.C functions */
/**************************************/
void  l1s_clear_mftab             (T_FRM *frmlst);
void  l1s_exec_mftab              (void);
void  l1s_load_mftab              (const T_FCT *fct,
                                   const UWORD8 size,
                                   UWORD8 frame,
                                   T_FRM  *frmlst);

/**************************************/
/* prototypes of L1_CMPLX.C functions */
/**************************************/
void  l1s_new_synchro             (UWORD8 param1, UWORD8 param2);
void  l1s_abort                   (UWORD8 param1, UWORD8 param2);
void  l1s_ctrl_ADC                (UWORD8 param1, UWORD8 param2);

void  l1s_ctrl_msagc              (UWORD8 task,   UWORD8 param2);
void  l1s_ctrl_fb                 (UWORD8 param1, UWORD8 param2);
void  l1s_ctrl_fb26               (UWORD8 task,   UWORD8 param2);
void  l1s_ctrl_sbgen              (UWORD8 task,   UWORD8 attempt);
void  l1s_ctrl_sb26               (UWORD8 task,   UWORD8 param2);
#if ((REL99 == 1) && (FF_BHO == 1))
void  l1s_ctrl_fbsb               (UWORD8 task  , UWORD8 param2);
#endif
void  l1s_ctrl_smscb              (UWORD8 task,   UWORD8 burst_id);

void  l1s_ctrl_snb_dl             (UWORD8 task, UWORD8 param2);
void  l1s_ctrl_snb_ul             (UWORD8 task, UWORD8 param2);
void  l1s_ctrl_nnb                (UWORD8 task, UWORD8 param2);

void  l1s_ctrl_rach               (UWORD8 task,   UWORD8 param2);
void  l1s_ctrl_tchtf              (UWORD8 task,   UWORD8 param2);
void  l1s_ctrl_tchth              (UWORD8 task,   UWORD8 param2);
void  l1s_ctrl_tchtd              (UWORD8 task,   UWORD8 param2);
void  l1s_ctrl_tcha               (UWORD8 task,   UWORD8 param2);
void  l1s_hopping_algo            (UWORD8 param1, UWORD8 param2);

void  l1s_ctrl_hwtest             (UWORD8 task, UWORD8 param2);
void  l1s_read_hwtest             (UWORD8 task, UWORD8 param2);

void  l1s_read_dummy              (UWORD8 task, UWORD8 param2);
void  l1s_read_msagc              (UWORD8 task, UWORD8 param2);
void  l1s_read_mon_result         (UWORD8 task, UWORD8 param2);

void  l1s_read_rx_result          (UWORD8 param1, UWORD8 attempt_for_sb2);

void  l1s_read_snb_dl             (UWORD8 task, UWORD8 burst_id);
void  l1s_read_nnb                (UWORD8 task, UWORD8 param);
void  l1s_read_dedic_dl           (UWORD8 task, UWORD8 burst_id);

void  l1s_read_tx_result          (UWORD8 param1, UWORD8 param2);

#if REL99
#if FF_EMR
  void  l1s_read_dedic_scell_meas   (UWORD8 meas,   UWORD8 sub_flag, T_EMR_PARAMS *emr_params);
#endif
#else //REL99
void  l1s_read_dedic_scell_meas   (UWORD8 meas,   UWORD8 sub_flag);
#endif //REL99
void  l1s_dedic_reporting         (void);

void  l1s_read_fb                 (UWORD8 task, UWORD32 fb_flag, UWORD32 toa, UWORD32 attempt,
                                   UWORD32 pm,  UWORD32 angle,   UWORD32 snr);
void  l1s_read_sb                 (UWORD8  task,UWORD32 flag,    API *data, UWORD32 toa, UWORD8 attempt,
                                   UWORD32 pm,  UWORD32 angle,   UWORD32 snr);
#if ((REL99 == 1) && (FF_BHO == 1))
void l1s_read_fbsb                (UWORD8 task, UWORD8 attempt, BOOL fb_flag,  BOOL sb_flag, API *data, 
                                   UWORD32 toa, UWORD32 pm, UWORD32 angle, UWORD32 snr);
#endif
void  l1s_read_sacch_dl           (API *info_address, UWORD32 task_rx);
void  l1s_read_dcch_dl            (API *info_address, UWORD32 task_rx);
void  l1s_read_l3frm              (UWORD8 pwr_level, API *info_address, UWORD32 task_rx);
void l1s_reset_tx_ptr(UWORD8 param1, UWORD8 param2);


/**************************************/
/* prototypes of L1_AFUNC functions   */
/**************************************/
void           l1a_reset_full_list            (void);
void           l1a_reset_ba_list              (void);
void           l1a_add_time_for_nb            (UWORD32 *time_alignmt, UWORD32 *fn_offset);
void           l1a_add_timeslot               (UWORD32 *time_alignmt, UWORD32 *fn_offset, UWORD8 tn);
void           l1a_sub_time_for_nb            (UWORD32 *time_alignmt, UWORD32 *fn_offset);
void           l1a_sub_timeslot               (UWORD32 *time_alignmt, UWORD32 *fn_offset, UWORD8 tn);

T_DEDIC_SET   *l1a_get_free_dedic_set         (void);
void           l1a_fill_bef_sti_param         (T_DEDIC_SET *set_ptr, BOOL start_time_present);
WORD32         l1a_decode_starting_time       (T_STARTING_TIME coded_starting_time);
void           l1a_reset_cell_info            (T_CELL_INFO *cell_info);
void           l1a_send_confirmation          (UWORD32 SignalCode,UWORD8 queue_type);
void           l1a_send_result                (UWORD32 SignalCode, xSignalHeaderRec *msg, UWORD8 queue);
UWORD8         l1a_encode_rxqual              (UWORD32 inlevel);
void           l1a_report_failling_ncell_sync (UWORD32 SignalCode, UWORD8 neigh_id);
UWORD8         l1a_clip_txpwr                 (UWORD8 supplied_txpwr, UWORD16 radio_freq);
void         l1a_add_time_delta             (UWORD32 *time_alignmt, UWORD32 *fn_offset, WORD32 delta);
void         l1a_compensate_sync_ind        (T_MPHC_NCELL_SYNC_IND * msg);
void         l1a_compute_Eotd_data          (UWORD8 *first_scell, UWORD8 neigh_id, UWORD32 SignalCode, xSignalHeaderRec *msg);
void         l1a_correct_timing             (UWORD8 neigh_id,UWORD32 time_alignmt,UWORD32 fn_offset);

/**************************************/
/* prototypes of L1_FUNC functions    */
/**************************************/
void            dsp_power_on                (void);
#if ((ANLG_FAM == 1) || (ANLG_FAM == 2) || (ANLG_FAM == 3) || (ANLG_FAM == 11))
   void         l1_abb_power_on              (void);
#endif
void            tpu_init                    (void);

void            l1s_reset_db_mcu_to_dsp     (T_DB_MCU_TO_DSP *page_ptr);
#if (DSP == 38) || (DSP == 39)
void            l1s_reset_db_common_mcu_to_dsp   (T_DB_COMMON_MCU_TO_DSP *page_ptr);
#endif
void            l1s_reset_db_dsp_to_mcu     (T_DB_DSP_TO_MCU *page_ptr);
void            initialize_l1var            (void);
void            l1_initialize               (T_MMI_L1_CONFIG *mmi_l1_config);
void            l1_pwr_mgt_init             (void);
void            l1_dpll_init_var            (void);

void            l1s_increment_time          (T_TIME_INFO *time, UWORD32 fn_offset);
WORD16          l1s_encode_rxlev            (UWORD8 inlevel);
void            l1s_send_ho_finished        (UWORD8  cause);
void            l1s_reset_dedic_serving_meas(void);
UWORD32         l1s_swap_iq_dl              (UWORD16 radio_freq, UWORD8 task);
UWORD32         l1s_swap_iq_ul              (UWORD16 radio_freq, UWORD8 task);
UWORD8          l1s_ADC_decision_on_NP      (void);
#if (AMR == 1)
UWORD8          l1s_amr_get_ratscch_type    (API *a_ratscch_dl);
void            l1s_amr_update_from_ratscch (API *a_ratscch_dl);
#endif
void            l1_memcpy_16bit(void *dst,void* src,unsigned int len);


/**************************************/
/* prototypes of L1_DRIVE functions   */
/**************************************/
// MCU-DSP interface drivers.
//---------------------------

#if (FF_L1_FAST_DECODING == 1)
void l1ddsp_load_fast_dec_task(API task, UWORD8 burst_id);
#endif 
void   l1ddsp_load_info           (UWORD32   task,
                                   API       *info_ptr,
                                   UWORD8    *data);
void   l1ddsp_load_monit_task     (API       monit_task,
                                   API       fb_mode);
void   l1ddsp_load_afc            (API       afc);
void   l1ddsp_load_rx_task        (API       rx_task,
                                   UWORD8    burst_id,
                                   UWORD8    tsq);
void   l1ddsp_load_tx_task        (API       tx_task,
                                   UWORD8    burst_id,
                                   UWORD8    tsq);
void   l1ddsp_load_ra_task        (API       ra_task);


void   l1ddsp_load_txpwr          (UWORD8           txpwr,
                                   UWORD16          radio_freq);

// SAIC low level driver function
#if (L1_SAIC != 0)
void   l1ddsp_load_swh_flag       (UWORD16   SWH_flag, UWORD16 SAIC_flag);
#endif

void l1ddsp_read_iq_dump(UWORD8 task);
void l1ddsp_apc_load_apcctrl2(UWORD16 apcctrl2);
void l1ddsp_apc_set_manual_mode(void);
void l1ddsp_apc_set_automatic_mode(void);

#ifdef TESTMODE
  void l1ddsp_apc_load_apclev(UWORD16 apclev);
#endif  

#if (AMR == 1)
  #if (FF_L1_TCH_VOCODER_CONTROL == 1)
    // Add the AMR synchro bit in the driver's paramters
    void   l1ddsp_load_tch_param      (T_TIME_INFO      *next_time,
                                       UWORD8           chan_mode,
                                       UWORD8           chan_type,
                                       UWORD8           subchannel,
                                       UWORD8           tch_loop,
                                       UWORD8           sync_tch,
                                       UWORD8           sync_amr,
                                       UWORD8           reset_sacch,
                                     #if !FF_L1_IT_DSP_DTX           
                                       UWORD8           vocoder_on);
  #else
                                       UWORD8           vocoder_on,
                                       BOOL             dtx_dsp_interrupt);
                                     #endif
  #else
    void   l1ddsp_load_tch_param      (T_TIME_INFO      *next_time,
                                       UWORD8           chan_mode,
                                       UWORD8           chan_type,
                                       UWORD8           subchannel,
                                       UWORD8           tch_loop,
                                       UWORD8           sync_tch,
                                     #if !FF_L1_IT_DSP_DTX           
                                       UWORD8           sync_amr);
                                     #else
                                       UWORD8           sync_amr,
                                       BOOL             dtx_dsp_interrupt);
                                     #endif
  #endif
#else
  #if (FF_L1_TCH_VOCODER_CONTROL == 1)
    void   l1ddsp_load_tch_param      (T_TIME_INFO      *next_time,
                                       UWORD8           chan_mode,
                                       UWORD8           chan_type,
                                       UWORD8           subchannel,
                                       UWORD8           tch_loop,
                                       UWORD8           sync_tch,
                                       UWORD8           reset_sacch,
                                     #if !FF_L1_IT_DSP_DTX           
                                       UWORD8           vocoder_on);
  #else
                                       UWORD8           vocoder_on,
                                       BOOL             dtx_dsp_interrupt);
                                     #endif
  #else
    void   l1ddsp_load_tch_param      (T_TIME_INFO      *next_time,
                                       UWORD8           chan_mode,
                                       UWORD8           chan_type,
                                       UWORD8           subchannel,
                                       UWORD8           tch_loop,
                                     #if !FF_L1_IT_DSP_DTX           
                                       UWORD8           sync_tch);
                                     #else
                                       UWORD8           sync_tch,
                                       BOOL             dtx_dsp_interrupt);
                                     #endif
  #endif
#endif

#if (L1_VOCODER_IF_CHANGE == 0)
BOOL enable_tch_vocoder               (BOOL             vocoder_on);
#endif // L1_VOCODER_IF_CHANGE == 0
BOOL   l1_select_mcsi_port            (UWORD8           port);

void   l1ddsp_load_ciph_param         (UWORD8           a5mode,
                                       T_ENCRYPTION_KEY *ciph_key);
void   l1ddsp_load_tch_mode           (UWORD8           dai_mode,
                                       BOOL             dtx_allowed);
#if (AMR == 1)
  void l1ddsp_load_amr_param          (T_AMR_CONFIGURATION amr_param, UWORD8 cmip);
#endif
void   l1ddsp_stop_tch                (void);
void l1ddsp_load_afc                  (API afc);

// MCU-TPU interface drivers.
//---------------------------
void l1dtpu_meas                  (UWORD16  radio_freq,
                                   WORD8    agc,
                                   UWORD8   lna_off,
                                   UWORD16   win_id,
                                   UWORD16  tpu_synchro,
                                   UWORD8   adc_active
#if(RF_FAM == 61)
                                 ,UWORD8 afc_mode
                                 ,UWORD8 if_ctl
#endif
                                                 );
void l1dtpu_neig_fb               (UWORD16  radio_freq,
                                   WORD8    agc,
                                   UWORD8   lna_off);
void l1dtpu_neig_fb26             (UWORD16  radio_freq,
                                   WORD8    agc,
                                   UWORD8   lna_off,
                                   UWORD32  offset_serv);
void l1dtpu_neig_sb               (UWORD16  radio_freq,
                                   WORD8    agc,
                                   UWORD8   lna_off,
                                   UWORD32  time_alignmt,
                                   UWORD32  offset_serv,
                                   UWORD8   reload_flag,
                                   UWORD8   attempt
#if(RF_FAM == 61)
                                 ,UWORD8 if_ctl
#endif
                                                       );
void l1dtpu_neig_sb26             (UWORD16  radio_freq,
                                   WORD8    agc,
                                   UWORD8   lna_off,
                                   UWORD32  time_alignmt,
                                   UWORD32  fn_offset,
                                   UWORD32  offset_serv
#if(RF_FAM == 61)
                                  ,UWORD8 if_ctl
#endif
                                                      );
void l1dtpu_serv_rx_nb            (UWORD16  radio_freq,
                                   WORD8    agc,
                                   UWORD8   lna_off,
                                   UWORD32  synchro_serv,
                                   UWORD32  new_offset,
                                   BOOL     change_offset,
                                   UWORD8   adc_active
#if(RF_FAM == 61)                                   
                                  ,UWORD8   csf_filter_choice
                                  ,UWORD8 if_ctl
#endif
#if (NEW_SNR_THRESHOLD == 1)
                                  ,UWORD8 saic_flag
#endif /* NEW_SNR_THRESHOLD*/
                                                        );
void l1dtpu_serv_tx_nb            (UWORD16  radio_freq,
                                   UWORD8   timing_advance,
                                   UWORD32  offset_serv,
                                   UWORD8   txpwr,
                                   UWORD8   adc_active);
void l1dtpu_neig_rx_nb            (UWORD16  radio_freq,
                                   WORD8    agc,
                                   UWORD8   lna_off,
                                   UWORD32  time_alignmt,
                                   UWORD32  offset_serv,
                                   UWORD8   reload_flag,
                                   UWORD8   nop
#if(RF_FAM == 61)
                                  ,UWORD8 if_ctl
#endif
#if (NEW_SNR_THRESHOLD == 1)
                                  ,UWORD8 saic_flag
#endif /* NEW_SNR_THRESHOLD*/
                                                      );

void l1dtpu_serv_tx_ra            (UWORD16  radio_freq,
                                   UWORD32  offset_serv,
                                   UWORD8   txpwr,
                                   UWORD8   adc_active);

// MCU-DSP interface drivers for POWER-ON.
//----------------------------------------
void l1dtpu_init_dpram            (UWORD8   process);

// MCU-DSP interface drivers for RESET.
//-------------------------------------
void l1ddsp_end_scenario          (UWORD8 type);
void l1dtpu_end_scenario          (void);
void l1d_reset_hw                 (UWORD32 offset_value);



/**************************************/
/* Prototypes for L1 ASYNCH task      */
/**************************************/

void  l1a_task                                   (UWORD32 argc, void *argv);
void  l1a_balance_l1a_tasks                      (void);

void  l1a_mmi_adc_req                            (xSignalHeaderRec *msg);
void  l1a_network_lost                           (xSignalHeaderRec *msg);
void  l1a_idle_6strongest_monitoring_process     (xSignalHeaderRec *msg);
void  l1a_idle_serving_cell_bcch_reading_process (xSignalHeaderRec *msg);
void  l1a_idle_serving_cell_paging_process       (xSignalHeaderRec *msg);
void  l1a_idle_smscb_process                     (xSignalHeaderRec *msg);
void  l1a_initial_network_sync_process           (xSignalHeaderRec *msg);
void  l1a_cres_process                           (xSignalHeaderRec *msg);
void  l1a_dedic_ba_list_meas_process             (xSignalHeaderRec *msg);

void  l1a_full_list_meas_process      (xSignalHeaderRec *msg);
void  l1a_csel_bcch_process           (xSignalHeaderRec *msg);

void  l1a_idle_serv_meas_process      (xSignalHeaderRec *msg);

void  l1a_idle_neigh_meas_process     (xSignalHeaderRec *msg);
void  l1a_idle_neigh_full_bcch_process(xSignalHeaderRec *msg);
void  l1a_idle_neigh_norm_bcch_process(xSignalHeaderRec *msg);
void  l1a_idle_neigh_ext_bcch_process (xSignalHeaderRec *msg);
void  l1a_idle_6conf_process          (xSignalHeaderRec *msg);

void  l1a_idle_smscb_process          (xSignalHeaderRec *msg);

void  l1a_access_process              (xSignalHeaderRec *msg);
void  l1a_dedicated_process           (xSignalHeaderRec *msg);
void  l1a_dedic_bcch_process          (xSignalHeaderRec *msg);
void  l1a_dedic6_process              (xSignalHeaderRec *msg);
void  l1a_dedic_neigh_meas_process    (xSignalHeaderRec *msg);

void  l1a_idle_ba_list_meas_process   (xSignalHeaderRec *msg);
void  l1a_idle_full_list_meas_process (xSignalHeaderRec *msg);
void  l1a_test_process                (xSignalHeaderRec *msg);
void  l1a_freq_band_configuration     (xSignalHeaderRec *msg);
void  l1a_at_power_process            (xSignalHeaderRec *msg);    

#if(L1_CHECK_COMPATIBLE == 1)
void l1a_checkmsg_compatibility    (xSignalHeaderRec *msg);
#endif
#if (OP_L1_STANDALONE == 1)
// Dynamic configuration process for L1 standalone only
  void   l1a_test_config_process        (xSignalHeaderRec *msg);
#endif

// ...................NEW FOR ALR....................
void  l1a_neighbour_cell_bcch_reading_process (xSignalHeaderRec *msg);
// ...................NEW FOR ALR....................

/**************************************/
/* Prototypes for l3 task             */
/**************************************/
void  l3_task                         (UWORD32 argc, void *argv);
void  l3_expire_fct                   (UWORD32 id);

#if TESTMODE
  void  mmi_task                      (UWORD32 argc, void *argv);
#endif





/**************************************/
/* Prototypes for Nu_main.            */
/**************************************/
void               Adc_timer       (UWORD32 id);

/**************************************/
/* Prototypes for l2 task             */
/**************************************/
T_RADIO_FRAME  *dll_read_dcch     (UWORD8  chn_mode);
T_RADIO_FRAME  *dll_read_sacch    (UWORD8  chn_mode);
void           l2_task            (UWORD32 argc, void *argv);

#if (DSP_BACKGROUND_TASKS == 1)
// Task for backgrounds DSP testing
void background_task(UWORD32 argc, void *argv);
#endif

void           rx_tch_data        (API     *data_address,
                                   UWORD8  channel_mode,
                                   UWORD8  blk_seq_number);
UWORD8           *tx_tch_data     (void);

#if (SEND_FN_TO_L2_IN_DCCH==1)
#if (L1_SAGEM_INTERFACE == 1)
void           dll_dcch_downlink  (API     *info_address,
                                   UWORD8  valid_flag, 
                                   UWORD32 frame_number,
				   UWORD8 channel_type);
#else
void           dll_dcch_downlink  (API     *info_address,
                                   UWORD8  valid_flag, 
                                   UWORD32 frame_number);
#endif
#else
#if (L1_SAGEM_INTERFACE == 1)
void           dll_dcch_downlink  (API     *info_address,
                                   UWORD8  valid_flag,
				   UWORD8  channel_type);
#else
void           dll_dcch_downlink  (API     *info_address,
                                   UWORD8  valid_flag);
#endif
#endif

/***************************************/
/* Prototypes of L1_TRACE.c functions  */
/***************************************/
void l1_trace_message          (xSignalHeaderRec *msg);
void send_debug_sig            (UWORD8 debug_code, UWORD8 task);
void l1_trace_cpu_load         (UWORD8 cpu_load);

#if (TRACE_TYPE==7) // CPU_LOAD
void l1_cpu_load_start          (void);
void l1_cpu_load_stop           (void);
void l1_cpu_load_init           (void);
void l1_cpu_load_interm         (void);
#endif

/***************************************/
/* Prototypes of HW_DEBUG.c functions  */
/***************************************/
void  wait_for_next_message       (CHAR *);

/***************************************/
/* Prototypes of L1_DEBUG.c functions  */
/***************************************/
void scenario_and_log_files   (void);
void decode_msg               (xSignalHeaderRec *msg, CHAR *filename);
void trace_mft                (CHAR *fct_name, WORD32 frame);
#if (L1_EOTD ==1)
  void trace_EOTD_serving     (UWORD16 arfcn, UWORD32 timetag, CHAR *text);
  void trace_EOTD_serving1    (CHAR *text);
  void trace_EOTD_neighbour   (UWORD8 nbr, UWORD16 arfcn, UWORD32 delta_fn, WORD32 delta_qbit,
                                 UWORD32 timetag, CHAR *text);
#endif
void trace_ULPD               (CHAR *text, UWORD32 frame_number);
void log_fct                  (CHAR *fct_name, UWORD32 radio_freq);
void trace_msg                (CHAR *msg_name, CHAR *queue_name);
void log_msg                  (CHAR *msg_name, CHAR *queue_name);
void trace_dedic              (void);
void trace_fct_simu           (CHAR *fct_name, UWORD32 radio_freq);
void trace_flowchart_msg      (CHAR *msg_name, CHAR *dest_queue_name);
void trace_flowchart_l1tsk    (UWORD32 bit_register, UWORD32 *src_register_set);
void trace_flowchart_dedic    (WORD32 SignalCode);
void trace_flowchart_tpu      (CHAR *task_name);
void trace_flowchart_dsp      (CHAR *task_name);
void trace_flowchart_dsp_tpu  (CHAR *task_name);
void trace_flowchart_dspres   (CHAR *task_name);
void trace_flowchart_dsptx    (CHAR *task_name);
void trace_flowchart_header   (void);
void trace_sim_freq_band_configuration  (UWORD8 freq_band_config);
#if (TOA_ALGO == 2)
  void trace_toa_sim_ctrl   (UWORD16 SNR_val, UWORD16 TOA_val, UWORD32 l1_mode,
                             UWORD32 frames_counter, UWORD32 cumul_counter,WORD16 cumul);
  void trace_toa_sim_update (WORD16 toa_shift, UWORD32 tpu_offset);
#endif
#if (L1_SAIC != 0)
  void trace_saic_sim (UWORD32 Il_for_rxlev, UWORD32 l1_mode, UWORD32 SWH_flag);
#endif

/**************************************/
/* prototypes of control functions    */
/**************************************/
#if (VCXO_ALGO == 0)
WORD16 l1ctl_afc                 (UWORD8  phase,
                                 UWORD32  *frame_count,
                                 WORD16   angle,
                                 WORD32   snr,
                                 UWORD16  radio_freq);
#else
WORD16 l1ctl_afc                 (UWORD8  phase,
                                 UWORD32  *frame_count,
                                 WORD16   angle,
                                 WORD32   snr,
                                 UWORD16  radio_freq,
                                 UWORD32  l1_mode);
#endif

#if (TOA_ALGO == 2)
WORD16 l1ctl_toa                 (UWORD8 phase, 
                                  UWORD32 l1_mode,
                                  UWORD16 SNR_val,
                                  UWORD16 TOA_val);
#else
WORD16 l1ctl_toa                 (UWORD8  phase,
                                 UWORD32  l1_mode,
                                 UWORD16  SNR_val,
                                 UWORD16  TOA_val,
                                 BOOL     *toa_update,
                                 UWORD16  *toa_period_count
#if (FF_L1_FAST_DECODING == 1)                                 
                                ,UWORD8   skipped_values
#endif /* FF_L1_FAST_DECODING */                                
                                 );
#endif

// SAIC Control Function
#if (L1_SAIC != 0)
UWORD8 l1ctl_saic                (UWORD8  IL_for_rxlev,
                                  UWORD32 l1_mode
#if (NEW_SNR_THRESHOLD == 1)
                                  ,UWORD8  task,
                                  UWORD8   * saic_flag
#endif /* NEW_SNR_THRESHOLD*/
);
#endif

UWORD8 l1ctl_txpwr             (UWORD8    target_txpwr,
                                 UWORD8   current_txpwr);

// Utility for agc control algorithms

void l1ctl_encode_lna  (UWORD8  input_level,
                        UWORD8  * lna_state,
                        UWORD16 radio_freq);
UWORD8 l1ctl_find_max  (UWORD8  *buff,
                        UWORD8  buffer_len);

// Automatic Gain Control Algorithms
void l1ctl_pgc2          (UWORD8        pm_high_agc,
                          UWORD8        pm_low_agc,
                          UWORD16       radio_freq);
UWORD8 l1ctl_csgc        (UWORD8        pm,
                          UWORD16       radio_freq);
UWORD8 l1ctl_pgc         (UWORD8        pm,
                          UWORD8        used_IL,
                          UWORD8        lna_off,
                          UWORD16       radio_freq);
UWORD8 l1ctl_pagc        (UWORD8        pm,
                          UWORD16       radio_freq,
                          T_INPUT_LEVEL *traffic_meas_ptr);
UWORD8 l1ctl_dpagc       (BOOL          dtx_on,
                          BOOL          beacon,
                          UWORD8        pm,
                          UWORD16       radio_freq,
                          T_INPUT_LEVEL *traffic_meas_ptr);
#if (AMR == 1)
  UWORD8 l1ctl_dpagc_amr   (BOOL          dtx_on,
                            BOOL          beacon,
                            UWORD8        pm,
                            UWORD16       radio_freq,
                            T_INPUT_LEVEL *traffic_meas_ptr);
#endif

UWORD16 l1ctl_get_g_magic (UWORD16 radio_freq);
UWORD16 l1ctl_get_lna_att (UWORD16 radio_freq);
UWORD16 l1ctl_update_TPU_with_toa(void);

#if (FF_L1_FAST_DECODING == 1)
void l1ctl_pagc_missing_bursts (UWORD8 skipped_values);
#endif  

//functions for customization
void Cust_init_std         (void);
void Cust_init_params      (void);
WORD8 Cust_get_agc_from_IL (UWORD16 radio_freq, UWORD16 agc_index, UWORD8 table_id,UWORD8 lna_off_val);
WORD8 l1ctl_encode_delta1  (UWORD16 radio_freq);
WORD8 l1ctl_encode_delta2  (UWORD16 radio_freq);
void Cust_get_ramp_tab     (API *a_ramp, UWORD8 txpwr_ramp_up, UWORD8 txpwr_ramp_down, UWORD16 radio_freq);
#if ((ANLG_FAM == 1) || (ANLG_FAM == 2) || (ANLG_FAM == 3) || (RF_FAM == 61))
  UWORD16 Cust_get_pwr_data(UWORD8 txpwr, UWORD16 radio_freq
  										  #if(REL99 && FF_PRF)
  										  ,UWORD8 number_uplink_timeslot
  										  #endif
  										  );
#endif

#if (RF_FAM == 61)
void l1_drp_wrapper_init (void);
void l1_drp_init (void);
void l1dapc_init_ramp_tables(void );
void   cust_get_if_dco_ctl_algo (UWORD16* dco_algo_ctl, UWORD8* if_ctl,  
  UWORD8 input_level_flag, UWORD8 input_level, UWORD16 radio_freq, UWORD8 if_threshold);
#endif

void l1s_restore_synchro(void);

#if (FF_L1_FAST_DECODING == 1)
BOOL l1s_check_deferred_control(UWORD8 task, UWORD8 burst_id);
BOOL l1s_check_fast_decoding_authorized(UWORD8 task);
#endif /* FF_L1_FAST_DECODING */

#if (DRP_FW_EXT == 1)
void l1_get_boot_result_and_version(T_L1_BOOT_VERSION_CODE * p_version);
#endif /* DRP_FW_EXT */

/*-----------------------------------------------------------------*/
/* Prototypes of MULTIBAND related functions                       */
/*-----------------------------------------------------------------*/
#if (L1_FF_MULTIBAND == 1)
#if 0
UWORD16 l1_multiband_radio_freq_convert_into_operative_radio_freq(UWORD16 radio_freq);
UWORD8 l1_multiband_radio_freq_convert_into_physical_band_id(UWORD16 radio_freq);
UWORD8  l1_multiband_radio_freq_convert_into_effective_band_id(UWORD16 radio_freq);
void l1_multiband_fill_power_meas_array(UWORD16 power_array_size, T_FULL_LIST_MEAS *full_list);
UWORD8 l1_multiband_map_radio_freq_into_tpu_table(UWORD16 radio_freq);
void l1_multiband_increment_effective_band_id(UWORD8 *effective_band_id);
void l1_multiband_trace_params(UWORD8 multiband_table_id, UWORD8 multiband_trace_id) ;
void l1_multiband_tpu_get_power_classes(T_L1_MULTIBAND_POWER_CLASS multiband_power_class[]);
void l1_multiband_error_handler(UWORD16 radio_freq);
#endif // if 0

#endif /*if(L1_FF_MULTIBAND == 1)*/


