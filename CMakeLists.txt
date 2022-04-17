cmake_minimum_required(VERSION 3.16)

project(vm)

add_executable(vm
    src/vm.cpp 
)

target_include_directories(vm
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/include
)

project(translate)

add_executable(translate
    src/translate.cpp 
)

