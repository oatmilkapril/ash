cmake_minimum_required(VERSION 3.15)
project(cash C)

add_subdirectory(deps/libapril)

add_executable(cash src/evaluate.c
        src/main.c
        src/parse.c
        src/command.c
        src/builtin.c
        src/jobs.c
        src/signals.c)

target_include_directories(cash PRIVATE include/)
target_link_libraries(cash PRIVATE april)
