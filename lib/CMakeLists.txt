include(FetchContent)

project_log("FetchContent: fmt")

FetchContent_Declare(
        fmt
        SYSTEM
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG 4a4a2a2bd6a712a1250a0777e74b0261f003043b
)

project_log("FetchContent: spdlog")
FetchContent_Declare(
        spdlog
        SYSTEM
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.11.0
)

project_log("FetchContent: sfeMovie")
FetchContent_Declare(
        sfeMovie
        SYSTEM
        GIT_REPOSITORY https://github.com/Yalir/sfeMovie.git
        GIT_TAG master
)

set(SPDLOG_BUILD_PIC ON)
set(SPDLOG_ENABLE_PCH ON)
FetchContent_MakeAvailable(spdlog)

find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} PARENT_SCOPE)