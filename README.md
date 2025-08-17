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
Add `"collection"` to the `dependencies` array in your `vcpkg.json` file, then run: `vcpkg install`

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
---

## 🛠️ Build
### Debug
```shell
mkdir -p build && cd build
cmake -DBUILD_TESTS=ON -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .
ctest -C Debug
```

### Release
```shell
mkdir -p build-release && cd build-release
cmake -DBUILD_TESTS=ON -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
ctest -C Release
```
### Documentation
```shell
brew install doxygen && doxygen -g # Installation and setup
doxygen && open docs/index.html # To re-generate docs
```

### CI / Sanitizer Flags 
```shell
cmake -B build \
 -DCMAKE_C_FLAGS="-Wall -Werror -fsanitize=address,undefined -fno-sanitize-recover=all -fno-omit-frame-pointer" \
 -DCMAKE_EXE_LINKER_FLAGS="-fsanitize=address,undefined" \
 -DCMAKE_BUILD_TYPE=Debug
```
---

### 💻 Development
**Windows**:
* Install CMake via Chocolatey and ensure `C:\Program Files\CMake\bin` is in your environment PATH   
  `choco install cmake --installargs '"ADD_CMAKE_TO_PATH=System"'`
* Follow the guide to install the MinGW-w64 toolchain via MSYS2: [VS Code MinGW Setup](https://code.visualstudio.com/docs/cpp/config-mingw)

**Mac**: `brew install cmake && echo 'export PATH="/opt/homebrew/bin:$PATH"' >> "$HOME/.zshrc"`
**Linux**:`sudo apt update && sudo apt install cmake build-essential && sudo apt install mingw-w64`

---

### 🚀 VCPKG Publishing Guide

* **Setup vcpkg:**
  Fork and Clone vcpkg Repository: https://github.com/microsoft/vcpkg
 
  ```shell
  ./bootstrap-vcpkg.sh  # Linux/macOS  
  .\bootstrap-vcpkg.bat # Windows
  .\vcpkg integrate install # it's only useful for Visual Studio integration on Windows.
  
  echo 'export VCPKG_ROOT="/path/to/vcpkg"' >> ~/.zshrc # Set VCPKG_ROOT (adjust the path as needed)
  echo 'export PATH="$VCPKG_ROOT:$PATH"' >> ~/.zshrc # Add VCPKG to your PATH
  source ~/.zshrc && vcpkg --version # reload and check the version
  ```

* Pass to CMake: `-DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake`

* **Develop Your Library**  [Tutorial](https://learn.microsoft.com/en-us/vcpkg/get_started/get-started?pivots=shell-powershell)

* **Create a vcpkg Port**
  - Add your port under `ports/collection/` with [vcpkg.json](https://learn.microsoft.com/en-us/vcpkg/reference/vcpkg-json), [usage](https://learn.microsoft.com/en-us/vcpkg/maintainers/handling-usage-files) and [portfile.cmake](https://learn.microsoft.com/en-us/vcpkg/concepts/ports)
  - Generate `SHA512` checksum:
      ```shell
      shasum -a 512 /path/to/collection.tar.gz
      vcpkg hash /path/to/collection-1.0.0.tar.gz
      curl -L https://github.com/open-source-patterns/collection/archive/refs/tags/1.0.0.tar.gz | sha512sum
      ```
      **SHA512 Hash:** Copy the output checksum into the `SHA512` field in `portfile.cmake`.

* **Test Your Port**
  * **Ports**: 
    * Inside your local ports/ directory, create a folder named after your library containing portfile.cmake, usage and vcpkg.json
    * Edit vcpkg.json and Replace `vcpkg_from_github` with a local path override:
        `set(SOURCE_PATH "/path/to/library")`
    * Cleanup: consumer/cmake_build-debug (File - Reload CMake Project) | consumer/vcpkg_installed | ~/.cache/vcpkg/archives | /path/to/vcpkg/packages
  * **Consumer**:
    * Pass the option to CMake: `-DVCPKG_OVERLAY_PORTS=/path/to/ports`
    * Install the port using an overlay: `vcpkg install --overlay-ports=/path/to/ports` 

* **Register Version**
  `vcpkg x-add-version collection`
* **Submit PR to Official vcpkg**

---

### 🔄 Update an Existing vcpkg Port

* **Update Your Library**: 
  * Bump a version in `vcpkg.json`, `CMakeLists.txt`, and `CHANGELOG.md`
  * Tag the release: `git tag v1.1.0 && git push origin v1.1.0`

* **Update vcpkg Port**
  * Update `vcpkg.json` version on vcpkg repository.
  * Update portfile.cmake `SHA512`
    ```shell
    shasum -a 512 /path/to/collection.tar.gz
    vcpkg hash /path/to/collection-1.0.0.tar.gz
    curl -L https://github.com/open-source-patterns/collection/archive/refs/tags/1.0.0.tar.gz | sha512sum
    ```

* **Update version**  
  `vcpkg x-add-version collection --overwrite-version`
  This updates version metadata and links the correct git tree.
---

## 📌 Reference
* [Effective CMake](https://www.youtube.com/watch?v=rLopVhns4Zs)  
* [It's Time To Do CMake Right](https://pabloariasal.github.io/2018/02/19/its-time-to-do-cmake-right/) 
* [Versioning](https://github.com/MicrosoftDocs/vcpkg-docs/blob/main/vcpkg/contributing/maintainer-guide.md#versioning)
* [git-tree](https://learn.microsoft.com/en-us/vcpkg/maintainers/registries#obtain-git-tree-sha)  
