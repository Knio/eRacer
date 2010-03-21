#include <iostream>
#include "CarBody.h"

#include "Core/Consts.h"
extern Constants CONSTS;
#include "Graphics/Track.h"


namespace Physics{
CarBody::CarBody(float mass, const Point3& pos, const Matrix& orient){
  Matrix rot = orient*CreateMatrix(ORIGIN, PI/2.0f, X);

  NxMaterialDesc material;
  material.restitution =    (NxReal)0.9;
  material.dynamicFriction =  (NxReal)0.3;
  material.staticFriction =  (NxReal)0.3;

  // Add a single-shape actor to the scene
  NxActorDesc actorDesc;

  //right, long capsule
  NxCapsuleShapeDesc capDesc1;
  capDesc1.materialIndex = PhysicsLayer::g_PhysicsLayer->AddMaterialReturnIndex(material);
  capDesc1.height = (NxReal)9.03;
  capDesc1.radius = (NxReal)1.2;
  capDesc1.localPose.M = Matrix_NxMat33(rot);
  capDesc1.localPose.t = NxVec3((NxReal)2.2, (NxReal)0, (NxReal)1.11);
  assert(capDesc1.isValid());
  actorDesc.shapes.pushBack(&capDesc1);

  //left, long capsule
  NxCapsuleShapeDesc capDesc2;
  capDesc2.materialIndex = PhysicsLayer::g_PhysicsLayer->AddMaterialReturnIndex(material);
  capDesc2.height = (NxReal)9.03;
  capDesc2.radius = (NxReal)1.2;
  capDesc2.localPose.M = Matrix_NxMat33(rot);
  capDesc2.localPose.t = NxVec3((NxReal)-2.2, (NxReal)0, (NxReal)1.11);
  assert(capDesc2.isValid());
  actorDesc.shapes.pushBack(&capDesc2);

  //middle, shorter box
  NxBoxShapeDesc boxDesc;
  boxDesc.materialIndex = PhysicsLayer::g_PhysicsLayer->AddMaterialReturnIndex(material);
  boxDesc.dimensions.set((NxReal)2.0, (NxReal)0.7, (NxReal)3.2);
  boxDesc.localPose.t = NxVec3((NxReal)0, (NxReal)0, (NxReal)0.0);
  assert(boxDesc.isValid());
  actorDesc.shapes.pushBack(&boxDesc);
  
  
  NxBoxShapeDesc triggerBoxDesc;
  triggerBoxDesc.dimensions = NxVec3((NxReal)10.0, (NxReal)2.0, (NxReal)30.0);
  triggerBoxDesc.shapeFlags |= NX_TRIGGER_ON_STAY;
  triggerBoxDesc.localPose.t = NxVec3((NxReal)0.0, (NxReal)0, (NxReal)22.5);
  actorDesc.shapes.pushBack(&triggerBoxDesc);
  
  
  NxBodyDesc bodyDesc;
  bodyDesc.mass = mass;
  assert(bodyDesc.isValid());
  actorDesc.body  = &bodyDesc;
  actorDesc.globalPose.t  = NxVec3(pos.x, pos.y, pos.z);
  actorDesc.globalPose.M = Matrix_NxMat33(orient);

  assert(actorDesc.isValid());
  
  CreateActor(actorDesc); 
  
  SetGroup(CAR);
  
  
  for (int i=0;i<4;i++)
    wheelvel[i] = ORIGIN;
  
}

CarBody::~CarBody(){
}

float CarBody::SimWheel(const int i, const Point3& localpos, const Frame& frame, const float turning, const float enginespeed, const bool braking)
{
  float SPRING_K         = (CONSTS.CAR_MASS * CONSTS.CAR_GRAVITY) / (4 * CONSTS.CAR_DISPLACEMENT);
  float DAMPING          = 2.f * sqrt(SPRING_K * CONSTS.CAR_MASS);
  
  const Matrix tx = GetTransform();
  
  // apply forces to -1 on car body
  const Point3 localapplypoint = Point3(localWheel.x, -1, localWheel.z);
  
  const Point3  worldpos  = mul1(tx, localpos);
  const Vector3 worldvel  = GetLocalPointWorldVelocity(localpos)
  
  // smooth out wheel velocity
  wheelvel[i] = wheelvel[i]*(1-CONSTS.WHEELVEL_ALPHA) + worldvel*CONSTS.WHEELVEL_ALPHA;

  // find distance to road
  /*
  
  -------
         |
  CAR    |
         |
  -------|- where forces are applied (applypoint)
         |
         |
         -   -- Where the wheel is defined to be at rest (pos)  \           \
         |                                                      |-disp (-)  |
         |                                                      |           |
  -------o----- acctual wheel on road                           /           | - dist (+)
         |                                                                  |
         |                                                                  |
         -  -- where the wheel will be with no load                         /
  
  
  
  */
  
  const float upamount = 2.f;
  const Vector3 worldroadnormal = frame.up;
  const Point3 localsuspoint = Point3(localpos.x, localpos.y + upamount, localpos.z);
  const Point3 worldsuspoint = mul1(tx, localsuspoint);
  
  const float dist = dot(up, (worldsuspoint - frame.position)) - upamount
  if (length(worldsuspoint - frame.position) > 26) dist = 1e99; // off the road
  const float disp = CONSTS.CAR_DISPLACEMENT - dist;
  
  if (dist < localpos.y)  return dist; // car is inside road
  if (disp < 0)           return dist; // in the air
  
  // springs
  const Vector3 downforce = worldroadnormal * (disp * SPRING_K * CONSTS.SPRING_MAGIC);
  AddWorldForceAtLocalPos(downforce, localapplypoint);
  
  // shocks
  const float linearvel = dot(worldvel, worldroadnormal);
  linearvel = linearvel>0 ? pow(linearvel, 0.95) : -pow(-leanearvel, 0.95);
  Vector3 slowforce = worldroadnormal * linearvel * DAMPING * self.DAMPING_MAGIC;
  AddWorldForceAtLocalPos(slowforce, localapplypoint);
  
  const float weight = length(downforce+slowforce);
  
  const float angle  = turning * * min(1.f,(60.f / max(1.f, pow(length(avgworldvel)), 1.3));
  // wheel rolling direction
  const Vector3 worldrollingdir     = mul0(tx, mul0(Matrix(ORIGIN, angle, Y), Z));
  // motion along the wheel's rolling direction
  const Vector3 worldrollingvel     = project(avgworldvel, worldrollingdir);
  const Vector3 worldrollingvelroad = projectOnto(worldrollingvel, worldroadnormal);
  // motion the wheel WANTS to be going
  const Vector3 worldforwardvel     = worldrollingdir * enginespeed;
  const Vector3 worldforwardvelroad = projectOnto(worldforwardvel, worldroadnormal);
  // wheel's current velocity projected on the surface of the road
  const Vector3 worldvelroad        = projectOnto(avgworldvel,  worldroadnormal);
  
  // difference of where the wheel wants to go, and where it is really going.
  // lateral force by friction
  const Vector3 frictionforce   = (worldrollingvelroad - worldvelroad);
  Vector3 powerforce;
  if (!braking) powerforce = +worldforwardvelroad;
  else          powerforce = -worldrollingvelroad;
  
  const Vector3 totalforce = powerforce + frictionforce;
  
  // ####################################################
  
  
  
  
  
  
  
  return dist;
}

}