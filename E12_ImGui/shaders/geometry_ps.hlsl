Texture2D texture0 : register(t0);
SamplerState Sampler0 : register(s0);

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

float4 main(InputType input) : SV_TARGET
{
	// color the pixel yellow
    //return float4(1.0, 1.0, 0.0, 1.0); // color the pixel yellow - equal read and equal green makes yellow
    float4 t;
   // float4 textureColor2;
    t.w = 1.0f;
	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
    t = texture0.Sample(Sampler0, input.tex.xy).rgba;
    //textureColor2 = texture1.Sample(Sampler0, input.tex);

	// invert colors on texture1
	//return 1 - textureColor1;

	// invert colors on texture2
	//return 1 - textureColor2;

	// blend texture1 and texture 1
    // return lerp(textureColor1, textureColor2, 0.5);

	// color shifting
    //float4 finalCol;
	//finalCol.x = textureColor1.z;
	//finalCol.y = textureColor1.y;
	//finalCol.z = textureColor1.x;
	//finalCol.w = 1.0f ;

	// return texture colour
    return t;
}