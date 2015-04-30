/*******************************************************************************
* File Name: isr_LPComp.h
* Version 1.70
*
*  Description:
*   Provides the function definitions for the Interrupt Controller.
*
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/
#if !defined(CY_ISR_isr_LPComp_H)
#define CY_ISR_isr_LPComp_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void isr_LPComp_Start(void);
void isr_LPComp_StartEx(cyisraddress address);
void isr_LPComp_Stop(void);

CY_ISR_PROTO(isr_LPComp_Interrupt);

void isr_LPComp_SetVector(cyisraddress address);
cyisraddress isr_LPComp_GetVector(void);

void isr_LPComp_SetPriority(uint8 priority);
uint8 isr_LPComp_GetPriority(void);

void isr_LPComp_Enable(void);
uint8 isr_LPComp_GetState(void);
void isr_LPComp_Disable(void);

void isr_LPComp_SetPending(void);
void isr_LPComp_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the isr_LPComp ISR. */
#define isr_LPComp_INTC_VECTOR            ((reg32 *) isr_LPComp__INTC_VECT)

/* Address of the isr_LPComp ISR priority. */
#define isr_LPComp_INTC_PRIOR             ((reg32 *) isr_LPComp__INTC_PRIOR_REG)

/* Priority of the isr_LPComp interrupt. */
#define isr_LPComp_INTC_PRIOR_NUMBER      isr_LPComp__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable isr_LPComp interrupt. */
#define isr_LPComp_INTC_SET_EN            ((reg32 *) isr_LPComp__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the isr_LPComp interrupt. */
#define isr_LPComp_INTC_CLR_EN            ((reg32 *) isr_LPComp__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the isr_LPComp interrupt state to pending. */
#define isr_LPComp_INTC_SET_PD            ((reg32 *) isr_LPComp__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the isr_LPComp interrupt. */
#define isr_LPComp_INTC_CLR_PD            ((reg32 *) isr_LPComp__INTC_CLR_PD_REG)



#endif /* CY_ISR_isr_LPComp_H */


/* [] END OF FILE */
