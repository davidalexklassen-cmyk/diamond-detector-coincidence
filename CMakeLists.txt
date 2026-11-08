cmake_minimum_required(VERSION 3.16...4.4 FATAL_ERROR)


if(APPLE)
  set(CMAKE_C_COMPILER /usr/bin/clang)
  set(CMAKE_CXX_COMPILER /usr/bin/clang++)
endif()

project(sc_cvd_coincidence)

if(APPLE)
  set(EXPAT_INCLUDE_DIR $ENV{CONDA_PREFIX}/include)
  set(EXPAT_LIBRARY $ENV{CONDA_PREFIX}/lib/libexpat.dylib)
endif()

add_compile_definitions(PROJECT_DATA_DIR="${PROJECT_SOURCE_DIR}/data")
find_package(Geant4 REQUIRED ui_all vis_all)

include(${Geant4_USE_FILE})

file(GLOB sources ${PROJECT_SOURCE_DIR}/sources/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/headers/*.hh)
include_directories(${PROJECT_SOURCE_DIR}/headers)

add_executable(sim ${sources} ${headers})
target_link_libraries(sim ${Geant4_LIBRARIES})

add_custom_target(sc_cvd_coincidence DEPENDS sim)

add_executable(ppKinematicTest ${PROJECT_SOURCE_DIR}/validation/ppKinematicTest.cc ${PROJECT_SOURCE_DIR}/sources/ppkinematics.cc)
target_link_libraries(ppKinematicTest ${Geant4_LIBRARIES})