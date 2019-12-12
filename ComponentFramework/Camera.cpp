#include <glew.h>
#include "Camera.h"
#include "MMath.h"

Camera::Camera(){
	createProjection(45.0f, (16.0f / 9.0f), 0.5f, 100.0f);
	createView(Vec3(0.0, 0.0, 10.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f)); // By doing pos z to 1 makes FPS view
}

Camera::~Camera() {}

bool Camera::OnCreate() {
	skybox = new SkyBox();
	trackball = new Trackball();
	skybox->OnCreate();

	return true;
}

// fovy = field of view y
void Camera::OnDestroy() {
	if (skybox) delete skybox, skybox = nullptr;
	if (trackball) delete trackball, trackball = nullptr;
	skybox->OnDestroy();
}

void Camera::createProjection(float fovy, float aspect, float near, float far) {
	projection = MMath::perspective(fovy, aspect, near, far);
}

void Camera::createView(Vec3 pos_, Vec3 at_, Vec3 up_) {
	pos = pos_;
	at = at_;
	up = up_;

	//	rotation = MMath::rotate(-15.0, Vec3(0.0, 1.0, 0.0));
//	translation = MMath::translate(0.0, 0.0, -10.0);
//	view = rotation * translation;
	view = MMath::lookAt(pos, at, up); 
	view.print();
}

void Camera::HandleEvents(const SDL_Event& sdlEvent)
{
	trackball->HandleEvents(sdlEvent);
}

void Camera::Update(const float deltaTime_)
{
	skybox->Update(deltaTime_);
}

void Camera::Render()
{
	if (skybox != nullptr) {
		glDepthMask(GL_FALSE);
		glUseProgram(skybox->getShader()->getProgram());
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->getTextureID());
		glUniformMatrix4fv(skybox->getShader()->getUniformID("projectionMatrix"), 1, GL_FALSE, projection * trackball->getMatrix4());
		skybox->Render();

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		glDepthMask(GL_TRUE);
	}
}



