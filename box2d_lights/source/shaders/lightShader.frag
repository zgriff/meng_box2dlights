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

uniform vec4 uPlayers[8];

// The output color
out vec4 frag_color;

// The inputs from the vertex shader
in vec2 outPosition;
in vec4 outColor;
in float outFrac;

/**
 * Performs the main fragment shading.
 */
void main(void) {
    frag_color = outColor;
}

)"

//"#ifdef GL_ES\n" //
//            + "precision lowp float;\n" //
//            + "#define MED mediump\n"
//            + "#else\n"
//            + "#define MED \n"
//            + "#endif\n" //
//                + "varying vec4 v_color;\n" //
//                + "void main()\n"//
//                + "{\n" //
//                + "  gl_FragColor = "+gamma+"(v_color);\n" //
//                + "}";
