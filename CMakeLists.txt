cmake_minimum_required(VERSION 3.20)
project (pubsub_example LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG v1.12.0
)
FetchContent_MakeAvailable(spdlog)

add_executable(pubsub_example
    src/main.cpp
)

target_include_directories(pubsub_example PRIVATE src)
target_link_libraries(pubsub_example PRIVATE spdlog::spdlog)

# Some distros need this explicitly when linking with libncursesw.
if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
  target_link_libraries(pubsub_example PRIVATE pthread)
endif()

# Tests
enable_testing()

add_executable(pubsub_tests
    tests/broker_test.cpp
    tests/channel_test.cpp
)

target_include_directories(pubsub_tests PRIVATE src)
target_link_libraries(pubsub_tests GTest::gtest_main spdlog::spdlog pthread)

include(GoogleTest)
gtest_discover_tests(pubsub_tests)