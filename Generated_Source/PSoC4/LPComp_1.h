/*******************************************************************************
* File Name: LPComp_1.h
* Version 2.0
*
* Description:
*  This file contains the function prototypes and constants used in
*  the Low Power Comparator component.
*
* Note:
*  None
*
********************************************************************************
* Copyright 2013-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_COMPARATOR_LPComp_1_H)
#define CY_COMPARATOR_LPComp_1_H

#include "cytypes.h"
#include "cyfitter.h"


extern uint8  LPComp_1_initVar;


/***************************************
*  Conditional Compilation Parameters
****************************************/

#define LPComp_1_CY_LPCOMP_V0 (CYIPBLOCK_m0s8lpcomp_VERSION == 0u) 
#define LPComp_1_CY_LPCOMP_V2 (CYIPBLOCK_m0s8lpcomp_VERSION >= 2u) 


/**************************************
*        Function Prototypes
**************************************/

void    LPComp_1_Start(void);
void    LPComp_1_Init(void);
void    LPComp_1_Enable(void);
void    LPComp_1_Stop(void);
void    LPComp_1_SetSpeed(uint32 speed);
void    LPComp_1_SetInterruptMode(uint32 mode);
uint32  LPComp_1_GetInterruptSource(void);
void    LPComp_1_ClearInterrupt(uint32 interruptMask);
void    LPComp_1_SetInterrupt(uint32 interruptMask);
void    LPComp_1_SetHysteresis(uint32 hysteresis);
uint32  LPComp_1_GetCompare(void);
uint32  LPComp_1_ZeroCal(void);
void    LPComp_1_LoadTrim(uint32 trimVal);
void    LPComp_1_Sleep(void);
void    LPComp_1_Wakeup(void);
void    LPComp_1_SaveConfig(void);
void    LPComp_1_RestoreConfig(void);
#if(LPComp_1_CY_LPCOMP_V2)
    void    LPComp_1_SetOutputMode(uint32 mode);
    void    LPComp_1_SetInterruptMask(uint32 interruptMask);
    uint32  LPComp_1_GetInterruptMask(void);
    uint32  LPComp_1_GetInterruptSourceMasked(void);
#endif /* (LPComp_1_CY_LPCOMP_V2) */


/**************************************
*           API Constants
**************************************/

#if(LPComp_1_CY_LPCOMP_V2)
    /* Constants for LPComp_1_SetOutputMode(), mode parameter */
    #define LPComp_1_OUT_PULSE      (0x00u)
    #define LPComp_1_OUT_SYNC       (0x01u)
    #define LPComp_1_OUT_DIRECT     (0x02u)
#endif /* (LPComp_1_CY_LPCOMP_V2) */

#define LPComp_1_INTR_PARAM_MASK    (0x03u)
#define LPComp_1_SPEED_PARAM_MASK   (0x03u)

/* Constants for LPComp_1_SetSpeed(), speed parameter */
#define LPComp_1_MED_SPEED          (0x00u)
#define LPComp_1_HIGH_SPEED         (0x01u)
#define LPComp_1_LOW_SPEED          (0x02u)

/* Constants for LPComp_1_SetInterruptMode(), mode parameter */
#define LPComp_1_INTR_DISABLE       (0x00u)
#define LPComp_1_INTR_RISING        (0x01u)
#define LPComp_1_INTR_FALLING       (0x02u)
#define LPComp_1_INTR_BOTH          (0x03u)

/* Constants for LPComp_1_SetHysteresis(), hysteresis parameter */
#define LPComp_1_HYST_ENABLE        (0x00u)
#define LPComp_1_HYST_DISABLE       (0x01u)

/* Constants for LPComp_1_ZeroCal() */
#define LPComp_1_TRIMA_MASK         (0x1Fu)
#define LPComp_1_TRIMA_SIGNBIT      (4u)
#define LPComp_1_TRIMA_MAX_VALUE    (15u)
#define LPComp_1_TRIMB_MASK         (0x1Fu)
#define LPComp_1_TRIMB_SHIFT        (8u)
#define LPComp_1_TRIMB_SIGNBIT      (4u)
#define LPComp_1_TRIMB_MAX_VALUE    (15u)

/* Constants for LPComp_1_GetInterruptSource() and 
* LPComp_1_ClearInterrupt(), interruptMask parameter 
*/
#define LPComp_1_INTR_SHIFT         (LPComp_1_cy_psoc4_lpcomp_1__LPCOMP_INTR_SHIFT)
#define LPComp_1_INTR               ((uint32)0x01u << LPComp_1_INTR_SHIFT)

/* Constants for LPComp_1_SetInterrupt(), interruptMask parameter */
#define LPComp_1_INTR_SET_SHIFT     (LPComp_1_cy_psoc4_lpcomp_1__LPCOMP_INTR_SET_SHIFT)
#define LPComp_1_INTR_SET           ((uint32)0x01u << LPComp_1_INTR_SHIFT)

#if(LPComp_1_CY_LPCOMP_V2)
    /* Constants for LPComp_1_GetInterruptMask() and 
    * LPComp_1_SetInterruptMask(), interruptMask parameter 
    */
    #define LPComp_1_INTR_MASK_SHIFT    (LPComp_1_cy_psoc4_lpcomp_1__LPCOMP_INTR_MASK_SHIFT)
    #define LPComp_1_INTR_MASK          ((uint32)0x01u << LPComp_1_INTR_MASK_SHIFT)

    /* Constants for LPComp_1_GetInterruptSourceMasked() */ 
    #define LPComp_1_INTR_MASKED_SHIFT  (LPComp_1_cy_psoc4_lpcomp_1__LPCOMP_INTR_MASKED_SHIFT)
    #define LPComp_1_INTR_MASKED        ((uint32)0x01u << LPComp_1_INTR_MASKED_SHIFT)
#endif /* (LPComp_1_CY_LPCOMP_V2) */


/***************************************
* Enumerated Types and Parameters 
***************************************/

/* Enumerated Types LPCompSpeedType, Used in parameter Speed */
#define LPComp_1__LPC_LOW_SPEED 2
#define LPComp_1__LPC_MED_SPEED 0
#define LPComp_1__LPC_HIGH_SPEED 1


/* Enumerated Types LPCompInterruptType, Used in parameter Interrupt */
#define LPComp_1__LPC_INT_DISABLE 0
#define LPComp_1__LPC_INT_RISING 1
#define LPComp_1__LPC_INT_FALLING 2
#define LPComp_1__LPC_INT_BOTH 3


/* Enumerated Types LPCompHysteresisType, Used in parameter Hysteresis */
#define LPComp_1__LPC_DISABLE_HYST 1
#define LPComp_1__LPC_ENABLE_HYST 0


/* Enumerated Types OutputModeType, Used in parameter OutputMode */
#define LPComp_1__OUT_MODE_SYNC 1
#define LPComp_1__OUT_MODE_DIRECT 2
#define LPComp_1__OUT_MODE_PULSE 0



/***************************************
*   Initial Parameter Constants
****************************************/

#define LPComp_1_INTERRUPT    (1u)
#define LPComp_1_SPEED        (0u)
#define LPComp_1_HYSTERESIS   (0u)
#if (LPComp_1_CY_LPCOMP_V2)
    #define LPComp_1_OUT_MODE       (0u)
    #define LPComp_1_INTERRUPT_EN   (0u)
#endif /* (LPComp_1_CY_LPCOMP_V2) */


/**************************************
*             Registers
**************************************/

#define LPComp_1_CONFIG_REG     (*(reg32 *)LPComp_1_cy_psoc4_lpcomp_1__LPCOMP_CONFIG)
#define LPComp_1_CONFIG_PTR     ( (reg32 *)LPComp_1_cy_psoc4_lpcomp_1__LPCOMP_CONFIG)

#define LPComp_1_DFT_REG        (*(reg32 *)CYREG_LPCOMP_DFT)
#define LPComp_1_DFT_PTR        ( (reg32 *)CYREG_LPCOMP_DFT)

#define LPComp_1_INTR_REG       (*(reg32 *)LPComp_1_cy_psoc4_lpcomp_1__LPCOMP_INTR)
#define LPComp_1_INTR_PTR       ( (reg32 *)LPComp_1_cy_psoc4_lpcomp_1__LPCOMP_INTR)

#define LPComp_1_INTR_SET_REG   (*(reg32 *)LPComp_1_cy_psoc4_lpcomp_1__LPCOMP_INTR_SET)
#define LPComp_1_INTR_SET_PTR   ( (reg32 *)LPComp_1_cy_psoc4_lpcomp_1__LPCOMP_INTR_SET)

#define LPComp_1_TRIMA_REG      (*(reg32 *)LPComp_1_cy_psoc4_lpcomp_1__TRIM_A)
#define LPComp_1_TRIMA_PTR      ( (reg32 *)LPComp_1_cy_psoc4_lpcomp_1__TRIM_A)

#define LPComp_1_TRIMB_REG      (*(reg32 *)LPComp_1_cy_psoc4_lpcomp_1__TRIM_B)
#define LPComp_1_TRIMB_PTR      ( (reg32 *)LPComp_1_cy_psoc4_lpcomp_1__TRIM_B)

#if(LPComp_1_CY_LPCOMP_V2)
    #define LPComp_1_INTR_MASK_REG    (*(reg32 *)LPComp_1_cy_psoc4_lpcomp_1__LPCOMP_INTR_MASK) 
    #define LPComp_1_INTR_MASK_PTR    ( (reg32 *)LPComp_1_cy_psoc4_lpcomp_1__LPCOMP_INTR_MASK) 

    #define LPComp_1_INTR_MASKED_REG  (*(reg32 *)LPComp_1_cy_psoc4_lpcomp_1__LPCOMP_INTR_MASKED) 
    #define LPComp_1_INTR_MASKED_PTR  ( (reg32 *)LPComp_1_cy_psoc4_lpcomp_1__LPCOMP_INTR_MASKED) 
#endif /* (LPComp_1_CY_LPCOMP_V2) */


/***************************************
*        Registers Constants
***************************************/

#define LPComp_1_CONFIG_REG_SHIFT           (LPComp_1_cy_psoc4_lpcomp_1__LPCOMP_CONFIG_SHIFT)

/* LPComp_1LPComp_1_CONFIG_REG */
#define LPComp_1_CONFIG_SPEED_MODE_SHIFT    (0u)    /* [1:0]    Operating mode for the comparator      */
#define LPComp_1_CONFIG_HYST_SHIFT          (2u)    /* [2]      Add 10mV hysteresis to the comparator: 0-enable, 1-disable */
#define LPComp_1_CONFIG_INTR_SHIFT          (4u)    /* [5:4]    Sets Pulse/Interrupt mode              */
#define LPComp_1_CONFIG_OUT_SHIFT           (6u)    /* [6]      Current output value of the comparator    */
#define LPComp_1_CONFIG_EN_SHIFT            (7u)    /* [7]      Enable comparator */
#if(LPComp_1_CY_LPCOMP_V2)
    #define LPComp_1_CONFIG_DSI_BYPASS_SHIFT    (16u)   /* [16]   Bypass comparator output synchronization for DSI output  */
    #define LPComp_1_CONFIG_DSI_LEVEL_SHIFT     (17u)   /* [17]   Comparator DSI (trigger) out level: 0-pulse, 1-level  */
#endif /* (LPComp_1_CY_LPCOMP_V2) */

#define LPComp_1_CONFIG_SPEED_MODE_MASK     (((uint32) 0x03u << LPComp_1_CONFIG_SPEED_MODE_SHIFT) << \
                                                    LPComp_1_CONFIG_REG_SHIFT)

#define LPComp_1_CONFIG_HYST                (((uint32) 0x01u << LPComp_1_CONFIG_HYST_SHIFT) << \
                                                    LPComp_1_CONFIG_REG_SHIFT)

#define LPComp_1_CONFIG_INTR_MASK           (((uint32) 0x03u << LPComp_1_CONFIG_INTR_SHIFT) << \
                                                    LPComp_1_CONFIG_REG_SHIFT)

#define LPComp_1_CONFIG_OUT                 (((uint32) 0x01u << LPComp_1_CONFIG_OUT_SHIFT) << \
                                                    LPComp_1_CONFIG_REG_SHIFT)

#define LPComp_1_CONFIG_EN                  (((uint32) 0x01u << LPComp_1_CONFIG_EN_SHIFT) << \
                                                    LPComp_1_CONFIG_REG_SHIFT)
#if(LPComp_1_CY_LPCOMP_V2)
    #define LPComp_1_CONFIG_DSI_BYPASS          (((uint32) 0x01u << LPComp_1_CONFIG_DSI_BYPASS_SHIFT) << \
                                                        (LPComp_1_CONFIG_REG_SHIFT/2))

    #define LPComp_1_CONFIG_DSI_LEVEL           (((uint32) 0x01u << LPComp_1_CONFIG_DSI_LEVEL_SHIFT) << \
                                                        (LPComp_1_CONFIG_REG_SHIFT/2))
#endif /* (LPComp_1_CY_LPCOMP_V2) */


/* LPComp_1LPComp_1_DFT_REG */
#define LPComp_1_DFT_CAL_EN_SHIFT    (0u)    /* [0] Calibration enable */

#define LPComp_1_DFT_CAL_EN          ((uint32) 0x01u << LPComp_1_DFT_CAL_EN_SHIFT)


/***************************************
*       Init Macros Definitions
***************************************/

#define LPComp_1_GET_CONFIG_SPEED_MODE(mode)    ((uint32) ((((uint32) (mode) << LPComp_1_CONFIG_SPEED_MODE_SHIFT) << \
                                                            LPComp_1_CONFIG_REG_SHIFT) & \
                                                            LPComp_1_CONFIG_SPEED_MODE_MASK))

#define LPComp_1_GET_CONFIG_HYST(hysteresis)    ((0u != (hysteresis)) ? (LPComp_1_CONFIG_HYST) : (0u))

#define LPComp_1_GET_CONFIG_INTR(intType)   ((uint32) ((((uint32)(intType) << LPComp_1_CONFIG_INTR_SHIFT) << \
                                                    LPComp_1_CONFIG_REG_SHIFT) & \
                                                    LPComp_1_CONFIG_INTR_MASK))
#if(LPComp_1_CY_LPCOMP_V2)
    #define LPComp_1_GET_CONFIG_DSI_BYPASS(bypass)  ((0u != ((bypass) & LPComp_1_OUT_DIRECT)) ? \
                                                                    (LPComp_1_CONFIG_DSI_BYPASS) : (0u))
   
    #define LPComp_1_GET_CONFIG_DSI_LEVEL(level)    ((0u != ((level) & LPComp_1_OUT_SYNC)) ? \
                                                                    (LPComp_1_CONFIG_DSI_LEVEL) : (0u))
    
    #define LPComp_1_GET_INTR_MASK(mask)            ((0u != (mask)) ? (LPComp_1_INTR_MASK) : (0u))
#endif /* (LPComp_1_CY_LPCOMP_V2) */

#if(LPComp_1_CY_LPCOMP_V0)
    #define LPComp_1_CONFIG_REG_DEFAULT (LPComp_1_GET_CONFIG_SPEED_MODE(LPComp_1_SPEED) |\
                                                 LPComp_1_GET_CONFIG_INTR(LPComp_1_INTERRUPT)   |\
                                                 LPComp_1_GET_CONFIG_HYST(LPComp_1_HYSTERESIS))
#else
    #define LPComp_1_CONFIG_REG_DEFAULT (LPComp_1_GET_CONFIG_SPEED_MODE(LPComp_1_SPEED) |\
                                                 LPComp_1_GET_CONFIG_INTR(LPComp_1_INTERRUPT)   |\
                                                 LPComp_1_GET_CONFIG_HYST(LPComp_1_HYSTERESIS)  |\
                                                 LPComp_1_GET_CONFIG_DSI_BYPASS(LPComp_1_OUT_MODE) |\
                                                 LPComp_1_GET_CONFIG_DSI_LEVEL(LPComp_1_OUT_MODE))
#endif /* (LPComp_1_CY_LPCOMP_V0) */

#if(LPComp_1_CY_LPCOMP_V2)
    #define LPComp_1_INTR_MASK_REG_DEFAULT  (LPComp_1_GET_INTR_MASK(LPComp_1_INTERRUPT_EN))
#endif /* (LPComp_1_CY_LPCOMP_V2) */


/***************************************
* The following code is DEPRECATED and 
* should not be used in new projects.
***************************************/

#define LPComp_1_CONFIG_FILT_SHIFT          (3u)    
#define LPComp_1_CONFIG_FILT                ((uint32)((uint32)((uint32)0x01u << \
                                                    LPComp_1_CONFIG_FILT_SHIFT) << LPComp_1_CONFIG_REG_SHIFT))

#define LPComp_1_DIGITAL_FILTER             (0u)

/* LPComp_1_SetFilter() parameters */
#define LPComp_1_FILT_DISABLE               (0x00u)
#define LPComp_1_FILT_ENABLE                (0x01u)

/* LPComp_1_SetSpeed() parameters */
#define LPComp_1_MEDSPEED                   (LPComp_1_MED_SPEED)
#define LPComp_1_HIGHSPEED                  (LPComp_1_HIGH_SPEED)
#define LPComp_1_LOWSPEED                   (LPComp_1_LOW_SPEED)

void    LPComp_1_SetFilter(uint32 filter);

#endif    /* CY_COMPARATOR_LPComp_1_H */


/* [] END OF FILE */
