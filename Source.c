/*
 * File: Source.c
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

#ifndef __C_ABS_H__
#define __C_ABS_H__

#define ABS(x) ((x)<0 ? -(x) : (x))

#endif

plcbit EncoderEmulatorFn(struct EncoderEmulator_typ* Encoder)
{
	if(Encoder==0 || Encoder->IN.CFG ==0){
		return 1;
	}

	Encoder->IO.doMovEnable = Encoder->IN.CMD.Enable;
	
	//Calculate how far we will move in this scan to know if we will pass the reference pulse in this scan
	Encoder->Internal.ScanDistance = Encoder->IN.PAR.Position - Encoder->Internal.CurrentPosition;	
	Encoder->Internal.CurrentPosition = Encoder->IN.PAR.Position;

	//Calculate each scan length and get the current net time to figure out the reference pulses
	DINT netTime = AsIOTimeCyclicStart();
	Encoder->Internal.DeltaNetTime = netTime - Encoder->Internal.CurrentNetTime;
	Encoder->Internal.CurrentNetTime = 	netTime;


	Encoder->Internal.ScaledLastPosition = Encoder->Internal.ScaledPosition;
	Encoder->Internal.ScaledPosition = ScaleEncoder(Encoder->IN.PAR.Position,
		Encoder->IN.CFG->Scale.Axis1,
		Encoder->IN.CFG->Scale.Axis2,
		Encoder->IN.CFG->Scale.Encoder1,
		Encoder->IN.CFG->Scale.Encoder2);
			
	//Scale the encoder from position to pulses
	Encoder->Internal.oMoveTarget = Encoder->IO.doMovTargetPosition;
	Encoder->IO.doMovTargetPosition = (DINT)(Encoder->Internal.ScaledPosition);
	
	//Scale the encoder reference pulse position to pulses
	Encoder->IO.doMovReferenceStart = ScaleEncoder(Encoder->IN.PAR.PositionRefPulse,
		Encoder->IN.CFG->Scale.Axis1,
		Encoder->IN.CFG->Scale.Axis2,
		Encoder->IN.CFG->Scale.Encoder1,
		Encoder->IN.CFG->Scale.Encoder2);

	//Scale the encoder reference pulse length to pulses
	Encoder->IO.doMovReferenceStopMargin = ScaleEncoder(Encoder->IN.PAR.LengthRefPulse,
		Encoder->IN.CFG->Scale.Axis1,
		Encoder->IN.CFG->Scale.Axis2,
		Encoder->IN.CFG->Scale.Encoder1,
		Encoder->IN.CFG->Scale.Encoder2);	
	
	if((Encoder->IO.doMovTargetPosition - Encoder->Internal.oMoveTarget) != 0){
		Encoder->Internal.MoveTime = Encoder->Internal.DeltaNetTime * (Encoder->IO.doMovTargetPosition - Encoder->Internal.ScaledLastPosition)/(Encoder->Internal.ScaledPosition - Encoder->Internal.ScaledLastPosition);
	}
	else{
		Encoder->Internal.MoveTime = Encoder->Internal.DeltaNetTime;
	}

	Encoder->Internal.oMoveTargetTime = Encoder->IO.doMovTargetTime;
	Encoder->IO.doMovTargetTime = netTime + Encoder->Internal.MoveTime + Encoder->IN.CFG->NetTimeOffset;
	
	if(Encoder->IO.doMovTargetTime - Encoder->Internal.oMoveTargetTime != 0){
		Encoder->OUT.Velocity = (Encoder->IO.doMovTargetPosition - Encoder->Internal.oMoveTarget)/(LREAL)(Encoder->IO.doMovTargetTime - Encoder->Internal.oMoveTargetTime);
		
		Encoder->Internal.RemainingTime = Encoder->Internal.DeltaNetTime - Encoder->Internal.MoveTime;
	}
	else{
		Encoder->OUT.Velocity=0;		
	}

	return 0;
	
}

plcbit SignalGeneratorFn(struct SignalGenerator_typ* Encoder)
{	

	if(Encoder==0 || Encoder->IN.CFG ==0){
		return 1;
	}
	//Calculate how far we will move in this scan to know if we will pass the reference pulse in this scan
	Encoder->Internal.ScanDistance = ABS(Encoder->IN.PAR.Position - Encoder->Internal.CurrentPosition);	
	
	//Calculate each scan length and get the current net time to figure out the reference pulses
	DINT netTime = AsIOTimeCyclicStart() + Encoder->IN.CFG->NetworkDelay;
	Encoder->Internal.DeltaNetTime = netTime - Encoder->Internal.CurrentNetTime;
	Encoder->Internal.CurrentNetTime = 	netTime;

	//If the next scan requires pulsing, increase the sequence number to indicate so
	if(Encoder->IN.PAR.PositionPulse <= Encoder->IN.PAR.Position
		&& Encoder->IN.PAR.PositionPulse >= Encoder->Internal.CurrentPosition
		&& Encoder->Internal.PositionAdded!= 1
		&& Encoder->Internal.ScanDistance != 0
		&& Encoder->IO.doEnable == 1
	&& Encoder->IN.CMD.Enable){
		
		//Output for debug
		Encoder->Internal.Debug.NetTimeLatch = netTime - Encoder->IN.CFG->NetworkDelay;
		Encoder->Internal.Debug.ScanDistanceLatch = Encoder->Internal.ScanDistance;
		Encoder->Internal.Debug.CurrentPositionLatch = Encoder->Internal.CurrentPosition;
		Encoder->Internal.Debug.SetPositionLatch = 	Encoder->IN.PAR.Position;

		//Calculate the timestamp by interpolating the pulse position
		//Scale delta position to delta time per scan
		Encoder->IO.Timestamp1 = ScaleEncoder(Encoder->IN.PAR.PositionPulse,
			Encoder->Internal.CurrentPosition,
			Encoder->Internal.CurrentPosition + Encoder->Internal.ScanDistance,
			Encoder->Internal.CurrentNetTime,
			Encoder->Internal.CurrentNetTime + Encoder->Internal.DeltaNetTime);
	
		//Use the time on delay directly
		Encoder->IO.Offset1_OnDelay = Encoder->IN.CFG->TimeToOn;

		switch(Encoder->IN.PAR.PulseLengthMode){
			case TRIG_END_MODE_TIME:
				Encoder->IO.Offset2_OffTime = Encoder->IN.PAR.TimePulse;
				break;

			case TRIG_END_MODE_LENGTH:
				//Scale the time off delay from distance to time
				Encoder->IO.Offset2_OffTime = ScaleEncoder(Encoder->IN.PAR.LengthPulse,
					0,
					Encoder->Internal.ScanDistance,
					0,
					Encoder->Internal.DeltaNetTime);
				break;
		}
		Encoder->Internal.LastPositionAdded= Encoder->IN.PAR.PositionPulse;
		Encoder->Internal.PositionAdded = 1;
		Encoder->IO.doSequence++;		
	}

	//If we are not within the pulse region, reset position added
	if( (Encoder->IN.PAR.PositionPulse > Encoder->IN.PAR.Position 
	  || Encoder->IN.PAR.PositionPulse < Encoder->Internal.CurrentPosition)){
		Encoder->Internal.PositionAdded = 0;		
	}
	
	Encoder->IO.doEnable=  Encoder->IN.CMD.Enable;
	
	Encoder->Internal.CurrentPosition = Encoder->IN.PAR.Position;
	
	Encoder->OUT.ModuleError=0;	
	if(Encoder->IO.diEdgeError && !Encoder->IO.doEdgeErrorAcknowledge){
		Encoder->IO.doEdgeErrorAcknowledge=1;
		Encoder->OUT.ModuleError=1;
	}
	else if(!Encoder->IO.diEdgeError){
		Encoder->IO.doEdgeErrorAcknowledge=0;
	}

	Encoder->OUT.EdgeWarning=0;	
	if(Encoder->IO.diEdgeWarning && !Encoder->IO.doEdgeWarningAcknowledge){
		Encoder->IO.doEdgeWarningAcknowledge=1;
		Encoder->OUT.EdgeWarning=1;
	}
	else if(!Encoder->IO.diEdgeWarning){
		Encoder->IO.doEdgeWarningAcknowledge=0;
	}	
	
	return 0;
	
}


//Scale value given from X To Y given two points
double ScaleEncoder(double x,double x1,double x2,double y1,double y2){
	if( x2 - x1 == 0){
		return y2;
	}		
	else{
		return y1 + (x - x1)*((y2-y1)/(x2-x1));
	}	
}
