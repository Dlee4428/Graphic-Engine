#version 410
layout (location = 0) in  vec4 vVertex;
layout (location = 1) in  vec4 vNormal;
layout (location = 2) in  vec2 texCoords;

#define NUM_LIGHTS 2

out vec3 vertNormal;
out vec3 lightDir[NUM_LIGHTS];
out vec3 vertDir; 
out vec2 uvCoords;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;

uniform vec3 lightPos[NUM_LIGHTS];

void main() {
	uvCoords = texCoords;
	vertNormal = mat3(transpose(inverse(normalMatrix))) * vNormal.xyz; /// Rotate the normal to the correct orientation 
	vec3 vertPos = vec3(viewMatrix * modelMatrix * vVertex); /// Creates the vertex position (-eyeDir)  
	vertDir = normalize(vertPos);
	gl_Position =  projectionMatrix * viewMatrix * modelMatrix * vVertex; 
	
	for(int i = 0; i < NUM_LIGHTS; i++){
		lightDir[i] = normalize(lightPos[i] +  -vertPos); /// Create the light direction vector 
	} 
}
