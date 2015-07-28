add_executable(little-big-int-test unit.cpp)

target_link_libraries(little-big-int-test littlebigint gtest gtest_main)

add_test(LittleBigIntTest little-big-int-test)

