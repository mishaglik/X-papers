set(RENDER_ENGINE_HEADERS
)

set(RENDER_ENGINE_SOURCES
    engine.cpp
)

add_library(renderEngine ${RENDER_ENGINE_HEADERS} ${RENDER_ENGINE_SOURCES})
make_component(renderEngine RenderEngine)
target_include_directories(renderEngine PUBLIC ..)

find_package(OpenGL REQUIRED)

target_include_directories(renderEngine PUBLIC ${OPENGL_INCLUDE_DIR})
target_link_libraries(renderEngine PUBLIC ${OPENGL_LIBRARIES})

target_link_libraries(renderEngine PUBLIC windowEngine utilities)
set (COMPONENT_LIST ${COMPONENT_LIST} PARENT_SCOPE)

add_executable(testRender XCBStub.cpp)
prepare_executable(testRender TestRender)
target_link_libraries(testRender renderEngine utilities)