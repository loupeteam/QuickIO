/*
 * File: DS4389_DRV.c
 * Copyright (c) 2023 Loupe
 * https://loupe.team
 * 
 * This file is part of QuickIO, licensed under the MIT License.
 */
#include <bur/plctypes.h>
#ifdef __cplusplus
	extern "C"
	{
#endif
	#include "QuickIO.h"
	#include "string.h"
#ifdef __cplusplus
	};
#endif
/* TODO: Add your comment here */
void DS4389_DRV(struct DS4389_DRV* inst)
{
	
	inst->Internal.SetMask= 
		(inst->Force[0] || inst->ForceOn[0] || inst->ForceAll)*4 + //Bit 2
		(inst->Force[1] || inst->ForceOn[1] || inst->ForceAll)*8 + //Bit 3 
		(inst->Force[2] || inst->ForceOn[2] || inst->ForceAll)*64+ //Bit 6
		(inst->Force[3] || inst->ForceOn[3] || inst->ForceAll)*128;//Bit 7 
	
	inst->Internal.ClearMask= 
		(inst->Force[0] || inst->ForceOff[0] || inst->ForceAll)*4 + //Bit 2
		(inst->Force[1] || inst->ForceOff[1] || inst->ForceAll)*8 + //Bit 3
		(inst->Force[2] || inst->ForceOff[2] || inst->ForceAll)*64+ //Bit 6
		(inst->Force[3] || inst->ForceOff[3] || inst->ForceAll)*128;//Bit 7
	
	//If we haven't written the value send it down
	if( inst->Internal._AsIOAccWrite[0].value != inst->Internal.SetMask || !inst->Internal.Init[0]){
		inst->Internal._AsIOAccWrite[0].value = inst->Internal.SetMask;
		inst->Internal._AsIOAccWrite[0].enable = 1;
		strcpy(inst->Internal.ChannelName[0], "CfO_DirectIOClearMask0_7");
	}

	//If we haven't written the value send it down
	if( inst->Internal._AsIOAccWrite[1].value != inst->Internal.ClearMask || !inst->Internal.Init[1]){
		inst->Internal._AsIOAccWrite[1].value = inst->Internal.ClearMask;
		inst->Internal._AsIOAccWrite[1].enable = 1;
		strcpy(inst->Internal.ChannelName[1], "CfO_DirectIOSetMask0_7");
	}

	int index=0;	
	for(index=0;index<=1;index++){
		inst->Internal._AsIOAccWrite[index].pDeviceName=	inst->pDeviceName;
		inst->Internal._AsIOAccWrite[index].pChannelName= 	(UDINT)&inst->Internal.ChannelName[index];
		AsIOAccWrite(&inst->Internal._AsIOAccWrite[index]);
		if(inst->Internal._AsIOAccWrite[index].status != ERR_FUB_BUSY
		&& inst->Internal._AsIOAccWrite[index].status != ERR_FUB_ENABLE_FALSE
		){			
			inst->Internal._AsIOAccWrite[index].enable = 0;
			inst->Status[index]= inst->Internal._AsIOAccWrite[index].status;
		}
		if(inst->Internal._AsIOAccWrite[index].status == ERR_OK){
			inst->Internal.Init[index]=1;
		}
	}
	
	return;
	
}
