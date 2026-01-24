add_executable(ArrayTest ArrayTest.c)
target_link_libraries(ArrayTest PRIVATE collection::collection)
add_test(NAME Collection.ArrayTest COMMAND ArrayTest)

add_executable(DictionaryTest DictionaryTest.c)
target_link_libraries(DictionaryTest PRIVATE collection::collection)
add_test(NAME Collection.DictionaryTest COMMAND DictionaryTest)

add_executable(MutexTest MutexTest.c)
target_link_libraries(MutexTest PRIVATE collection::collection)
add_test(NAME Collection.MutexTest COMMAND MutexTest)
