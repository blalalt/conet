
find_package(Threads)

add_executable(logtest LogTest.cc ${LOG_SRC})
target_link_libraries (logtest conet_log)

add_executable(inetest INetAddr.cc )
target_link_libraries (inetest conet_net)

add_executable(socktest SocketTest.cc)
target_link_libraries (socktest conet_net)

add_executable(acceptest AcceptorTest.cc)
target_link_libraries (acceptest conet_net)

add_executable(conqueue CQueueTest.cc)
target_link_libraries(conqueue pthread)