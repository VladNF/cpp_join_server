cmake_minimum_required(VERSION 3.2)

if(NOT DEFINED ENV{TRAVIS_BUILD_NUMBER})
    set(ENV{TRAVIS_BUILD_NUMBER} 42)
endif()

project(hw10_join_server VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})

# Boost
find_package(Boost COMPONENTS system thread REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

# Immer
include_directories("immer")

# Targets
add_executable(
        join_server main.cpp
        command.h command.cpp
        db.h db.cpp
        table.h table.cpp
)

set_target_properties(join_server PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        )

target_link_libraries(join_server
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_THREAD_LIBRARY}
        ${CMAKE_THREAD_LIBS_INIT}
        )

if (MSVC)
    target_compile_options(
            join_server PRIVATE
            /W4
    )
else ()
    target_compile_options(
            join_server PRIVATE
            -Wall -Wextra -pedantic -Werror -Wno-unused-parameter
    )
endif()



install(TARGETS join_server RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT cpp-pro@otus.ru)

include(CPack)
