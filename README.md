# MINI-GREP (PAOO – Homework 1)

A small C++ **mini-grep** that searches text files for one or more patterns and (at the same time) demonstrates core OOP & resource-management concepts required in the **PAOO** lab.

## Features

* **CMake build** with a clean project layout (`inc/`, `src/`).
* **Encapsulation** across three classes:

  * `FileBuffer` – owns file bytes
  * `Searcher` – matching logic (case-insensitive, whole-word)
  * `Cache` – stores search results
* **Constructor member initialization** & **RAII destructors** that free heap memory.
* **Copy vs. move semantics** with real use cases:

  * **Copy**: snapshot results and duplicate buffers for independent searches.
  * **Move**: transfer heavy objects into containers without deep copies.
* **Interactive demo menu** including intentional “bad” examples (shallow copy, incorrect move, missing destructor).

## Build

```bash
mkdir -p build
cd build
cmake -S . -B build
cmake --build build
```

## Run

```bash
./build/mini-grep [-i] [-w] [-m] <pattern> <file> [pattern2] [pattern3]
```

**Options**

* `-i` – case-insensitive search
* `-w` – whole-word matching
* `-m` – force multi-pattern mode (even with one pattern)

**Examples**

```bash
./build/mini-grep error test.txt
./build/mini-grep -i error test.txt Error ERROR
./build/mini-grep -w test code.cpp
```

> Run **without arguments** to open the **Demonstration Menu** (good/bad constructor & destructor scenarios).

## Project Structure

```
inc/
  Cache.hpp
  FileBuffer.hpp
  Searcher.hpp
src/
  Cache.cpp
  FileBuffer.cpp
  Searcher.cpp
  main.cpp
CMakeLists.txt
test.txt
```

## What’s Demonstrated

1. **Build with CMake** respecting the lab structure.
2. **Encapsulation** of class internals.
3. **Member initialization** in constructors.
4. **Heap cleanup** in destructors (RAII).
5. **Copy constructor** (deep copy to avoid double-free).
6. **Move constructor** (efficient ownership transfer).

For items **3–6**, the app includes clear, representative examples showing *why* each concept is needed (e.g., snapshot copies vs. moves into containers) and “bad” demos that print the pitfalls to the console.


## License

MIT License - free for educational and personal use
