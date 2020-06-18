
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

add_executable(thptest ThreadPoolTest.cc)
target_link_libraries(thptest pthread)

add_executable(timestamptest TimeStampTest.cc)
target_link_libraries(timestamptest pthread)

add_executable(timertest TimerTest.cc)
target_link_libraries(timertest pthread)

add_executable(timermanagertest TimerManagerTest.cc)
target_link_libraries(timermanagertest conet_net)

