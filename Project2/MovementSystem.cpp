#include "MovementSystem.h"

extern ComponentSystemManager csm;

void MovementSystem::Init()
{
}

glm::vec3 MovementSystem::ProcessMovement(Camera_Movement direction, GLfloat deltaTime, glm::vec3 Front, glm::vec3 Right, GLfloat MovementSpeed) {
	GLfloat velocity = MovementSpeed * deltaTime;
	glm::vec3 _position(0, 0, 0);

	switch (direction) {
	case FORWARD:
		_position += Front * velocity;
		break;
	case BACKWARD:
		_position -= Front * velocity;
		break;
	case LEFT:
		_position -= Right * velocity;
		break;
	case RIGHT:
		_position += Right * velocity;
		break;
	}

	return _position;
}

int SceneDelay = 20;
bool ShouldRender = false;
void MovementSystem::Update(GLfloat deltaTime, bool keys[1024], std::set<Entity> SceneEntities, Camera& camera)
{
	if (delay > 0) delay--;
	if (SceneDelay > 0) SceneDelay--;
	for (auto const& entity : SceneEntities)
	{
		//Hitbox
		csm.GetComponent<BoundingBoxC>(entity).render = ShouldRender;
	}

	for (auto const& entity : mEntities)
	{
		auto& MovementComponent = csm.GetComponent<MovementC>(entity);
		auto& CollisionComponent = csm.GetComponent<CollisionC>(entity);
		//glm::vec3 ModelPosition = csm.GetComponent<RenderObjectC>(entity).model.getPosition();

		MovementComponent.Front = camera.GetFront();
		MovementComponent.Right = camera.GetRight();
		MovementComponent.MovementSpeed = camera.GetMovementSpeed();

		switch (MovementComponent.movementType) {
		case MovementType::Keyboard:
			if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
			{
				//ModelPosition += (ProcessMovement(FORWARD, deltaTime, MovementComponent.Front, MovementComponent.Right, MovementComponent.MovementSpeed));
				///camera.InteralProcessKeyboard(FORWARD, deltaTime);
			}
			if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
			{
				//ModelPosition += (ProcessMovement(BACKWARD, deltaTime, MovementComponent.Front, MovementComponent.Right, MovementComponent.MovementSpeed));
				///camera.ProcessKeyboard(BACKWARD, deltaTime);
			}
			if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
			{
				//ModelPosition += (ProcessMovement(LEFT, deltaTime, MovementComponent.Front, MovementComponent.Right, MovementComponent.MovementSpeed));
				///camera.ProcessKeyboard(LEFT, deltaTime);
			}
			if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
			{
				//ModelPosition += (ProcessMovement(RIGHT, deltaTime, MovementComponent.Front, MovementComponent.Right, MovementComponent.MovementSpeed));
				///camera.ProcessKeyboard(RIGHT, deltaTime);
			}
			//CollisionComponent.NextPosition = ModelPosition;
			break;

		case MovementType::Auto:
			break;

		default:
			break;
		}

		//Hitbox
		csm.GetComponent<BoundingBoxC>(entity).render = ShouldRender;
	}

#pragma region Polygon Mode Toggle
	if (keys[GLFW_KEY_TAB]) {
		if (!MeshMode && delay == 0) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			MeshMode = true; delay = 10;
		}
		else if (delay == 0) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			MeshMode = false; delay = 10;
		}
	}
#pragma endregion
#pragma region Hitbox Mode Toggle 
	if (keys[GLFW_KEY_F3] && keys[GLFW_KEY_B]) {

		if (!ShouldRender && SceneDelay == 0) {
			ShouldRender = true; SceneDelay = 20;
		}
		else if (SceneDelay == 0) {
			ShouldRender = false; SceneDelay = 20;
		}

	}
#pragma endregion
}
