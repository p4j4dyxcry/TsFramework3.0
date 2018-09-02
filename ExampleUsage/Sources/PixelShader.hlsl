float4 main(float4 pos : SV_Position) : SV_Target
{
	return float4(pos.xyz,1);
}