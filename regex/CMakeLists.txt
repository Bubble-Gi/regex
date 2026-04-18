add_library(reg STATIC regex.cpp regex.h)
target_link_libraries(reg PUBLIC DFA)