# add_executable(generate_decode ${CMAKE_CURRENT_SOURCE_DIR}/generate_decode/generate_decode.cpp)
# target_link_libraries(generate_decode PRIVATE meta)

# add_executable(generate_property ${CMAKE_CURRENT_SOURCE_DIR}/generate_property/generate_property.cpp)
# target_link_libraries(generate_property PRIVATE meta)

# add_executable(generate_rpc ${CMAKE_CURRENT_SOURCE_DIR}/generate_rpc/generate_rpc.cpp)
# target_link_libraries(generate_rpc PRIVATE meta)

add_subdirectory(generate_attr)
# add_executable(generate_component ${CMAKE_CURRENT_SOURCE_DIR}/generate_component/generate_component.cpp)
# target_link_libraries(generate_component PRIVATE meta)

# add_executable(generate_enum ${CMAKE_CURRENT_SOURCE_DIR}/generate_enum/generate_enum.cpp)
# target_link_libraries(generate_enum PRIVATE meta)

# add_executable(generate_behavior ${CMAKE_CURRENT_SOURCE_DIR}/generate_behavior/generate_behavior.cpp)
# target_link_libraries(generate_behavior PRIVATE meta)