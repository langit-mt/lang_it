cmake_minimum_required(VERSION 3.6)
project(lang_it)

option(USE_ETL "Use ETL library instead of STL" OFF)

if(USE_ETL)
    add_definitions(-DETL_BUILD)
    set(EXTRA_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/etl/include)
    set(EXTRA_DEFINES ETL_BUILD)
else()
    set(EXTRA_INCLUDES "")
    set(EXTRA_DEFINES "")
endif()

# Add the source file directly
add_executable(lang_it lgit.cpp)

target_include_directories(lang_it PRIVATE src ${EXTRA_INCLUDES})
target_compile_definitions(lang_it PRIVATE ${EXTRA_DEFINES})
target_compile_features(lang_it PRIVATE cxx_std_17)

if (MSVC)
    target_compile_options(lang_it PRIVATE /utf-8)
endif()

if(NOT MSVC)
    target_compile_options(lang_it PRIVATE -g -O2)
endif()

install(TARGETS lang_it RUNTIME DESTINATION bin)


#SUPER O´PTIMIZED ONE
#  cmake_minimum_required(VERSION 3.6)
#  project(lang_it)

#  option(USE_ETL "Use ETL library instead of STL" OFF)

#  if(USE_ETL)
#      add_definitions(-DETL_BUILD)
#      set(EXTRA_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/etl/include)
#      set(EXTRA_DEFINES ETL_BUILD)
#  else()
#      set(EXTRA_INCLUDES "")
#      set(EXTRA_DEFINES "")
#  endif()

#  add_executable(lang_it lgit.cpp)

#  target_include_directories(lang_it PRIVATE src ${EXTRA_INCLUDES})
#  target_compile_definitions(lang_it PRIVATE ${EXTRA_DEFINES})
#  target_compile_features(lang_it PRIVATE cxx_std_17)

#  if (MSVC)
#      target_compile_options(lang_it PRIVATE /utf-8)
#  else()
#              target_compile_options(lang_it PRIVATE 
#                  -Os                          # Optimize for size
#                 -fno-exceptions              
#                  -fno-rtti                    
#                  -fno-unwind-tables           
#                  -ffunction-sections          
#                  -fdata-sections
#                 -flto                        
#                 -fno-inline                  # REDUCE inlining
#                  -fno-inline-small-functions
#                  -fmerge-all-constants
#                  -fshort-enums
#              )

#            target_link_options(lang_it PRIVATE 
#                 -flto
#                 -Wl,--gc-sections            
#                 -Wl,--strip-all
#                 -Wl,--no-whole-archive
#                 -Wl,--relax                  # Relax alignments
#                 -s                           
#             )
#  endif()

#  install(TARGETS lang_it RUNTIME DESTINATION bin)