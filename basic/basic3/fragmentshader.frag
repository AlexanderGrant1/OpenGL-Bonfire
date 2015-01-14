// Minimal fragment shader

#version 420

in vec3 frag_normals;
in vec3 frag_position;
in vec3 frag_light_dir;
in vec2 ftexcoord;
out vec4 outputColor;

layout (binding=0) uniform sampler2D tex1;
layout (binding=1) uniform sampler2D bump_texture;//Using 2 textures.  I used your example to figure out how to do this

uniform uint brightness;
uniform uint applyLighting;
uniform uint is_shadow;
uniform uint flicker;//should flicker value
uniform uint is_bump; //should draw bump map
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
	float attenuation = brightness / (1.0 + attenuation_k * pow(distanceToLight, 2));
	vec4 texture_diffuse = vec4(1.0, 0.5, 0, 1.0);
	if(flicker == 1)
	{
		texture_diffuse = vec4(1.0, 0, 0, 1.0);//change the colour slightly if it's flickering
	}
	else
	{
		texture_diffuse = vec4(1.0, 0.4, 0, 1.0);//default lighting colour for no flicker
	}

	vec4 texcolour = texture(tex1, ftexcoord);
	vec3 NewNormal = frag_normals;
	if(is_bump == 1)
	{
		vec3 norm = vec3(texture2D(bump_texture, ftexcoord).xyz * 2.0 - 1.0);//http://stackoverflow.com/questions/22511073/libgdx-using-normal-map-texture-for-shader-in-3d
		NewNormal = frag_normals + norm;
	}
		vec3 diffuse = max(dot(NewNormal, normalized_light), 0.0) * texture_diffuse.xyz;
	if(applyLighting == 1)//Lighting is applied to everything except the skybox and bonfire logs
	{
		vec4 texture_diffuse = vec4(diffuse,1) * texcolour;
		if(is_shadow == 1)
		{
			outputColor = vec4(0, 0,0, 0.5);//colour the shadow
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