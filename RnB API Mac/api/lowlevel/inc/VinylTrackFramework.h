/*
 *  Copyright � 2004 Baby Talk FX, LLC  All Rights Reserved.
 *
 *  Questions and comments to:
 *       <factotum@mspinky.com>
 */

#ifndef VINYLTRACK_FRAMEWORK_H
#define VINYLTRACK_FRAMEWORK_H

#ifdef MSVC_DLL_BUILD
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT 
#endif

#ifndef Float32
#define Float32 float
#endif

#ifndef Float64
#define Float64 double
#endif

#ifndef Int32
#define	Int32	long
#endif

#ifndef TARGET_MACH_O
#ifdef __cplusplus
extern "C" {
#endif
#endif
	
DLLEXPORT void *MPVT2_CreateNew(int max_buffer_size, double fs); 

DLLEXPORT void MPVT2_Destruct(void *object);

DLLEXPORT void MPVT2_ProcessBuffer(void *object, Float32 *inBufferLeft, Float32 *inBufferRight, 
	long num_samps,  Float64  *velocityVals, Float64 *powerVals, Float64 *positionVals, 
	long &num_measurements);

DLLEXPORT void MPVT2_SetSignalPowerThreshold(void *object, Float64 inThresh);
DLLEXPORT void MPVT2_SetCutoffSharpness(void *object, Float64 inSharpness); 
DLLEXPORT void MPVT2_SetAbsoluteMode(void *object, bool inMode);
DLLEXPORT void MPVT2_SetVinylGeneration(void *object, long generation);
DLLEXPORT void MPVT2_SetSampleRate(void *object, Float64 in_sample_rate);

DLLEXPORT Float64 MPVT2_Query_ErrorMetric(void *object);
DLLEXPORT Float64 MPVT2_Query_Velocity(void *object);
DLLEXPORT Float64 MPVT2_Query_SignalPower(void *object);
DLLEXPORT Float64 MPVT2_Query_AbsolutePosition(void *object); 
DLLEXPORT Int32	  MPVT2_Query_VinylGeneration(void *object);
DLLEXPORT bool    MPVT2_Query_AbsoluteMode(void *object);

#ifndef TARGET_MACH_O
#ifdef __cplusplus
}
#endif
#endif

#endif