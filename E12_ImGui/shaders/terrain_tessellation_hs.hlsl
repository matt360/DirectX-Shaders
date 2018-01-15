// HULL SHADER

cbuffer CameraBuffer : register(cb0)
{
    float3 cameraPosition;
    float padding;
};

// Input control point
struct VertexOut // VS_CONTROL_POINT_OUTPUT
{
	//float3 vPosition : WORLDPOS;
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

// Output control point
struct HullOut
{
	//float3 vPosition : WORLDPOS; 
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

// Output patch constant data.
// Triangle
struct PatchTess // HS_CONSTANT_DATA_OUTPUT
{
	//float EdgeTessFactor[3]			: SV_TessFactor; // e.g. would be [4] for a quad domain
	//float InsideTessFactor			: SV_InsideTessFactor; // e.g. would be Inside[2] for a quad domain
	// TODO: change/add other stuff
    float edges[3] : SV_TessFactor;
    float inside : SV_InsideTessFactor;
};

#define NUM_CONTROL_POINTS 3

// Patch Constant Function
PatchTess CalcHSPatchConstants(
	InputPatch<VertexOut, NUM_CONTROL_POINTS> inputPatch,
	uint patchId : SV_PrimitiveID)
{
	PatchTess output;
    float tessellationAmount;

    float3 distance = (float3) inputPatch[patchId].position - cameraPosition;

    if (length(distance) > 8.0f)
        tessellationAmount = 4.0f;
    else
        tessellationAmount = 64.0f / length(distance);

    // Tessellating a triangle patch also consists of two parts:
	// 1. Three edge tessellation factors control how much to tessellate along each edge.
    // Set the tessellation factors for the three edges of the triangle.
	// Uniformly tessellate the patch 'tessellationAmount' times.
    output.edges[0] = tessellationAmount;
    output.edges[1] = tessellationAmount;
    output.edges[2] = tessellationAmount;
	// Insert code to compute Output here
    //output.EdgeTessFactor[0] =
	//output.EdgeTessFactor[1] =
	//output.EdgeTessFactor[2] =

    // 2. One interior tessellation factor indicates how much to tessellate the triangle patch.
    // Set the tessellation factor for tessallating inside the triangle.
	// Uniformly tessellate the patch 'tessellationAmount' times.

    output.inside = tessellationAmount;
	//output.InsideTessFactor = 15; // e.g. could calculate dynamic tessellation factors instead

    return output;
}

[domain("tri")]
[partitioning("fractional_even")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("CalcHSPatchConstants")]
HullOut main(
	InputPatch<VertexOut, NUM_CONTROL_POINTS> patch,
	uint pointId : SV_OutputControlPointID,
	uint patchId : SV_PrimitiveID)
{
    HullOut output;
	
    // Set the position for this control point as the output position.
    output.position = patch[pointId].position;

    // Set the input tex as the output tex.
    output.tex = patch[pointId].tex;

    output.normal = patch[pointId].normal;

    return output;
}
