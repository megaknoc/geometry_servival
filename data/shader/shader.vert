uniform vec3 center;
varying float depth;
varying vec2 texcoord;

void main()
{
    // transform the vertex position
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

    // transform the texture coordinates
    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;

	texcoord = gl_TexCoord[0].xy;

    // forward the vertex color
    gl_FrontColor = gl_Color;

	depth = gl_Position.z;
}

