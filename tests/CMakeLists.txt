set(This Tests)

set(TESTS_SRC
    Tests.cpp
)

add_executable(${This} ${TESTS_SRC})

target_compile_options(${This} PRIVATE
    ${PROJECT_CXX_FLAGS}
)

target_link_libraries(${This} PUBLIC
    gmock_main
)

include(GoogleTest)
gtest_add_tests(TARGET ${This})