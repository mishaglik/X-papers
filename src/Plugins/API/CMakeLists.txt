

add_library(paperpy MODULE PaperPy.cpp PaperPy.hpp PPython.c)
target_include_directories(paperpy PUBLIC .)
target_include_directories(paperpy PUBLIC "${PROJECT_SOURCE_DIR}/src")
find_package(PythonLibs REQUIRED)
target_include_directories(paperpy SYSTEM PRIVATE ${PYTHON_INCLUDE_DIRS})
target_link_libraries(paperpy ${PYTHON_LIBRARIES})
target_compile_options(paperpy PRIVATE "-fPIC")
