add_library(metacore 
    ./src/parser/callable.cpp
    ./src/parser/clang_utils.cpp
    ./src/parser/class.cpp
    ./src/parser/enum.cpp
    ./src/parser/forward.cpp
    ./src/parser/name_space.cpp
    ./src/parser/node.cpp
    ./src/parser/type_info.cpp
    ./src/parser/variable.cpp
)

target_include_directories(metacore PUBLIC include)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
find_package(Filesystem REQUIRED)


target_link_libraries(metacore PUBLIC clang_3rd nlohmann_json_3rd spdlog_3rd mustache_3rd PRIVATE)
