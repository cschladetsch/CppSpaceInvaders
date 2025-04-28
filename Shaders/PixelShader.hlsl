// Input structure from vertex shader
struct PS_INPUT {
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

// Pixel shader main function
float4 main(PS_INPUT input) : SV_TARGET {
    // Just return the interpolated color from vertex shader
    return input.color;
}
