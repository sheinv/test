cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME test)

set(CMAKE_CXX_STANDARD 17)

project(${PROJECT_NAME})

find_package(Boost 1.66.0 REQUIRED system log)

if(Boost_FOUND)

    message(STATUS "** Boost was found:")
    message(STATUS "** Boost Include: ${Boost_INCLUDE_DIR}")
    message(STATUS "** Boost Libraries: ${Boost_LIBRARY_DIRS}")
    message(STATUS "** Boost Libraries: ${Boost_LIBRARIES}")

    include_directories(${Boost_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARY_DIRS})

    IF(WIN32) # Check if we are on Windows
        if(MSVC) # Check if we are using the Visual Studio compiler

            message(STATUS "Detected WIN32 && MSVC")

            SET(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} /SUBSYSTEM:WINDOWS") # Tell the project how to behave in this environment

            add_definitions(
                #-D_SILENCE_CXX17_RESULT_OF_DEPRECATION_WARNING
                -DBOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE
                -DBOOST_ALL_DYN_LINK
                -D_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
                -D_WINSOCK_DEPRECATED_NO_WARNINGS
                -D_SCL_SECURE_NO_WARNINGS
                -DBOOST_USE_WINAPI_VERSION=0x0A00
                -wd4834
                -D_WIN32_WINNT=0x0A00
                )

            add_executable(${PROJECT_NAME}
                    main.cpp
                    proxy_srv.h
                    tds_proto.h
                )

        elseif(CMAKE_COMPILER_IS_GNUCXX)
                #SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows") # Tell the project how to behave in this environment
                message(SEND_ERROR "Unfortunately GNUCXX on Windows is not supported for now")
        else()
                message(SEND_ERROR "You are using an unsupported Windows compiler! (Not MSVC or GCC)")
        endif()
    elseif(UNIX)
        message(SEND_ERROR "Unfortunately Unix compiler is not supported for now")
    else()
      message(SEND_ERROR "You are on an unsupported platform! (Not Win32 or Unix)")
    ENDIF()

    target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})

    message(STATUS "Boost libraries: ${Boost_LIBRARIES}")
else()
    message(FATAL_ERROR "Boost 1.66 wasn't found.")
endif()
