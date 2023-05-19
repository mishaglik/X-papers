

add_library(baseWigets MODULE Module.cpp widgets/Label.cpp)
target_include_directories(baseWigets PUBLIC .)
target_include_directories(baseWigets PUBLIC "${PROJECT_SOURCE_DIR}/src")
target_compile_options(baseWigets PRIVATE "-fPIC")
target_link_libraries(baseWigets utilities_headers)
make_module(baseWigets BaseWigets)

