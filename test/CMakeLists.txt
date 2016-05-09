add_executable(little-big-int-test unit.cpp)

target_link_libraries(little-big-int-test littlebigint gtest gtest_main)

# We're overriding the test with a custom command for individual test output
# and colors, which cmake doesn't like.
set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS 1 CACHE INTERNAL "No dev warnings")

add_custom_target(
    test
    COMMAND little-big-int-test
    DEPENDS little-big-int-test
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)

