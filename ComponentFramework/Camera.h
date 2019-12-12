#ifndef CAMERA_H
#define CAMERA_H

#include "Vector.h"
#include "Matrix.h"
#include "SkyBox.h"
#include "Trackball.h"

union SDL_Event;
using namespace MATH;
class Camera {
private:
	SkyBox* skybox;
	Trackball* trackball;
	Matrix3 model;
	Matrix4 projection;
	Matrix4 view, rotation, translation;
	Vec3 pos, at, up;
public:
	inline Matrix4 getProjectionMatrix() const { return projection; }
	inline Matrix4 getViewMatrix() const { return view; }
	inline Matrix3 getModelMatrix() const { return model; }
	inline Vec3 getPosition() const { return pos; }

	bool OnCreate();
	void OnDestroy();
	void createProjection(float fovy, float aspectRatio, float near, float far);
	void createView(Vec3 pos_, Vec3 at_, Vec3 up_);
	void HandleEvents(const SDL_Event& sdlEvent);
	void Update(const float deltaTime_);
	void Render();
	Camera();
	~Camera();
};

#endif



