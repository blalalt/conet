set(UTILS_SRC ThreadPoll.cc ReadWriteLock.cc SpinLock.cc)

add_library(conet_util ${UTILS_SRC})
target_link_libraries(conet_util conet_log)