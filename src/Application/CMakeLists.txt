
add_executable(xpapers main.cpp Configurator.cpp)

target_link_libraries(xpapers PUBLIC utilities wallpaperEngine)

prepare_executable(xpapers XPapers)
