#pragma once 

#include <ndNewton.h>

void dGetWorkingFileName (const char* const name, char* const outPathName);

// endian conversion
ndUnsigned32 SWAP_INT32(ndUnsigned32 x);
ndUnsigned16 SWAP_INT16(ndUnsigned16 x);
ndUnsigned16 ndIndian16(ndUnsigned16 x);
ndUnsigned32 ndIndian32(ndUnsigned32 x);


