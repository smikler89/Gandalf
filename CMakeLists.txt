PROJECT(rpc)
CMAKE_MINIMUM_REQUIRED (VERSION 3.0.0)
SET(CMAKE_CXX_FLAGS "-g -Wall -Werror -std=c++17")

find_package(Protobuf REQUIRED)
find_package(absl REQUIRED)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

ADD_SUBDIRECTORY(proto)
ADD_SUBDIRECTORY(util)
ADD_SUBDIRECTORY(manager)

ADD_EXECUTABLE(main main.cpp)
TARGET_LINK_LIBRARIES(
    main
    proto
    util
    manager
    absl::log_internal_message
    absl::log_internal_check_op
    absl::status
)
