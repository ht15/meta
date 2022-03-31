set(THIRD_PARTY_DIR ${CMAKE_SOURCE_DIR}/third_party)
set(EXTERNAL_PREFIX ${CMAKE_BINARY_DIR}/external_prefix)
file(MAKE_DIRECTORY ${EXTERNAL_PREFIX}/include)
file(MAKE_DIRECTORY ${EXTERNAL_PREFIX}/lib)


## clang
add_library(clang_3rd SHARED IMPORTED GLOBAL)
if (LINUX OR APPLE)
    set(CLANG_APPLE_INCLUDE_DIRS "/Users/qiqi/Documents/github/llvm/clang/include")
    set_target_properties(clang_3rd PROPERTIES
        IMPORTED_LOCATION
        "/Users/qiqi/Documents/github/llvm/clang/lib/libclang.dylib"
        INTERFACE_INCLUDE_DIRECTORIES
        "${CLANG_APPLE_INCLUDE_DIRS}"
        )
endif ()


## nlohmann
add_library(nlohmann_json_3rd INTERFACE IMPORTED GLOBAL)
target_include_directories(nlohmann_json_3rd INTERFACE ${THIRD_PARTY_DIR}/nlohmann_json/include)


## spdlog
add_library(spdlog_3rd INTERFACE IMPORTED GLOBAL)
target_include_directories(spdlog_3rd INTERFACE ${THIRD_PARTY_DIR}/spdlog/include)


## mustache.hpp
add_library(mustache_3rd INTERFACE IMPORTED GLOBAL)
target_include_directories(mustache_3rd INTERFACE ${THIRD_PARTY_DIR}/Mustache)
