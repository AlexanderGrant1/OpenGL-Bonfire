//Basically creates a billboarded square from a billboarded point passed in from the vertex shader.
//I used your slides to figure out how this works
#version 400
layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

out vec2 tex_coord;
out vec4 position;
void main()
{
   for (int i = 0; i<gl_in.length(); i++)
   {
		vec4 P = gl_in[0].gl_Position;

		//Bottom left of the square
		vec2 bottom_left = P.xy + vec2(-2, -2);
		gl_Position = vec4(bottom_left, P.zw);
		tex_coord = vec2(0.0, 0.0);
		position = gl_Position;
		EmitVertex();  
  
		//Top left of the square
		vec2 top_left = P.xy + vec2(-2, 2);
		gl_Position = vec4(top_left, P.zw);
		tex_coord = vec2(0.0, 1.0);
		position = gl_Position;
		EmitVertex();  
  
		//Bottom right of the square
		vec2 bottom_right = P.xy + vec2(2, -2);
		gl_Position = vec4(bottom_right, P.zw);
		tex_coord = vec2(1.0, 0.0);
		position = gl_Position;
		EmitVertex();  

		//Top right of the square
		vec2 top_right = P.xy + vec2(2, 2);
		gl_Position = vec4(top_right, P.zw);
		tex_coord = vec2(1.0, 1.0);
		position = gl_Position;
		EmitVertex();  

		EndPrimitive();  
   }
}
