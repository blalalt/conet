/*
 * @Description: one thread per reactor
 * @version: 
 * @Author: blalalt
 * @Date: 2020-05-20 18:17:58
 */ 

#include <thread>
#include <unordered_map>
#include <memory>
namespace conet {

enum class EventType {
    WRITE_EVENT,
    READ_EVENT,
    SINGAL_EVENT,
    CLOSE_EVENT,
    TIMEOUT_EVENT
};
class EventHandler;
class Dispatcher;
class TimerManager;

std::thread_local Dispatcher* g_loopInThisThread = nullptr;

class Dispatcher {
public:
    typedef int handle_t;

private:
    std::unordered_map<handle_t, std::shared_ptr<EventHandler> > handlers_;
    std::unique_ptr<TimerManager> timers_;
};


}