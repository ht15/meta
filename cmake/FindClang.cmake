# by default, only use the libclang static lib on MSVC
set(Clang_SEARCH_PATHS
    ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/clang
)


# include directories
find_path(CLANG_INCLUDE_DIRS
    NAMES "clang-c/Index.h"
    PATHS ${LLVM_SEARCH_PATHS}
    PATH_SUFFIXES "include"
)