cmake_minimum_required(VERSION 3.14)

project(XPapers)

include(cmake/Logging.cmake)
include(cmake/Ccache.cmake)
include(cmake/Helpers.cmake)
include(cmake/CheckCompiler.cmake)
include(cmake/CompileOptions.cmake)
include(cmake/Sanitize.cmake)
include(cmake/Development.cmake)

add_subdirectory(lib)

# Must be after lib not to affect them.
# include(cmake/Werror.cmake)

set(COMPONENT_LIST "unknown" CACHE INTERNAL "components")

add_subdirectory(src)