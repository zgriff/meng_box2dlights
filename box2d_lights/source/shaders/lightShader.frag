R"(

#ifdef CUGLES
// This one line is all the difference
precision lowp float;  // highp required for gradient precision
#define MED mediump
#else
#define MED
#endif

// Bit vector for texturing, gradients, and scissoring
uniform int  uType;
// Blur offset for simple kernel blur
uniform vec2 uBlur;

// The texture for sampling
uniform sampler2D uTexture;

// The output color
out vec4 frag_color;

// The inputs from the vertex shader
in vec2 outPosition;
in vec4 outColor;
in float outFrac;
in vec2 outTexCoord;

/**
 * Performs the main fragment shading.
 */
void main(void) {
    frag_color = vec4(outColor.r,outColor.g,outColor.b,outColor.a*outFrac);;
}

)"

