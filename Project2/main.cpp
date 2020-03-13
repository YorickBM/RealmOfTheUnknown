// Std. Includes
#include <string>
#include <map>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Camera.h"
#include "AnimModel.h"
#include "Collision.h"
#include "ToolBox.h"
#include "ComponentSystemManager.h"
#include "ChunkManager.h"
#include "ModelLoader.h";

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/OpenGL/GL3Renderer.h>

// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
const char* TITLE = "Fighting Against The Coruption - (0.0.1)";
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);

// Camera
Camera camera(glm::vec3(-2, 2, 0));
bool keys[1024] = { false };
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

//ComponentSystem
ComponentSystemManager csm;
ChunkManager cm;

#include "Components.h"
#include "Systems.h"

//imGui 1.60
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"
#include "GuiManager.h"

int main()
{
#pragma region ComponentSystem
	csm.Init();

	/* Register The Components & Systems*/
	csm.RegisterComponent<TransformC>();
	csm.RegisterComponent<MotionC>();
	csm.RegisterComponent<ModelMeshC>();
	csm.RegisterComponent<CollisionC>();
	csm.RegisterComponent<HealthC>();
	csm.RegisterComponent<AiC>();
	csm.RegisterComponent<InputC>();

	auto inputSystem = csm.RegisterSystem<InputSystem>();
	{
		Signature signature;
		signature.set(csm.GetComponentType<MotionC>());
		signature.set(csm.GetComponentType<InputC>());
		csm.SetSystemSignature<InputSystem>(signature);
	}
	inputSystem->Init();

	auto movementSystem = csm.RegisterSystem<MovementSystem>();
	{
		Signature signature;
		signature.set(csm.GetComponentType<MotionC>());
		signature.set(csm.GetComponentType<TransformC>());
		csm.SetSystemSignature<MovementSystem>(signature);
	}
	movementSystem->Init();

	auto modelSystem = csm.RegisterSystem<ModelMeshSystem>();
	{
		Signature signature;
		signature.set(csm.GetComponentType<ModelMeshC>());
		signature.set(csm.GetComponentType<TransformC>());
		csm.SetSystemSignature<ModelMeshSystem>(signature);
	}
	modelSystem->Init();

	auto collisionSystem = csm.RegisterSystem<CollisionSystem>(); //Model Postion Transformation in here ???
	{
		Signature signature;
		signature.set(csm.GetComponentType<CollisionC>());
		signature.set(csm.GetComponentType<ModelMeshC>());
		signature.set(csm.GetComponentType<TransformC>());
		csm.SetSystemSignature<CollisionSystem>(signature);
	}
	collisionSystem->Init();

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
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return -1;
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
		return -1;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// OpenGL options
	glEnable(GL_DEPTH_TEST);
#pragma endregion
#pragma region ImGui
	// Setup ImGui binding
	//ImGui::CreateContext();
	//ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	//ImGui_ImplGlfwGL3_Init(window, true);

	// Setup style
	//ImGui::StyleColorsDark();
#pragma endregion

	// Setup and compile our shaders
	ShaderLoader* shaderLoader = new ShaderLoader();
	shaderLoader->loadShaders("vertexShader.glsl", "fragmentShader.glsl");

	cm.InitChunks("res/Chunks/ChunkData.txt", "", 0.2f);

	auto currEntity0 = csm.CreateEntity();
	csm.AddComponent(currEntity0, TransformC{ vec3(0), 0.2f });
	AnimModel camModel("tree.dae", glm::vec3(0, 0.3f, 0), 0.2f);
	csm.AddComponent(currEntity0, ModelMeshC{ camModel, camModel.GetBoundingBoxModel() });

	auto currEntity = csm.CreateEntity();
	csm.AddComponent(currEntity, MotionC{});
	csm.AddComponent(currEntity, TransformC{ vec3(0), 0.2f });
	csm.AddComponent(currEntity, InputC{ Keyboard });
	csm.AddComponent(currEntity, ModelMeshC{ camModel, camModel.GetBoundingBoxModel() });
	csm.AddComponent(currEntity, CollisionC{ SolidCollision, false });

	AnimModel model0("tree.fbx", glm::vec3(0, 0, 0), 0.2f);
	auto newEntity = csm.CreateEntity();
	csm.AddComponent(newEntity, TransformC{ vec3(0), 0.2f });

	csm.AddComponent(newEntity, ModelMeshC{ model0, model0.GetBoundingBoxModel() });
	csm.AddComponent(newEntity, CollisionC{ SolidCollision, true });
	//model0.playAnimation(new Animation("Armature", vec2(0, 55), 0.2, 10, true), false); //forcing our model to play the animation (name, frames, speed, priority, loop)

	///csm.InitEntities("res/System/Entities.txt");
	///cm.InitializeChunks("res/System/ChunkMap.png");

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 5.0f); //Render Distance

	std::cout << cm.GetChunks().size() << std::endl;
	collisionSystem->Update();

	int frame = 0;
	bool debuggerWindow = false;
	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Set frame time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check and call events
		glfwPollEvents();

		inputSystem->Update(keys);
		movementSystem->Update(deltaTime, camera);
		collisionSystem->CollisionCheck();

		// Clear the colorbuffer
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Z-Buffer
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		/* UI Stuff here */

		//Animated Model
		shaderLoader->use();
		//Camera Stuff to Shaders
		camera.Update(shaderLoader, projection); //Do projection calculation also inside Camera class?!

		glUniformMatrix4fv(glGetUniformLocation(shaderLoader->ID, "view"), 1, GL_FALSE, value_ptr(camera.GetViewMatrix())); //send the view matrix to the shader
		glUniformMatrix4fv(glGetUniformLocation(shaderLoader->ID, "projection"), 1, GL_FALSE, value_ptr(projection)); //send the projection matrix to the shader

		modelSystem->Update(shaderLoader);

		//Draw all Chunks
		for (Chunk chunk : cm.GetChunks()) {
			mat4 objectModel;
			glUniformMatrix4fv(glGetUniformLocation(shaderLoader->ID, "model"), 1, GL_FALSE, value_ptr(objectModel)); //send the empty model matrix to the shader
			chunk.model.Draw(shaderLoader);
		}

		shaderLoader->unuse();

		/*ImGui_ImplGlfwGL3_NewFrame();
		//Create GUI's
		{
			ImGui::Begin("Debugger", &debuggerWindow);
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		//Render GUI's
		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());*/

		// Swap the buffers
		glfwSwapBuffers(window);
	}

	//ImGui_ImplGlfwGL3_Shutdown();
	//ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
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
void MouseCallback(GLFWwindow* window, double xPos, double yPos)
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