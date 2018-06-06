// Tessellation pixel shader
// Output colour passed to stage.
Texture2D texture0 : register(t0);
Texture2D texture1 : register(t4);
SamplerState Sampler0 : register(s0);


struct InputType
{
    float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

float4 main(InputType input) : SV_TARGET
{
	float4 landTextureColor;
	float4 waterTextureColor;
	

	landTextureColor = texture0.SampleLevel(Sampler0, input.tex, 0);//sample land texture

	waterTextureColor = texture1.SampleLevel(Sampler0, input.tex, 0);//sample water texture

	if (landTextureColor.x == 0)//if pixel is black it is water
	{
			landTextureColor = waterTextureColor;//set colour to texture colour
	}
	else if (landTextureColor.x >= 0.01)
	{
		landTextureColor = float4(0.0f, 0.6f, 0.0f, 1.0f); //set land to be green
	}

return landTextureColor;

}
