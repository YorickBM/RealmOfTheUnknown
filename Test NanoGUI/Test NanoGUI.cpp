#pragma region Includes
// STD Includes
#include <string>
#include <map>
#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Other Libs
#include "SOIL2/SOIL2.h"

//Nano Gui
#pragma region NanoGUI Settings (GLAD & Appel & Prototypes)
#if defined(NANOGUI_GLAD)
#if defined(NANOGUI_SHARED) && !defined(GLAD_GLAPI_EXPORT)
#define GLAD_GLAPI_EXPORT
#endif

#include <glad/glad.h>
#else
#if defined(__APPLE__)
#define GLFW_INCLUDE_GLCOREARB
#else
#define GL_GLEXT_PROTOTYPES
#endif
#endif
#pragma endregion
#include <nanogui/nanogui.h>

//Personal Includes
#include "Components.h"
#include "Systems.h"
#include "Camera.h"
#include "AnimModel.h"
#include "Collision.h"
#include "ToolBox.h"
#include "ComponentSystemManager.h"
#include "ChunkManager.h"
#include "ModelLoader.h";
#pragma endregion
#pragma region Vars
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
int frame = 0;

//ComponentSystem
ComponentSystemManager csm;
ChunkManager cm;

//Nano GUi
enum test_enum {
    Item1 = 0,
    Item2,
    Item3
};

bool bvar = true;
int ivar = 12345678;
double dvar = 3.1415926;
float fvar = (float)dvar;
std::string strval = "A string";
test_enum enumval = Item2;
Color colval(0.5f, 0.5f, 0.7f, 1.f);

Screen* screen = nullptr;

//OpenGL
bool CLOSEWINDOW = false;
#pragma endregion

using namespace nanogui;
int main(int /* argc */, char** /* argv */) {
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

    #pragma region Initialize glfw
    glfwInit();
    glfwSetTime(0);
    #pragma endregion
    #pragma region GlfwWindow Properties
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_SAMPLES, 0);
    glfwWindowHint(GLFW_RED_BITS, 8);
    glfwWindowHint(GLFW_GREEN_BITS, 8);
    glfwWindowHint(GLFW_BLUE_BITS, 8);
    glfwWindowHint(GLFW_ALPHA_BITS, 8);
    glfwWindowHint(GLFW_STENCIL_BITS, 8);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    #pragma endregion
    #pragma region GlfwWindow Creation
    // Create a GLFWwindow object
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    #pragma endregion
    #pragma region NanoGui Glad
    #if defined(NANOGUI_GLAD)
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        throw std::runtime_error("Could not initialize GLAD!");
    glGetError(); // pull and ignore unhandled errors like GL_INVALID_ENUM
    #endif
    #pragma endregion
    #pragma region OpenGl Stuff & NanoGUi Init Stuff
    glClearColor(0.2f, 0.25f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Create a nanogui screen and pass the glfw pointer to initialize
    screen = new Screen();
    screen->initialize(window, true);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glfwSwapInterval(0);
    glfwSwapBuffers(window);
    #pragma endregion  
    #pragma region GLEW
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }
    #pragma endregion
    #pragma region FrameBuffer
    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    #pragma endregion
    #pragma region OpenGL Options
    // Define the viewport dimensions
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    // OpenGL options
    glEnable(GL_DEPTH_TEST);
    #pragma endregion
    #pragma region NanoGui GUI
    // Create nanogui gui
    bool enabled = true;
    FormHelper* gui = new FormHelper(screen);
    Window* nanoguiWindow = gui->addWindow(Eigen::Vector2i(10, 10), "Form helper example");
    gui->addGroup("Basic types");
    gui->addVariable("bool", bvar)->setTooltip("Test tooltip.");
    gui->addVariable("string", strval);

    gui->addGroup("Validating fields");
    gui->addVariable("int", ivar)->setSpinnable(true);
    gui->addVariable("float", fvar)->setTooltip("Test.");
    gui->addVariable("double", dvar)->setSpinnable(true);

    gui->addGroup("Complex types");
    gui->addVariable("Enumeration", enumval, enabled)->setItems({ "Item 1", "Item 2", "Item 3" });
    gui->addVariable("Color", colval)
        ->setFinalCallback([](const Color& c) {
        std::cout << "ColorPicker Final Callback: ["
            << c.r() << ", "
            << c.g() << ", "
            << c.b() << ", "
            << c.w() << "]" << std::endl;
            });

    gui->addGroup("Other widgets");
    gui->addButton("A button", []() { std::cout << "Button pressed." << std::endl; })->setTooltip("Testing a much longer tooltip, that will wrap around to new lines multiple times.");;

    screen->setVisible(true);
    screen->performLayout();
    //nanoguiWindow->center();
    #pragma endregion
    #pragma region glfw Callbacks to NanoGUI & ECS
    glfwSetCursorPosCallback(window,
        [](GLFWwindow*, double x, double y) {
            screen->cursorPosCallbackEvent(x, y);

            if (firstMouse)
            {
                lastX = x;
                lastY = y;
                firstMouse = false;
            }

            GLfloat xOffset = x - lastX;
            GLfloat yOffset = lastY - y;  // Reversed since y-coordinates go from bottom to left

            lastX = x;
            lastY = y;

            camera.ProcessMouseMovement(xOffset, yOffset);
        }
    );

    glfwSetMouseButtonCallback(window,
        [](GLFWwindow*, int button, int action, int modifiers) {
            screen->mouseButtonCallbackEvent(button, action, modifiers);
        }
    );

    glfwSetKeyCallback(window,
        [](GLFWwindow*, int key, int scancode, int action, int mods) {
            screen->keyCallbackEvent(key, scancode, action, mods);

            if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
            {
                CLOSEWINDOW = true;
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
    );

    glfwSetCharCallback(window,
        [](GLFWwindow*, unsigned int codepoint) {
            screen->charCallbackEvent(codepoint);
        }
    );

    glfwSetDropCallback(window,
        [](GLFWwindow*, int count, const char** filenames) {
            screen->dropCallbackEvent(count, filenames);
        }
    );

    glfwSetScrollCallback(window,
        [](GLFWwindow*, double x, double y) {
            screen->scrollCallbackEvent(x, y);
        }
    );

    glfwSetFramebufferSizeCallback(window,
        [](GLFWwindow*, int width, int height) {
            screen->resizeCallbackEvent(width, height);
        }
    );
    #pragma endregion

    #pragma region Shaders
    // Setup and compile our shaders
    ShaderLoader* shaderLoader = new ShaderLoader();
    shaderLoader->loadShaders("vertexShader.glsl", "fragmentShader.glsl");
    #pragma endregion
    #pragma region Entity Creation & Chunk Loading
    cm.InitChunks("res/Chunks/ChunkData.txt", "", 0.2f);
    ///csm.InitEntities("res/System/Entities.txt");

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
    #pragma endregion
    //model0.playAnimation(new Animation("Armature", vec2(0, 55), 0.2, 10, true), false); //forcing our model to play the animation (name, frames, speed, priority, loop)

    #pragma region Pre Game Loop
    glm::mat4 projection = glm::perspective(camera.GetZoom(), static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT), 0.1f, 5.0f); //Render Distance
    collisionSystem->Update();
    #pragma endregion
    // Game loop
    while (!glfwWindowShouldClose(window)) {
        #pragma region Frame & Poll Events & Clear Buffers/Color
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();

        //Clear Buffers & Color
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        #pragma endregion
        #pragma region GUI

        /* GUI */
        screen->drawContents();
        screen->drawWidgets();

        #pragma endregion
        #pragma region Game Objects
        //Game Objects
        inputSystem->Update(keys);
        movementSystem->Update(deltaTime, camera);
        collisionSystem->CollisionCheck();

        #pragma endregion
        #pragma region Draw Models
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
        #pragma endregion

        if(CLOSEWINDOW) glfwSetWindowShouldClose(window, GL_TRUE);
    }

    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}
