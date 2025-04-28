// Input structure from vertex buffer
struct VS_INPUT {
    float3 position : POSITION;
    float4 color : COLOR;
};

// Output structure to pixel shader
struct VS_OUTPUT {
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

// World-view-projection matrix
cbuffer ConstantBuffer : register(b0) {
    matrix worldViewProj;
};

// Vertex shader main function
VS_OUTPUT main(VS_INPUT input) {
    VS_OUTPUT output;
    
    // For now, just pass through without transformation matrix
    // This will be replaced with proper matrix multiplication once we add constants
    output.position = float4(input.position, 1.0f);
    output.color = input.color;
    
    return output;
}
