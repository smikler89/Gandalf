set(SOURCES util.cpp io.cpp)

# Specify the Iostreams component explicitly
find_package(Boost REQUIRED COMPONENTS iostreams)
find_package(ZLIB REQUIRED)

add_library(util ${SOURCES})

# Ensure you're linking against the specific Boost Iostreams library
target_link_libraries(util 
    proto 
    Boost::iostreams  # Use this instead of ${Boost_LIBRARIES}
    ${ZLIB_LIBRARIES}
)

# Use target_include_directories instead of include_directories
target_include_directories(util PRIVATE 
    ${Boost_INCLUDE_DIRS} 
    ${ZLIB_INCLUDE_DIRS}
)
