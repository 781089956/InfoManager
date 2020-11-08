# cmake_minimum_required(VERSION <specify CMake version here>)
cmake_minimum_required(VERSION 3.17)
project(InfoManager C)

set(CMAKE_C_STANDARD 99)

include_directories(include)
link_directories(lib)
add_compile_options(-fexec-charset=gbk -finput-charset=UTF-8)
add_executable(Main src/main.c src/linkList.c src/fileHandler.c src/dataUtil.c src/stack.c src/query.c src/util.c src/sys.c include/sys.h src/analysis.c)
add_executable(Test test.c)
set_target_properties(Main PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
target_link_libraries(Main regex)