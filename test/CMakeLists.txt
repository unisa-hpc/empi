# Find catch_discore_tests
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/lib/Catch2/contrib)
include(CTest)
include(Catch)

# Cache Catch2 compilation by creating a static library
add_library(Catch2Main STATIC static_main.cpp)
target_link_libraries(Catch2Main Catch2::Catch2)

# Link examples to Catch2Main
# add_executable(foo foo.cpp)
# target_link_libraries(foo Catch2Main)
# catch_discover_tests(foo)