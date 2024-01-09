#pragma once
#include "ndBaseEntity.h"

class ndPhysicsViewer;
class ndAnimationSequence;

ndAnimationSequence* LoadFbxAnimation(const char* const meshName);
ndMeshEffectNode* LoadFbxMeshEffectNode(const char* const meshName);

