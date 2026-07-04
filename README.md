# Collection

**A modular C library offering generic, thread-safe data structures designed for efficient collection management.**

[![License](https://img.shields.io/badge/license-BSD--3--Clause-blue.svg)](LICENSE)
[![CMake on multiple platforms](https://github.com/open-source-patterns/collection/actions/workflows/cmake-multi-platform.yml/badge.svg)](https://github.com/open-source-patterns/collection/actions/workflows/cmake-multi-platform.yml)

---

## Features

- 🧵 **Thread-safe** design for concurrent usage.
- 📦 **Generic interface** using `void *` for data storage.
- 🔁 **Singly linked list** implementation (`IArray`) with methods like `push`, `pop`, `unshift`, `shift`, `remove_item`, and `clone`.
- 🔑 **Hash-based dictionary** (`IDictionary`) with `get`, `put`, `replace`, `remove_item`, `contains_key`, and `clear`.
- 🧪 Optional item-level cleanup via callbacks.
- 💡 Clear separation of interfaces and concrete implementations.
- 🧩 Object-oriented architecture in C using interface structs and function pointers to simulate encapsulation and polymorphism.
- 🎯 Design pattern foundation: utilizes common design principles to enable flexible component interchangeability and extensibility.
---

## Getting Started

### 📖 Documentation 
[API Docs](https://open-source-patterns.github.io/collection)  
[Examples](https://github.com/open-source-patterns/collection/tree/main/examples)  
[Developer Guide](https://github.com/open-source-patterns/collection/blob/main/DEVELOPMENT.md)  

### 📦 Installation
#### vcpkg.json
```json
{
  "name": "example",
  "version": "1.0.0",
  "dependencies": [ "osp-collection" ]
}
```

#### CMakeLists.txt
```cmake
find_package(collection CONFIG REQUIRED)
target_link_libraries(example PRIVATE collection::collection)
```

## 📚 Usage

### IArray

```c++
#include "collection/collection.h"

struct IArray *array = collection_array_new();

array->push(array, "Apple");
array->push(array, "Banana");
array->push(array, "Orange");

while (array->count(array) > 0) {
    printf("Fruit: %s ", (const char *) array->shift(array)); // Prints: Apple Banana Orange
}

collection_array_dealloc(&array, NULL); // pass free if items are heap allocated
```

### IDictionary

```c++
#include "collection/collection.h"

struct IDictionary *dictionary = collection_dictionary_new();;

dictionary->put(dictionary, "name", "Alice");
dictionary->put(dictionary, "age", "30");

printf("Name: %s\n", (const char *) dictionary->get(dictionary, "name")); // Name: Alice
printf("Age: %s\n", (const char *) dictionary->get(dictionary, "age")); // Age: 30
    
collection_dictionary_dealloc(&dictionary, NULL); // pass free if items are heap allocated
```

## License

Copyright (c) 2025–2026 Open Source Patterns Inc.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.
3. Neither the name of the copyright holder nor the names of its contributors
   may be used to endorse or promote products derived from this software
   without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.