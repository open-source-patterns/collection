# Collection — Developer Guide

**A modular C library offering generic, thread-safe data structures for efficient collection management.**

---

## Building the Library

### 💻 Environment
**Windows**:
* Install CMake via Chocolatey `choco install cmake --installargs '"ADD_CMAKE_TO_PATH=System"'` (adds `C:\Program Files\CMake\bin` to PATH automatically) or download manually from [CMake.org](https://cmake.org/download) 
* Install Visual Studio Build Tools (MSVC): https://visualstudio.microsoft.com/downloads
* Add MSVC tools to PATH: `C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\<VERSION>\bin\Hostx64\x64`
* Verify: `cl && link && cmake --version`

**Mac**:
  * Install Xcode Command Line Tools: `xcode-select --install`  
  * Install Brew: `brew install cmake && echo 'export PATH="/opt/homebrew/bin:$PATH"' >> "$HOME/.zshrc"`
  * Verify: `clang --version && make --version && cmake --version`

**Linux**:
  * Install Build Tools: `sudo apt update && sudo apt install build-essential cmake`
  * Verify: `gcc --version && g++ --version && make --version && cmake --version`

---

## 🛠️ Build
### Debug + Test
```shell
mkdir -p build-debug
cmake -B build-debug -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTS=ON -DCMAKE_TOOLCHAIN_FILE=/Users/username/Documents/microsoft/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build-debug --parallel
ctest --test-dir build-debug --output-on-failure
```

### Debug + Sanitizer + Test
```shell
mkdir -p build-debug
cmake -B build-debug \
 -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTS=ON -DCMAKE_TOOLCHAIN_FILE=/Users/username/Documents/microsoft/vcpkg/scripts/buildsystems/vcpkg.cmake \
 -DCMAKE_C_FLAGS="-Wall -Werror -fsanitize=address,undefined -fno-sanitize-recover=all -fno-omit-frame-pointer" \
 -DCMAKE_EXE_LINKER_FLAGS="-fsanitize=address,undefined" 
cmake --build build-debug
ctest --test-dir build-debug --output-on-failure
```

### Release + Test
```shell
mkdir -p build-release
cmake -B build-release -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=ON -DCMAKE_TOOLCHAIN_FILE=/Users/username/Documents/microsoft/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build-release --parallel
ctest --test-dir build-release
```

### Documentation
```shell
brew install doxygen && doxygen -g # Installation and setup
doxygen && open docs/index.html # To re-generate docs
```

---

## VCPKG Publishing Guide 🚀

* **For and Clone VCPKG:**
    VCPKG Repository: https://github.com/microsoft/vcpkg
* Bootstrap 
  * Linux/MacOS: `./bootstrap-vcpkg.sh`
  * Windows: `.\bootstrap-vcpkg.bat` | `.\vcpkg integrate install`
* Add to PATH
    ```shell
    echo 'export VCPKG_ROOT="/path/to/vcpkg"' >> ~/.zshrc # Set VCPKG_ROOT (adjust the path as needed)
    echo 'export PATH="$VCPKG_ROOT:$PATH"' >> ~/.zshrc # Add VCPKG to your PATH
    source ~/.zshrc && vcpkg --version # reload and check the version
    ```
* **Develop Your Library**
  * Configure CLion: Settings → Build, Execution, Deployment → CMake -DCMAKE_TOOLCHAIN_FILE/Users/username/Documents/microsoft/vcpkg/scripts/buildsystems/vcpkg.cmake
  * [Tutorial](https://learn.microsoft.com/en-us/vcpkg/get_started/get-started?pivots=shell-powershell)

* **Create a VCPKG Port**
    - Add your port under `ports/collection/` with [vcpkg.json](https://learn.microsoft.com/en-us/vcpkg/reference/vcpkg-json), [usage](https://learn.microsoft.com/en-us/vcpkg/maintainers/handling-usage-files) and [portfile.cmake](https://learn.microsoft.com/en-us/vcpkg/concepts/ports)
    - Generate `SHA512` checksum:
        ```shell
        shasum -a 512 /path/to/collection.tar.gz
        vcpkg hash /path/to/collection-1.0.0.tar.gz
        curl -L https://github.com/open-source-patterns/collection/archive/refs/tags/1.0.0.tar.gz | sha512sum
        ```
      **SHA512 Hash:** Copy the output checksum into the `SHA512` field in `portfile.cmake`.

* **Test Your Port**
    * **VCPKG**:
        * In the `vcpkg/ports` directory, Edit vcpkg.json and Replace `vcpkg_from_github` with a local path override:
          `set(SOURCE_PATH "/Users/username/Documents/open-source-patterns/collection")`
        * Cleanup: Delete `project/cmake_build-debug`, `project/vcpkg_installed`, `~/.cache/vcpkg/archives`, File → Reload CMake Project
    * **Consumer**:
        * Configure CMake: `-DVCPKG_OVERLAY_PORTS=/Users/username/Documents/microsoft/vcpkg/ports`

* **Register Version**
  `vcpkg x-add-version collection`

* **Submit PR to Official vcpkg**

---

### 🔄 Update an Existing vcpkg Port

* **Update Your Library**:
    * Bump a version in `vcpkg.json`, `CMakeLists.txt`, and `CHANGELOG.md`
    * Tag the release: `git tag v1.0.1 && git push origin v1.0.1`

* **Update VCPKG Port**1
    * Update `vcpkg.json` version on VCPKG repository.
    * Update portfile.cmake: Update SHA512 and revert to `vcpkg_from_github` instead of a local path.
      ```shell
      shasum -a 512 /path/to/collection.tar.gz
      vcpkg hash /path/to/collection-1.0.1.tar.gz
      curl -L https://github.com/open-source-patterns/collection/archive/refs/tags/1.0.1.tar.gz | sha512sum
      ```

* **Update version**  
  `vcpkg x-add-version collection --overwrite-version`
  This updates version metadata and links the correct git tree.

* **Submit PR to Official vcpkg**
---

## Coding Guidelines

* Follow **thread-safety practices**: always lock mutexes when modifying shared data.
* Use **interface structs** (`IArray`, `IDictionary`) instead of direct implementation structs.
* Prefer **generic functions** to keep API consistent.
* Document public functions with Doxygen-style comments:

```c
/**
 * @brief Adds an item to the end of the array.
 * @param array Pointer to the IArray instance.
 * @param item Pointer to the item to insert.
 */
void array_push(IArray *array, void *item);
```

---

## Extending the Library

* Add new data structures by defining **interface structs** with function pointers.
* Implement concrete structs in `src/` and provide factory functions.
* Ensure **thread safety** if your structure is concurrent.
* Update **CMakeLists.txt** to include new sources and optionally add unit tests.

---

## Debugging & Logging

* Use `assert` for internal consistency checks.
* Optional: add error out-params (`const char **error`) for caller-friendly error handling.
* Logging is minimal — library errors should propagate via error strings.

---

## Contributing

* Fork the repository and submit pull requests.
* Follow BSD 3-Clause License guidelines.
* Include **unit tests** for any new functionality.
* Maintain **consistent code style and documentation**.

---

## References
* [Effective CMake](https://www.youtube.com/watch?v=rLopVhns4Zs)
* [It's Time To Do CMake Right](https://pabloariasal.github.io/2018/02/19/its-time-to-do-cmake-right/)
* [Versioning](https://github.com/MicrosoftDocs/vcpkg-docs/blob/main/vcpkg/contributing/maintainer-guide.md#versioning)
* [git-tree](https://learn.microsoft.com/en-us/vcpkg/maintainers/registries#obtain-git-tree-sha)
* [vcpkg](https://github.com/microsoft/vcpkg)
* [CMake Documentation](https://cmake.org/documentation/)
