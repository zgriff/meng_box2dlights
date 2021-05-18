R"(

//// Positions
in vec4 aPosition;
out vec2 outPosition;
//
// Colors
in  vec4 aColor;
out vec4 outColor;

// Texture coordinates
in  float aFrac;
out float outFrac;

// Matrices
uniform mat4 uPerspective;
//uniform mat4 uProjTrans;


// Transform and pass through
void main(void) {
    gl_Position = uPerspective*aPosition;
//    outPosition = aPosition.xy; // Need untransformed for scissor
//    outColor = s * quad_colors;
    outColor = aFrac * aColor;
}

)"
