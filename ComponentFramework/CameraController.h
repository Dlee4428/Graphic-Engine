#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include <vector>

#include "Vector.h"
#include "Matrix.h"
#include "VMath.h"
#include "MMath.h"

using namespace  MATH;

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 9.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class CameraController {
private:
	Vec3 position;
	Vec3 front;
	Vec3 up;
	Vec3 right;
	Vec3 worldUp;

	float yaw;
	float pitch;

	float movementSpeed;
	float mouseSensitivity;
	float zoom;

	void updateCameraVectors()
	{
		Vec3 _front;
		_front.x = cos(DEGREES_TO_RADIANS * yaw) * cos(DEGREES_TO_RADIANS * pitch);
		_front.y = sin(DEGREES_TO_RADIANS * pitch);
		_front.z = sin(DEGREES_TO_RADIANS * yaw) * cos(DEGREES_TO_RADIANS * pitch);
		front = VMath::normalize(_front);
		// re-calculate the right and up vectors
		right = VMath::normalize(VMath::cross(front, worldUp));
		up = VMath::normalize(VMath::cross(right, front));
	}

public:
	CameraController(Vec3 _position = Vec3(0.0f, 0.0f, 0.0f), Vec3 _up = Vec3(0.0f, 1.0f, 0.0f), float _yaw = YAW, float _pitch = PITCH) : 
		front(Vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM) {
		position = _position;
		worldUp = _up;
		yaw = _yaw;
		pitch = _pitch;
		updateCameraVectors();
	}

	CameraController(float _posX, float _posY, float _posZ, float _upX, float _upY, float _upZ, float _yaw, float _pitch) : 
		front(Vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM) {
		position = Vec3(_posX, _posY, _posZ);
		worldUp = Vec3(_upX, _upY, _upZ);
		yaw = _yaw;
		pitch = _pitch;
		updateCameraVectors();
	}

	Matrix4 GetViewMatrix() {
		return MMath::lookAt(position, position + front, up);
	}

	Vec3 GetPosition() {
		return position;
	}

	Vec3 GetFront() {
		return front;
	}

	void ProcessKeyboard(Camera_Movement _direction, float _deltaTime)
	{
		float velocity = movementSpeed * _deltaTime;
		if (_direction == FORWARD)
			position += front * velocity;
		if (_direction == BACKWARD)
			position -= front * velocity;
		if (_direction == LEFT)
			position -= right * velocity;
		if (_direction == RIGHT)
			position += right * velocity;
	}

	void ProcessMouseMovement(float _xOffset, float _yOffset, bool constrainPitch = true) {
		_xOffset *= mouseSensitivity;
		_yOffset *= mouseSensitivity;

		yaw += _xOffset;
		pitch += _yOffset;

		if (constrainPitch) {
			if (pitch > 89.0f) {
				pitch = 89.0f;
			}
			if (pitch < -89.0f) {
				pitch = -89.0f;
			}
		}
		updateCameraVectors();
	}

	void ProcessMouseScroll(float _yOffset) {
		if (zoom >= 1.0f && zoom <= 45.0f) {
			zoom -= _yOffset;
		}

		if (zoom <= 1.0f) {
			zoom = 1.0f;
		}

		if (zoom >= 45.0f) {
			zoom = 45.0f;
		}
	}
	float GetZoom() {
		return zoom;
	}

};
#endif // !CAMERACONTROLLER_H