/**
 * @file gil_api.h
 *
 * API Definition for GIL: Generic interface layer.
 */

/*
 * History:
 *
 *	Date       	Author        		Modification
 *	-------------------------------------------------------------------
 *	04/03/2003	Frederic Maria 		Create.
 *
 * (C) Copyright 2004 by Texas Instruments, All Rights Reserved
 */

#ifndef __GIL_GPF_H_
#define __GIL_GPF_H_


// GPF includes.
#include "typedefs.h"
#include "vsi.h"

// Riviera includes.
#include "rv/rv_general.h"
#include "gil/gil.h"


// GPF specialization:
// A new primitive (opcode MMI_GIL_IND) is added to the SAP MMI_DL.
// Please refer to mmi.sap document in directory g23m/condat/ms/doc/sap.


// Macro definitions.

/**
 * Allocate a new primitive from GPF frame.
 *
 * @param _type @in  The type of the data to allocate.
 * @param _opc  @in  The opcode of the primitive.
 *                   This value is generated by SAP tool chain.
 *
 * @return A pointer to the allocated primitive. It is the caller
 *         responsibility to free this resource using the
 *         gil_gpf_free_primitive() macro.
 */
#define gil_gpf_allocate_primitive(_type, _opc) \
    (_type*)vsi_c_pnew(SIZ(_type),_opc FILE_LINE_MACRO )


/**
 * Allocate a new primitive from GPF frame.
 *
 * @param _prim @in  A pointer to the allocated primitive to free.
 *
 * @noreturn
 */
#define gil_gpf_free_primitive(_prim) \
    PFREE(_prim)


/**
 * Send the primitive @a _prim to the GPF task @a _recipient.
 *
 * @param _recipient @in  The handle to the recipient GPF task.
 * @param _prim      @in  A pointer to the allocated primitive to free.
 *
 * @retval RV_OK  The primitive was successfully sent.
 * @retval RV_INTERNAL_ERR  An error occurred during the operation.
 *                          The primitive was not sent.
 */
#define gil_gpf_send_primitive(_recipient, _prim) \
    ((PSEND(_recipient, _prim) == VSI_OK) ? RV_OK : RV_INTERNAL_ERR)



#ifdef __cplusplus
extern "C"
{
#endif


/**
 * @name API functions
 *
 * API functions declarations.
 */
/*@{*/


/// Dispatches the GPF message (primitive) to the associated callback function.
extern void gil_gpf_dispatch_message(void* prim);


/*@}*/

#ifdef __cplusplus
}
#endif


#endif //__GIL_GPF_H_
