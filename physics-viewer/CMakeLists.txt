cmake_minimum_required(VERSION 3.9.0 FATAL_ERROR)
set (projectName "physics-viewer")
project(projectName)
message (${projectName})
set(NEWTON_ROOT "/mnt/work2/artur/newton-dynamics/newton-4.00")
add_compile_options(-Wfatal-errors)

set(CMAKE_CONFIGURATION_TYPES Debug RelWithDebInfo Release)
set(CMAKE_DEBUG_POSTFIX "_d")

include(CheckTypeSize)
CHECK_TYPE_SIZE("void*" PTR_SIZE BUILTIN_TYPES_ONLY)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NEWTON_NO_SIMD_INTRINSICS)
	add_definitions(-DD_SCALAR_VECTOR_CLASS)
endif()

if(NEWTON_BUILD_SINGLE_THREADED)
	add_definitions(-DD_USE_THREAD_EMULATION)
endif()
  
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING
          "Choose the type of build, options are: Debug RelWithDebInfo Release"
          FORCE)
endif(NOT CMAKE_BUILD_TYPE)



find_package(glfw3 REQUIRED)

if(NOT_glfw3_FOUND)
    message(WARNING "GLFW headers were not found.")
else(NOT_glfw3_FOUND)
    message("Found GLFW library")
endif(NOT_glfw3_FOUND)

find_package(OpenAL REQUIRED)

if(NOT_OpenAL_FOUND)
    message(WARNING "OpenAL headers were not found.")
else(NOT_OpenAL_FOUND)
    message("Found OpenAL library")
endif(NOT_OpenAL_FOUND)

find_package(OpenGL REQUIRED)

if(NOT_OpenGL_FOUND)
    message(WARNING "OpenGL headers were not found.")
else(NOT_OpenGL_FOUND)
    message("Found OpenGL library")
endif(NOT_OpenGL_FOUND)

# add_subdirectory("${NEWTON_ROOT}/sdk")

# source and header files
file(GLOB CPP_SOURCE 
	*.h
	*.cpp
	toolbox/*.h
	toolbox/*.cpp 
	animation/*.h
	animation/*.cpp 
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/" FILES ${CPP_SOURCE})

include_directories(.)
include_directories(toolbox)
include_directories(animation)
include_directories(${NEWTON_ROOT}/sdk/dCore)
include_directories(${NEWTON_ROOT}/sdk/dNewton)
include_directories(${NEWTON_ROOT}/sdk/dTinyxml)
include_directories(${NEWTON_ROOT}/sdk/dCollision)
include_directories(${NEWTON_ROOT}/sdk/dNewton/dJoints)
include_directories(${NEWTON_ROOT}/sdk/dNewton/dModels)
include_directories(${NEWTON_ROOT}/sdk/dNewton/dIkSolver)
include_directories(${NEWTON_ROOT}/sdk/dNewton/dParticles)
include_directories(${NEWTON_ROOT}/sdk/dNewton/dModels/dVehicle)
include_directories(${NEWTON_ROOT}/sdk/dNewton/dModels/dCharacter)

include_directories(${NEWTON_ROOT}/thirdParty/brain)
include_directories(${NEWTON_ROOT}/thirdParty/glfw/)
include_directories(${NEWTON_ROOT}/thirdParty/imgui/)
include_directories(${NEWTON_ROOT}/thirdParty/hacdNew)
include_directories(${NEWTON_ROOT}/thirdParty/openFBX/src/)
include_directories(${NEWTON_ROOT}/thirdParty/glfw/include/)
include_directories(${NEWTON_ROOT}/thirdParty/imgui/backends)
include_directories(${NEWTON_ROOT}/thirdParty/glatter/include/glatter/)
include_directories(${NEWTON_ROOT}/thirdParty/hacd/src/VHACD_Lib/public/)
include_directories(${NEWTON_ROOT}/thirdParty/openalRuntime/win32sdk/include)

add_executable(${projectName} ${CPP_SOURCE})

target_link_libraries("${projectName}" ndNewton_d)
target_link_libraries("${projectName}" ndBrain_d)
target_link_libraries("${projectName}" vhacd_d)
target_link_libraries("${projectName}" openfbx_d)
target_link_libraries("${projectName}" glfw)
target_link_libraries("${projectName}" imgui_d)
target_link_libraries("${projectName}" glatter_d)
# target_link_libraries("${projectName}" libndNewton_d)

if (NEWTON_BUILD_CREATE_SUB_PROJECTS)
	target_link_libraries (${projectName} ndCollision ndCore)
endif()
 
if (NEWTON_BUILD_PROFILER)
	target_link_libraries (${projectName} dProfiler)
endif ()

target_link_directories(${projectName} PRIVATE ${NEWTON_ROOT}/build/lib)

if(UNIX)
	target_link_libraries (${projectName} GL GLU dl X11 pthread openal)

	# if(NEWTON_ENABLE_AVX2_SOLVER)
	# 	target_link_libraries (${projectName} ndSolverAvx2)
	# endif(NEWTON_ENABLE_AVX2_SOLVER)

	if (NEWTON_ENABLE_CUDA_SOLVER)
		target_link_libraries (${projectName} ndSolverCuda)
	endif(NEWTON_ENABLE_CUDA_SOLVER)
	
	if (NEWTON_ENABLE_OPENCL_SOLVER)
		target_link_libraries (${projectName} ndSolverOpenCL OpenCL)
	endif(NEWTON_ENABLE_OPENCL_SOLVER)
    
    set (source "${CMAKE_SOURCE_DIR}/media")
    set (destination "${PROJECT_BINARY_DIR}/media")
    add_custom_command(
        TARGET ${projectName} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E create_symlink ${source} ${destination}
        DEPENDS ${destination}
        COMMENT "symbolic link resources folder from ${source} => ${destination}")
endif()

install(DIRECTORY ../media DESTINATION "${PROJECT_BINARY_DIR}/")
