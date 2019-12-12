#include <glew.h>
#include <iostream>

#include "Scene0.h"

#include "Window.h"
#include "Debug.h"
#include "Camera.h"
#include "GameObject.h"
#include "ObjLoader.h"
#include "Mesh.h"
#include "Shader.h"
#include "MMath.h"
#include "Noise.h"


Scene0::Scene0() :
	camera(nullptr),
	meshPtr(nullptr),
	shaderPtr(std::vector<Shader*>()),
	skyboxPtr(nullptr),
	lightSource(nullptr),
	deltaTime(0),
	noise(0){}

Scene0::~Scene0() {
}

bool Scene0::OnCreate() {
	camera = new Camera();
	shaderPtr.reserve(3);

	Noise perlinNoise;
	noise = perlinNoise.CreateNoise3D();
	perlinNoise.SetNoiseFrequency(50000);

	// Texture Creation
	if (ObjLoader::loadOBJ("skull.obj") == false) {
		return false;
	}
	meshPtr = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	shaderPtr.push_back(new Shader("reflectionVert.glsl", "reflectionFrag.glsl"));
	skyboxPtr = new SkyBox();

	if (meshPtr == nullptr || shaderPtr.empty()) {
		Debug::FatalError("Couldn't create game object assets", __FILE__, __LINE__);
		return false;
	}

	if (skyboxPtr->LoadSkyBox(skyboxPtr->faces) == false) {
		Debug::Error("Cannot Load Skybox texture", __FILE__, __LINE__);
		return false;
	}

	// Reflection pointers
	gameObjects.push_back(new GameObject(meshPtr, shaderPtr[0], skyboxPtr));
	gameObjects.push_back(new GameObject(meshPtr, shaderPtr[0], skyboxPtr));

	for (auto objects : gameObjects) {
		if (objects == nullptr) {
			Debug::FatalError("GameObject could not be created", __FILE__, __LINE__);
			return false;
		}
	}

	camera->OnCreate();

	// Allocating lightSource memory by assigned number
	lightSource = new Vec3[LIGHT_SOURCE_NUM];

	// Assign lightSource array of Vec3
	lightSource[1] = Vec3(-10.0, 20.0, 20.0);
	lightSource[2] = Vec3(10.0, -20.0, -20.0);

	return true;
}

void Scene0::OnDestroy() {
	if(camera) delete camera, camera = nullptr;
	if(meshPtr) delete meshPtr, meshPtr = nullptr;
	if(skyboxPtr) delete skyboxPtr, skyboxPtr = nullptr;

	if (shaderPtr.size() > 0) {
		for (auto& s : shaderPtr) {
			delete s;
			s = nullptr;
		}
		shaderPtr.clear();
	}

	// Free GameObject memory
	if (gameObjects.size() > 0) {
		for (auto g : gameObjects) {
			delete g;
			g = nullptr;
		}
		gameObjects.clear();
	}

	// Free lightSource memory
	if(lightSource) delete[] lightSource, lightSource = nullptr;
}

void Scene0::HandleEvents(const SDL_Event& sdlEvent) {
	camera->HandleEvents(sdlEvent);

	if (sdlEvent.type == SDL_EventType::SDL_KEYDOWN) {
		switch (sdlEvent.key.keysym.sym) {
		case SDLK_1:
			chromatic = false;
			printf("\nRef mix shader");
			break;
		case SDLK_2:
			chromatic = true;
			printf("\nChromatic Shader");
			break;
		}
	}
}

void Scene0::Update(const float deltaTime_) {
	deltaTime += deltaTime_;

	for (GameObject* objects : gameObjects) {
		objects->Update(deltaTime_);
	}
	// ModelMatrix manipulations here
	static float rotation = 0.0f;
	rotation += 0.5f;
	gameObjects[0]->setModelMatrix(MMath::rotate(rotation, Vec3(0.0, -5.0, 0.0)) 
		* MMath::translate(Vec3(7.0f, 0.0f, 0.0f))
		* MMath::scale(Vec3(0.5f, 0.5f, 0.5f)));
	gameObjects[1]->setModelMatrix(MMath::rotate(rotation, Vec3(0.0, -5.0, 0.0)) 
		* MMath::translate(Vec3(0.0f, 0.0f, 0.0f)) 
		* MMath::scale(Vec3(1.0f, 1.0f, 1.0f)));

	camera->Update(deltaTime_);
}

void Scene0::Render() const {
	/// Clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	/// Draw your scene here
	for (GameObject* objects : gameObjects) {
		GLuint program = objects->getShader()->getProgram();
		glUseProgram(program);

		/// These pass the matricies and the light position to the GPU
 		glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxPtr->getTextureID());
		glUniformMatrix4fv(objects->getShader()->getUniformID("projectionMatrix"), 1, GL_FALSE, camera->getProjectionMatrix()); // Projection Matrix uniform
		glUniformMatrix4fv(objects->getShader()->getUniformID("viewMatrix"), 1, GL_FALSE, camera->getViewMatrix()); // View Matrix uniform
		glUniform3fv(objects->getShader()->getUniformID("viewPos"), 2, camera->getPosition()); // Camera Pos uniform
			
		glUniform1fv(objects->getShader()->getUniformID("time"), 1, &deltaTime); // Time uniform
		glUniform1uiv(objects->getShader()->getUniformID("pNoise"), 1, &noise); // Noise Uniform
		glUniform1i(objects->getShader()->getUniformID("chromatic"), chromatic); // Chromatic Light switch uniform

 		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		objects->Render();
	}
	camera->Render();

	glUseProgram(0);
}
