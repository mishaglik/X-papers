

add_library(paperpy SHARED PaperPy.cpp PaperPy.hpp)
target_include_directories(paperpy PUBLIC .)
target_include_directories(paperpy PUBLIC "${PROJECT_SOURCE_DIR}/src")
