/*******************************************************************************
* File Name: Pin_FranklinComp_P_Dummy.h  
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

#if !defined(CY_PINS_Pin_FranklinComp_P_Dummy_H) /* Pins Pin_FranklinComp_P_Dummy_H */
#define CY_PINS_Pin_FranklinComp_P_Dummy_H

#include "cytypes.h"
#include "cyfitter.h"
#include "Pin_FranklinComp_P_Dummy_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    Pin_FranklinComp_P_Dummy_Write(uint8 value) ;
void    Pin_FranklinComp_P_Dummy_SetDriveMode(uint8 mode) ;
uint8   Pin_FranklinComp_P_Dummy_ReadDataReg(void) ;
uint8   Pin_FranklinComp_P_Dummy_Read(void) ;
uint8   Pin_FranklinComp_P_Dummy_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define Pin_FranklinComp_P_Dummy_DRIVE_MODE_BITS        (3)
#define Pin_FranklinComp_P_Dummy_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - Pin_FranklinComp_P_Dummy_DRIVE_MODE_BITS))

#define Pin_FranklinComp_P_Dummy_DM_ALG_HIZ         (0x00u)
#define Pin_FranklinComp_P_Dummy_DM_DIG_HIZ         (0x01u)
#define Pin_FranklinComp_P_Dummy_DM_RES_UP          (0x02u)
#define Pin_FranklinComp_P_Dummy_DM_RES_DWN         (0x03u)
#define Pin_FranklinComp_P_Dummy_DM_OD_LO           (0x04u)
#define Pin_FranklinComp_P_Dummy_DM_OD_HI           (0x05u)
#define Pin_FranklinComp_P_Dummy_DM_STRONG          (0x06u)
#define Pin_FranklinComp_P_Dummy_DM_RES_UPDWN       (0x07u)

/* Digital Port Constants */
#define Pin_FranklinComp_P_Dummy_MASK               Pin_FranklinComp_P_Dummy__MASK
#define Pin_FranklinComp_P_Dummy_SHIFT              Pin_FranklinComp_P_Dummy__SHIFT
#define Pin_FranklinComp_P_Dummy_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Pin_FranklinComp_P_Dummy_PS                     (* (reg32 *) Pin_FranklinComp_P_Dummy__PS)
/* Port Configuration */
#define Pin_FranklinComp_P_Dummy_PC                     (* (reg32 *) Pin_FranklinComp_P_Dummy__PC)
/* Data Register */
#define Pin_FranklinComp_P_Dummy_DR                     (* (reg32 *) Pin_FranklinComp_P_Dummy__DR)
/* Input Buffer Disable Override */
#define Pin_FranklinComp_P_Dummy_INP_DIS                (* (reg32 *) Pin_FranklinComp_P_Dummy__PC2)


#if defined(Pin_FranklinComp_P_Dummy__INTSTAT)  /* Interrupt Registers */

    #define Pin_FranklinComp_P_Dummy_INTSTAT                (* (reg32 *) Pin_FranklinComp_P_Dummy__INTSTAT)

#endif /* Interrupt Registers */


/***************************************
* The following code is DEPRECATED and 
* must not be used.
***************************************/

#define Pin_FranklinComp_P_Dummy_DRIVE_MODE_SHIFT       (0x00u)
#define Pin_FranklinComp_P_Dummy_DRIVE_MODE_MASK        (0x07u << Pin_FranklinComp_P_Dummy_DRIVE_MODE_SHIFT)


#endif /* End Pins Pin_FranklinComp_P_Dummy_H */


/* [] END OF FILE */
