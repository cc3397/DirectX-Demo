// Tessellation domain shader
// After tessellation the domain shader processes the all the vertices
//pass in both textures aswell as the sampler
Texture2D texture0 : register(t0);
Texture2D texture1 : register(t4);
SamplerState Sampler0 : register(s0);


cbuffer MatrixBuffer : register(cb1)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};
cbuffer TesBuffer : register(cb2)//pass in counter and height for height mapping
{
	float height;
	float tesFactor;
	float counter;
};
struct ConstantOutputType
{
    float edges[3] : SV_TessFactor;
    float inside : SV_InsideTessFactor;
};

struct InputType
{
    float3 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct OutputType
{
    float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
    
};

[domain("tri")]
OutputType main(ConstantOutputType input, float3 uvwCoord : SV_DomainLocation, const OutputPatch<InputType, 3> patch)
{
    float3 vertexPosition;
	float2 text;
	float3 norm;
    OutputType output;
	float2 textWater;

	float4 texColourWorld;
	float4 texColourWater;
	

    // Determine the position of the new vertex.
	vertexPosition = uvwCoord.x * patch[0].position + uvwCoord.y * patch[1].position + uvwCoord.z * patch[2].position;
	text = uvwCoord.x * patch[0].tex + uvwCoord.y * patch[1].tex + uvwCoord.z * patch[2].tex; //determine new texture co-ordinates
	norm = uvwCoord.x * patch[0].normal + uvwCoord.y * patch[1].normal + uvwCoord.z * patch[2].normal;//determine new normals
	
	norm = normalize(norm);


	texColourWorld = texture0.SampleLevel(Sampler0, text, 0);//sample world texture for height mapping

	//setup moving texture co-ords for water sim
	textWater = text;
	textWater.xy += counter;//make texture move around shape, simulating water

	texColourWater = texture1.SampleLevel(Sampler0, textWater, 0);//sample water texture

	//height mapping
	if (texColourWorld.x > 0.01)//land
	{
		vertexPosition.x += norm.x * ((texColourWorld.x / 2)* height);
		vertexPosition.y += norm.y * ((texColourWorld.x / 2) * height);
		vertexPosition.z += norm.z * ((texColourWorld.x /2)* height);
	}
	else if (texColourWorld.x <= 0)//"animate" water
	{
		vertexPosition.x += (norm.x * ((texColourWater.x / 2) * (height / 2)));
		vertexPosition.y += (norm.y * ((texColourWater.x / 2) * (height /2)));
		vertexPosition.z += (norm.z * ((texColourWater.x / 2) * (height/2 )));
	}
		

   // Calculate the position of the new vertex against the world, view, and projection matrices.
    output.position = mul(float4(vertexPosition, 1.0f), worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);


	//pass texture co-ordinates down pipeline
	output.tex = text;

	

    return output;
}

