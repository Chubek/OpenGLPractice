#pragma once

#include <vector>

#include <glad/glad.h>
#include <KHR/khrplatform.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

const GLfloat YAW = -90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 6.0f;
const GLfloat SENSITIVITY = 0.25f;
const GLfloat ZOOM = 45.0f;

class Camera
{
public:
	
	Camera() = default;

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW,
		GLfloat pitch = PITCH) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM)
	{
		this->position = position;
		this->worldUp = up;
		this->yaw = yaw;
		this->pitch = pitch;
		this->updateCameraVectors();

	}


	Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM)
	{
		this->position = glm::vec3(posX, posY, posZ);
		this->worldUp = glm::vec3(upX, upY, upZ);
		this->yaw = yaw;
		this->pitch = pitch;
		this->updateCameraVectors();
	}

	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(this->position, this->position + this->front, this->up);
	}
	
	void ProcessKeyboard(Camera_Movement direction, GLfloat deltatime)
	{
		GLfloat velocity = this->movementSpeed * deltatime;

		if (direction == FORWARD)
		{
			this->position += this->front * velocity;
		}
		if (direction == BACKWARD)
		{
			this->position -= this->front * velocity;
		}
		if (direction == LEFT)
		{
			this->position += this->right * velocity;
		}
		if (direction == RIGHT)
		{
			this->position -= this->right * velocity;
		}
	}
	void ProcessMouseMovement(GLfloat xOffset, GLfloat yOffset, GLboolean contrainPitch = true)
	{
		xOffset *= this->mouseSensitivity;
		yOffset *= this->mouseSensitivity;

		this->yaw -= xOffset;
		this->pitch += yOffset;

		if (contrainPitch)
		{
			if (this->pitch > 89)
			{
				this->pitch = 89;
			}
			if (this->pitch < -89)
			{
				this->pitch = 89;
			}
		}

		this->updateCameraVectors();
	}

	void ProcessMouseScroll(GLfloat yOffset)
	{
		if (this->zoom >= 1.0f && this->zoom <= 45.0f)
		{
			this->zoom -= yOffset;
		}

		if (this->zoom <= 1.0f)
		{
			this->zoom = 1.0f;
		}
		
		if (this ->zoom >= 45.0f)
		{
			this->zoom = 45.0f;
		}
	}
	GLfloat GetZoom()
	{
		return this->zoom;
	}

private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	GLfloat yaw;
	GLfloat pitch;

	GLfloat movementSpeed;
	GLfloat mouseSensitivity;
	GLfloat zoom;

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