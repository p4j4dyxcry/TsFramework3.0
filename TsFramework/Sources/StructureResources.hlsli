#ifndef _STRUCTURE_RESOURCE
#define _STRUCTURE_RESOURCE
#include "HlslType.hlsli"

TS_HLSL_CBUFFER CameraCbuffer 
TS_HLSL_REGISTER_B(0)
{
    TS_HLSL_MATRIX u_ViewMatrix;
    TS_HLSL_MATRIX u_ProjectionMatrix;
    TS_HLSL_MATRIX u_InvViewMatrix;
    TS_HLSL_MATRIX u_InvProjectionMatrix;
};

TS_HLSL_CBUFFER TransformCBuffer 
TS_HLSL_REGISTER_B(1)
{
    TS_HLSL_MATRIX u_WorldMatrix;
};

#endif