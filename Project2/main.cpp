// Std. Includes
#include <string>
#include <map>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
///#include "Model.h"
#include "Collision.h"
#include "ToolBox.h"
#include "ComponentSystemManager.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"

// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
const char* TITLE = "Realm Of The Unknown - Beta Release";
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void LoadModels();

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024] = {false};
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

std::unordered_map<int, Model> ModelList;
std::unordered_map<int, int> ModelEntityList;

//ComponentSystem
ComponentSystemManager csm;
std::vector<Entity> entities;

#include "Components.h"
#include "Systems.h"

/*
Basic Clothing (When no Armor)
Change clothing on armor (Every armor piece looks different)
Gravity for Camera
Camera clipped to player model
Movement Component
- Keyboard Type ✓
- Auto Type 
*/

int main()
{
	#pragma region ComponentSystem
	csm.Init();

	/* Register The Components & Systems*/
	csm.RegisterComponent<RenderObjectC>();
	csm.RegisterComponent<CollisionC>();
	csm.RegisterComponent<EntityC>();
	csm.RegisterComponent<NonEntityC>();
	csm.RegisterComponent<BoundingBoxC>();
	csm.RegisterComponent<MovementC>();

	auto renderSystem = csm.RegisterSystem<RenderSystem>();
	{
		Signature signature;
		signature.set(csm.GetComponentType<RenderObjectC>());
		csm.SetSystemSignature<RenderSystem>(signature);
	}
	renderSystem->Init();
	
	auto movementSystem = csm.RegisterSystem<MovementSystem>();
	{
		Signature signature;
		signature.set(csm.GetComponentType<MovementC>());
		signature.set(csm.GetComponentType<RenderObjectC>());
		signature.set(csm.GetComponentType<CollisionC>());
		csm.SetSystemSignature<MovementSystem>(signature);
	}
	///mSystem = movementSystem;
	movementSystem->Init();

	auto nonEntityColSystem = csm.RegisterSystem<NonEntityCollisionSystem>();
	{
		Signature signature;
		signature.set(csm.GetComponentType<CollisionC>());
		signature.set(csm.GetComponentType<BoundingBoxC>());
		signature.set(csm.GetComponentType<NonEntityC>());
		csm.SetSystemSignature<NonEntityCollisionSystem>(signature);
	}
	nonEntityColSystem->Init();

	auto entityColSystem = csm.RegisterSystem<EntityCollisionSystem>();
	{
		Signature signature;
		signature.set(csm.GetComponentType<CollisionC>());
		signature.set(csm.GetComponentType<BoundingBoxC>());
		signature.set(csm.GetComponentType<EntityC>());
		csm.SetSystemSignature<EntityCollisionSystem>(signature);
	}
	entityColSystem->Init();

	auto boundingBoxSystem = csm.RegisterSystem<BoundingBoxSystem>();
	{
		Signature signature;
		signature.set(csm.GetComponentType<BoundingBoxC>());
		signature.set(csm.GetComponentType<RenderObjectC>());
		csm.SetSystemSignature<BoundingBoxSystem>(signature);
	}

	#pragma endregion

	#pragma region Window Init
	//Tools->Options->Debugging->Automatically (Last Line)
	//Use above path to toggle if console should auto close or not.

	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// OpenGL options
	glEnable(GL_DEPTH_TEST);
	#pragma endregion

	// Setup and compile our shaders
	Shader shader("res/shaders/modelLoading.vs", "res/shaders/modelLoading.frag");

	LoadModels();
	boundingBoxSystem->Init();
	glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 5.0f); //Render Distance

	int frame = 0;
	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Set frame time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check and call events
		glfwPollEvents();
		movementSystem->Update(deltaTime, keys, nonEntityColSystem->mEntities, camera);

		// Clear the colorbuffer
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Z-Buffer
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		shader.Use();

		//Camera Stuff to Shaders
		camera.Update(shader, projection); //Do projection calculation also inside Camera class?!

		//Just draw model nothin special with pos or scale
		boundingBoxSystem->Update(shader);
		renderSystem->Update(shader);
		entityColSystem->Update(nonEntityColSystem->mEntities);

		// Swap the buffers
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

#pragma region Model Creation
void LoadModels() {
	//Create Models

	int id = 1;

	//Model 1
	Model model0 = Model("res/models/Trees/Type1/Alien/PineTree3Snowy.obj", glm::vec3(4.7f, 0.f, 0.0f), 0.2f);
	ModelList[id] = model0;

	auto csmEntity0 = csm.CreateEntity();
	csm.AddComponent(csmEntity0, RenderObjectC{ model0 });
	csm.AddComponent(csmEntity0, NonEntityC{});
	csm.AddComponent(csmEntity0, BoundingBoxC{
		BoundingBox{csm.GetComponent<RenderObjectC>(csmEntity0).model.GetMaxVertice(),
		csm.GetComponent<RenderObjectC>(csmEntity0).model.GetMinVertice()},
		true });
	csm.AddComponent(csmEntity0, CollisionC{});
	ModelEntityList[id++] = csmEntity0;

	//Model 2
	Model model1 = Model("res/models/Trees/Type1/Evergreen/PineTree1.obj", glm::vec3(4.0f, 0.0f, 0.0f), 0.2f);
	ModelList[id] = model1;

	auto csmEntity1 = csm.CreateEntity();
	csm.AddComponent(csmEntity1, RenderObjectC{ model1 });
	csm.AddComponent(csmEntity1, NonEntityC{});
	csm.AddComponent(csmEntity1, BoundingBoxC{
		BoundingBox{csm.GetComponent<RenderObjectC>(csmEntity1).model.GetMaxVertice(),
		csm.GetComponent<RenderObjectC>(csmEntity1).model.GetMinVertice()},
		true });
	csm.AddComponent(csmEntity1, CollisionC{});
	ModelEntityList[id++] = csmEntity1;

	//Model 3
	Model model2 = Model("res/models/Trees/Type1/Pine/PineStump.obj", glm::vec3(0.3f, 0.f, 0.6f), 0.2f);
	ModelList[id] = model2;

	auto csmEntity2 = csm.CreateEntity();
	csm.AddComponent(csmEntity2, RenderObjectC{ model2 });
	csm.AddComponent(csmEntity2, NonEntityC{});
	csm.AddComponent(csmEntity2, CollisionC{});
	csm.AddComponent(csmEntity2, BoundingBoxC{
		BoundingBox{csm.GetComponent<RenderObjectC>(csmEntity2).model.GetMaxVertice(),
		csm.GetComponent<RenderObjectC>(csmEntity2).model.GetMinVertice()},
		true });
	ModelEntityList[id++] = csmEntity2;

	//Model 4
	Model model3 = Model("res/models/Trees/Type1/Evergreen/PineTree2.obj", glm::vec3(0.f, 0.f, 0.f), 0.2f);
	ModelList[id] = model3;

	auto csmEntity3 = csm.CreateEntity();
	csm.AddComponent(csmEntity3, RenderObjectC{ model3 });
	csm.AddComponent(csmEntity3, NonEntityC{});
	csm.AddComponent(csmEntity3, BoundingBoxC{
		BoundingBox{csm.GetComponent<RenderObjectC>(csmEntity3).model.GetMaxVertice(),
		csm.GetComponent<RenderObjectC>(csmEntity3).model.GetMinVertice()},
		true });
	csm.AddComponent(csmEntity3, CollisionC{});
	ModelEntityList[id++] = csmEntity3;

	//Player Entity
	Model model4 = Model("res/models/Entity/Player/Male.obj", glm::vec3(0.0f, 0.0f, 3.0f), 0.1f);
	auto csmEntity4 = csm.CreateEntity();
	csm.AddComponent(csmEntity4, RenderObjectC{ model4 });
	csm.AddComponent(csmEntity4, BoundingBoxC{ 
		BoundingBox{csm.GetComponent<RenderObjectC>(csmEntity4).model.GetMaxVertice(), 
		csm.GetComponent<RenderObjectC>(csmEntity4).model.GetMinVertice()}, 
		false });
	csm.AddComponent(csmEntity4, CollisionC{});
	csm.AddComponent(csmEntity4, EntityC{});
	csm.AddComponent(csmEntity4, MovementC{ MovementType::Keyboard });
	entities.push_back(csmEntity4);

}
#pragma endregion

void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}
}
void MouseCallback(GLFWwindow *window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}

/*

Object loading in new thread get percentage text on screen :?

-------------------------

Update vbo shit
http://wiki.lwjgl.org/wiki/The_Quad_updating_a_VBO_with_BufferSubData.html

-------------------------

Assimp Animatie (Is in java :?)
https://www.youtube.com/watch?v=fDmMH8_WRok

-------------------------

Get from server wich models need to be created and destroyed as entity :?

*/