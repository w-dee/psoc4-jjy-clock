/*******************************************************************************
* File Name: Clock_5M.h
* Version 2.20
*
*  Description:
*   Provides the function and constant definitions for the clock component.
*
*  Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CLOCK_Clock_5M_H)
#define CY_CLOCK_Clock_5M_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
*        Function Prototypes
***************************************/
#if defined CYREG_PERI_DIV_CMD

void Clock_5M_StartEx(uint32 alignClkDiv);
#define Clock_5M_Start() \
    Clock_5M_StartEx(Clock_5M__PA_DIV_ID)

#else

void Clock_5M_Start(void);

#endif/* CYREG_PERI_DIV_CMD */

void Clock_5M_Stop(void);

void Clock_5M_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional);

uint16 Clock_5M_GetDividerRegister(void);
uint8  Clock_5M_GetFractionalDividerRegister(void);

#define Clock_5M_Enable()                         Clock_5M_Start()
#define Clock_5M_Disable()                        Clock_5M_Stop()
#define Clock_5M_SetDividerRegister(clkDivider, reset)  \
    Clock_5M_SetFractionalDividerRegister((clkDivider), 0u)
#define Clock_5M_SetDivider(clkDivider)           Clock_5M_SetDividerRegister((clkDivider), 1u)
#define Clock_5M_SetDividerValue(clkDivider)      Clock_5M_SetDividerRegister((clkDivider) - 1u, 1u)


/***************************************
*             Registers
***************************************/
#if defined CYREG_PERI_DIV_CMD

#define Clock_5M_DIV_ID     Clock_5M__DIV_ID

#define Clock_5M_CMD_REG    (*(reg32 *)CYREG_PERI_DIV_CMD)
#define Clock_5M_CTRL_REG   (*(reg32 *)Clock_5M__CTRL_REGISTER)
#define Clock_5M_DIV_REG    (*(reg32 *)Clock_5M__DIV_REGISTER)

#define Clock_5M_CMD_DIV_SHIFT          (0u)
#define Clock_5M_CMD_PA_DIV_SHIFT       (8u)
#define Clock_5M_CMD_DISABLE_SHIFT      (30u)
#define Clock_5M_CMD_ENABLE_SHIFT       (31u)

#define Clock_5M_CMD_DISABLE_MASK       ((uint32)((uint32)1u << Clock_5M_CMD_DISABLE_SHIFT))
#define Clock_5M_CMD_ENABLE_MASK        ((uint32)((uint32)1u << Clock_5M_CMD_ENABLE_SHIFT))

#define Clock_5M_DIV_FRAC_MASK  (0x000000F8u)
#define Clock_5M_DIV_FRAC_SHIFT (3u)
#define Clock_5M_DIV_INT_MASK   (0xFFFFFF00u)
#define Clock_5M_DIV_INT_SHIFT  (8u)

#else 

#define Clock_5M_DIV_REG        (*(reg32 *)Clock_5M__REGISTER)
#define Clock_5M_ENABLE_REG     Clock_5M_DIV_REG
#define Clock_5M_DIV_FRAC_MASK  Clock_5M__FRAC_MASK
#define Clock_5M_DIV_FRAC_SHIFT (16u)
#define Clock_5M_DIV_INT_MASK   Clock_5M__DIVIDER_MASK
#define Clock_5M_DIV_INT_SHIFT  (0u)

#endif/* CYREG_PERI_DIV_CMD */

#endif /* !defined(CY_CLOCK_Clock_5M_H) */

/* [] END OF FILE */
