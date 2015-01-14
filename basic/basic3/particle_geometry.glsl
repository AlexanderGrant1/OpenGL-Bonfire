//Basically creates a billboarded square from a billboarded point passed in from the vertex shader.
//I used your geometry pass through example to figure out how this works
#version 420 core
layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

out vec2 textureCoord;//Texture coordinates for each individual vertex
out vec4 position;
void main()
{
	vec4 pointPos = gl_in[0].gl_Position;

	//Bottom left of the square
	vec2 bottom_left = pointPos.xy + vec2(-2, -2);
	gl_Position = vec4(bottom_left, pointPos.zw);
	textureCoord = vec2(0.0, 0.0);
	position = gl_Position;
	EmitVertex();  
  
	//Top left of the square
	vec2 top_left = pointPos.xy + vec2(-2, 2);
	gl_Position = vec4(top_left, pointPos.zw);
	textureCoord = vec2(0.0, 1.0);
	position = gl_Position;
	EmitVertex();  
  
	//Bottom right of the square
	vec2 bottom_right = pointPos.xy + vec2(2, -2);
	gl_Position = vec4(bottom_right, pointPos.zw);
	textureCoord = vec2(1.0, 0.0);
	position = gl_Position;
	EmitVertex();  

	//Top right of the square
	vec2 top_right = pointPos.xy + vec2(2, 2);
	gl_Position = vec4(top_right, pointPos.zw);
	textureCoord = vec2(1.0, 1.0);
	position = gl_Position;
	EmitVertex();  

	EndPrimitive();  
}
