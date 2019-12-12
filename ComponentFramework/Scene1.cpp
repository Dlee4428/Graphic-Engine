#include <glew.h>
#include <iostream>

#include "Scene1.h"

#include "Window.h"
#include "Debug.h"
#include "Camera.h"
#include "GameObject.h"
#include "ObjLoader.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "MMath.h"


Scene1::Scene1() :
	camera(nullptr),
	meshPtr(nullptr),
	shaderPtr(nullptr),
	lightSource(nullptr),
	deltaTime(0),
	noise(0) {}

Scene1::~Scene1() {
}

bool Scene1::OnCreate() {
	camera = new Camera();
	texturePtr.reserve(3);

	// Texture Creation
	if (ObjLoader::loadOBJ("sphere.obj") == false) {
		return false;
	}
	meshPtr = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	shaderPtr = new Shader("textureVert.glsl", "textureFrag.glsl");
	texturePtr.push_back(new Texture());
	texturePtr.push_back(new Texture());

	if (meshPtr == nullptr || shaderPtr == nullptr || texturePtr.empty()) {
		Debug::FatalError("Couldn't create game object assets", __FILE__, __LINE__);
		return false;
	}
	if (texturePtr[0]->LoadImage("moon.jpg") == false) {
		Debug::FatalError("Couldn't load texture", __FILE__, __LINE__);
		return false;
	}
	if (texturePtr[1]->LoadImage("earthclouds.jpg") == false) {
		Debug::FatalError("Couldn't load texture", __FILE__, __LINE__);
		return false;
	}
	for (auto texture : texturePtr) {
		if (texture == nullptr) {
			Debug::FatalError("Texture pointer could not be created", __FILE__, __LINE__);
			return false;
		}
	}

	gameObjects.push_back(new GameObject(meshPtr, shaderPtr, texturePtr[0]));
	gameObjects.push_back(new GameObject(meshPtr, shaderPtr, texturePtr[1]));

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
	lightSource[1] = Vec3(0.0, 2.0, 3.0);
	lightSource[2] = Vec3(0.0, -10.0, -3.0);

	return true;
}

void Scene1::OnDestroy() {
	if (camera) delete camera, camera = nullptr;
	if (meshPtr) delete meshPtr, meshPtr = nullptr;
	if (shaderPtr) delete shaderPtr, shaderPtr = nullptr;

	// Free Texture pointer memory
	for (auto texture : texturePtr) {
		if (texture) delete texture;
		texture = nullptr;
	}
	texturePtr.clear();

	// Free GameObject memory
	for (auto objects : gameObjects) {
		if (objects) delete objects;
		objects = nullptr;
	}
	gameObjects.clear();

	// Free lightSource memory
	if (lightSource) delete[] lightSource, lightSource = nullptr;
}

void Scene1::HandleEvents(const SDL_Event& sdlEvent) {
	camera->HandleEvents(sdlEvent);
}

void Scene1::Update(const float deltaTime_) {
	deltaTime += deltaTime_;

	for (GameObject* objects : gameObjects) {
		objects->Update(deltaTime_);
	}

	static float rotation = 0.0f;
	rotation += 0.5f;
	gameObjects[0]->setModelMatrix(MMath::rotate(rotation, Vec3(0.0, -1.0, 0.0))
		* MMath::rotate(-90.0f, Vec3(1.0, 0.0, 0.0)) 
		* MMath::translate(Vec3(7.0f, 0.0f, 0.0f))
		* MMath::scale(Vec3(0.5f, 0.5f, 0.5f)));
	gameObjects[1]->setModelMatrix(MMath::rotate(rotation, Vec3(0.0, -1.0, 0.0))
		* MMath::rotate(-90.0f, Vec3(1.0, 0.0, 0.0)) 
		* MMath::translate(Vec3(0.0f, 0.0f, 0.0f))
		* MMath::scale(Vec3(1.0f, 1.0f, 1.0f)));

	camera->Update(deltaTime_);
}

void Scene1::Render() const {
	/// Clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	/// Draw your scene here
	for (GameObject* objects : gameObjects) {
		GLuint program = objects->getShader()->getProgram();
		glUseProgram(program);

		/// These pass the matricies and the light position to the GPU
		glUniformMatrix4fv(objects->getShader()->getUniformID("projectionMatrix"), 1, GL_FALSE, camera->getProjectionMatrix()); // Projection Matrix uniform
		glUniformMatrix4fv(objects->getShader()->getUniformID("viewMatrix"), 1, GL_FALSE, camera->getViewMatrix()); // View Matrix uniform
		glUniform3fv(objects->getShader()->getUniformID("viewPos"), 2, camera->getPosition()); // Camera Pos uniform

		glUniform3fv(objects->getShader()->getUniformID("lightPos[0]"), 1, lightSource[1]);
		glUniform3fv(objects->getShader()->getUniformID("lightPos[1]"), 1, lightSource[2]);

		objects->Render();
	}
	camera->Render();

	glUseProgram(0);
}
