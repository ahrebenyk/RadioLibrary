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
        src/services/JsonStorageService.cpp
        src/services/CLI.cpp)

add_custom_target(copy_data_file ALL
        COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_SOURCE_DIR}/data/components.json"
        "${CMAKE_CURRENT_BINARY_DIR}/components.json"
        COMMENT "Копіюю файл бази даних у папку збірки..."
)

add_dependencies(RadioLibrary copy_data_file)

if(WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++")
endif()