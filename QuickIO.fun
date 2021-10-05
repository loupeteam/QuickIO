
FUNCTION EncoderEmulatorFn : BOOL
	VAR_INPUT
		Encoder : EncoderEmulator_typ;
	END_VAR
END_FUNCTION

FUNCTION SignalGeneratorFn : BOOL
	VAR_INPUT
		Encoder : SignalGenerator_typ;
	END_VAR
END_FUNCTION

FUNCTION ScaleEncoder : LREAL
	VAR_INPUT
		x : LREAL;
		x1 : LREAL;
		x2 : LREAL;
		y1 : LREAL;
		y2 : LREAL;
	END_VAR
END_FUNCTION

FUNCTION_BLOCK DS4389_DRV
	VAR_INPUT
		pDeviceName : UDINT;
		ForceAll : BOOL;
		Force : ARRAY[0..3] OF BOOL;
		ForceOn : ARRAY[0..3] OF BOOL;
		ForceOff : ARRAY[0..3] OF BOOL;
	END_VAR
	VAR_OUTPUT
		Status : ARRAY[0..1] OF UINT;
	END_VAR
	VAR
		Internal : DS4389_DRV_Internal_typ;
	END_VAR
END_FUNCTION_BLOCK
