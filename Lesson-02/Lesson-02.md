# Lesson 2 - Basic CMake build system

A build system compiles your code, links against libraries, and resolves references across modules.  Some build systems, like Make files, are fairly portable across different OS/compiler combinations.  While others, like Visual Studio msbuild, are very limited as to where they can be used.

CMake is a meta-build system.  It can generate build files for many different build systems across OS/compiler combinations.  However, it's another language you have to learn.  Luckily, we're just going to look at the very basic usage.


## Step 1 - Create CMakeLists.txt

CMake will look for a file called CMakeLists.txt at the specified build point in your file system.  There can be many CMakeLists.txt files each referred to in a heirarchy beginning from that specified build point.

For our purposes, we will only need one.  You can create a file under Linux by "touch"ing it with the following command:
```
touch CMakeLists.txt
```

Technically, "touch" sets the date/time of the file to the date/time specified or now if none is specified.  However, it will create the file if it doesn't exist.  Therefore, it is very common for users to "touch" a file into existence, then edit it with "Vi" or another in-terminal editor.


## Step 2 - Define the minimum CMake version

The first thing you will add to every CMakeLists.txt file is a call to cmake_minimum_required().  This tells CMake the minimum version of CMake that can process the file due to the feature set it requires.  Modern versions of CMake will complain if this is less than 3.10, so that's what we'll tell it.

Add the following line:
```
cmake_minimum_required(VERSION 3.10)
```

If you found that you wanted to use a feature found in let's say 3.15, then you'd use that versin number instead.  Lower verions of CMake will stop processing the file when they see a greater version required.  The user then has to install a newer package in order to continue.


## Step 3 - Declare the project

Next, we need to declare the project.  This requires the name of the current project, and you can optionally add a version or the types of source files that are acceptable.  We'll just stick to the name.

Add the following line:
```
project(Lesson-02)
```

This declaration only needs to happen once, even in a huge project with many submodules.  However, you can break a larger project into multiple smaller ones, or your project may depend on another that is part of a third-party library.


## Step 4 - Prevent in-source builds

This isn't strictly necessary, but I don't like projects that are polluted with build files.  General wisdom is not to check build artifacts into your source repo, as they may cause problems for others or on other systems.  Some people try to hide these away by putting every kind of build artifact into their .gitignore file, but I find it easier to just not allow it in the first place.

Add the following lines:
```
if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
    message(FATAL_ERROR "In-source builds are not allowed.  Change directory to a different location and build again.")
endif()
```

"${CMAKE_SOURCE_DIR}" is the top-level source directory, and "${CMAKE_BINARY_DIR}" is the top-level binary (or output) directory.  Here we use a conditional with the "STREQUAL" operator to compare these strings.  We throw a **fatal** error if this occurs.


## Step 5 - Specify C++ standard

Modern C++ (usually versions since C++03) brings many new features that you may or may not want to allow in your code.  You can decide this by telling CMake which version you want, whether it is required versus optional, and whether you will allow compiler-specific extensions.

Add the following lines:
```
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
```

This tells the compiler that if we had C++ files, which we don't, we want them compiled with the C++17 standard.  We don't just want it, but it is **required** to compile our system.  Finally, we don't want any goofy Microsoft (or other) extensions to the standard.  We're using a cross-platform build system, so we probably want it to compile on as many systems as possible.


## Step 6 - Specify other compiler directives

There are many compiler directives and defintions that you could choose to add, and you'll need to figure those out for yourself when the time comes.  However, lets go ahead and add one for the sake of example now.

Add the following line:
```
add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/MP>)
```

This add the compiler option "/MP", which allows the compiler to build using multiple processes (copies of itself).  However, it only does this if the compiler is Microsoft Visual C (MSVC).  That generator expression will resolve to "/MP" when using MSVC, and it'll resolve to nothing using any other compiler.  This option can significantly decrease compile time when using MSVC, so I usually add it to my CMakeLists.txt.


## Step 7 - Tell CMake how to build the executable

CMake has several common target types, including static libraries, shared libraries, and executables.  Since we want it to build an executable from our main.c file, we need to tell it to do that.

Add the following lines:
```
add_executable(Lesson-02
    main.c
)
```

This directs CMake to build an executable called "Lesson-02" using main.c as the only source file.


## Step 8 - Generating the build system

To generate the build system, you need to tell CMake what directory will be the "${CMAKE_SOURCE_DIR}" and what directory will be the "${CMAKE_BINARY_DIR}".

Use the following command:
```
cmake -S . -B build
```

This tells CMake that the current directory will be the source directory, so it will start with the CMakeLists.txt found there.  It also tells it to build into a directory called "build", which is relative to the current directory, since it doesn't begin with "/" or another directory.


## Step 9 - Building it

To execute the build system, you can once again invoke CMake in a platform-independent way.  No need to directly call msbuild or make.

Use the following command:
```
cmake --build build --config Release
```

This tells CMake to invoke the build system for the current platform using the build system that is in the "build" directory.  It will use the "Release" configuration.  "Release" produces optimized code intended for release, while "Debug" produces unoptimized code that includes internal debug symbols.

When the build is complete, you should find a binary named "Lesson-02" under the build directory.  You can run it by invoking the following command:
```
cd build
./Lesson-02
```

The "./" means "in this directory", and it is required when you run a binary in the current directory on Linux.


## Conclusion

There is **so much more** to learn about CMake, but this covers the basics.  It should be enough to cover our needs.
