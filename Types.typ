
TYPE
	EncoderEmulator_typ : 	STRUCT 
		IN : EncoderEmulator_IN_typ;
		OUT : EncoderEmulator_OUT_typ;
		IO : EncoderEmulator_IO_typ;
		Internal : EncoderEmulator_Internal_typ;
	END_STRUCT;
	EncoderEmulator_IN_typ : 	STRUCT 
		CMD : EncoderEmulator_IN_CMD_typ;
		PAR : EncoderEmulator_IN_PAR_typ;
		CFG : REFERENCE TO EncoderEmulator_IN_CFG_typ;
	END_STRUCT;
	EncoderEmulator_IN_CMD_typ : 	STRUCT 
		AcknowledgeError : BOOL;
		Enable : BOOL;
	END_STRUCT;
	EncoderEmulator_IN_PAR_typ : 	STRUCT 
		Position : LREAL;
		PositionRefPulse : LREAL;
		LengthRefPulse : LREAL;
	END_STRUCT;
	EncoderEmulator_IN_CFG_typ : 	STRUCT 
		Scale : EncoderEmulator_Scale_typ;
		NetworkDelay : DINT;
		NetTimeOffset : DINT;
	END_STRUCT;
	EncoderEmulator_Scale_typ : 	STRUCT 
		Axis1 : LREAL := 0;
		Axis2 : LREAL := 1;
		Encoder1 : LREAL := 0.0;
		Encoder2 : LREAL := 1;
	END_STRUCT;
	EncoderEmulator_IO_typ : 	STRUCT 
		doMovEnable : BOOL;
		doMovTargetPosition : DINT;
		doMovTargetTime : DINT;
		diMovPosition : DINT;
		doMovReferenceStart : DINT;
		doMovReferenceStopMargin : DINT;
	END_STRUCT;
	EncoderEmulator_OUT_STAT_typ : 	STRUCT 
		Error : BOOL;
		ErrorID : UINT;
		ErrorString : STRING[320];
	END_STRUCT;
	EncoderEmulator_OUT_typ : 	STRUCT 
		STAT : EncoderEmulator_OUT_STAT_typ;
		Velocity : LREAL;
	END_STRUCT;
	EncoderEmulator_Internal_typ : 	STRUCT 
		oMoveTarget : DINT;
		oMoveTargetTime : DINT;
		CurrentNetTime : DINT;
		DeltaNetTime : DINT;
		RemainingTime : DINT;
		CurrentPosition : LREAL;
		ScanDistance : LREAL;
		ScaledPosition : LREAL;
		ScaledLastPosition : LREAL;
		MoveTime : LREAL;
	END_STRUCT;
END_TYPE

(*Interface to DS Module*)

TYPE
	SignalGenerator_typ : 	STRUCT 
		IN : SignalGenerator_IN_typ;
		IO : SignalGenerator_IO_typ;
		OUT : SignalGenerator_OUT_typ;
		Internal : SignalGenerator_Internal_typ;
	END_STRUCT;
	SignalGenerator_IN_typ : 	STRUCT 
		CMD : SignalGenerator_IN_CMD_typ;
		PAR : SignalGenerator_IN_PAR_typ;
		CFG : REFERENCE TO SignalGenerator_IN_CFG_typ;
	END_STRUCT;
	SignalGenerator_IN_CFG_typ : 	STRUCT 
		TimeToOn : DINT;
		NetworkDelay : DINT;
	END_STRUCT;
	SignalGenerator_IN_CMD_typ : 	STRUCT 
		AcknowledgeError : BOOL;
		Enable : BOOL;
	END_STRUCT;
	SignalGenerator_IN_PAR_typ : 	STRUCT 
		Position : REAL;
		PositionPulse : REAL;
		LengthPulse : REAL;
		TimePulse : REAL;
		PulseLengthMode : TRIG_END_MODE;
	END_STRUCT;
	SignalGenerator_IO_typ : 	STRUCT 
		doEnable : BOOL;
		diEnabled : BOOL;
		doSequence : SINT;
		diSequenceReadback : SINT;
		Timestamp1 : INT;
		Offset1_OnDelay : UINT;
		Offset2_OffTime : UINT;
		doEdgeWarningAcknowledge : BOOL;
		diEdgeWarning : BOOL;
		doEdgeErrorAcknowledge : BOOL;
		diEdgeError : BOOL;
	END_STRUCT;
	SignalGenerator_OUT_typ : 	STRUCT 
		ModuleError : BOOL;
		EdgeWarning : BOOL;
		Error : BOOL;
		ErrorID : UINT;
		ErrorString : STRING[320];
	END_STRUCT;
	SignalGenerator_debug_typ : 	STRUCT 
		NetTimeLatch : DINT;
		CurrentPositionLatch : REAL;
		SetPositionLatch : REAL;
		ScanDistanceLatch : REAL;
	END_STRUCT;
	SignalGenerator_Internal_typ : 	STRUCT 
		CurrentPosition : REAL;
		ScanDistance : REAL;
		CurrentNetTime : DINT;
		DeltaNetTime : DINT;
		LastPositionAdded : REAL;
		PositionAdded : BOOL;
		Debug : SignalGenerator_debug_typ;
	END_STRUCT;
	TRIG_END_MODE : 
		(
		TRIG_END_MODE_TIME,
		TRIG_END_MODE_LENGTH
		);
END_TYPE

(*Aux support for DS Module*)

TYPE
	DS4389_DRV_FORCE_typ : 	STRUCT 
		ForceAll : BOOL;
		Force : ARRAY[0..3]OF BOOL;
		ForceOff : ARRAY[0..3]OF BOOL;
		ForceOn : ARRAY[0..3]OF BOOL;
	END_STRUCT;
	DS4389_DRV_Internal_typ : 	STRUCT 
		_AsIOAccWrite : ARRAY[0..1]OF AsIOAccWrite;
		ClearMask : UDINT;
		SetMask : UDINT;
		ChannelName : ARRAY[0..1]OF STRING[40];
		Init : ARRAY[0..1]OF BOOL;
	END_STRUCT;
END_TYPE
