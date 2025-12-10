#include "collisionDetection/collisionManager.hpp"

CollisionManager::CollisionManager() = default;

CollisionManager::~CollisionManager(){
    shutdown();
}

void CollisionManager::init(){
    mCollisionConfig = std::make_unique<btDefaultCollisionConfiguration>();
    mDispatcher = std::make_unique<btCollisionDispatcher>(mCollisionConfig.get());
    mBroadphase = std::make_unique<btDbvtBroadphase>();
    mSolver = std::make_unique<btSequentialImpulseConstraintSolver>();

    mDynamicsWorld = std::make_unique<btDiscreteDynamicsWorld>(
        mDispatcher.get(),
        mBroadphase.get(),
        mSolver.get(),
        mCollisionConfig.get()
    );

    mDynamicsWorld->setGravity(btVector3(0.0f, 0.0f, 0.0f));

    btGImpactCollisionAlgorithm::registerAlgorithm(mDispatcher.get());
}

void CollisionManager::shutdown(){
    for(auto& obj : mObjects){
        if(obj->body){
            mDynamicsWorld->removeCollisionObject(obj->body);
        }
    }

    mObjects.clear();
    mShapes.clear();
    mTriangleMeshes.clear();

    mDynamicsWorld.reset();
    mSolver.reset();
    mBroadphase.reset();
    mDispatcher.reset();
    mCollisionConfig.reset();
}

void CollisionManager::setPlayerBody(CollisionObject* playerObj){
    mPlayerObj = playerObj;
    if(mPlayerObj) mPlayerObj->body->setActivationState(DISABLE_DEACTIVATION);
}

void CollisionManager::setMovementMode(MovementMode mode){
    if(mCurrentMode == mode || !mPlayerObj) return;

    mCurrentMode = mode;

    switch(mode){
        case MovementMode::FLOATING:
            setupFloatingMode();
            break;
        case MovementMode::WALKING:
            setupWalkingMode();
            break;
    }
}

void CollisionManager::setupFloatingMode(){
    mDynamicsWorld->setGravity(btVector3(0.0f, 0.0f, 0.0f));

    mPlayerObj->body->setLinearFactor(btVector3(1, 1, 1)); // movementon all axis
    mPlayerObj->body->setAngularFactor(btVector3(0, 0, 0)); // rotation on all axis (well for now nothing as it makes you fall over...)
    mPlayerObj->body->setDamping(0.5f, 0.8f); // linear and angular damping

    mPlayerObj->body->clearForces();
    mPlayerObj->body->setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
    mPlayerObj->body->setAngularVelocity(btVector3(0.0f, 0.0f, 0.0f));
}

void CollisionManager::setupWalkingMode(){
    mDynamicsWorld->setGravity(btVector3(0.0f, -9.81f, 0.0f));

    mPlayerObj->body->setLinearFactor(btVector3(1, 1, 1));
    mPlayerObj->body->setAngularFactor(btVector3(0, 0, 0)); // <- this took days to find, even movement on Y needs to be locked. ffs.
    mPlayerObj->body->setDamping(0.1f, 0.0f); // 0 damping to have responsive controls

    mPlayerObj->body->setAngularVelocity(btVector3(0.0f, 0.0f, 0.0f));
}

void CollisionManager::updateFloatingMovement(const glm::vec3& thrust, const glm::vec3& torque, float dt){
    if(!mPlayerObj) return;

    mPlayerObj->body->applyCentralForce(btVector3(thrust.x, thrust.y, thrust.z));
    mPlayerObj->body->applyTorque(btVector3(torque.x, torque.y, torque.z));
}

void CollisionManager::updateWalkingMovement(const glm::vec3& movement, float dt){
    if(!mPlayerObj) return;

    // bool onGround = checkIfOnGround();
    
    // if (onGround) {
    //     // On ground: use horizontal movement, cancel vertical velocity
    //     btVector3 currentVel = mPlayerObj->body->getLinearVelocity();
    //     btVector3 newVel(movement.x, 0.0f, movement.z); // No Y movement on ground
    //     mPlayerObj->body->setLinearVelocity(newVel);
        
    //     // Apply small upward force to stay on ground without bouncing
    //     // mPlayerObj->body->applyCentralForce(btVector3(0, 5.0f, 0));
    // } else {
    //     // In air: allow gravity to work normally
    //     btVector3 currentVel = mPlayerObj->body->getLinearVelocity();
    //     btVector3 newVel(movement.x, currentVel.y(), movement.z);
    //     mPlayerObj->body->setLinearVelocity(newVel);
    // }
    btVector3 currentVel = mPlayerObj->body->getLinearVelocity();
    btVector3 newVel(movement.x, currentVel.y(), movement.z);
    mPlayerObj->body->setLinearVelocity(newVel);
}

bool CollisionManager::checkIfOnGround(){
    // Raycast downward to see if we're close to ground
    glm::vec3 playerPos;
    glm::quat rotation;
    getObjectTransform(mPlayerObj, playerPos, rotation);

    glm::vec3 rayStart = playerPos;
    float playerHalfHeight = 0.8 / 2 + 0.2; // (height / 2 + 1xRadius)
    glm::vec3 rayEnd = playerPos + glm::vec3(0, -playerHalfHeight-0.01, 0); // Slightly below capsule
    
    auto result = raycast(rayStart, rayEnd, "");
    return result.hit && result.distance < 5.0f; // Bro fuck, idk, always is 3 if something is hit... shit sucks.
}

void CollisionManager::updatePlayerMovement(const glm::vec3& input, const glm::vec3& rotationInput, float dt){
    if(!mPlayerObj) return;

    switch(mCurrentMode){
        case MovementMode::FLOATING: {
            glm::vec3 thrust = input * mThrusterPower;
            glm::vec3 torque = rotationInput * mRotationPower;
            updateFloatingMovement(thrust, torque, dt);
            break;
        }
        case MovementMode::WALKING: {
            glm::vec3 movement = input * mWalkSpeed;
            updateWalkingMovement(movement, dt);
            break;
        }
    }
}

CollisionObject* CollisionManager::createSphere(float radius, const glm::vec3& position, bool collision, float mass, const std::string& tag){
    auto shape = std::make_unique<btSphereShape>(radius);
    auto* shapePtr = shape.get();
    mShapes.push_back(std::move(shape));

    btRigidBody* body = createRigidBody(shapePtr, position, mass);
    if(!collision)
        body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);

    auto obj = std::make_unique<CollisionObject>();
    obj->body = body;
    obj->tag = tag;
    obj->userData = nullptr;

    auto objPtr = obj.get();
    mObjects.push_back(std::move(obj));
    mDynamicsWorld->addRigidBody(body);

    return objPtr;
}

CollisionObject* CollisionManager::createBox(const glm::vec3& halfExtents, const glm::vec3& position,
                                            bool collision, float mass, const std::string& tag){
    auto shape = std::make_unique<btBoxShape>(btVector3(halfExtents.x, halfExtents.y, halfExtents.z));
    shape->setMargin(0.05f);
    auto shapePtr = shape.get();
    mShapes.push_back(std::move(shape));

    btRigidBody* body = createRigidBody(shapePtr, position, mass);
    if(!collision)
        body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
    
    body->setRestitution(0.0f);
    body->setFriction(1.0f);
    body->setRollingFriction(1.0f);
    body->setSpinningFriction(1.0f);

    auto obj = std::make_unique<CollisionObject>();
    obj->body = body;
    obj->tag = tag;
    obj->userData = nullptr;

    auto objPtr = obj.get();
    mObjects.push_back(std::move(obj));
    mDynamicsWorld->addRigidBody(body);

    return objPtr;
}

CollisionObject* CollisionManager::createCapsule(float radius, float height, const glm::vec3& position,
                                                bool collision, float mass, const std::string& tag) {
    auto shape = std::make_unique<btCapsuleShape>(radius, height);
    shape->setMargin(0.05f);
    auto* shapePtr = shape.get();
    mShapes.push_back(std::move(shape));

    btRigidBody* body = createRigidBody(shapePtr, position, mass);
    if(!collision)
        body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);

    body->setRestitution(0.0f);
    body->setFriction(1.0f);
    body->setRollingFriction(1.0f);
    body->setSpinningFriction(1.0f);
    
    auto obj = std::make_unique<CollisionObject>();
    obj->body = body;
    obj->tag = tag;
    obj->userData = nullptr;
    
    auto* objPtr = obj.get();
    mObjects.push_back(std::move(obj));
    mDynamicsWorld->addRigidBody(body);
    
    return objPtr;
}

CollisionObject* CollisionManager::createStaticTrimesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, 
                                                       const glm::vec3& position, bool collision, const std::string& tag){
    auto triangleMesh = std::make_unique<btTriangleMesh>();
    
    // Add triangles to the mesh
    for (size_t i = 0; i < indices.size(); i += 3) {
        unsigned int i0 = indices[i] * 3;
        unsigned int i1 = indices[i + 1] * 3;
        unsigned int i2 = indices[i + 2] * 3;
        
        btVector3 v0(vertices[i0], vertices[i0 + 1], vertices[i0 + 2]);
        btVector3 v1(vertices[i1], vertices[i1 + 1], vertices[i1 + 2]);
        btVector3 v2(vertices[i2], vertices[i2 + 1], vertices[i2 + 2]);
        
        triangleMesh->addTriangle(v0, v1, v2);
    }
    
    auto* meshPtr = triangleMesh.get();
    mTriangleMeshes.push_back(std::move(triangleMesh));
    
    // Use BvhTriangleMeshShape for static trimesh (optimized)
    auto shape = std::make_unique<btBvhTriangleMeshShape>(meshPtr, true);
    auto* shapePtr = shape.get();
    mShapes.push_back(std::move(shape));

    btRigidBody* body = createRigidBody(shapePtr, position, 0.0f); // Static = mass 0
    if(!collision)
        body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
    
    auto obj = std::make_unique<CollisionObject>();
    obj->body = body;
    obj->tag = tag;
    obj->userData = nullptr;
    
    auto* objPtr = obj.get();
    mObjects.push_back(std::move(obj));
    mDynamicsWorld->addRigidBody(body);
    
    return objPtr;
}

CollisionObject* CollisionManager::createDynamicTrimesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, 
                                                        const glm::vec3& position, bool collision, float mass, const std::string& tag){
    auto triangleMesh = std::make_unique<btTriangleMesh>();
    
    // Add triangles to the mesh
    for (size_t i = 0; i < indices.size(); i += 3) {
        unsigned int i0 = indices[i] * 3;
        unsigned int i1 = indices[i + 1] * 3;
        unsigned int i2 = indices[i + 2] * 3;
        
        btVector3 v0(vertices[i0], vertices[i0 + 1], vertices[i0 + 2]);
        btVector3 v1(vertices[i1], vertices[i1 + 1], vertices[i1 + 2]);
        btVector3 v2(vertices[i2], vertices[i2 + 1], vertices[i2 + 2]);
        
        triangleMesh->addTriangle(v0, v1, v2);
    }
    
    auto* meshPtr = triangleMesh.get();
    mTriangleMeshes.push_back(std::move(triangleMesh));
    
    // GImpactMeshShape for dynamic trimesh
    auto shape = std::make_unique<btGImpactMeshShape>(meshPtr);
    shape->updateBound();
    auto* shapePtr = shape.get();
    mShapes.push_back(std::move(shape));

    btRigidBody* body = createRigidBody(shapePtr, position, mass);
    if(!collision)
        body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
    
    auto obj = std::make_unique<CollisionObject>();
    obj->body = body;
    obj->tag = tag;
    obj->userData = nullptr;
    
    auto* objPtr = obj.get();
    mObjects.push_back(std::move(obj));
    mDynamicsWorld->addRigidBody(body);
    
    return objPtr;
}

btRigidBody* CollisionManager::createRigidBody(btCollisionShape* shape, const glm::vec3& position, float mass){
    btTransform transform;
    transform.setIdentity();
    transform.setOrigin(btVector3(position.x, position.y, position.z));

    btVector3 localIntertia(0, 0, 0);
    if(mass != 0.0f){
        shape->calculateLocalInertia(mass, localIntertia);
    }

    auto motionState = std::make_unique<btDefaultMotionState>(transform);
    auto* motionStatePtr = motionState.get();
    mMotionStates.push_back(std::move(motionState));

    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionStatePtr, shape, localIntertia);
    return new btRigidBody(rbInfo);
}

void CollisionManager::removeObject(CollisionObject* obj){
    if(!obj || !obj->body) return;

    mDynamicsWorld->removeCollisionObject(obj->body);

    auto it = std::find_if(mObjects.begin(), mObjects.end(), [obj](const std::unique_ptr<CollisionObject>& ptr)
                            {return ptr.get() == obj;});

    if(it != mObjects.end()){
        mObjects.erase(it);
    }
}

bool CollisionManager::checkCollision(CollisionObject* objA, CollisionObject* objB,
                                      glm::vec3& outNormal, glm::vec3& outPoint, float& outPenetration)
{
    if (!objA || !objA->body || !objB || !objB->body)
        return false;

    struct CustomContactResultCallback : public btCollisionWorld::ContactResultCallback {
        bool hitDetected = false;
        btVector3 hitNormalWorld;
        btVector3 hitPointWorld;
        btScalar penetrationDepth = 0.0f;

        btScalar addSingleResult(btManifoldPoint& cp,
                                 const btCollisionObjectWrapper*, int, int,
                                 const btCollisionObjectWrapper*, int, int) override {
            hitDetected = true;
            hitNormalWorld = cp.m_normalWorldOnB;
            hitPointWorld = cp.m_positionWorldOnB;
            penetrationDepth = cp.getDistance(); // negative = overlap
            return 0;
        }
    };

    CustomContactResultCallback callback;
    mDynamicsWorld->contactPairTest(objA->body, objB->body, callback);

    if (!callback.hitDetected)
        return false;

    outNormal = glm::vec3(callback.hitNormalWorld.x(), callback.hitNormalWorld.y(), callback.hitNormalWorld.z());
    outPoint = glm::vec3(callback.hitPointWorld.x(), callback.hitPointWorld.y(), callback.hitPointWorld.z());
    outPenetration = -callback.penetrationDepth; // make positive

    return true;
}

RaycastResult CollisionManager::raycast(const glm::vec3& from, const glm::vec3& to, const std::string& tagFilter){
    RaycastResult result;
    result.hit = false;
    result.obj = nullptr;
    result.distance = std::numeric_limits<float>::max();

    btVector3 rayFrom(from.x, from.y, from.z);
    btVector3 rayTo(to.x, to.y, to.z);

    btCollisionWorld::ClosestRayResultCallback rayCallback(rayFrom, rayTo);

    mDynamicsWorld->rayTest(rayFrom, rayTo, rayCallback);

    if(rayCallback.hasHit()){
        result.hit = true;
        result.position = glm::vec3(rayCallback.m_hitPointWorld.x(), rayCallback.m_hitPointWorld.y(), rayCallback.m_hitPointWorld.z());
        result.normal = glm::vec3(rayCallback.m_hitNormalWorld.x(), rayCallback.m_hitNormalWorld.y(), rayCallback.m_hitNormalWorld.z());
        result.distance = glm::length(result.position - from);

        for(auto& obj : mObjects){
            if(obj->body == rayCallback.m_collisionObject){
                result.obj = obj.get();
                break;
            }
        }
    }

    if(result.obj && !tagFilter.empty()){
        if(result.obj->tag == tagFilter)
            return result;
        else {
            RaycastResult empty;
            empty.hit = false;
            return empty;
        }
    }

    return result;
}

void CollisionManager::update(float dt){
    // For now not needed as all I have is static
    mDynamicsWorld->getSolverInfo().m_numIterations = 20;
    mDynamicsWorld->stepSimulation(dt, 10);

    checkCollisions();
}

void CollisionManager::checkCollisions(){
    if(!mCollisionCallback) return;

    for (size_t i = 0; i < mObjects.size(); ++i) {
        for (size_t j = i + 1; j < mObjects.size(); ++j) {
            glm::vec3 normal, point;
            float penetration = 0.0f;

            if (checkCollision(mObjects[i].get(), mObjects[j].get(), normal, point, penetration)) {
                mCollisionCallback(mObjects[i].get(), mObjects[j].get(), normal, point, penetration);
            }
        }
    }
}

// void CollisionManager::setCollisionCallback(std::function<void(CollisionObject*, CollisionObject*)> callback){
//     mCollisionCallback = callback;
// }

void CollisionManager::setCollisionCallback(std::function<void(
                CollisionObject*, CollisionObject*, glm::vec3&, glm::vec3&, float)> callback){
    mCollisionCallback = callback;
}

void CollisionManager::setObjectTransform(CollisionObject* obj, const glm::vec3& position, const glm::quat& rotation){
    if(!obj || !obj->body) return;

    btTransform transform;
    transform.setOrigin(btVector3(position.x, position.y, position.z));
    transform.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w));

    obj->body->setWorldTransform(transform);

    if(obj->body->getInternalType() == btCollisionObject::CO_RIGID_BODY){
        btRigidBody* rigidBody = btRigidBody::upcast(obj->body);
        if(rigidBody && rigidBody->getMotionState()){
            rigidBody->getMotionState()->setWorldTransform(transform);
        }
    }
}

void CollisionManager::getObjectTransform(CollisionObject* obj, glm::vec3& position, glm::quat& rotation) const{
    if(!obj || !obj->body) return;

    const btTransform& transform = obj->body->getWorldTransform();
    const btVector3& origin = transform.getOrigin();
    const btQuaternion& rot = transform.getRotation();

    // std::cout << origin.x() << " | " << origin.y() << " | " << origin.z() << std::endl;

    position = glm::vec3(origin.x(), origin.y(), origin.z());
    rotation = glm::quat(rot.w(), rot.x(), rot.y(), rot.z());
}