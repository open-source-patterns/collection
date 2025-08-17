add_executable(TestArray TestArray.c)
target_link_libraries(TestArray PRIVATE collection::collection)
add_test(NAME Collection.ArrayTest COMMAND TestArray)

add_executable(TestDictionary TestDictionary.c)
target_link_libraries(TestDictionary PRIVATE collection::collection)
add_test(NAME Collection.DictionaryTest COMMAND TestDictionary)
