find_package(Catch2 3.11.0 REQUIRED)
add_executable(test test.cpp)

target_link_libraries(test PRIVATE
        ST
        DFA
        reg
        Catch2::Catch2WithMain
)
include_directories(..)

add_compile_options(--coverage)
add_link_options(--coverage)