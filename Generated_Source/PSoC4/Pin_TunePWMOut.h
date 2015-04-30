/*******************************************************************************
* File Name: Pin_TunePWMOut.h  
* Version 2.10
*
* Description:
*  This file containts Control Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_Pin_TunePWMOut_H) /* Pins Pin_TunePWMOut_H */
#define CY_PINS_Pin_TunePWMOut_H

#include "cytypes.h"
#include "cyfitter.h"
#include "Pin_TunePWMOut_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    Pin_TunePWMOut_Write(uint8 value) ;
void    Pin_TunePWMOut_SetDriveMode(uint8 mode) ;
uint8   Pin_TunePWMOut_ReadDataReg(void) ;
uint8   Pin_TunePWMOut_Read(void) ;
uint8   Pin_TunePWMOut_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define Pin_TunePWMOut_DRIVE_MODE_BITS        (3)
#define Pin_TunePWMOut_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - Pin_TunePWMOut_DRIVE_MODE_BITS))

#define Pin_TunePWMOut_DM_ALG_HIZ         (0x00u)
#define Pin_TunePWMOut_DM_DIG_HIZ         (0x01u)
#define Pin_TunePWMOut_DM_RES_UP          (0x02u)
#define Pin_TunePWMOut_DM_RES_DWN         (0x03u)
#define Pin_TunePWMOut_DM_OD_LO           (0x04u)
#define Pin_TunePWMOut_DM_OD_HI           (0x05u)
#define Pin_TunePWMOut_DM_STRONG          (0x06u)
#define Pin_TunePWMOut_DM_RES_UPDWN       (0x07u)

/* Digital Port Constants */
#define Pin_TunePWMOut_MASK               Pin_TunePWMOut__MASK
#define Pin_TunePWMOut_SHIFT              Pin_TunePWMOut__SHIFT
#define Pin_TunePWMOut_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Pin_TunePWMOut_PS                     (* (reg32 *) Pin_TunePWMOut__PS)
/* Port Configuration */
#define Pin_TunePWMOut_PC                     (* (reg32 *) Pin_TunePWMOut__PC)
/* Data Register */
#define Pin_TunePWMOut_DR                     (* (reg32 *) Pin_TunePWMOut__DR)
/* Input Buffer Disable Override */
#define Pin_TunePWMOut_INP_DIS                (* (reg32 *) Pin_TunePWMOut__PC2)


#if defined(Pin_TunePWMOut__INTSTAT)  /* Interrupt Registers */

    #define Pin_TunePWMOut_INTSTAT                (* (reg32 *) Pin_TunePWMOut__INTSTAT)

#endif /* Interrupt Registers */


/***************************************
* The following code is DEPRECATED and 
* must not be used.
***************************************/

#define Pin_TunePWMOut_DRIVE_MODE_SHIFT       (0x00u)
#define Pin_TunePWMOut_DRIVE_MODE_MASK        (0x07u << Pin_TunePWMOut_DRIVE_MODE_SHIFT)


#endif /* End Pins Pin_TunePWMOut_H */


/* [] END OF FILE */
