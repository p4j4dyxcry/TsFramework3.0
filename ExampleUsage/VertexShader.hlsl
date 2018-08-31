cbuffer MaterialCB : register (b0)
{
    float4 g_Diffuse;       // xyz = diffuse w = alphaTest
    float4 g_Specluar;      // xyz = color w = power
    float4 g_Ambient;       // xyz = color w = not using
    float3 g_Emissive;      // xyz = emissive w
    float  g_Mataric;       // mataric
    float  g_Roughness;     // roughness
    matrix g_m;
}

float4 main( float4 pos : POSITION ) : SV_POSITION
{
    pos.w = g_Diffuse.w + g_Specluar.a + g_Ambient.x + g_Emissive.y + g_Mataric + g_Roughness;
	return pos;
}