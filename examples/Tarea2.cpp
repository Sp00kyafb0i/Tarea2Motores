
#include <MonaEngine.hpp>
#include "Rendering/DiffuseFlatMaterial.hpp"
#include "Rendering/PBRTexturedMaterial.hpp"
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <random>

constexpr unsigned int CUSTOM_EVENT_COIN_COLLECTED = 0;
constexpr unsigned int CUSTOM_EVENT_NEW_LEVEL = 1;


float gaussian(float x, float y, float s, float sigma, glm::vec2 mu) {
    return (s / (sigma * std::sqrt(2 * std::numbers::pi))) * std::exp((-1 / (2 * std::pow(sigma, 2))) * (std::pow((x - mu[0]), 2) + std::pow((y - mu[1]), 2)));
}


float hFunction(float x, float y) {
    return -0.2*y + sin(0.3*y) + 0.1*sin(x);
}



class Player : public Mona::GameObject
{
public:
    Player() : m_speed(0.0f)
    {
    }

    void UserStartUp(Mona::World& world) noexcept
    {
        m_transform = world.AddComponent<Mona::TransformComponent>(*this);
        m_transform->Rotate(glm::vec3(1.0f, 0.0f, 0.0f), 3.14f/2.0f);
        m_transform->Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 3.14f);
        m_transform->Scale(glm::vec3(0.4f, 0.4f, 0.4f));
        m_speed = 20.0;
        speed = 10.0;


        auto& config = Mona::Config::GetInstance();
        auto& meshManager = Mona::MeshManager::GetInstance();
        auto& textureManager = Mona::TextureManager::GetInstance();

        //auto mesh = meshManager.LoadMesh(config.getPathOfApplicationAsset("male_casual.fbx"), true);
        auto mesh = meshManager.LoadMesh(config.getPathOfApplicationAsset("Car.obj"), true);
        auto material = std::static_pointer_cast<Mona::PBRTexturedMaterial>(world.CreateMaterial(Mona::MaterialType::DiffuseTextured));
        //auto texture = textureManager.LoadTexture(config.getPathOfApplicationAsset("man_tex.png"));
        auto texture = textureManager.LoadTexture(config.getPathOfApplicationAsset("car.png"));
        material->SetAlbedoTexture(texture);

        world.AddComponent<Mona::StaticMeshComponent>(*this, mesh, material);

        glm::vec3 paddleScale(1.0f, 1.0f, 1.0f);
        m_transform->Scale(paddleScale);
        Mona::BoxShapeInformation boxInfo(paddleScale);
        Mona::RigidBodyHandle rb = world.AddComponent<Mona::RigidBodyComponent>(*this, boxInfo, Mona::RigidBodyType::DynamicBody);
        //rb->SetFriction(0.0f);
        //rb->SetRestitution(1.0f);
    }

    void setTransform(glm::vec3 position) {
        m_transform->SetTranslation(position);
    }

    void setSpeed(float v) {
        m_speed = v;
    }

    float getSpeed() {
        return m_speed;
    }

    void UserUpdate(Mona::World& world, float timeStep) noexcept
    {
        m_transform->Translate(glm::vec3(0.0f, m_speed * timeStep, 0.0f));
        auto& input = world.GetInput();
        if (input.IsKeyPressed(MONA_KEY_A) or
            input.IsKeyPressed(MONA_KEY_LEFT) or
            input.IsGamepadButtonPressed(MONA_JOYSTICK_1, MONA_GAMEPAD_BUTTON_DPAD_LEFT) or
            input.GetGamepadAxisValue(MONA_JOYSTICK_1, MONA_GAMEPAD_AXIS_LEFT_X) < -0.25f)
        {
            m_transform->Translate(glm::vec3(-speed * timeStep, 0.0f, 0.0f));
            if (input.IsKeyPressed(MONA_KEY_S) or
                input.IsKeyPressed(MONA_KEY_DOWN) or
                input.IsGamepadButtonPressed(MONA_JOYSTICK_1, MONA_GAMEPAD_BUTTON_DPAD_DOWN) or
                input.GetGamepadAxisValue(MONA_JOYSTICK_1, MONA_GAMEPAD_AXIS_LEFT_Y) > 0.25f)
            {
                m_transform->Translate(glm::vec3(0.0f, -speed * timeStep, 0.0f));
            }
        }
        else if (input.IsKeyPressed(MONA_KEY_D) or
            input.IsKeyPressed(MONA_KEY_RIGHT) or
            input.IsGamepadButtonPressed(MONA_JOYSTICK_1, MONA_GAMEPAD_BUTTON_DPAD_RIGHT) or
            input.GetGamepadAxisValue(MONA_JOYSTICK_1, MONA_GAMEPAD_AXIS_LEFT_X) > 0.25f)
        {
            m_transform->Translate(glm::vec3(speed * timeStep, 0.0f, 0.0f));
            if (input.IsKeyPressed(MONA_KEY_S) or
                input.IsKeyPressed(MONA_KEY_DOWN) or
                input.IsGamepadButtonPressed(MONA_JOYSTICK_1, MONA_GAMEPAD_BUTTON_DPAD_DOWN) or
                input.GetGamepadAxisValue(MONA_JOYSTICK_1, MONA_GAMEPAD_AXIS_LEFT_Y) > 0.25f)
            {
                m_transform->Translate(glm::vec3(0.0f, -speed * timeStep, 0.0f));
            }
        }
        else if (input.IsKeyPressed(MONA_KEY_W) or
            input.IsKeyPressed(MONA_KEY_UP) or
            input.IsGamepadButtonPressed(MONA_JOYSTICK_1, MONA_GAMEPAD_BUTTON_DPAD_UP) or
            input.GetGamepadAxisValue(MONA_JOYSTICK_1, MONA_GAMEPAD_AXIS_LEFT_Y) < -0.25f)
        {
            m_transform->Translate(glm::vec3(0.0f, m_speed * timeStep, 0.0f));
        }
        else if (input.IsKeyPressed(MONA_KEY_S) or
            input.IsKeyPressed(MONA_KEY_DOWN) or
            input.IsGamepadButtonPressed(MONA_JOYSTICK_1, MONA_GAMEPAD_BUTTON_DPAD_DOWN) or
            input.GetGamepadAxisValue(MONA_JOYSTICK_1, MONA_GAMEPAD_AXIS_LEFT_Y) > 0.25f)
        {
            m_transform->Translate(glm::vec3(0.0f, -speed * timeStep, 0.0f));
        }
        if (m_speed <= 10) {
            m_speed = 20;
        }
        if (m_speed >= 30) {
            m_speed = 30;
        }
    }

    glm::vec3 getLocation() const
    {
        return m_transform->GetLocalTranslation();
    }


private:
    Mona::TransformHandle m_transform;
    float m_speed;
    float speed;
};

class TopDownCamera : public Mona::GameObject
{
public:
    void UserStartUp(Mona::World& world) noexcept override
    {
        m_cameraOffset = glm::vec3(0.0f, -4.0f, 4.0f);
        auto transform = world.AddComponent<Mona::TransformComponent>(*this, m_cameraOffset);
        transform->Rotate(glm::vec3(1.0f, 0.0f, 0.0f), -0.5f);
        world.SetMainCamera(world.AddComponent<Mona::CameraComponent>(*this));
        world.AddComponent<Mona::DirectionalLightComponent>(*this, glm::vec3(1.0f));
        world.SetAudioListenerTransform(transform);
        auto& config = Mona::Config::GetInstance();
        auto& audioClipManager = Mona::AudioClipManager::GetInstance();
        auto audioClipPtr = audioClipManager.LoadAudioClip(config.getPathOfApplicationAsset("AudioFiles/musica.wav"));
        auto audioSource = world.AddComponent<Mona::AudioSourceComponent>(*this, audioClipPtr);
        audioSource->SetIsLooping(true);
        audioSource->SetVolume(0.02f);
        audioSource->Play();
    }

    void UserUpdate(Mona::World& world, float timeStep) noexcept override
    {
        /* We are adding a dependency, camera depends on the player,
         * the camera game object should be updated after the player game object...
         * sadly, current version of MonaEngine does not allow us to specify this
         * dependency. Here we just collect the current value, which could have been
         * updated or not the current frame.
         * Worse case scenario: player information will correspond to the previous
         * frame. */
        if (m_playerPtr == nullptr)
            return;

        /* The camera remains over the player */
        Player& player = *m_playerPtr;
        auto playerTransform = world.GetComponentHandle<Mona::TransformComponent>(player);
        glm::vec3 target = playerTransform->GetLocalTranslation();
        glm::vec3 newCameraLocation = target + m_cameraOffset;

        auto cameraTransform = world.GetComponentHandle<Mona::TransformComponent>(*this);
        cameraTransform->SetTranslation(newCameraLocation);
    }

    void SetPlayerPtr(Player* playerPtr)
    {
        m_playerPtr = playerPtr;
    }

private:
    glm::vec3 m_cameraOffset;
    Player* m_playerPtr;
};

class Floor : public Mona::GameObject
{
public:

    

    void UserStartUp(Mona::World& world) noexcept override
    {
        auto transform = world.AddComponent<Mona::TransformComponent>(*this);
        //transform->SetTranslation(glm::vec3(-20, 780, 60));
        //transform->SetRotation(glm::angleAxis(glm::radians(45.f), glm::vec3(1, 0, 0)));
        //transform->Rotate(glm::vec3(0, 1, 0), glm::radians(90.f));
        //transform->SetScale(glm::vec3(400, 400, 400));

        auto& config = Mona::Config::GetInstance();
        auto& meshManager = Mona::MeshManager::GetInstance();
        auto& textureManager = Mona::TextureManager::GetInstance();

        //auto mesh = meshManager.LoadMesh(config.getPathOfApplicationAsset("suelo.obj"), true);
        auto material = std::static_pointer_cast<Mona::PBRTexturedMaterial>(world.CreateMaterial(Mona::MaterialType::DiffuseTextured));
        auto texture = textureManager.LoadTexture(config.getPathOfApplicationAsset("nieve2.png"));
        material->SetAlbedoTexture(texture);

        //auto meshComponentHandle = world.AddComponent<Mona::StaticMeshComponent>(*this, mesh, material);

        
        ////////
        glm::vec2 minXY(-100, -10);
        glm::vec2 maxXY(100, 20000);
        int numInnerVerticesWidth = 2000;
        int numInnerVerticesHeight = 10000;
        

        //meshComponentHandle = world.AddComponent<Mona::StaticMeshComponent>(*this, meshManager.GenerateTerrain(minXY, maxXY, numInnerVerticesWidth,
        //    numInnerVerticesHeight, heighFunc), material);
        meshComponentHandle = world.AddComponent<Mona::StaticMeshComponent>(*this, meshManager.GenerateTerrain(minXY, maxXY, numInnerVerticesWidth,
            numInnerVerticesHeight, hFunction), material);

        ///////////





        

    }

    void UserUpdate(Mona::World& world, float timeStep) noexcept override {}

    Mona::StaticMeshHandle getMesh() {
        return meshComponentHandle;
    }
    

private:
    Mona::StaticMeshHandle meshComponentHandle;
    
};

class Coin : public Mona::GameObject
{
public:
    Coin() : m_angle(0.f), m_speed(360), m_transform(), m_playerPtr(nullptr) {}

    void UserStartUp(Mona::World& world) noexcept override
    {
        m_transform = world.AddComponent<Mona::TransformComponent>(*this);
        m_transform->Rotate(glm::vec3(1, 0, 0), 3.14f / 2);
        m_transform->Scale(glm::vec3(2, 2, 2));

        auto& config = Mona::Config::GetInstance();
        auto& meshManager = Mona::MeshManager::GetInstance();
        auto& textureManager = Mona::TextureManager::GetInstance();
        auto& audioClipManager = Mona::AudioClipManager::GetInstance();

        auto mesh = meshManager.LoadMesh(config.getPathOfApplicationAsset("Coin_A.gltf"), true);
        auto material = std::static_pointer_cast<Mona::PBRTexturedMaterial>(world.CreateMaterial(Mona::MaterialType::DiffuseTextured));
        auto texture = textureManager.LoadTexture(config.getPathOfApplicationAsset("prototypebits_texture.png"));
        material->SetAlbedoTexture(texture);

        world.AddComponent<Mona::StaticMeshComponent>(*this, mesh, material);

        m_audioClipPtr = audioClipManager.LoadAudioClip(config.getPathOfApplicationAsset("SFX_Powerup_21.wav"));
    }

    void UserUpdate(Mona::World& world, float timeStep) noexcept override
    {
        // Rotation animation
        {
            m_angle += m_speed * timeStep;
            const glm::fquat baseOrientation = glm::angleAxis(glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f));
            const glm::fquat variableRotation = glm::angleAxis(glm::radians(m_angle), glm::vec3(0.f, 1.f, 0.f));
            const glm::fquat totalRotation = baseOrientation * variableRotation;

            m_transform->SetRotation(totalRotation);
        }

        // check collision with the player
        if (m_playerPtr == nullptr)
            return;

        glm::vec3 coinLocation = m_transform->GetLocalTranslation();

        Player& player = *m_playerPtr;
        auto playerTransform = world.GetComponentHandle<Mona::TransformComponent>(player);
        glm::vec3 playerLocation = playerTransform->GetLocalTranslation();

        if (glm::distance(coinLocation, playerLocation) <= 1.f)
        {
            world.PlayAudioClip2D(m_audioClipPtr, 1.f, 1.f);
            m_playerPtr->setSpeed(m_playerPtr->getSpeed() + 5);
            world.DestroyGameObject(*this);

            // publishing a message: a coin has been collected!
            auto& eventManager = world.GetEventManager();
            Mona::CustomUserEvent customEvent;
            customEvent.eventID = CUSTOM_EVENT_COIN_COLLECTED;
            eventManager.Publish(customEvent);
        }
    }

    void SetLocation(const glm::vec3& newLocation)
    {
        m_transform->SetTranslation(newLocation);
    }

    void SetPlayerPtr(Player* playerPtr)
    {
        m_playerPtr = playerPtr;
    }

private:
    float m_angle;
    float m_speed;
    Mona::TransformHandle m_transform;
    Player* m_playerPtr;
    std::shared_ptr<Mona::AudioClip> m_audioClipPtr;
};

class Obstacle : public Mona::GameObject
{
public:
    Obstacle() : m_angle(0.f), m_speed(360), m_transform(), m_playerPtr(nullptr) {}

    void UserStartUp(Mona::World& world) noexcept override
    {
        m_transform = world.AddComponent<Mona::TransformComponent>(*this);
        m_transform->Rotate(glm::vec3(1, 0, 0), glm::radians(90.0f));
        m_transform->Scale(glm::vec3(0.01f, 0.01f, 0.01f));

        auto& config = Mona::Config::GetInstance();
        auto& meshManager = Mona::MeshManager::GetInstance();
        auto& textureManager = Mona::TextureManager::GetInstance();
        auto& audioClipManager = Mona::AudioClipManager::GetInstance();

        auto mesh = meshManager.LoadMesh(config.getPathOfApplicationAsset("Tree_Winter_001.fbx"), true);
        auto material = std::static_pointer_cast<Mona::PBRTexturedMaterial>(world.CreateMaterial(Mona::MaterialType::DiffuseTextured));
        auto texture = textureManager.LoadTexture(config.getPathOfApplicationAsset("T_Tree_winter.png"));
        material->SetAlbedoTexture(texture);

        world.AddComponent<Mona::StaticMeshComponent>(*this, mesh, material);

        m_audioClipPtr = audioClipManager.LoadAudioClip(config.getPathOfApplicationAsset("AudioFiles/Efectos/Lose/WAV/Lose1.wav"));
    }

    void UserUpdate(Mona::World& world, float timeStep) noexcept override
    {
        

        // check collision with the player
        if (m_playerPtr == nullptr)
            return;

        glm::vec3 coinLocation = m_transform->GetLocalTranslation();

        Player& player = *m_playerPtr;
        auto playerTransform = world.GetComponentHandle<Mona::TransformComponent>(player);
        glm::vec3 playerLocation = playerTransform->GetLocalTranslation();

        if (glm::distance(coinLocation, playerLocation) <= 1.f)
        {
            world.PlayAudioClip2D(m_audioClipPtr, 1.f, 1.f);
            m_playerPtr->setSpeed(m_playerPtr->getSpeed() - 10);
            world.DestroyGameObject(*this);

            // publishing a message: a coin has been collected!
            auto& eventManager = world.GetEventManager();
            Mona::CustomUserEvent customEvent;
            customEvent.eventID = CUSTOM_EVENT_COIN_COLLECTED;
            eventManager.Publish(customEvent);
        }
    }

    void SetLocation(const glm::vec3& newLocation)
    {
        m_transform->SetTranslation(newLocation);
    }

    void SetPlayerPtr(Player* playerPtr)
    {
        m_playerPtr = playerPtr;
    }

private:
    float m_angle;
    float m_speed;
    Mona::TransformHandle m_transform;
    Player* m_playerPtr;
    std::shared_ptr<Mona::AudioClip> m_audioClipPtr;
};


class FinishLine : public Mona::GameObject
{
public:
    FinishLine() : m_angle(0.f), m_speed(360), m_transform(), m_playerPtr(nullptr) {}

    void UserStartUp(Mona::World& world) noexcept override
    {
        m_transform = world.AddComponent<Mona::TransformComponent>(*this);
        m_transform->Rotate(glm::vec3(0, 0, 1), glm::radians(45.0f));
        m_transform->Scale(glm::vec3(0.1f, 0.1f, 0.1f));

        auto& config = Mona::Config::GetInstance();
        auto& meshManager = Mona::MeshManager::GetInstance();
        auto& textureManager = Mona::TextureManager::GetInstance();
        auto& audioClipManager = Mona::AudioClipManager::GetInstance();

        auto mesh = meshManager.LoadMesh(config.getPathOfApplicationAsset("long_bush.fbx"), true);
        auto material = std::static_pointer_cast<Mona::PBRTexturedMaterial>(world.CreateMaterial(Mona::MaterialType::DiffuseTextured));
        auto texture = textureManager.LoadTexture(config.getPathOfApplicationAsset("Hedge.jpg"));
        material->SetAlbedoTexture(texture);

        world.AddComponent<Mona::StaticMeshComponent>(*this, mesh, material);

        m_audioClipPtr = audioClipManager.LoadAudioClip(config.getPathOfApplicationAsset("AudioFiles/Efectos/1up/WAV/1up1.wav"));
    }

    void UserUpdate(Mona::World& world, float timeStep) noexcept override
    {


        // check collision with the player
        if (m_playerPtr == nullptr)
            return;

        glm::vec3 coinLocation = m_transform->GetLocalTranslation();

        Player& player = *m_playerPtr;
        auto playerTransform = world.GetComponentHandle<Mona::TransformComponent>(player);
        glm::vec3 playerLocation = playerTransform->GetLocalTranslation();
        float metax = coinLocation[0];
        float metay = coinLocation[1];
        float playerx = playerLocation[0];
        float playery = playerLocation[1];

        if (glm::distance(coinLocation, playerLocation) <= 12.f and playerx >= (metax - 11) and playerx <= (metax + 11))
        {
            world.PlayAudioClip2D(m_audioClipPtr, 1.f, 1.f);
            m_playerPtr->setSpeed(m_playerPtr->getSpeed() - 10);
            world.DestroyGameObject(*this);

            // At the moment, MonaEngine does not have a cleaner way to close the app.
            exit(EXIT_SUCCESS);

            
        }
    }

    void SetLocation(const glm::vec3& newLocation)
    {
        m_transform->SetTranslation(newLocation);
    }

    void SetPlayerPtr(Player* playerPtr)
    {
        m_playerPtr = playerPtr;
    }

private:
    float m_angle;
    float m_speed;
    Mona::TransformHandle m_transform;
    Player* m_playerPtr;
    std::shared_ptr<Mona::AudioClip> m_audioClipPtr;
};



struct HighLevelGame
{
    unsigned int currentLevel = 0;
    unsigned int coinsCollected = 0;
    float currentTime = 0.f;
    unsigned int points = 0;

    struct Level
    {
        unsigned int coins;
        unsigned int monsters; // pending implementation
    };

    const std::vector<Level> levels
    {
        {10, 1},
        {5, 2},
        {10, 3},
        {20, 4},
        {30, 4},
        {5, 8},
        {3, 10},
        {50, 1}
    };
};

class LevelProgression : public Mona::GameObject
{
public:

    void UserStartUp(Mona::World& world) noexcept override
    {
        auto& eventManager = world.GetEventManager();
        m_worldPtr = &world;
        eventManager.Subscribe(m_coinCollectedHandle, this, &LevelProgression::CollectCoin);
        auto floorHandle = world.CreateGameObject<Floor>();
        //Floor& floor = *floorHandle;
        meshHandle = floorHandle->getMesh();
        x0 = 0;
        y0 = 0;
        xf = 0;
        yf = 0;
    }

    void UserUpdate(Mona::World& world, float timeStep) noexcept override
    {
        m_status.currentTime += timeStep;
        if (checkPlayerPos(*m_worldPtr, m_playerPtr)[0] >= x0 + 60 or checkPlayerPos(*m_worldPtr, m_playerPtr)[1] >= y0 + 60) {
            ConfigureNewLevel();
            y0 += 60;
            x0 += 60;
            
        }
        if (checkPlayerPos(*m_worldPtr, m_playerPtr)[0] >= xf + 60 or checkPlayerPos(*m_worldPtr, m_playerPtr)[1] >= yf + 300) {
            GenerateRandomEnd(1);
            xf += 60;
            yf += 300;
        }
    }

    void CollectCoin(const Mona::CustomUserEvent& event)
    {
        if (event.eventID != CUSTOM_EVENT_COIN_COLLECTED)
            return;

        



        if (m_status.coinsCollected == m_status.levels.at(m_status.currentLevel).coins)
        {
            m_status.coinsCollected = 0;
            unsigned int levelScore = static_cast<unsigned int>(1000.f / (m_status.currentTime + .1f));
            m_status.points += levelScore;
            m_status.currentTime = 0.f;


            ++m_status.currentLevel;
            if (m_status.currentLevel < m_status.levels.size())
                ConfigureNewLevel();
            else
            {


                // At the moment, MonaEngine does not have a cleaner way to close the app.
                exit(EXIT_SUCCESS);
            }
        }
    }

    void SetPlayerPtr(Player* playerPtr)
    {
        m_playerPtr = playerPtr;
    }

    glm::vec3 checkPlayerPos(Mona::World& world, Player* playerPtr) {
        auto playerTransform = world.GetComponentHandle<Mona::TransformComponent>(*playerPtr);
        glm::vec3 playerLocation = playerTransform->GetLocalTranslation();
        return playerLocation;
    }

    void ConfigureNewLevel()
    {
        const HighLevelGame::Level& level = m_status.levels.at(m_status.currentLevel);
        GenerateRandomCoins(40);
        GenerateRandomObstacles(40);
        

    }

    float getZ(float x, float y) {


        Mona::HeightMap* heightMap = meshHandle->GetHeightMap();

        float height = heightMap->getHeight(x, y);

        return height;
    }

private:

    void GenerateRandomCoins(unsigned int numberOfCoins)
    {
        if (m_worldPtr == nullptr)
            return;

        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_real_distribution<> dist(0, 60.f);

        for (int i = 0; i < numberOfCoins; ++i)
        {
            float x = dist(rng) + checkPlayerPos(*m_worldPtr, m_playerPtr)[0] - 30;
            float y = dist(rng) + checkPlayerPos(*m_worldPtr, m_playerPtr)[1] + 60;
            glm::vec3 coinLocation(x, y, getZ(x, y));

            auto coinHandle = m_worldPtr->CreateGameObject<Coin>();
            coinHandle->SetLocation(coinLocation);

            if (m_playerPtr == nullptr)
                return;

            Player& player = *m_playerPtr;
            coinHandle->SetPlayerPtr(&player);
        }
    }

    void GenerateRandomObstacles(unsigned int numberOfCoins)
    {
        if (m_worldPtr == nullptr)
            return;

        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_real_distribution<> dist(0, 60.f);

        for (int i = 0; i < numberOfCoins; ++i)
        {
            float x = dist(rng) + checkPlayerPos(*m_worldPtr, m_playerPtr)[0] - 30;
            float y = dist(rng) + checkPlayerPos(*m_worldPtr, m_playerPtr)[1] + 60;
            glm::vec3 coinLocation(x, y, getZ(x, y));

            auto coinHandle = m_worldPtr->CreateGameObject<Obstacle>();
            coinHandle->SetLocation(coinLocation);

            if (m_playerPtr == nullptr)
                return;

            Player& player = *m_playerPtr;
            coinHandle->SetPlayerPtr(&player);
        }
    }

    void GenerateRandomEnd(unsigned int numberOfCoins)
    {
        if (m_worldPtr == nullptr)
            return;

        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_real_distribution<> dist(0, 60.f);

        for (int i = 0; i < numberOfCoins; ++i)
        {
            float x = dist(rng) + checkPlayerPos(*m_worldPtr, m_playerPtr)[0] - 30;
            float y = dist(rng) + checkPlayerPos(*m_worldPtr, m_playerPtr)[1] + 300;
            glm::vec3 coinLocation(x, y, getZ(x, y));

            auto coinHandle = m_worldPtr->CreateGameObject<FinishLine>();
            coinHandle->SetLocation(coinLocation);

            if (m_playerPtr == nullptr)
                return;

            Player& player = *m_playerPtr;
            coinHandle->SetPlayerPtr(&player);
        }
    }

    Mona::World* m_worldPtr;
    Player* m_playerPtr;
    HighLevelGame m_status;
    Mona::SubscriptionHandle m_coinCollectedHandle;
    Mona::GameObjectHandle<Floor> floorHandle;
    Mona::StaticMeshHandle meshHandle;
    float x0;
    float y0;
    float xf;
    float yf;
};

class TopDownDemo : public Mona::Application
{
public:
    void UserStartUp(Mona::World& world) noexcept override
    {
        // Setting up our Game Objects
        playerHandle = world.CreateGameObject<Player>();
        world.SetGravity(glm::vec3(0.0f, 0.0f, 0.0f));
        world.SetAmbientLight(glm::vec3(0.7f, 0.7f, 0.7f));
        world.SetBackgroundColor(0.5f, 0.5f, 1, 1);



        // We will use a reference so selected game objects can access player information
        Player& player = *playerHandle;


        auto floorHandle = world.CreateGameObject<Floor>();
        //Floor& floor = *floorHandle;
        meshHandle = floorHandle->getMesh();
        

        // This game object is able to spawn more game objects, amazing!
        auto levelProgressionHandle = world.CreateGameObject<LevelProgression>();
        levelProgressionHandle->SetPlayerPtr(&player);
        levelProgressionHandle->ConfigureNewLevel();

        auto cameraHandle = world.CreateGameObject<TopDownCamera>();
        cameraHandle->SetPlayerPtr(&player);

        // Setting up lighting
        //auto directionalLight = world.AddComponent<Mona::DirectionalLightComponent>(cameraHandle, glm::vec3(1.0f));

        //directionalLight->SetLightColor(glm::vec3(1.0f, 1.0f, 1.0f));
    }

    void UserShutDown(Mona::World& world) noexcept override
    {
        spdlog::info("Bye!");
    }

    void UserUpdate(Mona::World& world, float timeStep) noexcept override
    {
        auto& input = world.GetInput();
        auto& window = world.GetWindow();

        auto playerTransform = world.GetComponentHandle<Mona::TransformComponent>(playerHandle);
        glm::vec3 playerLocation = playerTransform->GetLocalTranslation();
        playerHandle->setTransform(glm::vec3(playerLocation[0], playerLocation[1], getZ(world)));

        if (input.IsKeyPressed(MONA_KEY_ESCAPE))
        {
            //window.ShutDown();
            // At the moment, MonaEngine does not have a cleaner way to close the app.
            exit(EXIT_SUCCESS);
        }
    }

    float getZ(Mona::World& world) {


        Mona::HeightMap* heightMap = meshHandle->GetHeightMap();

        auto playerTransform = world.GetComponentHandle<Mona::TransformComponent>(playerHandle);

        glm::vec3 playerLocation = playerTransform->GetLocalTranslation();

        float height = heightMap->getHeight(playerLocation[0], playerLocation[1]);

        return height;
    }

private:
    Mona::GameObjectHandle<Player> playerHandle;
    Player* player;
    Mona::GameObjectHandle<Floor> floorHandle;
    Mona::StaticMeshHandle meshHandle;


};

int main()
{
    spdlog::info("Top Down Demo");
    TopDownDemo app;
    Mona::Engine engine(app);
    engine.StartMainLoop();
}