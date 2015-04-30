/*******************************************************************************
* File Name: Pin_AMux.h  
* Version 2.5
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

#if !defined(CY_PINS_Pin_AMux_H) /* Pins Pin_AMux_H */
#define CY_PINS_Pin_AMux_H

#include "cytypes.h"
#include "cyfitter.h"
#include "Pin_AMux_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    Pin_AMux_Write(uint8 value) ;
void    Pin_AMux_SetDriveMode(uint8 mode) ;
uint8   Pin_AMux_ReadDataReg(void) ;
uint8   Pin_AMux_Read(void) ;
uint8   Pin_AMux_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define Pin_AMux_DRIVE_MODE_BITS        (3)
#define Pin_AMux_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - Pin_AMux_DRIVE_MODE_BITS))
#define Pin_AMux_DRIVE_MODE_SHIFT       (0x00u)
#define Pin_AMux_DRIVE_MODE_MASK        (0x07u << Pin_AMux_DRIVE_MODE_SHIFT)

#define Pin_AMux_DM_ALG_HIZ         (0x00u << Pin_AMux_DRIVE_MODE_SHIFT)
#define Pin_AMux_DM_DIG_HIZ         (0x01u << Pin_AMux_DRIVE_MODE_SHIFT)
#define Pin_AMux_DM_RES_UP          (0x02u << Pin_AMux_DRIVE_MODE_SHIFT)
#define Pin_AMux_DM_RES_DWN         (0x03u << Pin_AMux_DRIVE_MODE_SHIFT)
#define Pin_AMux_DM_OD_LO           (0x04u << Pin_AMux_DRIVE_MODE_SHIFT)
#define Pin_AMux_DM_OD_HI           (0x05u << Pin_AMux_DRIVE_MODE_SHIFT)
#define Pin_AMux_DM_STRONG          (0x06u << Pin_AMux_DRIVE_MODE_SHIFT)
#define Pin_AMux_DM_RES_UPDWN       (0x07u << Pin_AMux_DRIVE_MODE_SHIFT)

/* Digital Port Constants */
#define Pin_AMux_MASK               Pin_AMux__MASK
#define Pin_AMux_SHIFT              Pin_AMux__SHIFT
#define Pin_AMux_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Pin_AMux_PS                     (* (reg32 *) Pin_AMux__PS)
/* Port Configuration */
#define Pin_AMux_PC                     (* (reg32 *) Pin_AMux__PC)
/* Data Register */
#define Pin_AMux_DR                     (* (reg32 *) Pin_AMux__DR)
/* Input Buffer Disable Override */
#define Pin_AMux_INP_DIS                (* (reg32 *) Pin_AMux__PC2)


#if defined(Pin_AMux__INTSTAT)  /* Interrupt Registers */

    #define Pin_AMux_INTSTAT                (* (reg32 *) Pin_AMux__INTSTAT)

#endif /* Interrupt Registers */

#endif /* End Pins Pin_AMux_H */


/* [] END OF FILE */
