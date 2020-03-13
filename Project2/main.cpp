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

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_GLFW_GL3_IMPLEMENTATION
#define NK_KEYSTATE_BASED_INPUT

#include "Nuklear.h"
#include "nuklear_glfw_gl3.h"

#define WINDOW_WIDTH 800 
#define WINDOW_HEIGHT 600

#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024


// Properties
const GLuint WIDTH = WINDOW_WIDTH, HEIGHT = WINDOW_HEIGHT;
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
	if (!glfwInit()) {
		fprintf(stdout, "[GFLW] failed to init!\n");
		exit(1);
	}

	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	///glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

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
#pragma region Nuklear
	struct nk_context* ctx;
	struct nk_colorf bg;
	ctx = nk_glfw3_init(window, NK_GLFW3_INSTALL_CALLBACKS);
	/* Load Fonts: if none of these are loaded a default font will be used  */
	/* Load Cursor: if you uncomment cursor loading please hide the cursor */
	{struct nk_font_atlas* atlas;
	nk_glfw3_font_stash_begin(&atlas);
	/*struct nk_font *droid = nk_font_atlas_add_from_file(atlas, "../../../extra_font/DroidSans.ttf", 14, 0);*/
	/*struct nk_font *roboto = nk_font_atlas_add_from_file(atlas, "../../../extra_font/Roboto-Regular.ttf", 14, 0);*/
	/*struct nk_font *future = nk_font_atlas_add_from_file(atlas, "../../../extra_font/kenvector_future_thin.ttf", 13, 0);*/
	/*struct nk_font *clean = nk_font_atlas_add_from_file(atlas, "../../../extra_font/ProggyClean.ttf", 12, 0);*/
	/*struct nk_font *tiny = nk_font_atlas_add_from_file(atlas, "../../../extra_font/ProggyTiny.ttf", 10, 0);*/
	/*struct nk_font *cousine = nk_font_atlas_add_from_file(atlas, "../../../extra_font/Cousine-Regular.ttf", 13, 0);*/
	nk_glfw3_font_stash_end();
	/*nk_style_load_all_cursors(ctx, atlas->cursors);*/
	/*nk_style_set_font(ctx, &droid->handle);*/}

#ifdef INCLUDE_STYLE
	/*set_style(ctx, THEME_WHITE);*/
	/*set_style(ctx, THEME_RED);*/
	/*set_style(ctx, THEME_BLUE);*/
	/*set_style(ctx, THEME_DARK);*/
#endif
	bg.r = 0.10f, bg.g = 0.18f, bg.b = 0.24f, bg.a = 1.0f;
#pragma endregion

	//https://cpp.libhunt.com/imgui-alternatives
	//https://sourceforge.net/projects/ctocpp/
	//https://github.com/Tropby/NuklearCPP

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

		/* GUI */
		if (nk_begin(ctx, "Demo", nk_rect(50, 50, 230, 250),
			NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
			NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE))
		{
			enum { EASY, HARD };
			static int op = EASY;
			static int property = 20;
			nk_layout_row_static(ctx, 30, 80, 1);
			if (nk_button_label(ctx, "button"))
				fprintf(stdout, "button pressed\n");

			nk_layout_row_dynamic(ctx, 30, 2);
			if (nk_option_label(ctx, "easy", op == EASY)) op = EASY;
			if (nk_option_label(ctx, "hard", op == HARD)) op = HARD;

			nk_layout_row_dynamic(ctx, 25, 1);
			nk_property_int(ctx, "Compression:", 0, &property, 100, 10, 1);

			nk_layout_row_dynamic(ctx, 20, 1);
			nk_label(ctx, "background:", NK_TEXT_LEFT);
			nk_layout_row_dynamic(ctx, 25, 1);
			if (nk_combo_begin_color(ctx, nk_rgb_cf(bg), nk_vec2(nk_widget_width(ctx), 400))) {
				nk_layout_row_dynamic(ctx, 120, 1);
				bg = nk_color_picker(ctx, bg, NK_RGBA);
				nk_layout_row_dynamic(ctx, 25, 1);
				bg.r = nk_propertyf(ctx, "#R:", 0, bg.r, 1.0f, 0.01f, 0.005f);
				bg.g = nk_propertyf(ctx, "#G:", 0, bg.g, 1.0f, 0.01f, 0.005f);
				bg.b = nk_propertyf(ctx, "#B:", 0, bg.b, 1.0f, 0.01f, 0.005f);
				bg.a = nk_propertyf(ctx, "#A:", 0, bg.a, 1.0f, 0.01f, 0.005f);
				nk_combo_end(ctx);
			}
		}
		nk_end(ctx);
		nk_glfw3_render(NK_ANTI_ALIASING_ON, 512 * 1024, 128 * 1024);

		//Game Objects
		inputSystem->Update(keys);
		movementSystem->Update(deltaTime, camera);
		collisionSystem->CollisionCheck();

		// Clear the colorbuffer
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Z-Buffer
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

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
		// Swap the buffers
		glfwSwapBuffers(window);
	}

	//ImGui_ImplGlfwGL3_Shutdown();
	//ImGui::DestroyContext();
	nk_glfw3_shutdown();
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