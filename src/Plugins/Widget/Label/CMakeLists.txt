

add_library(label MODULE Label.cpp)
target_include_directories(label PUBLIC .)
target_include_directories(label PUBLIC "${PROJECT_SOURCE_DIR}/src")
target_compile_options(label PRIVATE "-fPIC")
target_link_libraries(label utilities_headers)
make_module(label Label)

