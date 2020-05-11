
set(LOG_SRC LogFile.cc Logger.cc Logging.cc )
add_library(conet_log ${LOG_SRC})
target_link_libraries(conet_log pthread)