cmake_minimum_required(VERSION 3.2)

if($ENV{TRAVIS_BUILD_NUMBER})
    set(VERSION_BUILD $ENV{TRAVIS_BUILD_NUMBER})
else()
    set(VERSION_BUILD 0)
endif()

project(matrix VERSION 1.0.${VERSION_BUILD})

add_subdirectory(src)
add_subdirectory(tests)

enable_testing()
