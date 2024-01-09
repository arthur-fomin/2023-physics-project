#pragma once

class ndPhysicsViewer;
ndBodyKinematic* BuildHeightFieldTerrain(ndPhysicsViewer* const scene, const ndMatrix& location = ndGetIdentityMatrix());
void AddHeightfieldSubShape(ndPhysicsViewer* const scene, ndShapeInstance& sceneInstance, ndBaseEntity* const rootEntity, const ndMatrix& matrix);

