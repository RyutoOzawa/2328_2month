#include"Basic.hlsli"

Texture2D<float4> tex:register(t0);		//0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);		//0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
	float4 texcolor  = float4(tex.Sample(smp,input.uv)) * color;


	float3 light = normalize(float3(1,-1,1));	//右下奥　向きのライト
	float diffuse = saturate(dot(-light, input.normal));//diffuseを[0,1]の範囲にClampする
	float brightness = diffuse + 0.3f;//アンビエント項を0.3として計算
	return float4(texcolor.rgb * brightness,texcolor.a);//輝度をRGBに代入して出力
	
	//ポリゴンを白で塗りつぶし
	//return float4(1,1,1,0.5f);

	//return color;
}