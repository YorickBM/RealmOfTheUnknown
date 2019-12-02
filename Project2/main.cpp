// Std. Includes
#include <string>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Collision.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"

//LowPolyMesh Library
#include <LPML/Map.h>
#include <LPML/Mesh.h>

// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
const char* TITLE = "Realm Of The Unknown - Beta Release";
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main()
{
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

	#pragma region Water Mesh
	//Create Map
	const int mapsize = 128; //8
	const int start = 64;

	LowPolyMeshLibrary::Map heightMap(mapsize + 1, mapsize + 1);
	heightMap.setElement(0, 0, start);
	heightMap.setElement(0, mapsize, start);
	heightMap.setElement(mapsize, 0, start);
	heightMap.setElement(mapsize, mapsize, start);
	heightMap.generate(mapsize + 1, 1);
	heightMap.capData(0, 255);
	heightMap.smooth(1, 2);

	vector<GLfloat> heightdata;
	for (int y = 0; y != heightMap.getY(); y++)
	{
		for (int x = 0; x != heightMap.getX(); x++)
		{
			heightdata.push_back((GLfloat(heightMap.getElement(x, y))) / 255);
		}
	}

	//Create Mesh
	LowPolyMeshLibrary::Mesh mesh(mapsize, 2.0f);
	glm::mat4 MeshModel;
	MeshModel = glm::translate(MeshModel, glm::vec3(0, 0, 0));
	MeshModel = glm::scale(MeshModel, glm::vec3(1, 1, 1));

	mesh.genTriangleMesh(heightdata);
	mesh.genTriangleNormals();
	mesh.genShader();
	#pragma endregion


	// Setup and compile our shaders
	Shader shader("res/shaders/modelLoading.vs", "res/shaders/modelLoading.frag");

	// Load models
	///Model ourModel("res/models/suit/nanosuit.obj");
	Model PineTree3Snowy("res/models/OBJ/Alien/PineTree3Snowy.obj");

	Model PineTree1("res/models/OBJ/Evergreen/PineTree1.obj", glm::vec3(4.0f, 0.f, 0.0f), 0.2f);
	Model PineStump("res/models/OBJ/Pine/PineStump.obj", glm::vec3(0.0f, 0.f, 0.0f), 0.2f);

	// Draw in wireframe
	///glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

	PineTree3Snowy.setPosition(glm::vec3(4.2f, 0.f, 0.0f));
	PineTree3Snowy.setScale(0.2f);

	//Init Mesh stuff
	mesh.initVertArray();

	std::vector<glm::vec3> x;
	std::vector<glm::vec3> y;

	x.push_back(glm::vec3(200, 300, 20));
	x.push_back(glm::vec3(100, 100, 20));

	y.push_back(glm::vec3(200, 180, 10));
	y.push_back(glm::vec3(320, 180, 10));

	Collision* col = new Collision();
	col->detectCollision(x, y);

	PineStump.DetectCollision(PineTree3Snowy);
	PineTree1.DetectCollision(PineTree3Snowy);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Set frame time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check and call events
		glfwPollEvents();
		DoMovement();

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
		PineTree3Snowy.Draw(shader);
		PineStump.Draw(shader);
		PineTree1.Draw(shader);

		//Water Mesh Rendering Stuff
		mesh.disbaleShader();
		mesh.enableShader();
		mesh.setShaderUniformMatrix4((GLchar*)"model", MeshModel);
		mesh.draw();

		// Swap the buffers
		glfwSwapBuffers(window);
	}

	mesh.disableVertArrays();
	glfwTerminate();
	return 0;
}

#pragma region SystemMovement
// Moves/alters the camera positions based on user input
bool MeshMode = false;
int delay = 0;
void DoMovement()
{
	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);
	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}

	#pragma region Polygon Mode Toggle
	if (keys[GLFW_KEY_TAB]) {
		if (!MeshMode && delay == 0) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			MeshMode = true; delay = 10;
			return;
		}else if(delay == 0){
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			MeshMode = false; delay = 10;
			return;
		}
	}
	if (delay > 0) delay--;
	#pragma endregion
}
// Is called whenever a key is pressed/released via GLFW
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
#pragma endregion

/*

Object loading in new thread get percentage text on screen :?

-------------------------

http://www.peroxide.dk/papers/collision/collision.pdf
http://www.codercorner.com/SAP.pdf

-------------------------

Collision Shit :? > https://www.youtube.com/watch?v=NGh-Vh_NYO0
@Teddy C You need 8 vertices to make a cube, and these can be created from the max and min x, y and z values. For example, the front top left vertex of the box is (minX, maxY, minZ). The back right bottom vertex is (maxX, minY, maxZ) etc. The calculations with AABBs often don't require these vertices though, and are instead done with the 6 max/min values.
For example, to check if 2 AABBs intersect:
if(box1.minX < box2.maxX && box1.maxX > box2.minX){
		 //boxes intersect on x axis
}
Do this for y and z as well, and if the boxes intersect on all 3 axis then the two AABBs are intersecting.

-------------------------

How would i go about getting the sizeX, sizeY, and a sizeZ from an Entity in the openGL 3D game tutorial for a broad collision detection?
@G4_Genocide When reading in the vertices in your OBJ loader keep track of the largest x, y and z values. So for each vertex that you read in you would do something like:
if(vertex.x > maxX){
	  maxX = vertex.x;
}
if(vertex.y > maY){
	 maxY = vertex.y;
}
if(vertex.z > maxZ){
	 maxZ = vertex.z;
}
Do the same for min values as well. When you've finished loading all the vertices you'll have the ma and min x,y,z values for that model.

-------------------------

Update vbo shit
http://wiki.lwjgl.org/wiki/The_Quad_updating_a_VBO_with_BufferSubData.html

-------------------------

Triangle face collision

-------------------------

Assimp Animatie (Is in java :?)
https://www.youtube.com/watch?v=fDmMH8_WRok

*/