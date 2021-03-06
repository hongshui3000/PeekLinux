/*=============================================================================
 *    Copyright 2004 Texas Instruments Inc. All rights reserved.
 */
#ifndef BSP_TWL3029_PWR_MAP_HEADER
#define BSP_TWL3029_PWR_MAP_HEADER

#if (ANLG_PG == 1)
#include "pg1x/bspTwl3029_Pwr_Map_pg1x.h"
#elif (ANLG_PG == 2)
#include "pg2x/bspTwl3029_Pwr_Map_pg2x.h"
#endif
/*=============================================================================
 * Component Description:
 *    Register map for Triton Analog Base Band processor - module barpwr.
 *    Map for all Triton power related registers and RTC registers
 */

/*=============================================================================
 * Constants
 */

/*=============================================================================
 */
#define BSP_TWL_3029_MAP_RTC_SECOND_REG_OFFSET         0    /* RTC: SECOND_REG   */  
#define BSP_TWL_3029_MAP_RTC_MINUTES_REG_OFFSET        1    /* RTC: MINUTES_REG   */  
#define BSP_TWL_3029_MAP_RTC_HOURS_REG_OFFSET          2    /* RTC: HOURS_REG   */  
#define BSP_TWL_3029_MAP_RTC_DAYS_REG_OFFSET           3    /* RTC: DAYS_REG   */  
#define BSP_TWL_3029_MAP_RTC_MONTHS_REG_OFFSET         4    /* RTC: MONTHS_REG   */  
#define BSP_TWL_3029_MAP_RTC_YEARS_REG_OFFSET          5    /* RTC: YEARS_REG   */  
#define BSP_TWL_3029_MAP_RTC_WEEK_REG_OFFSET           6    /* RTC: WEEK_REG   */  
#define BSP_TWL_3029_MAP_RTC_RESERVED_1_OFFSET         7    /* RTC: RESERVED   */  
#define BSP_TWL_3029_MAP_RTC_ALARM_SECONDS_REG_OFFSET  8    /* RTC: ALARM_SECONDS_REG   */  
#define BSP_TWL_3029_MAP_RTC_ALARM_MINUTES_REG_OFFSET  9    /* RTC: ALARM_MINUTES_REG   */  
#define BSP_TWL_3029_MAP_RTC_ALARM_HOURS_REG_OFFSET    10   /* RTC: ALARM_HOURS_REG   */  
#define BSP_TWL_3029_MAP_RTC_ALARM_DAYS_REG_OFFSET     11   /* RTC: ALARM_DAYS_REG   */  
#define BSP_TWL_3029_MAP_RTC_ALARM_MONTHS_REG_OFFSET   12   /* RTC: ALARM_MONTHS_REG   */  
#define BSP_TWL_3029_MAP_RTC_ALARM_YEARS_REG_OFFSET    13   /* RTC: ALARM_YEARS_REG   */  
#define BSP_TWL_3029_MAP_RTC_RESERVED_2_OFFSET         14   /* RTC: RESERVED */
#define BSP_TWL_3029_MAP_RTC_RESERVED_3_OFFSET         15   /* RTC: RESERVED  */
#define BSP_TWL_3029_MAP_RTC_RTC_CTRL_REG_OFFSET       16   /* RTC: RTC_CTRL_REG */
#define BSP_TWL_3029_MAP_RTC_RTC_STATUS_REG_OFFSET     17   /* RTC: RTC_STATUS_REG */
#define BSP_TWL_3029_MAP_RTC_RTC_INTERRUPTS_REG_OFFSET 18   /* RTC: RTC_INTERRUPTS_REG */
#define BSP_TWL_3029_MAP_RTC_RTC_COMP_LSB_REG_OFFSET   19   /* RTC: RTC_COMP_LSB_REG */
#define BSP_TWL_3029_MAP_RTC_RTC_COMP_MSB_REG_OFFSET   20   /* RTC: RTC_COMP_MSB_REG */
#define BSP_TWL_3029_MAP_RTC_RTC_RES_PROG_REG_OFFSET   21   /* RTC: RTC_RES_PROG_REG */
#define BSP_TWL_3029_MAP_RTC_RTC_RESET_STATUS_REG_OFFSET  22   /* RTC: RTC_RESET_STATUS_REG */

#define BSP_TWL_3029_MAP_PMC_MASTER_GP_CFG_OFFSET              23   /*  PMC_MASTER:  GP_CFG */
#define BSP_TWL_3029_MAP_PMC_MASTER_CFG_PU_PD_MSB_OFFSET       24   /*  PMC_MASTER:  CFG_PU_PD_MSB */
#define BSP_TWL_3029_MAP_PMC_MASTER_CFG_PU_PD_LSB_OFFSET       25   /*  PMC_MASTER:  CFG_PU_PD_LSB */
#define BSP_TWL_3029_MAP_PMC_MASTER_P1_CFG_TRANSITION_OFFSET   26   /*  PMC_MASTER:  P1_CFG_TRANSITION */
#define BSP_TWL_3029_MAP_PMC_MASTER_P2_CFG_TRANSITION_OFFSET   27   /*  PMC_MASTER:  P2_CFG_TRANSITION */
#define BSP_TWL_3029_MAP_PMC_MASTER_P3_CFG_TRANSITION_OFFSET   28   /*  PMC_MASTER:  P3_CFG_TRANSITION */
#define BSP_TWL_3029_MAP_PMC_MASTER_P123_CFG_TRANSITION_OFFSET 29   /*  PMC_MASTER:  P123_CFG_TRANSITION */
#define BSP_TWL_3029_MAP_PMC_MASTER_STS_HW_CONDITIONS_OFFSET   30   /*  PMC_MASTER:  STS_HW_CONDITIONS */
#define BSP_TWL_3029_MAP_PMC_MASTER_SEQ_P123_STATE_OFFSET      31   /*  PMC_MASTER:  SEQ_P123_STATE */
#define BSP_TWL_3029_MAP_PMC_MASTER_SEQ_CFG_MODE_OFFSET        32   /*  PMC_MASTER:  SEQ_CFG_MODE */
#define BSP_TWL_3029_MAP_PMC_MASTER_SEQ_CFG_TRANSITION_OFFSET  33   /*  PMC_MASTER:  SEQ_CFG_TRANSITION */
#define BSP_TWL_3029_MAP_PMC_MASTER_PB_CFG_OFFSET              34   /*  PMC_MASTER:  PB_CFG */
#define BSP_TWL_3029_MAP_PMC_MASTER_PB_CFG_I2C_OFFSET          35   /*  PMC_MASTER:  PB_CFG_I2C */
#define BSP_TWL_3029_MAP_PMC_MASTER_PB_CFG_TEST_OFFSET         36   /*  PMC_MASTER:  PB_CFG_TEST */
#define BSP_TWL_3029_MAP_PMC_MASTER_SEQ_MEM_O2C_OFFSET         37   /*  PMC_MASTER:  SEQ_MEM_O2C */
#define BSP_TWL_3029_MAP_PMC_MASTER_SEQ_MEM_C2A_OFFSET         38   /*  PMC_MASTER:  SEQ_MEM_C2A */
#define BSP_TWL_3029_MAP_PMC_MASTER_SEQ_MEM_A2O_OFFSET         39   /*  PMC_MASTER:   SEQ_MEM_A2O */
#define BSP_TWL_3029_MAP_PMC_MASTER_SEQ_MEM_A2S_OFFSET         40   /*  PMC_MASTER:   SEQ_MEM_A2S */
#define BSP_TWL_3029_MAP_PMC_MASTER_SEQ_MEM_S2A_OFFSET         41   /*  PMC_MASTER:   SEQ_MEM_S2A */
#define BSP_TWL_3029_MAP_PMC_MASTER_SEQ_MEM_C2O_OFFSET         42   /*  PMC_MASTER:   SEQ_MEM_C2O */
#define BSP_TWL_3029_MAP_PMC_MASTER_SEQ_MEM_ADD_OFFSET         43   /*  PMC_MASTER:   SEQ_MEM_ADD */
#define BSP_TWL_3029_MAP_PMC_MASTER_SEQ_MEM_DATA_OFFSET        44   /*  PMC_MASTER:   SEQ_MEM_DATA  */
#define BSP_TWL_3029_MAP_PMC_MASTER_PROTECT_PKEY_OFFSET        45   /*  PMC_MASTER:   PROTECT_PKEY */
#define BSP_TWL_3029_MAP_PMC_MASTER_PROTECT_TKEY_OFFSET        46   /*  PMC_MASTER:   PROTECT_TKEY */
#define BSP_TWL_3029_MAP_PMC_MASTER_PROTECT_EN_OFFSET          47   /*  PMC_MASTER:   PROTECT_EN */
#define BSP_TWL_3029_MAP_PMC_MASTER_P1_DEV_OFFSET              48   /*  PMC_MASTER:   P1_DEV */
#define BSP_TWL_3029_MAP_PMC_MASTER_P2_DEV_OFFSET              49   /*  PMC_MASTER:   P2_DEV */      
#define BSP_TWL_3029_MAP_PMC_MASTER_P3_DEV_OFFSET              50   /*  PMC_MASTER:   P3_DEV */
#define BSP_TWL_3029_MAP_PMC_MASTER_PB_I2C_WORD_A_OFFSET       51   /*  PMC_MASTER:   PB_I2C_WORD_A */
#define BSP_TWL_3029_MAP_PMC_MASTER_PB_I2C_WORD_B_OFFSET       52   /*  PMC_MASTER:   PB_I2C_WORD_B */
#define BSP_TWL_3029_MAP_PMC_MASTER_PB_I2C_WORD_C_OFFSET       53   /*  PMC_MASTER:   PB_I2C_WORD_C */
#define BSP_TWL_3029_MAP_PMC_MASTER_PB_I2C_WORD_D_OFFSET       54   /*  PMC_MASTER:   PB_I2C_WORD_D  */

#define BSP_TWL_3029_MAP_PMC_SLAVE_RECEIVER_CFG_OFFSET        56  /*  PMC_SLAVE   RECEIVER_CFG  */
#define BSP_TWL_3029_MAP_PMC_SLAVE_PROTECT_SKEY_OFFSET        57  /*  PMC_SLAVE   PROTECT_SKEY  */
#define BSP_TWL_3029_MAP_PMC_SLAVE_BBSPOR_CFG_OFFSET          58  /*  PMC_SLAVE   BBSPOR_CFG  */

#define BSP_TWL_3029_MAP_PMC_SLAVE_VCORE_CFG_STS_OFFSET       59  /*  PMC_SLAVE    VCORE_CFG_STS  */
#define BSP_TWL_3029_MAP_PMC_SLAVE_VCORE_RES_ID_OFFSET        60  /*  PMC_SLAVE    VCORE_RES_ID  */
#define BSP_TWL_3029_MAP_PMC_SLAVE_VCORE_CFG_STATE_OFFSET     61  /*  PMC_SLAVE    VCORE_CFG_STATE  */
#define BSP_TWL_3029_MAP_PMC_SLAVE_VCORE_CFG_VMODE_OFFSET     62  /*  PMC_SLAVE    VCORE_CFG_VMODE  */
#define BSP_TWL_3029_MAP_PMC_SLAVE_VCORE_CFG_VSEL_OFFSET      63  /*  PMC_SLAVE    VCORE_CFG_VSEL  */
#define BSP_TWL_3029_MAP_PMC_SLAVE_VCORE_CFG_ROOF_OFFSET      64  /*  PMC_SLAVE    VCORE_CFG_ROOF  */
#define BSP_TWL_3029_MAP_PMC_SLAVE_VCORE_CFG_FLOOR_OFFSET     65  /*  PMC_SLAVE    VCORE_CFG_FLOOR  */
#define BSP_TWL_3029_MAP_PMC_SLAVE_VCORE_CFG_TRIM_0_OFFSET    66  /*  PMC_SLAVE    VCORE_CFG_TRIM_0  */
#define BSP_TWL_3029_MAP_PMC_SLAVE_VCORE_CFG_TRIM_1_OFFSET    67  /*  PMC_SLAVE    VCORE_CFG_TRIM_1  */
#define BSP_TWL_3029_MAP_PMC_SLAVE_VCORE_CFG_TRIM_2_OFFSET    68  /*  PMC_SLAVE    VCORE_CFG_TRIM_2  */

#define BSP_TWL_3029_MAP_PMC_SLAVE_VRSIM_CFG_STS_OFFSET       69  /*  PMC_SLAVE    VRSIM_CFG_STS  */
#define BSP_TWL_3029_MAP_PMC_SLAVE_VRSIM_RES_ID_OFFSET        70  /*  PMC_SLAVE    VRSIM_RES_ID  */
#define BSP_TWL_3029_MAP_PMC_SLAVE_VRSIM_CFG_STATE_OFFSET     71  /*  PMC_SLAVE    VRSIM_CFG_STATE  */
#define BSP_TWL_3029_MAP_PMC_SLAVE_VRSIM_CFG_DEDICATED_OFFSET 72  /*  PMC_SLAVE    VRSIM_CFG_DEDICATED  */

#define BSP_TWL_3029_MAP_PMC_SLAVE_VRMMC_CFG_STS_OFFSET       73  /*  PMC_SLAVE    VRMMC_CFG_STS  */
#define BSP_TWL_3029_MAP_PMC_SLAVE_VRMMC_RES_ID_OFFSET        74  /*  PMC_SLAVE    VRMMC_RES_ID  */
#define BSP_TWL_3029_MAP_PMC_SLAVE_VRMMC_CFG_STATE_OFFSET     75  /*  PMC_SLAVE    VRMMC_CFG_STATE  */
#define BSP_TWL_3029_MAP_PMC_SLAVE_VRMMC_CFG_DEDICATED_OFFSET 76  /*  PMC_SLAVE    VRMMC_CFG_DEDICATED  */

#define BSP_TWL_3029_MAP_PMC_SLAVE_VREXTH_CFG_STS_OFFSET      77  /*  PMC_SLAVE    VREXTH_CFG_STS  */
#define BSP_TWL_3029_MAP_PMC_SLAVE_VREXTH_RES_ID_OFFSET       78  /*  PMC_SLAVE    VREXTH_RES_ID  */
#define BSP_TWL_3029_MAP_PMC_SLAVE_VREXTH_CFG_STATE_OFFSET    79  /*  PMC_SLAVE    VREXTH_CFG_STATE  */
#define BSP_TWL_3029_MAP_PMC_SLAVE_VREXTH_CFG_DEDICATED_OFFSET 80  /*  PMC_SLAVE    VREXTH_CFG_DEDICATED  */

#define BSP_TWL_3029_MAP_PMC_SLAVE_VRPLL_CFG_STS_OFFSET       81  /*  PMC_SLAVE    VRPLL_CFG_STS  */
#define BSP_TWL_3029_MAP_PMC_SLAVE_VRPLL_RES_ID_OFFSET        82  /*  PMC_SLAVE    VRPLL_RES_ID  */
#define BSP_TWL_3029_MAP_PMC_SLAVE_VRPLL_CFG_STATE_OFFSET     83  /*  PMC_SLAVE    VRPLL_CFG_STATE  */
#define BSP_TWL_3029_MAP_PMC_SLAVE_VRPLL_CFG_DEDICATED_OFFSET 84  /*  PMC_SLAVE    VRPLL_CFG_DEDICATED  */

#define BSP_TWL_3029_MAP_PMC_SLAVE_VRIO_CFG_STS_OFFSET        85  /*  PMC_SLAVE    VRIO_CFG_STS  */
#define BSP_TWL_3029_MAP_PMC_SLAVE_VRIO_RES_ID_OFFSET         86  /*  PMC_SLAVE    VRIO_RES_ID  */
#define BSP_TWL_3029_MAP_PMC_SLAVE_VRIO_CFG_STATE_OFFSET      87  /*  PMC_SLAVE    VRIO_CFG_STATE  */
#define BSP_TWL_3029_MAP_PMC_SLAVE_VRIO_CFG_DEDICATED_OFFSET  88  /*  PMC_SLAVE    VRIO_CFG_DEDICATED  */

#define BSP_TWL_3029_MAP_PMC_SLAVE_VRMEM_CFG_STS_OFFSET       89  /*  PMC_SLAVE    VRMEM_CFG_STS  */
#define BSP_TWL_3029_MAP_PMC_SLAVE_VRMEM_RES_ID_OFFSET        90  /*  PMC_SLAVE    VRMEM_RES_ID  */
#define BSP_TWL_3029_MAP_PMC_SLAVE_VRMEM_CFG_STATE_OFFSET     91  /*  PMC_SLAVE    VRMEM_CFG_STATE  */
#define BSP_TWL_3029_MAP_PMC_SLAVE_VRMEM_CFG_DEDICATED_OFFSET 92  /*  PMC_SLAVE    VRMEM_CFG_DEDICATED  */

#define BSP_TWL_3029_MAP_PMC_SLAVE_VRUSB_CFG_STS_OFFSET       93  /*  PMC_SLAVE    VRUSB_CFG_STS  */
#define BSP_TWL_3029_MAP_PMC_SLAVE_VRUSB_RES_ID_OFFSET        94  /*  PMC_SLAVE    VRUSB_RES_ID  */
#define BSP_TWL_3029_MAP_PMC_SLAVE_VRUSB_CFG_STATE_OFFSET     95  /*  PMC_SLAVE    VRUSB_CFG_STATE  */
#define BSP_TWL_3029_MAP_PMC_SLAVE_VRUSB_CFG_DEDICATED_OFFSET 96  /*  PMC_SLAVE    VRUSB_CFG_DEDICATED  */

#define BSP_TWL_3029_MAP_PMC_SLAVE_VRABB_CFG_STS_OFFSET       97  /*  PMC_SLAVE    VRABB_CFG_STS  */
#define BSP_TWL_3029_MAP_PMC_SLAVE_VRABB_RES_ID_OFFSET        98  /*  PMC_SLAVE    VRABB_RES_ID  */
#define BSP_TWL_3029_MAP_PMC_SLAVE_VRABB_CFG_STATE_OFFSET     99  /*  PMC_SLAVE    VRABB_CFG_STATE  */
#define BSP_TWL_3029_MAP_PMC_SLAVE_VRABB_CFG_DEDICATED_OFFSET  100  /*  PMC_SLAVE    VRABB_CFG_DEDICATED  */

#define BSP_TWL_3029_MAP_PMC_SLAVE_VRVBUS_CFG_STS_OFFSET       101  /*  PMC_SLAVE    VRVBUS_CFG_STS  */
#define BSP_TWL_3029_MAP_PMC_SLAVE_VRVBUS_RES_ID_OFFSET        102  /*  PMC_SLAVE    VRVBUS_RES_ID  */
#define BSP_TWL_3029_MAP_PMC_SLAVE_VRVBUS_CFG_STATE_OFFSET     103  /*  PMC_SLAVE    VRVBUS_CFG_STATE  */
#define BSP_TWL_3029_MAP_PMC_SLAVE_VRVBUS_CFG_DEDICATED_OFFSET 104  /*  PMC_SLAVE    VRVBUS_CFG_DEDICATED  */

#define BSP_TWL_3029_MAP_PMC_SLAVE_VREXTL_CFG_STS_OFFSET       105  /*  PMC_SLAVE    VREXTL_CFG_STS  */
#define BSP_TWL_3029_MAP_PMC_SLAVE_VREXTL_RES_ID_OFFSET        106  /*  PMC_SLAVE    VREXTL_RES_ID  */
#define BSP_TWL_3029_MAP_PMC_SLAVE_VREXTL_CFG_STATE_OFFSET     107  /*  PMC_SLAVE    VREXTL_CFG_STATE  */
#define BSP_TWL_3029_MAP_PMC_SLAVE_VREXTL_CFG_DEDICATED_OFFSET 108  /*  PMC_SLAVE    VREXTL_CFG_DEDICATED  */

#define BSP_TWL_3029_MAP_PMC_SLAVE_EXT_REGEN_CFG_STS_OFFSET    109  /*  PMC_SLAVE    EXT_REGEN_CFG_STS  */
#define BSP_TWL_3029_MAP_PMC_SLAVE_EXT_REGEN_RES_ID_OFFSET     110  /*  PMC_SLAVE    EXT_REGEN_RES_ID  */
#define BSP_TWL_3029_MAP_PMC_SLAVE_EXT_REGEN_CFG_STATE_OFFSET  111  /*  PMC_SLAVE    EXT_REGEN_CFG_STATE  */

#define BSP_TWL_3029_MAP_PMC_SLAVE_CBAT32_CFG_STS_OFFSET       112  /*  PMC_SLAVE    CBAT32_CFG_STS  */
#define BSP_TWL_3029_MAP_PMC_SLAVE_CBAT32_RES_ID_OFFSET        113  /*  PMC_SLAVE    CBAT32_RES_ID  */
#define BSP_TWL_3029_MAP_PMC_SLAVE_CBAT32_CFG_STATE_OFFSET     114  /*  PMC_SLAVE    CBAT32_CFG_STATE  */

#define BSP_TWL_3029_MAP_PMC_SLAVE_BGAP_CFG_STS_OFFSET         118  /*  PMC_SLAVE    BGAP_CFG_STS  */
#define BSP_TWL_3029_MAP_PMC_SLAVE_BGAP_RES_ID_OFFSET          119  /*  PMC_SLAVE    BGAP_RES_ID  */
#define BSP_TWL_3029_MAP_PMC_SLAVE_BGAP_CFG_STATE_OFFSET       120  /*  PMC_SLAVE    BGAP_CFG_STATE  */
#define BSP_TWL_3029_MAP_PMC_SLAVE_BGAP_CFG_DEDICATED_OFFSET   121  /*  PMC_SLAVE    BGAP_CFG_DEDICATED  */
#define BSP_TWL_3029_MAP_PMC_SLAVE_BGSLP_CFG_STS_OFFSET        122  /*  PMC_SLAVE    BGSLP_CFG_STS  */
#define BSP_TWL_3029_MAP_PMC_SLAVE_BGSLP_RES_ID_OFFSET         123  /*  PMC_SLAVE    BGSLP_RES_ID  */
#define BSP_TWL_3029_MAP_PMC_SLAVE_BGSLP_CFG_STATE_OFFSET      124  /*  PMC_SLAVE    BGSLP_CFG_STATE  */

#define BSP_TWL_3029_MAP_PMC_SLAVE_TSHUTDOWN_CFG_STS_OFFSET    125  /*  PMC_SLAVE    TSHUTDOWN_CFG_STS  */
#define BSP_TWL_3029_MAP_PMC_SLAVE_TSHUTDOWN_RES_ID_OFFSET     126  /*  PMC_SLAVE    TSHUTDOWN_RES_ID  */
#define BSP_TWL_3029_MAP_PMC_SLAVE_TSHUTDOWN_CFG_STATE_OFFSET  127  /*  PMC_SLAVE    TSHUTDOWN_CFG_STATE  */
#define BSP_TWL_3029_MAP_PMC_SLAVE_TSHUTDOWN_CFG_DEDICATED_OFFSET   128  /*  PMC_SLAVE    TSHUTDOWN_CFG_DEDICATED  */

#define BSP_TWL_3029_MAP_PMC_SLAVE_BIAS_CFG_STS_OFFSET         129  /*  PMC_SLAVE    BIAS_CFG_STS  */
#define BSP_TWL_3029_MAP_PMC_SLAVE_BIAS_RES_ID_OFFSET          130  /*  PMC_SLAVE    BIAS_RES_ID  */
#define BSP_TWL_3029_MAP_PMC_SLAVE_BIAS_CFG_STATE_OFFSET       131  /*  PMC_SLAVE    BIAS_CFG_STATE  */

#define BSP_TWL_3029_MAP_PMC_SLAVE_CLKON_CFG_STS_OFFSET        132  /*  PMC_SLAVE    CLKON_CFG_STS  */
#define BSP_TWL_3029_MAP_PMC_SLAVE_CLKON_RES_ID_OFFSET         133  /*  PMC_SLAVE    CLKON_RES_ID  */
#define BSP_TWL_3029_MAP_PMC_SLAVE_CLKON_CFG_STATE_OFFSET      134  /*  PMC_SLAVE    CLKON_CFG_STATE  */

#define BSP_TWL_3029_MAP_PMC_SLAVE_STATE_MNGT_CFG_STS_OFFSET   135  /*  PMC_SLAVE    STATE_MNGT_CFG_STS  */
#define BSP_TWL_3029_MAP_PMC_SLAVE_STATE_MNGT_RES_ID_OFFSET    136  /*  PMC_SLAVE    STATE_MNGT_RES_ID  */
#define BSP_TWL_3029_MAP_PMC_SLAVE_STATE_MNGT_CFG_STATE_OFFSET 137  /*  PMC_SLAVE    STATE_MNGT_CFG_STATE  */

#define BSP_TWL_3029_MAP_PMC_SLAVE_SLEEP_MNGT_CFG_STS_OFFSET   138  /*  PMC_SLAVE    SLEEP_MNGT_CFG_STS  */
#define BSP_TWL_3029_MAP_PMC_SLAVE_SLEEP_MNGT_RES_ID_OFFSET    139  /*  PMC_SLAVE    SLEEP_MNGT_RES_ID  */
#define BSP_TWL_3029_MAP_PMC_SLAVE_SLEEP_MNGT_CFG_STATE_OFFSET 140  /*  PMC_SLAVE    SLEEP_MNGT_CFG_STATE  */


#define BSP_TWL_3029_MAP_PMC_SLAVE_BAT_PRES_CHECK_CFG_STS_OFFSET   144  /*  PMC_SLAVE    BAT_PRES_CHECK_CFG_STS  */
#define BSP_TWL_3029_MAP_PMC_SLAVE_BAT_PRES_CHECK_RES_ID_OFFSET    145  /*  PMC_SLAVE    BAT_PRES_CHECK_RES_ID  */
#define BSP_TWL_3029_MAP_PMC_SLAVE_BAT_PRES_CHECK_CFG_STATE_OFFSET 146  /*  PMC_SLAVE    BAT_PRES_CHECK_CFG_STATE  */

#endif

