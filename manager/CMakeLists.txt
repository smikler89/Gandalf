set(SOURCES manager_factory.cpp)

add_library(manager ${SOURCES})

# Ensure you're linking against the specific Boost Iostreams library
target_link_libraries(
    manager
    util 
    proto 
)

# Use target_include_directories instead of include_directories
target_include_directories(manager PRIVATE 
)
