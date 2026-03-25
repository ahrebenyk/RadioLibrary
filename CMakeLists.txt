cmake_minimum_required(VERSION 4.0)
project(RadioLibrary)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_executable(RadioLibrary src/main.cpp
        src/Component.cpp
        src/Resistor.cpp
        src/Diode.cpp
        src/Transistor.cpp
        src/Capacitor.cpp
        src/DataService.cpp
        src/UserInterface.cpp
        src/JsonFileService.cpp)

add_custom_target(copy_data_files ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/data
        ${CMAKE_CURRENT_BINARY_DIR}/data
        COMMENT "Оновлюю файли бази даних у папці збірки..."
)

add_dependencies(RadioLibrary copy_data_files)