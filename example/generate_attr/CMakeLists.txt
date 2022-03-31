add_executable(generate_attr
    generate_attr.cpp)
target_link_libraries(generate_attr PUBLIC metacore)
set_target_properties(generate_attr PROPERTIES INSTALL_RPATH "$ORIGIN;$ORIGIN/${LIBRARY_RELATIVE_PATH}")