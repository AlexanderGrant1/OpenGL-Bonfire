// Minimal fragment shader

#version 420

in vec3 frag_normals;
in vec3 frag_position;
in vec3 frag_light_dir;
in vec2 ftexcoord;
out vec4 outputColor;

uniform sampler2D tex1;
uniform uint applyLighting;
uniform uint is_shadow;
uniform uint flicker;
// Global constants (for this vertex shader)
vec3 specular_albedo = vec3(1.0, 0.8, 0.6);
int shininess = 8;
vec3 normalized_light;
void main()
{
	float distanceToLight = length(frag_light_dir);	// For attenuation
	normalized_light = normalize(frag_light_dir);					// Normalise our light vector


	// Calculate the specular component using Phong specular reflection
	vec3 V = normalize(-frag_position.xyz);
	vec3 R = reflect(-normalized_light, frag_normals);
	vec3 specular = pow(max(dot(R, V), 0.0), shininess) * specular_albedo;
	
	// Calculate the attenuation factor;
	float attenuation_k = 0.75;
	float attenuation = 200 / (1.0 + attenuation_k * pow(distanceToLight, 2));
	vec4 texture_diffuse = vec4(1.0, 0.5, 0, 1.0);
	if(flicker == 1)
	{
		texture_diffuse = vec4(1.0, 0, 0, 1.0);//change the colour slightly if it's flickering
	}
	else
	{
		texture_diffuse = vec4(1.0, 0.4, 0, 1.0);//default lighting colour for no flicker
	}
	vec3 diffuse = max(dot(frag_normals, normalized_light), 0.0) * texture_diffuse.xyz;
	vec4 texcolour = texture(tex1, ftexcoord);
	if(applyLighting == 1)//Lighting is applied to everything except the skybox
	{
		vec4 texture_diffuse = vec4(diffuse,1) * texcolour;
		if(is_shadow == 1)
		{
			outputColor = vec4(0, 0, 0, 1);
		}
		else
		{
			outputColor = attenuation * (texture_diffuse);
		}
	}
	else
	{
		//At the moment this is only used for the skybox and sun.  I want them to show fully regardless of light positions
		outputColor = texcolour;
	}
}