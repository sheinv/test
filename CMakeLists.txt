cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME test)

set(CMAKE_CXX_STANDARD 17)

project(${PROJECT_NAME})

find_package(Boost 1.66.0 REQUIRED system log)

if(Boost_FOUND)

    message(status "** Boost was found:")
    message(status "** Boost Include: ${Boost_INCLUDE_DIR}")
    message(status "** Boost Libraries: ${Boost_LIBRARY_DIRS}")
    message(status "** Boost Libraries: ${Boost_LIBRARIES}")

    include_directories(${Boost_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARY_DIRS})

    add_definitions(
        #-D_SILENCE_CXX17_RESULT_OF_DEPRECATION_WARNING
        -DBOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE
        -DBOOST_ALL_DYN_LINK
        -D_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
        -D_WINSOCK_DEPRECATED_NO_WARNINGS
        -DBOOST_USE_WINAPI_VERSION=0x0601
        -wd4834
        -D_WIN32_WINNT=0x0A00
        )

    add_executable(${PROJECT_NAME}
        main.cpp
        proxy_srv.h
        )

    target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})

    message(STATUS "Boost libraries: ${Boost_LIBRARIES}")
else()
    message(FATAL_ERROR "Boost 1.66 wasn't found.")
endif()
