

add_library(paperpy MODULE PaperPy.cpp PaperPy.hpp PPython.cpp)
target_include_directories(paperpy PUBLIC .)
target_include_directories(paperpy PUBLIC "${PROJECT_SOURCE_DIR}/src")
find_package(PythonLibs REQUIRED)
target_include_directories(paperpy SYSTEM PRIVATE ${PYTHON_INCLUDE_DIRS})
target_link_libraries(paperpy PUBLIC ${PYTHON_LIBRARIES})
target_link_libraries(paperpy PUBLIC utilities_headers)
target_compile_options(paperpy PRIVATE "-fPIC")

make_module(paperpy PaperPy)
