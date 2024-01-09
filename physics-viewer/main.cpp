#include "ndPhysicsViewer.h"
#include "ndLeakTracker.h"

int main(int, char**)
{
	ndSetAllocators setAllocators;
	ndPhysicsViewer viewer;
    // Добавить загрузку объектов из текстового файла по командным аргументам, из ямла.
	viewer.Run();
	return 0;
}
