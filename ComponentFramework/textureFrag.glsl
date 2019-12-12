#version 410

#define NUM_LIGHTS 2

in vec3 vertNormal;
in vec3 lightDir[NUM_LIGHTS];
in vec3 vertDir;
in vec2 uvCoords;
out vec4 fragColor;

uniform sampler2D myTexture;
uniform sampler3D pNoise;

float spec[NUM_LIGHTS];
vec4 specLightColor[NUM_LIGHTS];
float diff[NUM_LIGHTS];

vec3 reflection[NUM_LIGHTS];


void main() { 
	spec[0] = 0.1;
	specLightColor[0] = vec4(0.0, 1.0, 0.0, 0.2);
	spec[1] = 0.1;
	specLightColor[1] = vec4(1.0, 0.0, 0.0, 0.2);
	
	for(int i = 0; i < NUM_LIGHTS; i++){
		diff[i] = max(dot(vertNormal, lightDir[i]), 0.0);
		vec4 textureColor = texture(myTexture, uvCoords);
		
		if(diff[i] > 0.0){
			reflection[i] = normalize(reflect(lightDir[i], vertNormal));
			spec[i] = max(dot(vertDir, reflection[i]), 0.0);
			spec[i] = pow(spec[i], 10.0);
		}
		vec4 ka = 0.1 * textureColor;
		fragColor +=  ka + (diff[i] * textureColor)  + (spec[i] * specLightColor[i]);	
	}	
} 

