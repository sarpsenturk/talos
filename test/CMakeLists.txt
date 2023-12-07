find_package(GTest CONFIG REQUIRED)
include(GoogleTest)

function(talos_add_test name)
    set(test_name ${name}_test)
    set(test_file ${name}.cpp)
    add_executable(${test_name} ${test_file})
    target_link_libraries(${test_name} talos_lib GTest::gtest_main)
    gtest_discover_tests(${test_name})
endfunction()

talos_add_test(talos)
talos_add_test(lexer)
