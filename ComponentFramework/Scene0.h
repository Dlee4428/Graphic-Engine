#ifndef SCENE0_H
#define SCENE0_H

#include <vector>
#include "Scene.h"
#include "Vector.h"

using namespace MATH;

#ifndef LIGHT_SOURCE_NUM
#define LIGHT_SOURCE_NUM 3
#endif // !LIGHT_SOURCE_NUM

/// Forward declarations 
union SDL_Event;
class Camera;
class GameObject;
class Mesh;
class Shader;
class SkyBox;
class Timer;

class Scene0 : public Scene {
private:
	Camera* camera;
	std::vector<GameObject*> gameObjects;
	std::vector<Shader*> shaderPtr;
	Mesh* meshPtr;
	SkyBox* skyboxPtr;
	Vec3* lightSource;
	GLuint noise;
	float deltaTime;
	bool chromatic = false;
public:
	explicit Scene0();
	virtual ~Scene0();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime_) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event &sdlEvent) override;
};


#endif // SCENE0_H