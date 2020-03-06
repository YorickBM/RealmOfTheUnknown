#pragma once

// Std. Includes
#include <vector>
#include <map>

// GL Includes
#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ShaderLoader.h"

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Default camera values
const GLfloat YAW = -90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 1.0f;
const GLfloat SENSITIVTY = 0.25f;
const GLfloat ZOOM = 45.0f; //FOV

// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
	// Constructor with vectors
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVTY), zoom(ZOOM)
	{
		this->position = position;
		this->worldUp = up;
		this->yaw = yaw;
		this->pitch = pitch;
		this->updateCameraVectors();
	}

	// Constructor with scalar values
	Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVTY), zoom(ZOOM)
	{
		this->position = glm::vec3(posX, posY, posZ);
		this->worldUp = glm::vec3(upX, upY, upZ);
		this->yaw = yaw;
		this->pitch = pitch;
		this->updateCameraVectors();
	}

	// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix() {
		return glm::lookAt(this->position, this->position + this->front, this->up);
	}

	glm::mat4 GetProjectionViewMatrix(glm::mat4 projectionMatrix) {
		return (projectionMatrix * this->GetViewMatrix());
	}

	void SetPosition(glm::vec3 pos) {
		std::cout << "Modifying Camera Position" << std::endl;
		this->position = pos;
	}
	void printPosition() {
		std::cout << this->position.x << ";" << this->position.y << ";" << this->position.z << std::endl;
	}

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void InteralProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
	{
		GLfloat velocity = this->movementSpeed * deltaTime;

		switch (direction) {
		case FORWARD:
			this->position += this->front * velocity;
			break;
		case BACKWARD:
			this->position -= this->front * velocity;
			break;
		case LEFT:
			this->position -= this->right * velocity;
			break;
		case RIGHT:
			this->position += this->right * velocity;
			break;
		}
	}
	void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime, bool isColliding)
	{
		InteralProcessKeyboard(direction, deltaTime);
	}
	void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
	{
		InteralProcessKeyboard(direction, deltaTime);
	}

	glm::vec3 VectorDifference(Camera_Movement direction, GLfloat deltaTime)
	{
		glm::vec3 _pos(0, 0, 0);
		GLfloat velocity = this->movementSpeed * deltaTime;

		switch (direction) {
		case FORWARD:
			_pos += this->front * velocity;
			break;
		case BACKWARD:
			_pos -= this->front * velocity;
			break;
		case LEFT:
			_pos -= this->right * velocity;
			break;
		case RIGHT:
			_pos += this->right * velocity;
			break;
		}

		return _pos;
	}

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(GLfloat xOffset, GLfloat yOffset, GLboolean constrainPitch = true)
	{
		xOffset *= this->mouseSensitivity;
		yOffset *= this->mouseSensitivity;

		this->yaw += xOffset;
		this->pitch += yOffset;

		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch)
		{
			if (this->pitch > 89.0f)
			{
				this->pitch = 89.0f;
			}

			if (this->pitch < -89.0f)
			{
				this->pitch = -89.0f;
			}
		}

		// Update Front, Right and Up Vectors using the updated Eular angles
		this->updateCameraVectors();
	}

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(GLfloat yOffset)
	{

	}

	GLfloat GetZoom() {
		return this->zoom;
	}
	glm::vec3 GetPosition() {
		return this->position;
	}
	glm::vec3 GetFront() {
		return this->front;
	}
	glm::vec3 GetRight() {
		return this->right;
	}
	GLfloat GetMovementSpeed() {
		return this->movementSpeed;
	}

	void Update(ShaderLoader* shader, glm::mat4 projection, glm::mat4 view) {
		glUniformMatrix4fv(glGetUniformLocation(shader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shader->ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
	}
	void Update(ShaderLoader* shader, glm::mat4 projection) {
		glm::mat4 view = this->GetViewMatrix();
		glUniformMatrix4fv(glGetUniformLocation(shader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shader->ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
	}

	void CollisionCheck() {

	}

private:
	// Camera Attributes
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	// Eular Angles
	GLfloat yaw;
	GLfloat pitch;

	// Camera options
	GLfloat movementSpeed;
	GLfloat mouseSensitivity;
	GLfloat zoom;

	// Calculates the front vector from the Camera's (updated) Eular Angles
	void updateCameraVectors()
	{
		// Calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
		front.y = sin(glm::radians(this->pitch));
		front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
		this->front = glm::normalize(front);
		// Also re-calculate the Right and Up vector
		this->right = glm::normalize(glm::cross(this->front, this->worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		this->up = glm::normalize(glm::cross(this->right, this->front));
	}
};