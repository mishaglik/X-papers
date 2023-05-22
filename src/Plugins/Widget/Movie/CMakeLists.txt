

add_library(movie MODULE Movie.cpp)
target_include_directories(movie PUBLIC .)
target_include_directories(movie PUBLIC "${PROJECT_SOURCE_DIR}/src")
target_compile_options(movie PRIVATE "-fPIC")
target_link_libraries(movie utilities_headers)
make_module(movie Movie)

