float4 main(float3 pos : POSITION) : SV_POSITION
{
	return float(pos,1.0f);
}