# Sanitizers

# --------------------------------------------------------------------
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(UBSAN_COMPILE_FLAGS -fsanitize=undefined -fno-sanitize-recover=all)
    set(UBSAN_LINK_FLAGS -fsanitize=undefined)
    message(STATUS "Sanitize with UB Sanitizer (UBSAN)")
    add_compile_options(${UBSAN_COMPILE_FLAGS})
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${UBSAN_LINK_FLAGS}")
    
    set(ASAN_COMPILE_FLAGS -fsanitize=address,undefined -fno-sanitize-recover=all)
    set(ASAN_LINK_FLAGS -fsanitize=address,undefined)
    message(STATUS "Sanitize with Address Sanitizer (ASAN)")
    add_compile_options(${ASAN_COMPILE_FLAGS})
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${ASAN_LINK_FLAGS}")
    
    set(TSAN_COMPILE_FLAGS -fsanitize=thread -fno-sanitize-recover=all)
    set(TSAN_LINK_FLAGS -fsanitize=thread)
    message(STATUS "Sanitize with Thread Sanitizer (TSAN)")
    add_compile_options(${TSAN_COMPILE_FLAGS})
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${TSAN_LINK_FLAGS}")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set (GNU_SANITIZE_FLAGS
    -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr
    )
    add_compile_options(${GNU_SANITIZE_FLAGS})
    add_link_options(${GNU_SANITIZE_FLAGS})
endif()
