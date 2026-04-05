cmake_minimum_required(VERSION 4.0)
project(RadioLibrary)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_executable(RadioLibrary src/main.cpp
        src/components/Component.cpp
        src/components/Resistor.cpp
        src/components/Diode.cpp
        src/components/Transistor.cpp
        src/components/Capacitor.cpp
        src/services/DataService.cpp
        src/services/UserInterface.cpp
        src/services/JsonStorageService.cpp
        src/services/CLI.cpp)

add_custom_target(copy_data_files ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/data
        ${CMAKE_CURRENT_BINARY_DIR}/data
        COMMENT "Оновлюю файли бази даних у папці збірки..."
)

add_dependencies(RadioLibrary copy_data_files)

if(WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++")
endif()