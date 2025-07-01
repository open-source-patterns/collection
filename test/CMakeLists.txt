add_executable(array_test test_array.c)
target_link_libraries(array_test PRIVATE collection)
add_test(NAME Collection.ArrayTest COMMAND array_test)

add_executable(dictionary_test test_dictionary.c)
target_link_libraries(dictionary_test PRIVATE collection)
add_test(NAME Collection.DictionaryTest COMMAND dictionary_test)
