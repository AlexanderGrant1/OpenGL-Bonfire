//Converts particle points to billboarded points
#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 point;

uniform mat4 VP; // Model-View-Projection matrix, but without the Model (the position is in BillboardPos; the orientation depends on the camera)

void main()
{
	// Output position of the vertex
	gl_Position = VP * vec4(point, 1.0f);
	
}

