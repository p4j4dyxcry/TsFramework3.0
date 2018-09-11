#ifndef _HLSL_TYPE_
#define _HLSL_TYPE_
    #ifdef TS_FRAMEWORK
        
        #define TS_HLSL_CBUFFER struct
        #define TS_HLSL_TBUFFER struct

        #define TS_HLSL_FLOAT2 TS::Vector2
        #define TS_HLSL_FLOAT3 TS::Vector3
        #define TS_HLSL_FLOAT4 TS::Vector4

        #define TS_HLSL_MATRIX TS::Matrix4

        template<unsigned N = 0> struct ts_cbuffer_register { unsigned RegisterIndex() { return N; } };

        #define TS_HLSL_REGISTER_B(register_id) : public ts_cbuffer_register<register_id>
        #define TS_HLSL_SEMANTICS(semantics_name)
        #define TS_HLSL_PACKOFFSET(offset_name)

    #else // hlsl

        #define TS_HLSL_CBUFFER cbuffer
        #define TS_HLSL_TBUFFER tbuffer

        #define TS_HLSL_FLOAT2 float2
        #define TS_HLSL_FLOAT3 float3
        #define TS_HLSL_FLOAT4 float4

        #define TS_HLSL_MATRIX float4x4

        #define TS_HLSL_REGISTER_B(register_id)   : register(b##register_id)
        #define TS_HLSL_SEMANTICS(semantics_name) : semantics_name
        #define TS_HLSL_PACKOFFSET(offset_name)   : packoffset(offset_name)
    #endif
#endif