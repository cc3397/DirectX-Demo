// Tessellation Hull Shader
// Prepares control points for tessellation
cbuffer TesBuffer : register(cb0)
{
	float height;
	float tesFactor;

};
struct InputType
{
	float3 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct ConstantOutputType
{
    float edges[3] : SV_TessFactor;
    float inside : SV_InsideTessFactor;
};

struct OutputType
{
    float3 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

ConstantOutputType PatchConstantFunction(InputPatch<InputType, 3> inputPatch, uint patchId : SV_PrimitiveID)
{    
    ConstantOutputType output;

    // Set the tessellation factors for the three edges of the triangle to the user specified factor.
	output.edges[0] = tesFactor;
	output.edges[1] = tesFactor;
	output.edges[2] = tesFactor;

    // Set the tessellation factor for tessallating inside the triangle to the user specified factor.
	output.inside = tesFactor;

    return output;
}


[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("PatchConstantFunction")]
OutputType main(InputPatch<InputType, 3> patch, uint pointId : SV_OutputControlPointID, uint patchId : SV_PrimitiveID)
{
    OutputType output;
    // Set the position for this control point as the output position.
    output.position = patch[pointId].position;

	//pass texture co-ordinates down pipeline
	output.tex = patch[pointId].tex;

	//pass normals down pipeline
	output.normal = patch[pointId].normal;

    return output;
}