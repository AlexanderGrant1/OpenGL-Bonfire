#version 330 core

// Interpolated values from the vertex shaders
in vec2 tex_coord;
in vec4 position;
// Ouput data
out vec4 color;

uniform sampler2D myTextureSampler;
uniform mat4 VP;
void main(){
	// Output color = color of the texture at the specified UV
	vec4 pos = inverse(VP) * position;
	color = texture2D(myTextureSampler, tex_coord);
	if (color.w < 0.4)
	{
		discard;//this removes a bad square outline effect i was having.  This makes it look much nicer.
	}
	color = vec4(0.8,1-(pos.y + 13)/10,0,1);//temporary fire colour code
}