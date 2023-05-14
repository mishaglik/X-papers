

add_library(label MODULE Label.cpp)
target_include_directories(label PUBLIC .)
target_include_directories(label PUBLIC "${PROJECT_SOURCE_DIR}/src")
target_compile_options(paperpy PRIVATE "-fPIC")
