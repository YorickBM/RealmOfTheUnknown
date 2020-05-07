#pragma region Includes
#pragma once
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
#include <sstream>

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
#include "Inventory.h"
#include "Components.h"
#include "Systems.h"
#include "Camera.h"
#include "AnimModel.h"
#include "Collision.h"
#include "ToolBox.h"
#include "ComponentSystemManager.h"
#include "ChunkManager.h"
#include "ModelLoader.h";
#include "ScreenManager.h"
#include "CollisionUtility.h"
#include "AudioMaster.h"
#include "Settings.h"
#include "MobManager.h"
#pragma endregion
using namespace nanogui;
#pragma region Vars
// Properties
const GLuint WIDTH = 800, HEIGHT = 800;
const char* TITLE = "Fighting Against The Coruption - (0.0.1)";
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera camera(glm::vec3(0.7f, 6.f, 106.f));
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

Screen* screen = nullptr;
Screen* startScreenScreen = nullptr;
Screen* startScreenImgScreen = nullptr;
Screen* loadingScreenScreen = nullptr;
Screen* settingsScreenScreen = nullptr;
Screen* settingsScreenImgScreen = nullptr;
Screen* classSelectorInteractiveScreen = nullptr;
Screen* classSelectorNonInteractiveScreen = nullptr;

Inventory* inv;
StartScreen* startScreen;
LoadingScreen* loadingScreen;
ClassSelector* classSelector;
SettingsScreen* settingsScreen;


std::unordered_map<string, string> settings;
std::unordered_map<string, string> inventory;
std::unordered_map<string, Item> invItems;
std::vector<std::string> resolutions;
std::unordered_map<string, string> playerData;

std::shared_ptr<ItemEntitySystem> entitySystem;
std::shared_ptr<AttackSystem> damageSystem;

AudioMaster* audioMaster;
Entity CameraEntity;
MobManager mobManager;

#pragma endregion

int main(int /* argc */, char** /* argv */) {
    #pragma region Resolutions
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
    #pragma region Audio
    //Item{ "", "", "", InventoryCataType::Tools, 1, "",ENTYPO_ICON_LAB_FLASK, "", ENTYPO_ICON_NEWSLETTER, "", ENTYPO_ICON_CLIPBOARD }
    audioMaster = new AudioMaster();
    #pragma endregion

    #pragma region Loading Settings
    //Thread Loading Settings
    auto f = []() {
        settings = FileLoader::loadDataFile("Settings.data");
        playerData = FileLoader::loadDataFile("PlayerData.data");
    };
    std::thread thread_object(f);
    #pragma endregion
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
    csm.RegisterComponent<ChunkC>();
    csm.RegisterComponent<EntityC>();
    csm.RegisterComponent<NPCC>(); 
    csm.RegisterComponent<DataC>();
    csm.RegisterComponent<PathfindingC>();
    csm.RegisterComponent<XpC>();
    csm.RegisterComponent<SpellC>();
    csm.RegisterComponent<ManaC>();

    auto dataSystem = csm.RegisterSystem<DataSystem>();
    {
        Signature signature;
        signature.set(csm.GetComponentType<DataC>());
        csm.SetSystemSignature<DataSystem>(signature);
    }
    dataSystem->Init();

    auto inputSystem = csm.RegisterSystem<InputSystem>();
    {
        Signature signature;
        signature.set(csm.GetComponentType<MotionC>());
        signature.set(csm.GetComponentType<InputC>());
        csm.SetSystemSignature<InputSystem>(signature);
    }
    inputSystem->Init();

    entitySystem = csm.RegisterSystem<ItemEntitySystem>();
    {
        Signature signature;
        signature.set(csm.GetComponentType<EntityC>());
        signature.set(csm.GetComponentType<TransformC>());
        csm.SetSystemSignature<ItemEntitySystem>(signature);
    }
    entitySystem->Init();

    auto movementSystem = csm.RegisterSystem<MovementSystem>();
    {
        Signature signature;
        signature.set(csm.GetComponentType<MotionC>());
        signature.set(csm.GetComponentType<TransformC>());
        csm.SetSystemSignature<MovementSystem>(signature);
    }
    movementSystem->Init();

    auto pathfindingSystem = csm.RegisterSystem<PathfindingSystem>();
    {
        Signature signature;
        signature.set(csm.GetComponentType<TransformC>());
        signature.set(csm.GetComponentType<PathfindingC>()); 
        signature.set(csm.GetComponentType<EntityC>());
        signature.set(csm.GetComponentType<ModelMeshC>());
        csm.SetSystemSignature<PathfindingSystem>(signature);
    }
    pathfindingSystem->Init();

    auto chunkSystem = csm.RegisterSystem<ChunkSystem>();
    {
        Signature signature;
        signature.set(csm.GetComponentType<ChunkC>());
        csm.SetSystemSignature<ChunkSystem>(signature);
    }
    chunkSystem->Init();

    auto modelSystem = csm.RegisterSystem<ModelMeshSystem>();
    {
        Signature signature;
        signature.set(csm.GetComponentType<ModelMeshC>());
        signature.set(csm.GetComponentType<TransformC>());
        csm.SetSystemSignature<ModelMeshSystem>(signature);
    }
    modelSystem->Init();

    auto collisionSystem = csm.RegisterSystem<CollisionSystem>();
    {
        Signature signature;
        signature.set(csm.GetComponentType<CollisionC>());
        signature.set(csm.GetComponentType<ModelMeshC>());
        signature.set(csm.GetComponentType<TransformC>());
        csm.SetSystemSignature<CollisionSystem>(signature);
    }
    collisionSystem->Init();

    auto xpSystem = csm.RegisterSystem<XpSystem>();
    {
        Signature signature;
        signature.set(csm.GetComponentType<XpC>());
        signature.set(csm.GetComponentType<HealthC>());
        signature.set(csm.GetComponentType<SpellC>());
        csm.SetSystemSignature<XpSystem>(signature);
    }
    xpSystem->Init();

    damageSystem = csm.RegisterSystem<AttackSystem>();
    {
        Signature signature;
        signature.set(csm.GetComponentType<HealthC>());
        signature.set(csm.GetComponentType<TransformC>());
        signature.set(csm.GetComponentType<EntityC>());
        csm.SetSystemSignature<AttackSystem>(signature);
    }
    damageSystem->Init();

    auto healthSystem = csm.RegisterSystem<HealthSystem>();
    {
        Signature signature;
        signature.set(csm.GetComponentType<HealthC>());
        csm.SetSystemSignature<HealthSystem>(signature);
    }
    healthSystem->Init();

    auto spellSystem = csm.RegisterSystem<SpellSystem>();
    {
        Signature signature;
        signature.set(csm.GetComponentType<ManaC>());
        signature.set(csm.GetComponentType<SpellC>());
        csm.SetSystemSignature<SpellSystem>(signature);
    }
    spellSystem->Init();

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
    std::string displaySize = settings.at("DisplaySize");
    std::vector<std::string> heightAndWidth = FileLoader::Split(displaySize += "x0", "x");

    GLFWwindow* window = glfwCreateWindow(std::stoi(heightAndWidth[0]), std::stoi(heightAndWidth[1]), TITLE, nullptr, nullptr);
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
    screen->initialize(window, false);
    startScreenScreen = new Screen();
    startScreenScreen->initialize(window, false);
    startScreenImgScreen = new Screen();
    startScreenImgScreen->initialize(window, false);
    loadingScreenScreen = new Screen();
    loadingScreenScreen->initialize(window, false);
    classSelectorInteractiveScreen = new Screen();
    classSelectorInteractiveScreen->initialize(window, false);
    classSelectorNonInteractiveScreen = new Screen();
    classSelectorNonInteractiveScreen->initialize(window, false);

    settingsScreenScreen = new Screen();
    settingsScreenScreen->initialize(window, false);
    settingsScreenImgScreen = new Screen();
    settingsScreenImgScreen->initialize(window, false);

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
    inv = new Inventory(window, SCREEN_WIDTH, SCREEN_HEIGHT, camera);
    inv->ShowInfo();

    classSelector = new ClassSelector(classSelectorInteractiveScreen, SCREEN_WIDTH, SCREEN_HEIGHT, window, classSelectorNonInteractiveScreen);
    startScreen = new StartScreen(startScreenScreen, SCREEN_WIDTH, SCREEN_HEIGHT, window, startScreenImgScreen);
    loadingScreen = new LoadingScreen(loadingScreenScreen, SCREEN_WIDTH, SCREEN_HEIGHT);
    settingsScreen = new SettingsScreen(settingsScreenScreen, SCREEN_WIDTH, SCREEN_HEIGHT, window, settingsScreenImgScreen);

    classSelector->UpdateParentClasses(startScreen);
    startScreen->UpdateParentClasses(classSelector, settingsScreen);
    settingsScreen->UpdateParentClasses(startScreen);
    #pragma endregion
    #pragma region glfw Callbacks to NanoGUI & ECS
    glfwSetCursorPosCallback(window,
        [](GLFWwindow*, double x, double y) {
            for(Screen* screen : inv->getScreens())
                screen->cursorPosCallbackEvent(x, y);
            inv->realignWindows(SCREEN_WIDTH, SCREEN_HEIGHT); //Prevent the movement this way
            startScreen->getScreen()->cursorPosCallbackEvent(x, y);
            classSelector->getScreen()->cursorPosCallbackEvent(x, y);
            classSelector->getScreenOtherTheme()->cursorPosCallbackEvent(x, y);
            settingsScreen->getScreen()->cursorPosCallbackEvent(x, y);

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
            for (Screen* screen : inv->getScreens())
                screen->mouseButtonCallbackEvent(button, action, modifiers);
            startScreen->getScreen()->mouseButtonCallbackEvent(button, action, modifiers);
            classSelector->getScreen()->mouseButtonCallbackEvent(button, action, modifiers);
            classSelector->getScreenOtherTheme()->mouseButtonCallbackEvent(button, action, modifiers);
            settingsScreen->getScreen()->mouseButtonCallbackEvent(button, action, modifiers);
            damageSystem->ExecuteAttack(camera, inv, button, action);
        }
    );

    glfwSetKeyCallback(window,[](GLFWwindow* window, int key, int scancode, int action, int mods) {
            screen->keyCallbackEvent(key, scancode, action, mods);
            inv->keyCallbackEvent(key, scancode, action, mods, window);
            startScreen->getScreen()->keyCallbackEvent(key, scancode, action, mods);
            classSelector->getScreen()->keyCallbackEvent(key, scancode, action, mods);
            classSelector->getScreenOtherTheme()->keyCallbackEvent(key, scancode, action, mods);
            settingsScreen->getScreen()->keyCallbackEvent(key, scancode, action, mods);

            entitySystem->Update(camera, inv, key, scancode, action, mods);
            damageSystem->ExecuteAttack(camera, inv, key, action);

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
            settingsScreen->getScreen()->charCallbackEvent(codepoint);
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
           classSelector->realignWindows(width, height);

           loadingScreen->realignWindows(width, height);
           loadingScreen->render();

        #pragma region Update in Settings Map
           std::string size = std::to_string(width) + "x" + std::to_string(height);
           settings.at("DisplaySize") = size;
        #pragma endregion
        }
    );
    #pragma endregion

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    #pragma region Audio
    audioMaster->genEffectsEngine();
    audioMaster->genMainEngine();

    audioMaster->SetEngineVolume(audioMaster->GetEffectsSoundEngine(), std::stof(settings.at("CombatVol")));
    audioMaster->SetEngineVolume(audioMaster->GetMainSoundEngine(), std::stof(settings.at("MusicVol")));
    audioMaster->PlaySound(audioMaster->GetMainSoundEngine(), "resources/Sounds/MainMenu.mp3", true);
    #pragma endregion

    ///REMOVE !
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

        startScreen->render();
        classSelector->render();
        settingsScreen->render();

        // Swap the buffers
        glfwSwapBuffers(window);
    }

    if (!glfwWindowShouldClose(window)) {

        #pragma region Shaders
        loadingScreen->specialRender(window, "Loading Shaders", width, height);
        // Setup and compile our shaders
        ShaderLoader* shaderLoader = new ShaderLoader();
        shaderLoader->loadShaders("vertexShader.glsl", "fragmentShader.glsl");
        #pragma endregion

        #pragma region Inventory
        loadingScreen->specialRender(window, "Loading Items", width, height);
        std::unordered_map<string, string> invItemsFile = FileLoader::loadDataFile("Inventory.data");
        invItems.insert(make_pair("Dummy Hammer", Item{ "Dummy Hammer", {"A hammer for all the hunter dummy's." ,"Deals: 3-5 damage per hit."}, "Inventory/DummyHammer", InventoryCataType::Tools, 1, "Min. Level --1",ENTYPO_ICON_LAB_FLASK, "Class: --Hunter", ENTYPO_ICON_NEWSLETTER, "Att. Spd: Slow", ENTYPO_ICON_FLASH, ItemType::game_dummy_hammer, -1, true, ArmorType::NoArmor, ItemTypeE::game_weapon }));
        invItems.insert(make_pair("Bone", Item{ "Bone", {"Look for a wandering trader, they ","might be intressted in this", " miscellaneous item."}, "Inventory/Bone", InventoryCataType::Miscellaneous, 1, "Misc Item",ENTYPO_ICON_LAB_FLASK, "", 0, "", 0, ItemType::game_bone, -1, true }));
        invItems.insert(make_pair("Worn Boots", Item{ "Worn Boots", {"Some old boots found in the pond", " nearby. Just sturdy enough for ", "some basic protection."}, "Inventory/Worn Boots", InventoryCataType::Armor, 1, "Min. Level --3",ENTYPO_ICON_LAB_FLASK, "Health Boost: +6", ENTYPO_ICON_CIRCLE_WITH_PLUS, "", 0, ItemType::game_worn_boots, 1, false, ArmorType::Boots, ItemTypeE::game_armor }));
        invItems.insert(make_pair("Mushroom", Item{ "Mushroom", {"Old fasion mushrooms.","Some people might say they", " are perfect for soup."}, "Inventory/Mushroom", InventoryCataType::Miscellaneous, 1, "Misc Item",ENTYPO_ICON_LAB_FLASK, "", 0, "", 0, ItemType::game_mushroom, -1, true }));

        loadingScreen->specialRender(window, "Adding items to player inventory", width, height);
        for (auto pair : invItemsFile) {
            for (int i = 0; i < std::stoi(pair.second); i++)
                inv->AddItem(invItems.at(pair.first));
        }

        loadingScreen->specialRender(window, "Initializing Quests", width, height);
        Quest quest1 = Quest("Protect your camp", { "Collect 15 bones", "", "Reward: 6 Currencry" }, QuestCataType::Open, QuestType::quest_protect_camp, 1, { "Hey there, can you collect 15 bones", "for me? If you do i will reward you", "with 6 tickets!" }, { "Keep going! You doing great.", "Small Tip my friend, at night most", "mobs spawn in the forest, or mine." }, { "Geez, thank you! I see great", "strength in you soldier.", "Here is your reward...!" }, { "Currency:6" }, {"15xgame_bone"});
        Quest quest2 = Quest("Spider Forest", { "Collect 6 Mushrooms", "", "Reward: 14 Currencry" }, QuestCataType::Open, QuestType::quest_forest, 1, { "Hey! Can you collect 6 mushrooms?", "They can be found in the spider", "infested forest behind me." }, { "Just keep going, you are almost", "there. Just a few more so i can", "make my soup again." }, { "Geez, thank you! Now i can finally", "make my famous soup again!", "Here is your reward...!" }, { "Currency:14" }, { "6xgame_mushroom" });
        Quest quest3 = Quest("Boat Repair", { "Repair your boat by the Miner", "Costst: 20 currency", "", "Reward: 24 Currencry & Acces to Ryton" }, QuestCataType::Open, QuestType::quest_repair_boat, 3, { "Hmm, you say you need a boat fixed?", "I can do that for 20 tickets my", "friend. When you got those find me!" }, { "Already got that money for me?", "If not come back later i don't do", "anything for free here." }, { "Hehehe, i knew you could do it.", "Soo, where is your boat? Ill fix it", "for you so you can be on your way." }, { "Currency:24" }, { "20xcurrency" });
        quest3.taskType = QuestTaskType::CurrencyTask;
        std::map<QuestType, Quest> map;
        map.insert(make_pair(quest1.type, quest1));
        map.insert(make_pair(quest2.type, quest2));
        map.insert(make_pair(quest3.type, quest3));

        inv->AddQuest(quest1);
        inv->AddQuest(quest2);
        inv->AddQuest(quest3);

        inv->DropItem(invItems.at("Mushroom"), camera.GetPosition(), false);
        inv->DropItem(invItems.at("Mushroom"), camera.GetPosition(), false);
        inv->DropItem(invItems.at("Mushroom"), camera.GetPosition(), false);
        inv->DropItem(invItems.at("Mushroom"), camera.GetPosition(), false);
        inv->DropItem(invItems.at("Mushroom"), camera.GetPosition(), false);
        inv->DropItem(invItems.at("Mushroom"), camera.GetPosition(), false);
        inv->DropItem(invItems.at("Mushroom"), camera.GetPosition(), false);
        #pragma endregion

        #pragma region Mobs
        loadingScreen->specialRender(window, "Loading Mobs", width, height);
        mobManager = MobManager();

        AnimModel spiderModel("resources/Mobs/SpiderMeshOnly.fbx");
        AnimModel skeletonModel ("resources/Mobs/Skeleton.fbx");

        mobManager.CreateMob(Mob(spiderModel, MobType::mob_spider, {"currency:2-9", "xp:10-26"}, .07f, 10), MobType::mob_spider);
        mobManager.CreateMob(Mob(skeletonModel, MobType::mob_skeleton, { "currency:2-9", "xp:10-26" }, .07f, 10), MobType::mob_skeleton);
        //mobManager.CreateMob(Mob(cowModel, MobType::mob_cow, { "currency:1-3", "xp:6-20" }, .07f, 10), MobType::mob_cow);
        //mobManager.CreateMob(Mob(sheepModel, MobType::mob_sheep, { "currency:1-3", "xp:6-20" }, .07f, 10), MobType::mob_sheep);

        mobManager.SpawnMob(MobType::mob_spider, camera.GetPosition(), true, 0.02f); // vec3(-39.212929, 5.305047, -109.676712)
        mobManager.SpawnMob(MobType::mob_spider, vec3(-40.212929, 5.305047, -106.676712), true, 0.02f);
        mobManager.SpawnMob(MobType::mob_skeleton, vec3(-42.212929, 5.305047, -108.676712), true, 0.02f);
        mobManager.SpawnMob(MobType::mob_skeleton, camera.GetPosition(), true, 0.02f);
        #pragma endregion

        #pragma region Entity Creation & Chunk Loading
        loadingScreen->specialRender(window, "Initializing Chunks/Loading Chunks", width, height);
        cm.InitChunks("res/Chunks/ChunkData.txt", "", 0.2f);
        ///csm.InitEntities("res/System/Entities.txt");

        loadingScreen->specialRender(window, "Initializing Model Data", width, height);
        std::vector<ModelDataClass*> modelData = FileLoader::ReadModelData("ModelData.data");

        int modelnum = 0;
        int amountmodels = modelData.size();

        #pragma region Camera/Player Entity
        CameraEntity = csm.CreateEntity();
        csm.AddComponent(CameraEntity, MotionC{2.f, Camera_Movement::NONE, true});
        csm.AddComponent(CameraEntity, InputC{ Keyboard });
        csm.AddComponent(CameraEntity, TransformC{ camera.GetPosition(), 1.f });
        csm.AddComponent(CameraEntity, DataC{});
        csm.AddComponent(CameraEntity, HealthC{});
        csm.AddComponent(CameraEntity, SpellC{});
        csm.AddComponent(CameraEntity, XpC{ 1, 0, 0});
        csm.AddComponent(CameraEntity, ManaC{ 20, 20 });
        #pragma endregion

        for (ModelDataClass* data : modelData) {
            std::stringstream ss;
            ss << "Loading Models (" << modelnum++ << "/" << amountmodels << ")";
            std::string str = ss.str();
            loadingScreen->specialRender(window, str, width, height);
            bool allowRender = false;

            if (settings.at("GraphicsDetail") == "high") {
                if (data->detail == "low") allowRender = true;
                else if (data->detail == "medium") allowRender = true;
                else if (data->detail == "high") allowRender = true;
            } 
            else if (settings.at("GraphicsDetail") == "medium") {
                if (data->detail == "low") allowRender = true;
                else if (data->detail == "medium") allowRender = true;
                else if (data->detail == "high") allowRender = false;
            }
            else if (settings.at("GraphicsDetail") == "low") {
                if (data->detail == "low") allowRender = true;
                else if (data->detail == "medium") allowRender = false;
                else if (data->detail == "high") allowRender = false;
            }

            if (allowRender) {
                AnimModel model(data->path, vec3(data->x, data->y, data->z), data->scale, vec3(data->rx, data->ry, data->rz));
                vec3 min, max;
                std::map<pair<float, float>, float> worldMapDataMap;
                std::vector<vec2> positionsMap;
                auto Entity = csm.CreateEntity();

                if (data->colType == 3) {
                    vector<Mesh*> meshes = model.GetMeshes();
                    for (Mesh* mesh : meshes) {
                        vector<vec3> vertices = mesh->translateVertices(data->scale, vec3(data->x, data->y, data->z), vec3(data->rx, data->ry, data->rz));

                        for (vec3 vertice : vertices) {
                            if (worldMapDataMap.count(make_pair(vertice.x, vertice.z)) == 0) {
                                worldMapDataMap.insert(make_pair(make_pair(vertice.x, vertice.z), vertice.y));
                                positionsMap.push_back(vec2(vertice.x, vertice.z));
                            }
                        }
                    }

                    csm.AddComponent(Entity, ChunkC{ worldMapDataMap, positionsMap });
                }

                if (data->colType != 2) { model.GetMinAndMaxVertice(min, max); }
                if (data->colType != 2) csm.AddComponent(Entity, CollisionC{ data->colType, BoundingBox{min, max} });
                if (data->questType != "N/A") { csm.AddComponent(Entity, EntityC{ Item{}, true }); csm.AddComponent(Entity, NPCC{ map.at(EnumUtility::StringToQuestType(data->questType)) });
            }
                csm.AddComponent(Entity, TransformC{ vec3(data->x, data->y, data->z), data->scale });
                csm.AddComponent(Entity, ModelMeshC{ model });
            }
        }
        #pragma endregion
        map.clear(); //Remove Temp Stored Quests

        #pragma region Levels
        auto& xpC = csm.GetComponent<XpC>(CameraEntity);

        loadingScreen->specialRender(window, "Loading Levels", width, height);
        xpSystem->CreateLevel(1, 0, {"Spell:spell_hunter_bash"});
        xpSystem->CreateLevel(2, 100, { "Health:5", "SkillPoints:2" });
        xpSystem->CreateLevel(3, 170, { "Health:5", "SkillPoints:2" });
        xpSystem->CreateLevel(4, 250, { "Health:5", "SkillPoints:2" });
        xpSystem->CreateLevel(5, 340, { "Health:5", "SkillPoints:2" });
        xpSystem->CreateLevel(6, 440, { "Health:5", "SkillPoints:2" });
        xpSystem->CreateLevel(7, 560, { "Health:5", "SkillPoints:2" });
        xpSystem->CreateLevel(8, 690, { "Health:5", "SkillPoints:2" });
        xpSystem->CreateLevel(9, 840, { "Health:5", "SkillPoints:2" });
        xpSystem->CreateLevel(10, 990, { "Health:5", "SkillPoints:2" });
        xpSystem->CreateLevel(11, 1160, { "Spell:spell_hunter_charge", "Health:5", "SkillPoints:2" });
        xpC.currentXp = std::stoi(playerData.at("xp"));
        #pragma endregion

        ///ANIMATION
        ///model0.playAnimation(new Animation("Armature", vec2(0, 55), 0.2, 10, true), false); //forcing our model to play the animation (name, frames, speed, priority, loop)

        #pragma region Pre Game Loop
        loadingScreen->specialRender(window, "Loading complete", width, height);
        glm::mat4 projection = glm::perspective(camera.GetZoom(), static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT), 0.1f, 40.0f); //Render Distance

        inv->realignWindows(SCREEN_WIDTH, SCREEN_HEIGHT);
        inv->Hide();
        inv->SetCurrencry(std::stoi(playerData.at("currency")));
        inv->SetXp(std::stoi(playerData.at("xp"))); 
        camera.SetPosition(vec3(std::stof(playerData.at("cameraPosX")), std::stof(playerData.at("cameraPosY")), std::stof(playerData.at("cameraPosZ"))));

        audioMaster->PlayNewSound(audioMaster->GetMainSoundEngine(), "resources/Sounds/Game.mp3", true);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
            #pragma endregion

            #pragma region Game Objects
            //Game Objects
            pathfindingSystem->Update(camera);
            pathfindingSystem->MoveToGoal(deltaTime);
            inputSystem->Update(keys, settings);
            movementSystem->Update(deltaTime, camera);
            chunkSystem->Update(camera, dataSystem->GetEntities());
            collisionSystem->Update(camera);
            healthSystem->Update(deltaTime, inv, CameraEntity);
            xpSystem->Update(inv);
            damageSystem->Update(camera, inv, CameraEntity, deltaTime);
            spellSystem->Update(inv);

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
            glUniform1f(glGetUniformLocation(shaderLoader->ID, "ambientStrength"), 0.2f);
            glUniform1f(glGetUniformLocation(shaderLoader->ID, "specularStrength"), 0.1f);

            modelSystem->Update(shaderLoader);
            
            ///DEBUG
            ///std::cout << camera.GetPosition().x << ";" << camera.GetPosition().y << ";" << camera.GetPosition().z << std::endl;

            shaderLoader->unuse();
            #pragma endregion

            inv->render();

            // Swap the buffers
            glfwSwapBuffers(window);
        }
    }
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    thread_object.detach();

    #pragma region Save Data
    auto& xpC = csm.GetComponent<XpC>(CameraEntity);
    playerData.at("currency") = inv->GetCurrency();
    playerData.at("xp") = std::to_string(xpC.currentXp);
    playerData.at("cameraPosX") = std::to_string(camera.GetPosition().x);
    playerData.at("cameraPosY") = std::to_string(camera.GetPosition().y);
    playerData.at("cameraPosZ") = std::to_string(camera.GetPosition().z);

    FileLoader::SaveFile("Settings.data", settings);
    FileLoader::SaveFile("PlayerData.data", playerData);
    #pragma endregion

    return 0;
}