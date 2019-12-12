#include "GameObject.h"
#include "Mesh.h"
#include "Shader.h"
#include "MMath.h"
#include "SkyBox.h"

GameObject::GameObject(Mesh *mesh_, Shader *shader_, Texture* texture_) : 
	mesh(nullptr), shader(nullptr), skybox(nullptr), texture(nullptr) {
	
	mesh = mesh_;
	shader = shader_;
	texture = texture_;
	modelMatrixID = shader->getUniformID("modelMatrix");
	normalMatrixID = shader->getUniformID("normalMatrix");

}

GameObject::GameObject(Mesh* mesh_, Shader* shader_, SkyBox* skybox_) : 
	mesh(nullptr), shader(nullptr), skybox(nullptr), texture(nullptr) {

	mesh = mesh_;
	shader = shader_;
	skybox = skybox_;
	modelMatrixID = shader->getUniformID("modelMatrix");
	normalMatrixID = shader->getUniformID("normalMatrix");
}

GameObject::~GameObject() {}

void GameObject::Update(float deltaTime_) {}

void GameObject::Render() const {
	Matrix3 normalMatrix = modelMatrix;
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, modelMatrix);
	glUniformMatrix3fv(normalMatrixID, 1, GL_FALSE, normalMatrix);
	
	if (texture) {
		glBindTexture(GL_TEXTURE_2D, texture->getTextureID());
	}
	if (skybox) {
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->getTextureID());
	}
	
	mesh->Render();

	/// Unbind the texture
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}