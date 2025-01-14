# Visualizing Sorting Algorithms

Visualizing different sorting algorithms is useful for a deeper understanding on how they work. In this project
the following algorithms are implemented:
- bubbleSort
- quickSort
- mergeSort
- heapSort
- selectionSort


## Prerequisites

1. Install cmake
2. If you use Linux, install SFML's dependencies using your system package manager. On Ubuntu and other Debian-based distributions you can use the following commands:
   ```
   sudo apt update
   sudo apt install \
       libxrandr-dev \
       libxcursor-dev \
       libudev-dev \
       libfreetype-dev \
       libflac-dev \
       libvorbis-dev \
       libgl1-mesa-dev \
       libegl1-mesa-dev \
       libfreetype-dev
   ```

## Build
1. Configure and build your project. Most popular IDEs support CMake projects with very little effort on your part.

   - [VS Code](https://code.visualstudio.com) via the [CMake extension](https://code.visualstudio.com/docs/cpp/cmake-linux)
   - [Visual Studio](https://docs.microsoft.com/en-us/cpp/build/cmake-projects-in-visual-studio?view=msvc-170)
   - [CLion](https://www.jetbrains.com/clion/features/cmake-support.html)
   - [Qt Creator](https://doc.qt.io/qtcreator/creator-project-cmake.html)

   Using CMake from the command line is straightforward as well.
   Be sure to run these commands in the root directory of the project you just created.

   ```
   cmake -B build
   cmake --build build
   ```
2. Compile with make (or ninja)
   ```
   cd build
   make
   ```
3. Run the binary `build/bin/main`

## Capture


https://github.com/user-attachments/assets/f84b68b3-7bdb-4ba6-a34b-3b22d7270da9



