//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#ifndef _SCENE_OBJECT_H_
#define _SCENE_OBJECT_H_

#ifndef _SCENE_H_
#include "2d/scene/Scene.h"
#endif

#ifndef _DEBUG_STATS_H_
#include "2d/scene/DebugStats.h"
#endif

#ifndef _SIMBASE_H_
#include "sim/simBase.h"
#endif

#ifndef _MMATH_H_
#include "math/mMath.h"
#endif

#ifndef _SCENE_WINDOW_H_
#include "2d/gui/SceneWindow.h"
#endif

#ifndef _BATCH_RENDER_H_
#include "2d/core/BatchRender.h"
#endif

#ifndef _UTILITY_H_
#include "2d/core/Utility.h"
#endif

#ifndef _SCENE_OBJECT_GROUP_H_Fs
#include "2d/sceneobject/SceneObjectGroup.h"
#endif

#ifndef _PHYSICS_PROXY_H_
#include "2d/scene/PhysicsProxy.h"
#endif

#ifndef _SCENE_RENDER_OBJECT_H_
#include "2d/scene/SceneRenderObject.h"
#endif

#ifndef _BEHAVIOR_COMPONENT_H_
#include "component/behaviors/behaviorComponent.h"
#endif

#ifndef _BEHAVIORINSTANCE_H_
#include "component/behaviors/behaviorInstance.h"
#endif

//-----------------------------------------------------------------------------

extern EnumTable bodyTypeTable;
extern b2BodyType getBodyTypeEnum(const char* label);
extern const char* getBodyTypeDescription(const b2BodyType bodyType);
extern b2Shape::Type getCollisionShapeTypeEnum(const char* label);
extern S32 getSrcBlendFactorEnum(const char* label);
extern S32 getDstBlendFactorEnum(const char* label);
extern const char* getSrcBlendFactorDescription(const GLenum factor);
extern const char* getDstBlendFactorDescription(const GLenum factor);
extern EnumTable srcBlendFactorTable;
extern EnumTable dstBlendFactorTable;
extern const char* getBodyTypeDescription(const b2BodyType bodyType);
extern const char* getCollisionShapeTypeDescription(const b2Shape::Type collisionShapeType);

//-----------------------------------------------------------------------------

struct tDestroyNotification
{
    SceneObject*     mpSceneObject;
    U32                 mRefCount;
};

//-----------------------------------------------------------------------------

typedef VectorPtr<b2FixtureDef*> typeCollisionFixtureDefVector;
typedef VectorPtr<b2Fixture*> typeCollisionFixtureVector;
typedef Vector<tDestroyNotification> typeDestroyNotificationVector;

//-----------------------------------------------------------------------------

const S32 GL_INVALID_BLEND_FACTOR = -1;

//-----------------------------------------------------------------------------

class SceneObject :
    public BehaviorComponent,
    public SceneRenderObject,
    public PhysicsProxy
{

private:
    typedef BehaviorComponent Parent;

public:
    friend class Scene;
    friend class SceneWindow;
    friend class SceneObjectGroup;
    friend class ContactFilter;
    friend class WorldQuery;
    friend class DebugDraw;
    friend class SceneObjectMoveToEvent;
    friend class SceneObjectRotateToEvent;

protected:
    /// Scene.
    SimObjectPtr<Scene>  mpScene;

    /// Target Scene.
    /// NOTE:   Unfortunately this is required as the scene can be set via a field which
    ///         occurs before the object is registered with the simulation therefore
    ///         any simulation related functionality cannot be used such as TorqueScript
    ///         callbacks.
    SimObjectPtr<Scene>     mpTargetScene;

    /// Lifetime.
    F32                     mLifetime;
    bool                    mLifetimeActive;
    
    /// Scene layers.
    U32                     mSceneLayer;
    U32                     mSceneLayerMask;
    F32                     mSceneLayerDepth;

    /// Scene groups.
    U32                     mSceneGroup;
    U32                     mSceneGroupMask;

    /// Area.
    Vector2                 mSize;
    bool                    mAutoSizing;
    b2AABB                  mPreTickAABB;
    b2AABB                  mCurrentAABB;
    Vector2                 mRenderOOBB[4];
    Vector2                 mLocalSizeVertices[4];
    S32                     mWorldProxyId;

    /// Position / Angle.
    Vector2                 mPreTickPosition;
    F32                     mPreTickAngle;
    Vector2                 mRenderPosition;
    F32                     mRenderAngle;
    bool                    mSpatialDirty;

    /// Body.
    b2Body*                 mpBody;
    b2BodyDef               mBodyDefinition;
    U32                     mWorldQueryKey;
    F32                     mAngularVelocityTolerance;
    F32                     mLinearVelocityTolerance;

    /// Collision control.
    U32                     mCollisionLayerMask;
    U32                     mCollisionGroupMask;
    bool                    mCollisionSuppress;
    b2FixtureDef            mDefaultFixture;
    bool                    mGatherContacts;
    typeContactVector*      mpCurrentContacts;

    /// General collision shape access.
    typeCollisionFixtureDefVector mCollisionFixtureDefs;
    typeCollisionFixtureVector mCollisionFixtures;

    /// Render visibility.
    bool                    mVisible;

    /// Render flipping.
    bool                    mFlipX;
    bool                    mFlipY;

    /// Render blending.
    bool                    mBlendMode;
    S32                     mSrcBlendFactor;
    S32                     mDstBlendFactor;
    ColorF                  mBlendColor;
    F32                     mAlphaTest;

    /// Render sorting.
    Vector2                 mSortPoint;

    /// Input events.
    bool                    mUseInputEvents;

    /// Script callbacks.
    bool                    mUpdateCallback;
    bool                    mCollisionCallback;
    bool                    mSleepingCallback;
    bool                    mLastAwakeState;

    /// Debug mode.
    U32                     mDebugMask;

    /// Camera mounting.
    SceneWindow*            mpAttachedCamera;

    /// GUI attachment.
    bool                    mAttachedGuiSizeControl;
    GuiControl*             mpAttachedGui;
    SceneWindow*            mpAttachedGuiSceneWindow;

    /// Pathing.
    SimObjectPtr<SceneObject> mAttachedToPath;

    /// Safe deletion.
    bool                    mBeingSafeDeleted;
    bool                    mSafeDeleteReady;

    /// Destroy notifications.
    typeDestroyNotificationVector mDestroyNotifyList;

    /// Miscellaneous.
    bool                    mBatchIsolated;
    bool                    mEnableChild;
    U32                     mSerialiseKey;
    SceneObjectGroup*       mpSceneObjectGroup;
    bool                    mEditorTickAllowed;
    bool                    mPickingAllowed;
    bool                    mAlwaysInScope;
    S32                     mPeriodicTimerID;
    U32                     mMoveToEventId;
    U32                     mRotateToEventId;
    U32                     mSerialId;

private:
    // Field Names.
    StringTableEntry        SCENEOBJECT_COLLISIONSHAPE_FIELDNAME;


    void                    ScaleFixtureDef( b2FixtureDef* pFixtureDef, const Vector2& size );

protected:
    static S32 QSORT_CALLBACK sceneObjectLayerDepthSort(const void* a, const void* b);


    /// Scene (un)registering.
    virtual void            OnRegisterScene( Scene* pScene );
    virtual void            OnUnregisterScene( Scene* pScene );

    /// Ticking.
    void                    resetTickSpatials( const bool resize = false );
    inline bool             getSpatialDirty( void ) const { return mSpatialDirty; }

    /// Contact processing.
    void                    initializeContactGathering( void );

    /// Taml callbacks.
    virtual void            onTamlPreWrite( void );
    virtual void            onTamlPostWrite( void );
    virtual void            onTamlPreRead( void );
    virtual void            onTamlPostRead( const TamlCollection& customCollection );
    virtual void            onTamlCustomWrite( TamlCollection& customCollection );
    virtual void            onTamlCustomRead( const TamlCollection& customCollection );

    /// Prefab.
    virtual bool            synchronizePrefab( void );

public:
    SceneObject();
    virtual ~SceneObject();

    /// Engine.
    virtual bool            onAdd();
    virtual void            onRemove();
    virtual void            onDestroyNotify( SceneObject* pSceneObject );
    static void             initPersistFields();

    /// Integration.
    virtual void            preIntegrate( const F32 totalTime, const F32 elapsedTime, DebugStats* pDebugStats );
    virtual void            integrateObject( const F32 totalTime, const F32 elapsedTime, DebugStats* pDebugStats );
    virtual void            postIntegrate(const F32 totalTime, const F32 elapsedTime, DebugStats *pDebugStats);
    virtual void            interpolateObject( const F32 timeDelta );
    inline bool             getIsEditorTickAllowed( void ) const { return mEditorTickAllowed; }

    /// Render batching.
    inline void             setBatchIsolated( const bool batchIsolated ) { mBatchIsolated = batchIsolated; }
    virtual bool            getBatchIsolated( void ) { return mBatchIsolated; }
    virtual bool            isBatchRendered( void ) { return true; }
    virtual bool            canRender( void ) const { return true; }
    
    /// Render Output.
    virtual bool            canPrepareRender( void ) const { return false; }
    virtual void            scenePrepareRender( const SceneRenderState* pSceneRenderState, SceneRenderQueue* pSceneRenderQueue ) {}
    virtual void            sceneRender( const SceneRenderState* pSceneRenderState, const SceneRenderRequest* pSceneRenderRequest, BatchRender* pBatchRenderer ) {}
    virtual void            sceneRenderFallback( const SceneRenderState* pSceneRenderState, const SceneRenderRequest* pSceneRenderRequest, BatchRender* pBatchRenderer );
    virtual void            sceneRenderOverlay( const SceneRenderState* pSceneRenderState );

    /// Networking.
    virtual U32             packUpdate(NetConnection * conn, U32 mask, BitStream *stream);
    virtual void            unpackUpdate(NetConnection * conn, BitStream *stream);

    /// Scene.
    inline Scene* const     getScene( void ) const                      { return mpScene; }
    inline F32              getSceneTime( void ) const                  { if ( mpScene ) return mpScene->getSceneTime(); else return 0.0f; }

    /// Enabled.
    virtual void            setEnabled( const bool enabled );

    /// Lifetime.
    void                    setLifetime( const F32 lifetime );
    inline F32              getLifetime( void ) const                   { return mLifetime; }
    void                    updateLifetime( const F32 elapsedTime );

    /// Scene layers.
    void                    setSceneLayer( const U32 sceneLayer );
    inline U32              getSceneLayer(void) const                   { return mSceneLayer; }
    inline U32              getSceneLayerMask( void ) const             { return mSceneLayerMask; }

    /// Scene Layer depth.
    inline void             setSceneLayerDepth( const F32 order )       { mSceneLayerDepth = order; };
    inline F32              getSceneLayerDepth( void ) const            { return mSceneLayerDepth; }
    bool                    setSceneLayerDepthFront( void );
    bool                    setSceneLayerDepthBack( void );
    bool                    setSceneLayerDepthForward( void );
    bool                    setSceneLayerDepthBackward( void );

    /// Scene groups.
    void                    setSceneGroup( const U32 sceneGroup );
    inline U32              getSceneGroup(void ) const                  { return mSceneGroup; }
    inline U32              getSceneGroupMask( void ) const             { return mSceneGroupMask; }

    /// Area.
    virtual void            setArea( const Vector2& corner1, const Vector2& corner2 );
    inline bool             getAutoSizing( void ) const                 { return mAutoSizing; }
    virtual void            setSize( const Vector2& size );
    inline void             setSize( const F32 width, const F32 height ){ setSize( Vector2(width, height) ); }
    inline Vector2          getSize( void ) const                       { return mSize; }
    inline Vector2          getHalfSize( void ) const                   { return mSize * 0.5f; }
    inline b2AABB           getAABB( void ) const                       { return mCurrentAABB; }
    inline RectF            getAABBRectangle( void ) const              { const b2Vec2 size = mCurrentAABB.upperBound-mCurrentAABB.lowerBound; return RectF( mCurrentAABB.lowerBound.x, mCurrentAABB.lowerBound.y, size.x, size.y ); }
    inline S32              getWorldProxy( void ) const                 { return mWorldProxyId; }

    /// Position / Angle.
    virtual void            setPosition( const Vector2& position );
    inline Vector2          getPosition(void) const                     { if ( mpScene ) return mpBody->GetPosition(); else return mBodyDefinition.position; }
    inline Vector2          getRenderPosition(void) const               { return mRenderPosition; }
    inline F32              getRenderAngle(void) const                  { return mRenderAngle; }
    inline const b2Vec2*    getRenderOOBB(void) const                   { return mRenderOOBB; }
    inline const b2Vec2*    getLocalSizeVertices( void ) const          { return mLocalSizeVertices; }
    virtual void            setAngle( const F32 radians );
    inline F32              getAngle(void) const                        { if ( mpScene ) return mpBody->GetAngle(); else return mBodyDefinition.angle; }
    virtual void            setFixedAngle( const bool fixed )           { if ( mpScene ) mpBody->SetFixedRotation( fixed ); else mBodyDefinition.fixedRotation = fixed; }
    inline bool             getFixedAngle(void) const                   { if ( mpScene ) return mpBody->IsFixedRotation(); else return mBodyDefinition.fixedRotation; }
    b2Transform             getTransform( void ) const                  { if ( mpScene ) return mpBody->GetTransform(); else return b2Transform( mBodyDefinition.position, b2Rot(mBodyDefinition.angle) ); }
    b2Transform             getRenderTransform( void ) const            { return b2Transform( getRenderPosition(), b2Rot( getRenderAngle()) ); }
    inline Vector2          getLocalCenter(void) const                  { if ( mpScene ) return mpBody->GetLocalCenter(); else return b2Vec2_zero; }
    inline Vector2          getWorldCenter(void) const                  { if ( mpScene ) return mpBody->GetWorldCenter(); else return mBodyDefinition.position; }
    Vector2                 getLocalPoint( const Vector2& worldPoint );
    Vector2                 getWorldPoint( const Vector2& localPoint );
    Vector2                 getLocalVector( const Vector2& worldVector );
    Vector2                 getWorldVector( const Vector2& localVector );
    bool                    getIsPointInOOBB( const Vector2& worldPoint );
    bool                    getIsPointInCollisionShape( const U32 shapeIndex, const Vector2& worldPoint );

    /// Body.
    virtual ePhysicsProxyType getPhysicsProxyType( void ) const         { return PhysicsProxy::PHYSIC_PROXY_SCENEOBJECT; }
    inline b2Body*          getBody( void ) const                       { return mpBody; }
    void                    setBodyType( const b2BodyType type );
    inline b2BodyType       getBodyType(void) const                     { if ( mpScene ) return mpBody->GetType(); else return mBodyDefinition.type; }
    inline void             setActive( const bool active )              { if ( mpScene ) mpBody->SetActive( active ); else mBodyDefinition.active = active; }
    inline bool             getActive(void) const                       { if ( mpScene ) return mpBody->IsActive(); else return mBodyDefinition.active; }
    inline void             setAwake( const bool awake )                { if ( mpScene ) mpBody->SetAwake( awake ); else mBodyDefinition.awake = awake; }
    inline bool             getAwake(void) const                        { if ( mpScene ) return mpBody->IsAwake(); else return mBodyDefinition.awake; }
    inline void             setBullet( const bool bullet )              { if ( mpScene ) mpBody->SetBullet( bullet ); else mBodyDefinition.bullet = bullet; }
    inline bool             getBullet(void) const                       { if ( mpScene ) return mpBody->IsBullet(); else return mBodyDefinition.bullet; }
    inline void             setSleepingAllowed( const bool allowed )    { if ( mpScene ) mpBody->SetSleepingAllowed( allowed ); else mBodyDefinition.allowSleep = allowed; }
    inline bool             getSleepingAllowed(void) const              { if ( mpScene ) return mpBody->IsSleepingAllowed(); else return mBodyDefinition.allowSleep; }
    inline F32              getMass( void ) const                       { if ( mpScene ) return mpBody->GetMass(); else return 0.0f; }
    inline F32              getInertia( void ) const                    { if ( mpScene ) return mpBody->GetInertia(); else return 0.0f; }

    /// Collision control.
    void                    setCollisionMasks( const U32 groupMask, const U32 layerMask = MASK_ALL );
    void                    setCollisionAgainst( const SceneObject* pSceneObject, const bool clearMasks );
    inline void             setCollisionLayers( const U32 layerMask )   { setCollisionMasks(getCollisionGroupMask(), layerMask); }
    inline void             setCollisionGroups( const U32 groupMask )   { setCollisionMasks(groupMask, getCollisionLayerMask()); }
    inline U32              getCollisionGroupMask(void) const           { return mCollisionGroupMask; }
    inline U32              getCollisionLayerMask(void) const           { return mCollisionLayerMask; }
    void                    setDefaultDensity( const F32 density, const bool updateShapes = true );
    inline F32              getDefaultDensity( void ) const             { return mDefaultFixture.density; }
    void                    setDefaultFriction( const F32 friction, const bool updateShapes = true );
    inline F32              getDefaultFriction( void ) const            { return mDefaultFixture.friction; }
    void                    setDefaultRestitution( const F32 restitution, const bool updateShapes = true );
    inline F32              getDefaultRestitution( void ) const         { return mDefaultFixture.restitution; }
    inline void             setAngularVelocityTolerance( const F32 tolerance ) { mAngularVelocityTolerance = tolerance; }
    inline F32              getAngularVelocityTolerance( void ) const   { return mAngularVelocityTolerance; }
    inline void             setLinearVelocityTolerance( const F32 tolerance )  { mLinearVelocityTolerance = tolerance; }
    inline F32              getLinearVelocityTolerance( void ) const    { return mLinearVelocityTolerance; }
    inline void             setCollisionSuppress( const bool status )   { mCollisionSuppress = status; }
    inline bool             getCollisionSuppress(void) const            { return mCollisionSuppress; }
    inline const typeContactVector* getCurrentContacts( void ) const    { return mpCurrentContacts; }
    inline U32              getCurrentContactCount( void ) const        { if ( mpCurrentContacts != NULL ) return mpCurrentContacts->size(); else return 0; }
    virtual void            setGatherContacts( const bool gatherContacts ) { mGatherContacts = gatherContacts; initializeContactGathering(); }
    inline bool             getGatherContacts( void ) const             { return mGatherContacts; }
    virtual void            onBeginCollision( const TickContact& tickContact );
    virtual void            onEndCollision( const TickContact& tickContact );

    /// Velocities.
    inline void             setLinearVelocity( const Vector2& velocity ) { if ( mpScene ) mpBody->SetLinearVelocity( velocity ); else mBodyDefinition.linearVelocity = velocity; }
    inline Vector2          getLinearVelocity(void) const               { if ( mpScene ) return mpBody->GetLinearVelocity(); else return mBodyDefinition.linearVelocity; }
    inline Vector2          getLinearVelocityFromWorldPoint( const Vector2& worldPoint ) { if ( mpScene ) return mpBody->GetLinearVelocityFromWorldPoint( worldPoint ); else return mBodyDefinition.linearVelocity; }
    inline Vector2          getLinearVelocityFromLocalPoint( const Vector2& localPoint ) { if ( mpScene ) return mpBody->GetLinearVelocityFromLocalPoint( localPoint ); else return mBodyDefinition.linearVelocity; }
    inline void             setAngularVelocity( const F32 velocity )    { if ( mpScene ) mpBody->SetAngularVelocity( velocity ); else mBodyDefinition.angularVelocity = velocity; }
    inline F32              getAngularVelocity(void) const              { if ( mpScene ) return mpBody->GetAngularVelocity(); else return mBodyDefinition.angularVelocity; }
    inline void             setLinearDamping( const F32 damping )       { if ( mpScene ) mpBody->SetLinearDamping( damping ); else mBodyDefinition.linearDamping = damping; }
    inline F32              getLinearDamping(void) const                { if ( mpScene ) return mpBody->GetLinearDamping(); else return mBodyDefinition.linearDamping; }
    inline void             setAngularDamping( const F32 damping )      { if ( mpScene ) mpBody->SetAngularDamping( damping ); else mBodyDefinition.angularDamping = damping; }
    inline F32              getAngularDamping(void) const               { if ( mpScene ) return mpBody->GetAngularDamping(); else return mBodyDefinition.angularDamping; }

    /// Move/Rotate to.
    bool                    moveTo( const Vector2& targetWorldPoint, const U32 time = 1000, const bool autoStop = true );
    bool                    rotateTo( const F32 targetAngle, const U32 time = 1000, const bool autoStop = true );
    void                    cancelMoveTo( const bool autoStop = true );
    void                    cancelRotateTo( const bool autoStop = true );
    inline bool             isMoveToComplete( void ) const              { return mMoveToEventId == 0; }
    inline bool             isRotateToComplete( void ) const            { return mRotateToEventId == 0; }

    /// Force and impulse.
    void                    applyForce( const Vector2& worldForce, const bool wake = true );
    void                    applyForce( const Vector2& worldForce, const Vector2& worldPoint, const bool wake = true);
    void                    applyTorque( const F32 torque, const bool wake = true );
    void                    applyLinearImpulse( const Vector2& worldImpulse, const bool wake = true );
    void                    applyLinearImpulse( const Vector2& worldImpulse, const Vector2& worldPoint, const bool wake = true );
    void                    applyAngularImpulse( const F32 impulse, const bool wake = true );

    /// Gravity scaling.
    inline void             setGravityScale( const F32 scale )          { if ( mpScene ) mpBody->SetGravityScale( scale ); else mBodyDefinition.gravityScale = scale; }
    inline F32              getGravityScale(void) const                 { if ( mpScene ) return mpBody->GetGravityScale(); else return mBodyDefinition.gravityScale; }

    /// General collision shape access.
    void                    deleteCollisionShape( const U32 shapeIndex );
    void                    clearCollisionShapes( void );
    inline U32              getCollisionShapeCount( void ) const        { if ( mpScene ) return mCollisionFixtures.size(); else return mCollisionFixtureDefs.size(); }
    b2Shape::Type           getCollisionShapeType( const U32 shapeIndex ) const;
    S32                     getCollisionShapeIndex( const b2Fixture* pFixture ) const;
    void                    setCollisionShapeDefinition( const U32 shapeIndex, const b2FixtureDef& fixtureDef );
    b2FixtureDef            getCollisionShapeDefinition( const U32 shapeIndex ) const;
    const b2CircleShape*    getCollisionCircleShape( const U32 shapeIndex ) const;
    const b2PolygonShape*   getCollisionPolygonShape( const U32 shapeIndex ) const;
    const b2ChainShape*     getCollisionChainShape( const U32 shapeIndex ) const;
    const b2EdgeShape*      getCollisionEdgeShape( const U32 shapeIndex ) const;
    void                    setCollisionShapeDensity( const U32 shapeIndex, const F32 density );
    F32                     getCollisionShapeDensity( const U32 shapeIndex ) const;
    void                    setCollisionShapeFriction( const U32 shapeIndex, const F32 friction );
    F32                     getCollisionShapeFriction( const U32 shapeIndex ) const;
    void                    setCollisionShapeRestitution( const U32 shapeIndex, const F32 restitution );
    F32                     getCollisionShapeRestitution( const U32 shapeIndex ) const;
    void                    setCollisionShapeIsSensor( const U32 shapeIndex, const bool isSensor );
    bool                    getCollisionShapeIsSensor( const U32 shapeIndex ) const;

    /// Circle collision shape creation.
    S32                     createCircleCollisionShape( const F32 radius );
    S32                     createCircleCollisionShape( const F32 radius, const b2Vec2& localPosition );

    /// Circle collision shape access.
    F32                     getCircleCollisionShapeRadius( const U32 shapeIndex ) const;
    Vector2                 getCircleCollisionShapeLocalPosition( const U32 shapeIndex ) const;

    /// Polygon collision shape creation.
    S32                     createPolygonCollisionShape( const U32 pointCount, const b2Vec2* localPoints );
    S32                     createPolygonBoxCollisionShape( const F32 width, const F32 height );
    S32                     createPolygonBoxCollisionShape( const F32 width, const F32 height, const b2Vec2& localCentroid );
    S32                     createPolygonBoxCollisionShape( const F32 width, const F32 height, const b2Vec2& localCentroid, const F32 rotation );

    /// Polygon collision shape access.
    U32                     getPolygonCollisionShapePointCount( const U32 shapeIndex ) const;
    Vector2                 getPolygonCollisionShapeLocalPoint( const U32 shapeIndex, const U32 pointIndex ) const;

    /// Chain collision shape creation.
    S32                     createChainCollisionShape( const U32 pointCount, const b2Vec2* localPoints );
    S32                     createChainCollisionShape(  const U32 pointCount, const b2Vec2* localPoints,
                                                        const bool hasAdjacentLocalPositionStart, const bool hasAdjacentLocalPositionEnd,
                                                        const b2Vec2& adjacentLocalPositionStart, const b2Vec2& adjacentLocalPositionEnd );

    /// Chain collision shape access.
    U32                     getChainCollisionShapePointCount( const U32 shapeIndex ) const;
    Vector2                 getChainCollisionShapeLocalPoint( const U32 shapeIndex, const U32 pointIndex ) const;
    bool                    getChainCollisionShapeHasAdjacentStart( const U32 shapeIndex ) const;
    bool                    getChainCollisionShapeHasAdjacentEnd( const U32 shapeIndex ) const;
    Vector2                 getChainCollisionShapeAdjacentStart( const U32 shapeIndex ) const;
    Vector2                 getChainCollisionShapeAdjacentEnd( const U32 shapeIndex ) const;

    /// Edge collision shape creation.
    S32                     createEdgeCollisionShape( const b2Vec2& localPositionStart, const b2Vec2& localPositionEnd );
    S32                     createEdgeCollisionShape(   const b2Vec2& localPositionStart, const b2Vec2& localPositionEnd,
                                                        const bool hasAdjacentLocalPositionStart, const bool hasAdjacentLocalPositionEnd,
                                                        const b2Vec2& adjacentLocalPositionStart, const b2Vec2& adjacentLocalPositionEnd );

    /// Edge collision shape access.
    Vector2                 getEdgeCollisionShapeLocalPositionStart( const U32 shapeIndex ) const;
    Vector2                 getEdgeCollisionShapeLocalPositionEnd( const U32 shapeIndex ) const;
    bool                    getEdgeCollisionShapeHasAdjacentStart( const U32 shapeIndex ) const;
    bool                    getEdgeCollisionShapeHasAdjacentEnd( const U32 shapeIndex ) const;
    Vector2                 getEdgeCollisionShapeAdjacentStart( const U32 shapeIndex ) const;
    Vector2                 getEdgeCollisionShapeAdjacentEnd( const U32 shapeIndex ) const;

    /// Collision shape formatting.
    S32                     formatCollisionShape( const U32 shapeIndex, char* pBuffer, U32 bufferSize ) const;
    S32                     formatCircleCollisionShape( const U32 shapeIndex, char* pBuffer, U32 bufferSize ) const;
    S32                     formatPolygonCollisionShape( const U32 shapeIndex, char* pBuffer, U32 bufferSize ) const;
    S32                     formatChainCollisionShape( const U32 shapeIndex, char* pBuffer, U32 bufferSize ) const;
    S32                     formatEdgeCollisionShape( const U32 shapeIndex, char* pBuffer, U32 bufferSize ) const;

    // Collision shape parsing.
    S32                     parseCollisionShape( const char* pBuffer );
    S32                     parseCircleCollisionShape( const char *pBuffer, b2FixtureDef& fixtureDef );
    S32                     parsePolygonCollisionShape( const char *pBuffer, b2FixtureDef& fixtureDef );
    S32                     parseChainCollisionShape( const char *pBuffer, b2FixtureDef& fixtureDef );
    S32                     parseEdgeCollisionShape( const char *pBuffer, b2FixtureDef& fixtureDef );

    /// Render visibility.
    inline void             setVisible( const bool status )             { mVisible = status; }
    inline bool             getVisible(void) const                      { return mVisible; }

    /// Render flipping.
    virtual void            setFlip( const bool flipX, bool flipY );
    virtual void            setFlipX( const bool flipX )                { setFlip( flipX, mFlipY ); }
    virtual void            setFlipY( bool flipY )                      { setFlip( mFlipX, flipY ); }
    inline bool             getFlipX(void) const                        { return mFlipX; }
    inline bool             getFlipY(void) const                        { return mFlipY; }

    /// Render blending.
    inline void             setBlendMode( const bool blendMode )        { mBlendMode = blendMode; }
    inline bool             getBlendMode( void ) const                  { return mBlendMode; }
    inline void             setSrcBlendFactor( const S32 blendFactor )  { mSrcBlendFactor = blendFactor; }
    inline S32              getSrcBlendFactor( void ) const             { return mSrcBlendFactor; }
    inline void             setDstBlendFactor( const S32 blendFactor )  { mDstBlendFactor = blendFactor; }
    inline S32              getDstBlendFactor( void ) const             { return mDstBlendFactor; }
    void                    setBlendColorString( const char* string );
    inline void             setBlendColor( const ColorF& blendColor )   { mBlendColor = blendColor; }
    inline const ColorF&    getBlendColor( void ) const                 { return mBlendColor; }
    inline void             setBlendAlpha( const F32 alpha )            { mBlendColor.alpha = alpha; }
    inline F32              getBlendAlpha( void ) const                 { return mBlendColor.alpha; }
    inline void             setAlphaTest( const F32 alpha )             { mAlphaTest = alpha; }
    inline F32              getAlphaTest( void ) const                  { return mAlphaTest; }
    void                    setBlendOptions( void );
    static                  void resetBlendOptions( void );

    /// Render sorting.
    inline void             setSortPoint( const Vector2& pt )         { mSortPoint = pt; }
    inline const Vector2&   getSortPoint(void) const                    { return mSortPoint; }

    /// Input events.
    inline void             setUseInputEvents( bool mouseStatus )       { mUseInputEvents = mouseStatus; }
    inline bool             getUseInputEvents( void ) const             { return mUseInputEvents; }
    virtual void            onInputEvent( StringTableEntry name, const GuiEvent& event, const Vector2& worldMousePoint );

    // Script callbacks.
    inline void             setUpdateCallback( bool status )            { mUpdateCallback = status; }
    inline bool             getUpdateCallback( void ) const             { return mUpdateCallback; }
    inline void             setCollisionCallback( const bool status )   { mCollisionCallback = status; }
    inline bool             getCollisionCallback(void) const            { return mCollisionCallback; }
    inline void             setSleepingCallback( bool status )          { mSleepingCallback = status; }
    inline bool             getSleepingCallback( void ) const           { return mSleepingCallback; }

    /// Debug mode.
    inline void             setDebugOn( const U32 debugMask )           { mDebugMask |= debugMask; }
    inline void             setDebugOff( const U32 debugMask )          { mDebugMask &= ~debugMask; }
    inline U32              getDebugMask( void ) const                  { return mDebugMask; }

    /// Camera mounting.
    inline void             addCameraMountReference( SceneWindow* pAttachedCamera ) { mpAttachedCamera = pAttachedCamera; }
    inline void             removeCameraMountReference( void )          { mpAttachedCamera = NULL; }
    inline void             dismountCamera( void )                      { if ( mpAttachedCamera ) mpAttachedCamera->dismountMe( this ); }

    // GUI attachment.
    void                    attachGui( GuiControl* pGuiControl, SceneWindow* pSceneWindow, const bool sizeControl );
    void                    detachGui( void );
    inline void             updateAttachedGui( void );

    /// Pathing.
    inline                  void setAttachedToPath(SceneObject* path){ mAttachedToPath = path; }
    inline SceneObject*     getAttachedToPath() const                   { return mAttachedToPath; }

    /// Cloning.
    virtual void            copyFrom( SceneObject* pSceneObject, const bool copyDynamicFields );
    virtual void            copyTo( SimObject* object );

    /// Safe deletion.
    inline void             setSafeDelete( const bool status )          { mSafeDeleteReady = status; }
    inline bool             getSafeDelete( void ) const                 { return mSafeDeleteReady; }
    inline bool             isBeingDeleted( void ) const                { return mBeingSafeDeleted; }
    virtual void            safeDelete();

    /// Destroy notifications.
    void                    addDestroyNotification( SceneObject* pSceneObject );
    void                    removeDestroyNotification( SceneObject* pSceneObject );
    void                    processDestroyNotifications( void );

    /// Component notifications.
    void                    notifyComponentsAddToScene( void );
    void                    notifyComponentsRemoveFromScene( void );
    void                    notifyComponentsUpdate( void );

    /// Miscellaneous.
    void                    setChild( bool status )                     { mEnableChild = status; }
    inline SceneObjectGroup* const getSceneObjectGroup(void) const      { return mpSceneObjectGroup; }
    inline const char*      scriptThis(void) const                      { return Con::getIntArg(getId()); }
    inline bool             getIsChild(void) const                      { return mEnableChild; }
    inline bool             getIsPickingAllowed(void) const             { return mPickingAllowed; }
    inline bool             getIsAlwaysInScope(void) const              { return mAlwaysInScope; }
    inline void             setPeriodicTimerID( S32 timerID )           { mPeriodicTimerID = timerID; }
    inline S32              getPeriodicTimerID( void ) const            { return mPeriodicTimerID; }
    inline void             setWorldQueryKey( const U32 key )           { mWorldQueryKey = key; }
    inline U32              getWorldQueryKey( void ) const              { return mWorldQueryKey; }
    BehaviorInstance*       behavior(const char *name);
    static U32              getGlobalSceneObjectCount( void );
    inline U32              getSerialId( void ) const                   { return mSerialId; }

    // Read / Write fields.
    virtual bool            writeField(StringTableEntry fieldname, const char* value);
    virtual void            writeFields(Stream& stream, U32 tabStop);

    /// Declare Console Object.
    DECLARE_CONOBJECT( SceneObject );

protected:
    /// Lifetime.
    static bool             setLifetime(void* obj, const char* data)    { static_cast<SceneObject*>(obj)->setLifetime(dAtof(data)); return false; }
    static bool             writeLifetime( void* obj, StringTableEntry pFieldName ) { PREFAB_WRITE_CHECK(SceneObject); return pCastObject->getLifetime() > 0.0f ; }

    /// Scene layers.
    static bool             setSceneLayer(void* obj, const char* data)  { static_cast<SceneObject*>(obj)->setSceneLayer(dAtoi(data)); return false; }
    static bool             writeSceneLayer( void* obj, StringTableEntry pFieldName ) { PREFAB_WRITE_CHECK(SceneObject); return pCastObject->getSceneLayer() > 0 ; }
    static bool             setSceneLayerDepth(void* obj, const char* data)  { static_cast<SceneObject*>(obj)->setSceneLayerDepth(dAtof(data)); return false; }
    static bool             writeSceneLayerDepth( void* obj, StringTableEntry pFieldName ) { return mNotZero(static_cast<SceneObject*>(obj)->getSceneLayerDepth()); } // MM: This is not recommended for T2DCore.

    /// Scene groups.
    static bool             setSceneGroup(void* obj, const char* data)  { static_cast<SceneObject*>(obj)->setSceneGroup(dAtoi(data)); return false; }
    static bool             writeSceneGroup( void* obj, StringTableEntry pFieldName ) { PREFAB_WRITE_CHECK(SceneObject); return pCastObject->getSceneGroup() > 0 ; }

    /// Area.
    static bool             setSize(void* obj, const char* data)
    {
        SceneObject* pSceneObject = static_cast<SceneObject*>(obj);
        if ( pSceneObject->getAutoSizing() )
        {
            Con::warnf( "Cannot set the size of a type '%s' as it automatically sizes itself.", pSceneObject->getClassName() );
            return false;
        }
            
        pSceneObject->setSize(Utility::mGetStringElementVector(data));
        return false;
    }
    static bool             writeSize( void* obj, StringTableEntry pFieldName ) { SceneObject* pSceneObject = static_cast<SceneObject*>(obj); return !pSceneObject->getAutoSizing() && pSceneObject->getSize().notEqual(Vector2::getOne()); }

    /// Position / Angle.
    static bool             setPosition(void* obj, const char* data)    { static_cast<SceneObject*>(obj)->setPosition(Utility::mGetStringElementVector(data)); return false; }
    static const char*      getPosition(void* obj, const char* data)    { return static_cast<SceneObject*>(obj)->getPosition().scriptThis(); }
    static bool             writePosition( void* obj, StringTableEntry pFieldName ) { return static_cast<SceneObject*>(obj)->getPosition().notZero(); }
    static bool             setAngle(void* obj, const char* data)       { static_cast<SceneObject*>(obj)->setAngle(mDegToRad(dAtof(data))); return false; }
    static const char*      getAngle(void* obj, const char* data)       { return Con::getFloatArg( mRadToDeg(static_cast<SceneObject*>(obj)->getAngle() ) ); }
    static bool             writeAngle( void* obj, StringTableEntry pFieldName ) { return mNotZero(static_cast<SceneObject*>(obj)->getAngle()); }
    static bool             setFixedAngle(void* obj, const char* data)  { static_cast<SceneObject*>(obj)->setFixedAngle(dAtob(data)); return false; }
    static const char*      getFixedAngle(void* obj, const char* data)  { return Con::getBoolArg( static_cast<SceneObject*>(obj)->getFixedAngle() ); }
    static bool             writeFixedAngle( void* obj, StringTableEntry pFieldName ) { PREFAB_WRITE_CHECK(SceneObject); return pCastObject->getFixedAngle() == true; }

    /// Body.
    static bool setBodyType(void* obj, const char* data)
    {
        // Fetch body type.
        const b2BodyType type = getBodyTypeEnum( data );

        // Check for error.
        if ( type != b2_staticBody && type != b2_kinematicBody && type != b2_dynamicBody )
            return false;

        static_cast<SceneObject*>(obj)->setBodyType(type);
        return false;
    }
    static const char*      getBodyType(void* obj, const char* data)        { return getBodyTypeDescription( static_cast<SceneObject*>(obj)->getBodyType() ); }
    static bool             writeBodyType( void* obj, StringTableEntry pFieldName ) { PREFAB_WRITE_CHECK(SceneObject); return pCastObject->getBodyType() != b2_dynamicBody; }
    static bool             setActive(void* obj, const char* data)          { static_cast<SceneObject*>(obj)->setActive(dAtob(data)); return false; }
    static const char*      getActive(void* obj, const char* data)          { return Con::getBoolArg( static_cast<SceneObject*>(obj)->getActive() ); }
    static bool             writeActive( void* obj, StringTableEntry pFieldName ) { PREFAB_WRITE_CHECK(SceneObject); return pCastObject->getActive() == false; }
    static bool             setAwake(void* obj, const char* data)           { static_cast<SceneObject*>(obj)->setAwake(dAtob(data)); return false; }
    static const char*      getAwake(void* obj, const char* data)           { return Con::getBoolArg( static_cast<SceneObject*>(obj)->getAwake() ); }
    static bool             writeAwake( void* obj, StringTableEntry pFieldName ) { PREFAB_WRITE_CHECK(SceneObject); return pCastObject->getAwake() == false; }
    static bool             setBullet(void* obj, const char* data)          { static_cast<SceneObject*>(obj)->setBullet(dAtob(data)); return false; }
    static const char*      getBullet(void* obj, const char* data)          { return Con::getBoolArg( static_cast<SceneObject*>(obj)->getBullet() ); }
    static bool             writeBullet( void* obj, StringTableEntry pFieldName ) { PREFAB_WRITE_CHECK(SceneObject); return pCastObject->getBullet() == true; }
    static bool             setSleepingAllowed(void* obj, const char* data) { static_cast<SceneObject*>(obj)->setSleepingAllowed(dAtob(data)); return false; }
    static const char*      getSleepingAllowed(void* obj, const char* data) { return Con::getBoolArg( static_cast<SceneObject*>(obj)->getSleepingAllowed() ); }
    static bool             writeSleepingAllowed( void* obj, StringTableEntry pFieldName ) { PREFAB_WRITE_CHECK(SceneObject); return pCastObject->getSleepingAllowed() == false; }

    /// Collision control.
    static bool             setDefaultDensity(void* obj, const char* data)  { static_cast<SceneObject*>(obj)->setDefaultDensity(dAtof(data)); return false; }
    static bool             writeDefaultDensity( void* obj, StringTableEntry pFieldName ) { PREFAB_WRITE_CHECK(SceneObject); return mNotEqual(pCastObject->getDefaultDensity(), 1.0f); }
    static bool             setDefaultFriction(void* obj, const char* data) { static_cast<SceneObject*>(obj)->setDefaultFriction(dAtof(data)); return false; }
    static bool             writeDefaultFriction( void* obj, StringTableEntry pFieldName ) { PREFAB_WRITE_CHECK(SceneObject); return mNotEqual(pCastObject->getDefaultFriction(), 0.2f); }
    static bool             setDefaultRestitution(void* obj, const char* data) { static_cast<SceneObject*>(obj)->setDefaultRestitution(dAtof(data)); return false; }
    static bool             writeDefaultRestitution( void* obj, StringTableEntry pFieldName ) { PREFAB_WRITE_CHECK(SceneObject); return mNotEqual(pCastObject->getDefaultRestitution(), 0.0f); }
    static bool             setCollisionGroups(void* obj, const char* data) { static_cast<SceneObject*>(obj)->setCollisionGroups(dAtoi(data)); return false; }
    static bool             writeCollisionGroups( void* obj, StringTableEntry pFieldName ) { PREFAB_WRITE_CHECK(SceneObject); return pCastObject->getCollisionGroupMask() != MASK_ALL; }
    static bool             setCollisionLayers(void* obj, const char* data) { static_cast<SceneObject*>(obj)->setCollisionLayers(dAtoi(data)); return false; }
    static bool             writeCollisionLayers( void* obj, StringTableEntry pFieldName ) { PREFAB_WRITE_CHECK(SceneObject); return pCastObject->getCollisionLayerMask() != MASK_ALL; }
    static bool             writeCollisionSuppress( void* obj, StringTableEntry pFieldName ) { PREFAB_WRITE_CHECK(SceneObject); return pCastObject->getCollisionSuppress() == true; }
    static bool             setGatherContacts(void* obj, const char* data)  { static_cast<SceneObject*>(obj)->setGatherContacts(dAtoi(data)); return false; }
    static bool             writeGatherContacts( void* obj, StringTableEntry pFieldName ) { PREFAB_WRITE_CHECK(SceneObject); return pCastObject->getGatherContacts() == true; }
    static bool             setAngularVelocityTolerance(void* obj, const char* data)  { static_cast<SceneObject*>(obj)->setAngularVelocityTolerance(dAtof(data)); return false; }
    static bool             writeAngularVelocityTolerance( void* obj, StringTableEntry pFieldName ) { PREFAB_WRITE_CHECK(SceneObject); return mNotEqual(pCastObject->getAngularVelocityTolerance(), 0.0f); }
    static bool             setLinearVelocityTolerance(void* obj, const char* data)  { static_cast<SceneObject*>(obj)->setLinearVelocityTolerance(dAtof(data)); return false; }
    static bool             writeLinearVelocityTolerance( void* obj, StringTableEntry pFieldName ) { PREFAB_WRITE_CHECK(SceneObject); return mNotEqual(pCastObject->getLinearVelocityTolerance(), 0.0f); }

    /// Velocities.
    static bool             setLinearVelocity(void* obj, const char* data)  { static_cast<SceneObject*>(obj)->setLinearVelocity(Utility::mGetStringElementVector(data)); return false; }
    static const char*      getLinearVelocity(void* obj, const char* data)  { return static_cast<SceneObject*>(obj)->getLinearVelocity().scriptThis(); }
    static bool             writeLinearVelocity( void* obj, StringTableEntry pFieldName ) { PREFAB_WRITE_CHECK(SceneObject); return pCastObject->getLinearVelocity().notZero(); }
    static bool             setAngularVelocity(void* obj, const char* data) { static_cast<SceneObject*>(obj)->setAngularVelocity(mDegToRad(dAtof(data))); return false; }
    static const char*      getAngularVelocity(void* obj, const char* data) { return Con::getFloatArg( mRadToDeg(static_cast<SceneObject*>(obj)->getAngularVelocity() ) ); }
    static bool             writeAngularVelocity( void* obj, StringTableEntry pFieldName ) { PREFAB_WRITE_CHECK(SceneObject); return mNotZero(pCastObject->getAngularVelocity()); }
    static bool             setLinearDamping(void* obj, const char* data)   { static_cast<SceneObject*>(obj)->setLinearDamping(dAtof(data)); return false; }
    static const char*      getLinearDamping(void* obj, const char* data)   { return Con::getFloatArg( static_cast<SceneObject*>(obj)->getLinearDamping() ); }
    static bool             writeLinearDamping( void* obj, StringTableEntry pFieldName ) { PREFAB_WRITE_CHECK(SceneObject); return mNotZero(pCastObject->getLinearDamping()); }
    static bool             setAngularDamping(void* obj, const char* data)  { static_cast<SceneObject*>(obj)->setAngularDamping(dAtof(data)); return false; }
    static const char*      getAngularDamping(void* obj, const char* data)  { return Con::getFloatArg( static_cast<SceneObject*>(obj)->getAngularDamping() ); }
    static bool             writeAngularDamping( void* obj, StringTableEntry pFieldName ) { PREFAB_WRITE_CHECK(SceneObject); return mNotZero(pCastObject->getAngularDamping()); }

    /// Gravity scaling.
    static bool             setGravityScale(void* obj, const char* data)    { static_cast<SceneObject*>(obj)->setGravityScale(dAtof(data)); return false; }
    static const char*      getGravityScale(void* obj, const char* data)    { return Con::getFloatArg( static_cast<SceneObject*>(obj)->getGravityScale() ); }
    static bool             writeGravityScale( void* obj, StringTableEntry pFieldName ) { PREFAB_WRITE_CHECK(SceneObject); return mNotEqual(pCastObject->getGravityScale(), 1.0f); }

    /// Render visibility.
    static bool             writeVisible( void* obj, StringTableEntry pFieldName ) { PREFAB_WRITE_CHECK(SceneObject); return pCastObject->getVisible() == false; }

    /// Render flipping.
    static bool             writeFlipX( void* obj, StringTableEntry pFieldName ) { PREFAB_WRITE_CHECK(SceneObject); return pCastObject->getFlipX() == true; }
    static bool             writeFlipY( void* obj, StringTableEntry pFieldName ) { PREFAB_WRITE_CHECK(SceneObject); return pCastObject->getFlipY() == true; }

    /// Render blending.
    static bool             writeBlendMode( void* obj, StringTableEntry pFieldName ) { PREFAB_WRITE_CHECK(SceneObject); return pCastObject->getBlendMode() == false; }
    static bool             writeSrcBlendFactor( void* obj, StringTableEntry pFieldName ) { PREFAB_WRITE_CHECK(SceneObject); return pCastObject->getSrcBlendFactor() != GL_SRC_ALPHA; }
    static bool             writeDstBlendFactor( void* obj, StringTableEntry pFieldName ) { PREFAB_WRITE_CHECK(SceneObject); return pCastObject->getDstBlendFactor() != GL_ONE_MINUS_SRC_ALPHA; }
    static bool             writeBlendColor( void* obj, StringTableEntry pFieldName ) { PREFAB_WRITE_CHECK(SceneObject); return pCastObject->getBlendColor() != ColorF(1.0f, 1.0f, 1.0f, 1.0f); }
    static bool             writeAlphaTest( void* obj, StringTableEntry pFieldName ) { PREFAB_WRITE_CHECK(SceneObject); return pCastObject->getAlphaTest() >= 0.0f; }

    /// Render sorting.
    static bool             writeSortPoint( void* obj, StringTableEntry pFieldName ) { PREFAB_WRITE_CHECK(SceneObject); return pCastObject->getSortPoint().notZero(); }

    /// Input events.
    static bool             writeUseInputEvents( void* obj, StringTableEntry pFieldName ) { PREFAB_WRITE_CHECK(SceneObject); return pCastObject->getUseInputEvents() == true; }

    /// Script callbacks.
    static bool             writeUpdateCallback( void* obj, StringTableEntry pFieldName ) { PREFAB_WRITE_CHECK(SceneObject); return pCastObject->getUpdateCallback() == true; }
    static bool             writeCollisionCallback( void* obj, StringTableEntry pFieldName ) { PREFAB_WRITE_CHECK(SceneObject); return pCastObject->getCollisionCallback() == true; }
    static bool             writeSleepingCallback( void* obj, StringTableEntry pFieldName ) { PREFAB_WRITE_CHECK(SceneObject); return pCastObject->getSleepingCallback() == true; }

    /// Scene.
    static bool             setScene(void* obj, const char* data)
    {
        Scene* pScene = dynamic_cast<Scene*>(Sim::findObject(data));
        SceneObject* object = static_cast<SceneObject*>(obj);
        if (pScene)
        {
            if (object->getScene()) object->getScene()->removeFromScene(object);

            // is the scene object registered?
            if ( object->isProperlyAdded() )
            {
                // Yes, so we can add to the scene now.
                pScene->addToScene(object);
            }
            else
            {
                // No, so just set the target scene directly and it will be added to that scene when registered.
                object->mpTargetScene = pScene;
            }
        }
        return false;
    }
    static bool             writeScene( void* obj, StringTableEntry pFieldName ) { return false; }

};

#endif // _SCENE_OBJECT_H_
