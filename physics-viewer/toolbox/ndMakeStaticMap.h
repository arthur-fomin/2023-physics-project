#pragma once
class ndPhysicsViewer;

ndBodyKinematic* BuildPlayArena(ndPhysicsViewer* const scene);
ndBodyKinematic* BuildFlatPlane(ndPhysicsViewer* const scene, bool optimized);
ndBodyKinematic* BuildFloorBox(ndPhysicsViewer* const scene, const ndMatrix& location);
ndBodyKinematic* BuildStaticMesh(ndPhysicsViewer* const scene, const char* const meshName, bool optimized);
ndBodyKinematic* BuildSplineTrack(ndPhysicsViewer* const scene, const char* const meshName, bool optimized);
ndBodyKinematic* BuildGridPlane(ndPhysicsViewer* const scene, ndInt32 grids, ndFloat32 gridSize, ndFloat32 perturbation);

