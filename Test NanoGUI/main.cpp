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
#include "FileLoader.h"

//Sleep Stuff
#include <thread>
#include <chrono>

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

#include "InventoryTheme.h"
#include "Inventory.h"
#include "ScreenManager.h"
#pragma endregion
using namespace nanogui;
#pragma region Vars
// Properties
const GLuint WIDTH = 1280, HEIGHT = 800; //1280x1024 <- lower then this = customScale
const char* TITLE = "Fighting Against The Coruption - (0.0.1)";
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera camera(glm::vec3(-37, 7, 109));
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
enum res_enum {
    resolution_1 = 0,
    resolution_2,
    resolution_3,
    resolution_4,
    resolution_5,
    resolution_6,
    resolution_7,
    resolution_8,
    resolution_9,
    resolution_10,
    resolution_11,
    resolution_12,
    resolution_13,
    resolution_14,
    resolution_15,
    resolution_16,
    resolution_17,
    resolution_18,
    resolution_19
};

bool bvar = true;
int ivar = 12345678;
double dvar = 3.1415926;
float fvar = (float)dvar;
std::string strval = "A string";
res_enum enumval = resolution_2;
Color colval(0.5f, 0.5f, 0.7f, 1.f);
Screen* screen = nullptr;
Screen* startScreenScreen = nullptr;
Screen* startScreenImgScreen = nullptr;
Screen* loadingScreenScreen = nullptr;
Screen* settingsScreenScreen = nullptr;
Screen* settingsScreenImgScreen = nullptr;

Inventory* inv;
StartScreen* startScreen;
LoadingScreen* loadingScreen;

#pragma endregion

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
    #pragma region Resolutions
    std::vector<std::string> resolutions;
    resolutions.push_back("640x360");
    resolutions.push_back("800x600");
    resolutions.push_back("1024x768");
    resolutions.push_back("1280x720");
    resolutions.push_back("1280x800");
    resolutions.push_back("1280x1024");
    resolutions.push_back("1360x768");
    resolutions.push_back("1366x768");
    resolutions.push_back("1440x900");
    resolutions.push_back("1536x864");
    resolutions.push_back("1600x600");
    resolutions.push_back("1680x1050");
    resolutions.push_back("1920x1080");
    resolutions.push_back("1920x1200");
    resolutions.push_back("2048x1152");
    resolutions.push_back("2560x1080");
    resolutions.push_back("2560x1440");
    resolutions.push_back("3440x1440");
    resolutions.push_back("3840x2160");
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
    startScreenScreen = new Screen();
    startScreenScreen->initialize(window, true);
    startScreenImgScreen = new Screen();
    startScreenImgScreen->initialize(window, true);
    loadingScreenScreen = new Screen();
    loadingScreenScreen->initialize(window, true);

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
    ///glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    #pragma endregion
    #pragma region OpenGL Options
    // Define the viewport dimensions
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    // OpenGL options
    glEnable(GL_DEPTH_TEST);
    #pragma endregion
    #pragma region NanoGui GUI
    //Create Inventory
    inv = new Inventory(screen, SCREEN_WIDTH, SCREEN_HEIGHT);
    inv->ShowInfo();

    startScreen = new StartScreen(startScreenScreen, SCREEN_WIDTH, SCREEN_HEIGHT, window, startScreenImgScreen);
    loadingScreen = new LoadingScreen(loadingScreenScreen, SCREEN_WIDTH, SCREEN_HEIGHT);
    #pragma endregion
    #pragma region glfw Callbacks to NanoGUI & ECS
    glfwSetCursorPosCallback(window,
        [](GLFWwindow*, double x, double y) {
            inv->getScreen()->cursorPosCallbackEvent(x, y);
            inv->realignWindows(SCREEN_WIDTH, SCREEN_HEIGHT); //Prevent the movement this way
            startScreen->getScreen()->cursorPosCallbackEvent(x, y);

            if (firstMouse)
            {
                lastX = x;
                lastY = y;
                firstMouse = false;
            }

            double xOffset = x - lastX;
            double yOffset = lastY - y;  // Reversed since y-coordinates go from bottom to left

            lastX = x;
            lastY = y;

            camera.ProcessMouseMovement(xOffset, yOffset);
        }
    );

    glfwSetMouseButtonCallback(window,
        [](GLFWwindow*, int button, int action, int modifiers) {
            screen->mouseButtonCallbackEvent(button, action, modifiers);
            startScreen->getScreen()->mouseButtonCallbackEvent(button, action, modifiers);
        }
    );

    glfwSetKeyCallback(window,[](GLFWwindow* window, int key, int scancode, int action, int mods) {
            screen->keyCallbackEvent(key, scancode, action, mods);
            inv->keyCallbackEvent(key, scancode, action, mods);
            startScreen->getScreen()->keyCallbackEvent(key, scancode, action, mods);

            ///if(key == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(window, GL_TRUE);

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
            //screen->scrollCallbackEvent(x, y);
        }
    );

    glfwSetFramebufferSizeCallback(window,
        [](GLFWwindow*, int width, int height) {
           screen->resizeCallbackEvent(width, height);
           inv->realignWindows(width, height);
           startScreen->realignWindows(width, height);

           loadingScreen->realignWindows(width, height);
           loadingScreen->render();
        }
    );
    #pragma endregion

    while (startScreen->IsActive() && !glfwWindowShouldClose(window)) {
        #pragma region Frame & Poll Events & Clear Buffers/Color
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();

        //Clear Buffers & Color
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        #pragma endregion
        #pragma region Game Objects
        //Game Objects
        inputSystem->Update(keys);
        movementSystem->Update(deltaTime, camera);
        collisionSystem->CollisionCheck();

    #pragma endregion

        startScreen->render();

        // Swap the buffers
        glfwSwapBuffers(window);
    }

    if (!glfwWindowShouldClose(window)) {
        loadingScreen->specialRender(window, "Loading Shaders", width, height);

        #pragma region Shaders
        // Setup and compile our shaders
        ShaderLoader* shaderLoader = new ShaderLoader();
        shaderLoader->loadShaders("vertexShader.glsl", "fragmentShader.glsl");
        #pragma endregion

        ///std::this_thread::sleep_for(std::chrono::milliseconds(3000));
        loadingScreen->specialRender(window, "Loading Model Data", width, height);

        #pragma region Entity Creation & Chunk Loading
        cm.InitChunks("res/Chunks/ChunkData.txt", "", 0.2f);
        ///csm.InitEntities("res/System/Entities.txt");

        auto Te = csm.CreateEntity();
        csm.AddComponent(Te, MotionC{});
        csm.AddComponent(Te, InputC{ Keyboard });
        csm.AddComponent(Te, TransformC{ vec3(0), 1.f });
        AnimModel model1("resources/tree.fbx", vec3(0), 1.f);
        csm.AddComponent(Te, ModelMeshC{ model1, model1.GetBoundingBoxModel() });

        //Load Modeldata File & Create Entity's
        std::vector<ModelDataClass*> modelData = FileLoader::ReadModelData("ModelData.data");
        for (ModelDataClass* data : modelData) {
            AnimModel model(data->path, vec3(data->x, data->y, data->z), data->scale, vec3(data->rx, data->ry, data->rz));


            auto Entity = csm.CreateEntity();
            csm.AddComponent(Entity, TransformC{ vec3(data->x, data->y, data->z), data->scale });
            csm.AddComponent(Entity, ModelMeshC{ model, model.GetBoundingBoxModel() });
            csm.AddComponent(Entity, CollisionC{ SolidCollision, true });

            //Movable with Keyboard
            csm.AddComponent(Entity, MotionC{});
            csm.AddComponent(Entity, InputC{ Keyboard });
        }
        #pragma endregion
        //model0.playAnimation(new Animation("Armature", vec2(0, 55), 0.2, 10, true), false); //forcing our model to play the animation (name, frames, speed, priority, loop)

        //Add temp items
        inv->SetItem(0, Item{ "Item in Inv", "desc", "test2" });
        inv->SetItem(50, Item{ "Item in Toolbar", "desc", "test2" });

        ///std::this_thread::sleep_for(std::chrono::milliseconds(3000));
        loadingScreen->specialRender(window, "Loading complete", width, height);
        ///std::this_thread::sleep_for(std::chrono::milliseconds(1490));

        #pragma region Pre Game Loop
        glm::mat4 projection = glm::perspective(camera.GetZoom(), static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT), 0.1f, 100.0f); //Render Distance
        collisionSystem->Update();
        inv->realignWindows(SCREEN_WIDTH, SCREEN_HEIGHT);
        inv->Hide();
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
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
#           pragma endregion

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

            //Lighting
            glUniform3f(glGetUniformLocation(shaderLoader->ID, "lightColor"), 1.f, 1.f, 1.f);
            glUniform3f(glGetUniformLocation(shaderLoader->ID, "lightPos"), -20.f, 70.f, 100.f);
            glUniform3f(glGetUniformLocation(shaderLoader->ID, "viewPos"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

            modelSystem->Update(shaderLoader);

            ///CAMPOS std::cout << camera.GetPosition().x << ";" << camera.GetPosition().y << ";" << camera.GetPosition().z << std::endl;

            //Draw all Chunks
            for (Chunk chunk : cm.GetChunks()) {
                mat4 objectModel;
                glUniformMatrix4fv(glGetUniformLocation(shaderLoader->ID, "model"), 1, GL_FALSE, value_ptr(objectModel)); //send the empty model matrix to the shader
                chunk.model.Draw(shaderLoader);
            }

            shaderLoader->unuse();
            #pragma endregion

            inv->render();

            // Swap the buffers
            glfwSwapBuffers(window);
        }
    }
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}
