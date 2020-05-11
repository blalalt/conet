
set(NET_SRC INetAddress.cc Socket.cc)

add_library(conet_net ${NET_SRC})
target_link_libraries(conet_net conet_log)