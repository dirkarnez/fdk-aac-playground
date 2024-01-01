cmake_minimum_required(VERSION 3.18)

project("fdk-aac-playground")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")

find_package(fdk-aac 2.0.3 EXACT REQUIRED) # PATHS "P:\\Downloads\\cpp-libraries\\portaudio\\build\\cmake\\portaudio")

if(fdk-aac_FOUND)
    message(STATUS "YES")
else()
    message(STATUS "NO")
endif()

add_executable(${PROJECT_NAME}
    "src/main.cpp"
    "src/aac_encoder.cpp"
    "src/aac_encoder.h"
)

target_link_libraries(${PROJECT_NAME} FDK-AAC::fdk-aac)
