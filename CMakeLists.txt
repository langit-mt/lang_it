cmake_minimum_required(VERSION 3.6)

project(lgit)

add_library(lang_it_lib INTERFACE)

target_include_directories(lang_it_lib INTERFACE src)

target_compile_definitions(lang_it_lib INTERFACE ALLOW_IMPORTS)

target_compile_features(lang_it_lib INTERFACE cxx_std_17)

if (MSVC)
    target_compile_options(lang_it_lib INTERFACE /utf-8)
endif()

add_executable(lgit lgit.cpp)

target_compile_definitions(lgit PRIVATE ALLOW_IMPORTS)

target_link_libraries(lgit PRIVATE lang_it_lib)

install(TARGETS lgit
    RUNTIME DESTINATION bin
)