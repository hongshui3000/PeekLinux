/*************************************************************************/
/*                                                                       */
/*               Copyright Mentor Graphics Corporation 2004              */
/*                         All Rights Reserved.                          */
/*                                                                       */
/* THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS  */
/* THE PROPERTY OF MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS   */
/* SUBJECT TO LICENSE TERMS.                                             */
/*                                                                       */
/*************************************************************************/

/*************************************************************************/
/*                                                                       */
/* FILE NAME                                               VERSION       */
/*                                                                       */
/*      tm_extr.h                                      Nucleus PLUS 1.15 */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      TM - Timer Management                                            */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains function prototypes of all functions          */
/*      accessible to other components.                                  */
/*                                                                       */
/* DATA STRUCTURES                                                       */
/*                                                                       */
/*      None                                                             */
/*                                                                       */
/* DEPENDENCIES                                                          */
/*                                                                       */
/*      tm_defs.h                           Timer Management constants   */
/*                                                                       */
/*************************************************************************/

#include        "plus/inc/tm_defs.h"        /* Include TM constants      */


/* Check to see if the file has been included already.  */

#ifndef TM_EXTR
#define TM_EXTR


/*  Initialization functions.  */

VOID            TMI_Initialize(VOID);


/* Error checking for supplemental functions.  */

STATUS          TMSE_Create_Timer(NU_TIMER *timer_ptr, CHAR *name, 
                        VOID (*expiration_routine)(UNSIGNED), UNSIGNED id,
                        UNSIGNED initial_time, UNSIGNED reschedule_time,
                        OPTION enable);
STATUS          TMSE_Delete_Timer(NU_TIMER *timer_ptr);
STATUS          TMSE_Reset_Timer(NU_TIMER *timer_ptr, 
                        VOID (*expiration_routine)(UNSIGNED), 
                        UNSIGNED initial_time, UNSIGNED reschedule_time,
                        OPTION enable);
STATUS          TMSE_Control_Timer(NU_TIMER *timer_ptr, OPTION enable);


/* Supplemental processing functions.  */

STATUS          TMS_Create_Timer(NU_TIMER *timer_ptr, CHAR *name, 
                        VOID (*expiration_routine)(UNSIGNED), UNSIGNED id,
                        UNSIGNED initial_time, UNSIGNED reschedule_time,
                        OPTION enable);
STATUS          TMS_Delete_Timer(NU_TIMER *timer_ptr);
STATUS          TMS_Reset_Timer(NU_TIMER *timer_ptr, 
                        VOID (*expiration_routine)(UNSIGNED), 
                        UNSIGNED initial_time, UNSIGNED reschedule_timer,
                        OPTION enable);
STATUS          TMS_Control_Timer(NU_TIMER *timer_ptr, OPTION enable);


/* Core processing functions.  */

VOID            TMC_Init_Task_Timer(TM_TCB *timer, VOID *information);
VOID            TMC_Start_Task_Timer(TM_TCB *timer, UNSIGNED time);
VOID            TMC_Stop_Task_Timer(TM_TCB *timer);


/* Information retrieval functions.  */ 

UNSIGNED        TMF_Established_Timers(VOID);
STATUS          TMF_Timer_Information(NU_TIMER *timer_ptr, CHAR *name, 
                  OPTION *enable, UNSIGNED *expirations, UNSIGNED *id,
                  UNSIGNED *initial_time, UNSIGNED *reschedule_time);
UNSIGNED        TMF_Timer_Pointers(NU_TIMER **pointer_list, 
                                                UNSIGNED maximum_pointers);
                                                
/* Target dependent functions.  */

VOID            TMT_Set_Clock(UNSIGNED new_value);
UNSIGNED        TMT_Retrieve_Clock(VOID);
NU_TASK        *TMT_Retrieve_TS_Task(VOID);
VOID            TMT_Adjust_Timer(UNSIGNED new_value);

#endif





