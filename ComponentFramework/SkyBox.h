#ifndef SKYBOX_H
#define SKYBOX_H

#include <glew.h>
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <vector>
#include "Matrix.h"
#include "Shader.h"

class SkyBox
{
private:
	int mode;
	GLuint textureID; // GLuint is the unsigned int typedef from glew
	SDL_Surface* textureSurface;

	class Mesh* meshPtr;
	class Shader* shaderPtr;
	SkyBox* skyboxPtr;
	class GameObject* skyObject;
public:
	SkyBox();
	~SkyBox();
	bool OnCreate();
	void OnDestroy();
	void Update(const float deltaTime_);
	void Render() const;
	inline GLuint getTextureID() const { return textureID; }
	inline Shader* getShader() const { return shaderPtr; }
	bool LoadSkyBox(std::vector<std::string> faces);

	// SkyBox OnCreate()
	std::vector<std::string> faces
	{
		"posx.jpg",
		"negx.jpg",
		"posy.jpg",
		"negy.jpg",
		"posz.jpg",
		"negz.jpg"
	};
};
#endif // SKYBOX_H

