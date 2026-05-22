cmake_minimum_required(VERSION 3.6)
project(lang_it)

add_executable(lang_it lgit.cpp)
target_include_directories(lang_it PRIVATE src)

# Maximum optimization, strip everything unnecessary
target_compile_options(lang_it PRIVATE 
    -O3                          # Maximum optimization
    -fno-exceptions              # Remove exception handling
    -fno-rtti                    # Remove RTTI
    -fno-unwind-tables           # Remove unwind tables
    -fno-asynchronous-unwind-tables
    -ffunction-sections          # Allow dead code removal
    -fdata-sections
    -flto                        # Link-time optimization
)

target_link_options(lang_it PRIVATE 
    -flto
    -Wl,--gc-sections            # Remove unused sections
    -s                           # Strip all symbols
)

install(TARGETS lang_it RUNTIME DESTINATION bin)