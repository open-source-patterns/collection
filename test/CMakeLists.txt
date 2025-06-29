message(STATUS "[Tests] BUILD_TESTS is ON — configuring test targets")

add_executable(array_test array_test.c)
target_link_libraries(array_test PRIVATE collection)
add_test(NAME Collection.ArrayTest COMMAND array_test)

add_executable(dictionary_test dictionary_test.c)
target_link_libraries(dictionary_test PRIVATE collection)
add_test(NAME Collection.DictionaryTest COMMAND dictionary_test)
