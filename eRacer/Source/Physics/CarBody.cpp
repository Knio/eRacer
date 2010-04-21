#include <iostream>
#include <float.h>
#include "CarBody.h"

#include "Core/Consts.h"
extern Constants CONSTS;



namespace Physics
{
  
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
  triggerBoxDesc.dimensions = NxVec3((NxReal)10.0, (NxReal)2.0, (NxReal)300.0);
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

float CarBody::SimWheel(
  int i, 
  const Point3 &localpos, 
  const Logic::Frame &frame, 
  float turning, 
  float enginespeed, 
  bool braking
){
  float SPRING_K         = (CONSTS.CAR_MASS * CONSTS.CAR_GRAVITY) / (4 * CONSTS.CAR_DISPLACEMENT);
  float DAMPING          = 2.f * sqrt(SPRING_K * CONSTS.CAR_MASS);
  
  const Matrix tx = GetTransform();
  
  // apply forces to -1 on car body
  const Point3 localapplypoint = Point3(localpos.x, -1, localpos.z);
  
  const Point3  worldpos  = mul1(tx, localpos);
  const Vector3 worldvel  = GetLocalPointWorldVelocity(localpos);
  
  // smooth out wheel velocity
  wheelvel[i] = wheelvel[i]*(1-CONSTS.WHEELVEL_ALPHA) + worldvel*CONSTS.WHEELVEL_ALPHA;
  
  // find distance to road
  const float upamount = 0.5;
  const Vector3 worldroadnormal = frame.up;
  const Point3 localsuspoint = Point3(localpos.x, localpos.y + upamount, localpos.z);
  const Point3 worldsuspoint = mul1(tx, localsuspoint);
  
  float dist = dot(frame.up, (worldsuspoint - frame.position)) - upamount;
  if (length(worldsuspoint - frame.position) > 26.f) dist = FLT_MAX; // off the road
  const float disp = CONSTS.CAR_DISPLACEMENT - dist;
  
  if (dist < localpos.y)  return dist; // car is inside road
  if (disp < 0)           return dist; // in the air
  
  // springs
  const Vector3 downforce = worldroadnormal * (disp * SPRING_K * CONSTS.SPRING_MAGIC);
  AddWorldForceAtLocalPos(downforce, localapplypoint);
  
  // shocks
  float linearvel = dot(worldvel, worldroadnormal);
  linearvel = linearvel>0 ? -powf(linearvel, 0.95f) : powf(-linearvel, 0.95f);
  Vector3 slowforce = worldroadnormal * linearvel * DAMPING * CONSTS.DAMPING_MAGIC;
  AddWorldForceAtLocalPos(slowforce, localapplypoint);
  
  const float weight = length(downforce+slowforce);
  
  if (i >= 2) turning = 0;
  const float angle  = turning * min(1.f, CONSTS.CAR_TURN1 / max(1.f, powf(length(wheelvel[i]), 1.3f)));
  // wheel rolling direction
  const Vector3 worldrollingdir     = mul0(tx, mul0(CreateMatrix(ORIGIN, angle, Y), Z));
  // motion along the wheel's rolling direction
  const Vector3 worldrollingvel     = project(wheelvel[i], worldrollingdir);
  const Vector3 worldrollingvelroad = projectOnto(worldrollingvel, worldroadnormal);
  // motion the wheel WANTS to be going
  const Vector3 worldforwardvel     = worldrollingdir * enginespeed * CONSTS.MAX_ENG_FORCE;
  const Vector3 worldforwardvelroad = projectOnto(worldforwardvel, worldroadnormal);
  // wheel's current velocity projected on the surface of the road
  const Vector3 worldvelroad        = projectOnto(wheelvel[i],  worldroadnormal);
  
  // difference of where the wheel wants to go, and where it is really going.
  // lateral force by friction
  const Vector3 frictionforce   = (worldrollingvelroad - worldvelroad);
  
  Vector3 powerforce;
  if (!braking) powerforce = +worldforwardvelroad;
  else          powerforce = -worldrollingvelroad;
  
  Vector3 totalforce = powerforce + frictionforce;
  
  
  if (length(totalforce) < 1) sliding[i] = false;
  if (length(totalforce) > 1) sliding[i] = true;
  
  normalize(totalforce);
  
  if (sliding[i]) totalforce *= (CONSTS.FRICTION_SLIDING * weight);
  else            totalforce *= (CONSTS.FRICTION_STATIC  * weight);

  AddWorldForceAtLocalPos(totalforce, localapplypoint);
  
  
  const Vector3 reversevel  = normalized(wheelvel[i]) * -1.0;
  const Vector3 rollfrict   = reversevel * (weight*CONSTS.FRICTION_ROLL);
  AddWorldForceAtLocalPos(rollfrict, localapplypoint);
  
  return dist;
}

}