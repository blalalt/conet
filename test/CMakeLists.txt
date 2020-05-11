
find_package(Threads)

include_directories(../log)
aux_source_directory(../log LOG_SRC)
add_executable(logtest LogTest.cc ${LOG_SRC})
target_link_libraries (logtest ${CMAKE_THREAD_LIBS_INIT})

include_directories(../net) # 引入头文件
aux_source_directory(../net NET_SRC) # 指定源文件目录
add_executable(inetest INetAddr.cc ${NET_SRC})