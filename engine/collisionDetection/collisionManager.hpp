#pragma once

// C++ STL
#include <vector>
#include <unordered_map>
#include <memory>
#include <iostream>
#include <functional>

// 3rd party libs
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

struct CollisionObject{
    btRigidBody* body;
    std::string tag = "";
    void* userData;
};

enum class MovementMode{
    FLOATING,
    WALKING
};

struct RaycastResult{
    bool hit = false;
    glm::vec3 position;
    glm::vec3 normal;
    CollisionObject* obj;
    float distance;
};

class CollisionManager{
    public:
        CollisionManager();
        ~CollisionManager();

        void init();
        void shutdown();

        void update(float dt);

        CollisionObject* createSphere(float radius, const glm::vec3& position, bool collision = true,
                                            float mass = 0.0f, const std::string& tag = "");
        CollisionObject* createBox(const glm::vec3& halfExtent, const glm::vec3& position, bool collision = true,
                                            float mass = 0.0f, const std::string& tag = "");
        CollisionObject* createCapsule(float radius, float height, const glm::vec3& position, bool collision = true,
                                            float mass = 0.0f, const std::string& tag = "");

        CollisionObject* createStaticTrimesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices,
                                            const glm::vec3& position, bool collision = true, const std::string& tag = "");
        CollisionObject* createDynamicTrimesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices,
                                            const glm::vec3& position, bool collision = true, float mass = 1.0f, const std::string& tag = "");

        void removeObject(CollisionObject* obj);

        // transform stuff
        void setObjectTransform(CollisionObject* obj, const glm::vec3& position, const glm::quat& rotation);
        void getObjectTransform(CollisionObject* obj, glm::vec3& position, glm::quat& rotation) const;

        bool checkCollision(CollisionObject* objA, CollisionObject* objB,
                            glm::vec3& outNormal, glm::vec3& outPoint, float& outPenetration);
        bool checkCollision(const std::string& tagA, const std::string& tagB);
        std::vector<CollisionObject*> getCollisions(CollisionObject* obj);
        std::vector<CollisionObject*> getCollisionsWithTag(const std::string& tag);

        RaycastResult raycast(const glm::vec3& from, const glm::vec3& to, const std::string& tagFilter = "");
        std::vector<RaycastResult> raycastAll(const glm::vec3& from, const glm::vec3& to, const std::string& tagFilter = "");

        // void setCollisionCallback(std::function<void(CollisionObject*, CollisionObject*)> callback);
        void setCollisionCallback(std::function<void(CollisionObject*, CollisionObject*,
                                  glm::vec3& normal, glm::vec3& point, float penetration)> callback);

        // Player related functions:
        void setPlayerBody(CollisionObject* playerObj);
        void setMovementMode(MovementMode mode);
        MovementMode getCurrentMode() const {return mCurrentMode;}
        
        void updatePlayerMovement(const glm::vec3& input, const glm::vec3& rotationInput, float dt);

    private:
        std::unique_ptr<btDefaultCollisionConfiguration> mCollisionConfig;
        std::unique_ptr<btCollisionDispatcher> mDispatcher;
        std::unique_ptr<btBroadphaseInterface> mBroadphase;
        std::unique_ptr<btDynamicsWorld> mDynamicsWorld;
        std::unique_ptr<btConstraintSolver> mSolver;

        std::vector<std::unique_ptr<CollisionObject>> mObjects;
        std::vector<std::unique_ptr<btCollisionShape>> mShapes;
        std::vector<std::unique_ptr<btTriangleMesh>> mTriangleMeshes;
        std::vector<std::unique_ptr<btMotionState>> mMotionStates;

        // std::function<void(CollisionObject*, CollisionObject*)> mCollisionCallback;
        std::function<void(CollisionObject*, CollisionObject*, glm::vec3&, glm::vec3&, float)> mCollisionCallback;

        btRigidBody* createRigidBody(btCollisionShape* shape, const glm::vec3& position, float mass);
        void checkCollisions();

        // Player related stuff:
        MovementMode mCurrentMode = MovementMode::FLOATING;
        CollisionObject* mPlayerObj = nullptr;
        float mWalkSpeed = 5.0f;
        float mThrusterPower = 10.0f;
        float mRotationPower = 2.0f;

        void setupFloatingMode();
        void setupWalkingMode();
        void updateFloatingMovement(const glm::vec3& thrust, const glm::vec3& torque, float dt);
        void updateWalkingMovement(const glm::vec3& movement, float dt);
        bool checkIfOnGround();
};