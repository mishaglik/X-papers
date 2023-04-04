# Common compile options for C++

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_compile_options( -O2 -fPIC -Weverything  
    -Wno-documentation    
    -Wno-implicit-int-conversion 
    -Wno-c++98-compat
    -Wno-c++98-compat-pedantic
    -Wno-padded
    -Wno-sign-conversion         # I'd fucked up. I know about sign and unsigned
    -Wno-covered-switch-default  # We cover uncoverable
    -Werror=return-type
    )
    
    if(DEBUG)
        add_compile_options(-g -fno-omit-frame-pointer)
    endif()

elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GCC")
    add_compile_options(-O2 -ggdb3 -Wall -Wextra -Wpedantic -Werror=return-type
    -Weffc++
    -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -pie -fPIE 
    )

    if(DEBUG)
    add_compile_options(-ggdb3 -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer)
    endif()

endif()

