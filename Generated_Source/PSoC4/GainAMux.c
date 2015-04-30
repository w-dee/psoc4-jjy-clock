/*******************************************************************************
* File Name: GainAMux.c
* Version 1.80
*
*  Description:
*    This file contains all functions required for the analog multiplexer
*    AMux User Module.
*
*   Note:
*
*******************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include "GainAMux.h"

static uint8 GainAMux_lastChannel = GainAMux_NULL_CHANNEL;


/*******************************************************************************
* Function Name: GainAMux_Start
********************************************************************************
* Summary:
*  Disconnect all channels.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void GainAMux_Start(void) 
{
    uint8 chan;

    for(chan = 0u; chan < GainAMux_CHANNELS ; chan++)
    {
#if (GainAMux_MUXTYPE == GainAMux_MUX_SINGLE)
        GainAMux_Unset(chan);
#else
        GainAMux_CYAMUXSIDE_A_Unset(chan);
        GainAMux_CYAMUXSIDE_B_Unset(chan);
#endif
    }

    GainAMux_lastChannel = GainAMux_NULL_CHANNEL;
}


#if (!GainAMux_ATMOSTONE)
/*******************************************************************************
* Function Name: GainAMux_Select
********************************************************************************
* Summary:
*  This functions first disconnects all channels then connects the given
*  channel.
*
* Parameters:
*  channel:  The channel to connect to the common terminal.
*
* Return:
*  void
*
*******************************************************************************/
void GainAMux_Select(uint8 channel) 
{
    GainAMux_DisconnectAll();        /* Disconnect all previous connections */
    GainAMux_Connect(channel);       /* Make the given selection */
    GainAMux_lastChannel = channel;  /* Update last channel */
}
#endif


/*******************************************************************************
* Function Name: GainAMux_FastSelect
********************************************************************************
* Summary:
*  This function first disconnects the last connection made with FastSelect or
*  Select, then connects the given channel. The FastSelect function is similar
*  to the Select function, except it is faster since it only disconnects the
*  last channel selected rather than all channels.
*
* Parameters:
*  channel:  The channel to connect to the common terminal.
*
* Return:
*  void
*
*******************************************************************************/
void GainAMux_FastSelect(uint8 channel) 
{
    /* Disconnect the last valid channel */
    if( GainAMux_lastChannel != GainAMux_NULL_CHANNEL)
    {
        GainAMux_Disconnect(GainAMux_lastChannel);
    }

    /* Make the new channel connection */
#if (GainAMux_MUXTYPE == GainAMux_MUX_SINGLE)
    GainAMux_Set(channel);
#else
    GainAMux_CYAMUXSIDE_A_Set(channel);
    GainAMux_CYAMUXSIDE_B_Set(channel);
#endif


    GainAMux_lastChannel = channel;   /* Update last channel */
}


#if (GainAMux_MUXTYPE == GainAMux_MUX_DIFF)
#if (!GainAMux_ATMOSTONE)
/*******************************************************************************
* Function Name: GainAMux_Connect
********************************************************************************
* Summary:
*  This function connects the given channel without affecting other connections.
*
* Parameters:
*  channel:  The channel to connect to the common terminal.
*
* Return:
*  void
*
*******************************************************************************/
void GainAMux_Connect(uint8 channel) 
{
    GainAMux_CYAMUXSIDE_A_Set(channel);
    GainAMux_CYAMUXSIDE_B_Set(channel);
}
#endif

/*******************************************************************************
* Function Name: GainAMux_Disconnect
********************************************************************************
* Summary:
*  This function disconnects the given channel from the common or output
*  terminal without affecting other connections.
*
* Parameters:
*  channel:  The channel to disconnect from the common terminal.
*
* Return:
*  void
*
*******************************************************************************/
void GainAMux_Disconnect(uint8 channel) 
{
    GainAMux_CYAMUXSIDE_A_Unset(channel);
    GainAMux_CYAMUXSIDE_B_Unset(channel);
}
#endif

#if (GainAMux_ATMOSTONE)
/*******************************************************************************
* Function Name: GainAMux_DisconnectAll
********************************************************************************
* Summary:
*  This function disconnects all channels.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void GainAMux_DisconnectAll(void) 
{
    if(GainAMux_lastChannel != GainAMux_NULL_CHANNEL) 
    {
        GainAMux_Disconnect(GainAMux_lastChannel);
        GainAMux_lastChannel = GainAMux_NULL_CHANNEL;
    }
}
#endif

/* [] END OF FILE */
