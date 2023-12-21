cmake_minimum_required(VERSION 3.5)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(
  shiro
  VERSION 0.0.1
  LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

include(FindPkgConfig)
find_package(PkgConfig REQUIRED)

pkg_check_modules(READLINE REQUIRED readline)

if(NOT READLINE_FOUND)
  message(ERROR "readline is required for building.")
endif()

set(SHIRO_INTEP_SOURCES shiro/shiro.c shiro/argparse.c shiro/shell.c
                        shiro/interp.c)

add_executable(shiro ${SHIRO_INTEP_SOURCES})
target_link_libraries(shiro PRIVATE ${READLINE_LIBRARIES})
target_include_directories(shiro PRIVATE ${READLINE_INCLUDE_DIRS})
target_compile_options(shiro PRIVATE ${READLINE_CFLAGS_OTHER})
