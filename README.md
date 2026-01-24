# Collection

**A modular C library offering generic, thread-safe data structures designed for efficient collection management.**

[![License](https://img.shields.io/badge/license-BSD--3--Clause-blue.svg)](LICENSE)
[![CMake on multiple platforms](https://github.com/open-source-patterns/collection/actions/workflows/cmake-multi-platform.yml/badge.svg)](https://github.com/open-source-patterns/collection/actions/workflows/cmake-multi-platform.yml)

---

## Features

- 🧵 **Thread-safe** design for concurrent usage.
- 📦 **Generic interface** using `void *` for data storage.
- 🔁 **Singly linked list** implementation (`IArray`) with methods like `push`, `pop`, `unshift`, `shift`, `removeItem`, and `clone`.
- 🔑 **Hash-based dictionary** (`IDictionary`) with `get`, `put`, `replace`, `removeItem`, `containsKey`, and `clear`.
- 🧪 Optional item-level cleanup via callbacks.
- 💡 Clear separation of interfaces and concrete implementations.
- 🧩 Object-oriented architecture in C using interface structs and function pointers to simulate encapsulation and polymorphism.
- 🎯 Design pattern foundation: utilizes common design principles to enable flexible component interchangeability and extensibility.
---

## Getting Started

### 📖 Documentation 
[API Docs](https://open-source-patterns.github.io/collection)

### 📦 Installation
Add `"osp-collection"` to the `dependencies` array in your `vcpkg.json` file, then run: `vcpkg install`

## 📚 Usage

### IArray

```c++
#include "collection/array.h"

struct IArray *array = collection_array_new();
array->push(array, "Apple");
array->push(array, "Banana");

while (array->size(array) > 0) {
    printf("Fruit: %s\n", (const char *)array->shift(array));
}

collection_array_free(&array);
```

### IDictionary

```c++
#include "collection/dictionary.h"

struct IDictionary *dict = collection_dictionary_new();
dict->put(dict, "name", "Alice");
dict->put(dict, "age", "30");

printf("Name: %s\n", (const char *)dict->get(dict, "name"));
dict->removeItem(dict, "age");

collection_dictionary_free(&dict);
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