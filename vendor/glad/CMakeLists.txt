cmake_minimum_required(VERSION 3.12)
project(glad C)

# Add the glad source file
set(GLAD_SRC
    src/gl.c
)

# Create a static library
add_library(glad STATIC ${GLAD_SRC})
add_library(glad::glad ALIAS glad)
set_target_properties(glad PROPERTIES OUTPUT_NAME glad)

# Set include directories
target_include_directories(glad PUBLIC include)

# If you're using C++, you might want to set the C++ standard
# Uncomment the following line if needed
# set_target_properties(glad PROPERTIES CXX_STANDARD 11)
target_compile_features(glad PRIVATE cxx_std_20)

# If you need to link against any system libraries (like dl on Linux),
# you can do so here. For example:
# if(UNIX AND NOT APPLE)
#     target_link_libraries(glad PRIVATE dl)
# endif()

# Install rules (optional)
# install(TARGETS glad
#         ARCHIVE DESTINATION lib
#         LIBRARY DESTINATION lib
#         RUNTIME DESTINATION bin)

# install(DIRECTORY include/ DESTINATION include)