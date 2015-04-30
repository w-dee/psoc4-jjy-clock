/*******************************************************************************
* File Name: PWM_Tune_PM.c
* Version 2.0
*
* Description:
*  This file contains the setup, control, and status commands to support
*  the component operations in the low power mode.
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

#include "PWM_Tune.h"

static PWM_Tune_BACKUP_STRUCT PWM_Tune_backup;


/*******************************************************************************
* Function Name: PWM_Tune_SaveConfig
********************************************************************************
*
* Summary:
*  All configuration registers are retention. Nothing to save here.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void PWM_Tune_SaveConfig(void)
{

}


/*******************************************************************************
* Function Name: PWM_Tune_Sleep
********************************************************************************
*
* Summary:
*  Stops the component operation and saves the user configuration.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void PWM_Tune_Sleep(void)
{
    if(0u != (PWM_Tune_BLOCK_CONTROL_REG & PWM_Tune_MASK))
    {
        PWM_Tune_backup.enableState = 1u;
    }
    else
    {
        PWM_Tune_backup.enableState = 0u;
    }

    PWM_Tune_Stop();
    PWM_Tune_SaveConfig();
}


/*******************************************************************************
* Function Name: PWM_Tune_RestoreConfig
********************************************************************************
*
* Summary:
*  All configuration registers are retention. Nothing to restore here.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void PWM_Tune_RestoreConfig(void)
{

}


/*******************************************************************************
* Function Name: PWM_Tune_Wakeup
********************************************************************************
*
* Summary:
*  Restores the user configuration and restores the enable state.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void PWM_Tune_Wakeup(void)
{
    PWM_Tune_RestoreConfig();

    if(0u != PWM_Tune_backup.enableState)
    {
        PWM_Tune_Enable();
    }
}


/* [] END OF FILE */
