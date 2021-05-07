R"(

// Positions
in vec4 aPosition;
out vec2 outPosition;

// Colors
in  vec4 aColor;
out vec4 outColor;

// Texture coordinates
in  vec2 aTexCoord;
out vec2 outTexCoord;

// Matrices
uniform mat4 uPerspective;
//uniform mat4 uProjTrans;
//
//attribute vec4 vertex_positions;
//attribute vec4 quad_colors;
//attribute float s;


// Transform and pass through
void main(void) {
//    gl_Position = uProjTrans*vertex_positions;
    gl_Position = uPerspective*aPosition;
    outPosition = aPosition.xy; // Need untransformed for scissor
//    outColor = s * quad_colors;
    outColor = aColor;
    outTexCoord = aTexCoord;
}

)"

//
//"attribute vec4 vertex_positions;\n" //
//+ "attribute vec4 quad_colors;\n" //
//+ "attribute float s;\n"
//+ "uniform mat4 u_projTrans;\n" //
//+ "varying vec4 v_color;\n" //
//+ "void main()\n" //
//+ "{\n" //
//+ "   v_color = s * quad_colors;\n" //
//+ "   gl_Position =  u_projTrans * vertex_positions;\n" //
//+ "}\n";
