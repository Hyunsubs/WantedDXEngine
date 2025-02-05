#ifndef _VS
#define _VS
float4 main( float3 pos : POSITION ) : SV_POSITION
{
    return float4(pos, 1.f);
}
#endif