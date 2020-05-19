/*
 * @Description: nothing
 * @version: 
 * @Author: blalalt
 * @Date: 2020-05-16 22:29:46
 */ 

enum class EventType {
    ACCEPT_EVENT = 01,
    READ_EVENT = 02,
    WRITE_EVENT = 04,
    TIMEOUT_EVENT = 010,
    SIGNAL_EVENT = 020,
    CLOSE_EVENT = 040
};

class Handle {
private:
    int fd_;
};

class Dispatcher {
public:
    // 当所有事件注册完成后，调用 handle_events 启动事件循环
    // 内部调用 Demultiplexer 等待 Handle 的事件发生
    void handle_events(int timeout) { // 事件循环 the event loop entry point
        std::vector<Handle> interest_handles; // 感兴趣的句柄
        std::vector<Handle> revents; // 返回事件的句柄
        demultiplexer_->select(interest_handles, revents);
        for (auto h : revents) {
            auto ev_handler = handlers_[h];
            ev_handler->handle_event(EventType et);
        }
    }
    void register_handler(EventHandler *h, EventType t); // 注册事件
    void remove_handler(EventHandler *h, EventType t); // 移除事件

private:
    // Dispatcher 需要 传递每个 EventHandler 的内部 Handle
    // 可以将两种不同类型的事件处理程序与相同的句柄关联
    std::map<Handle, EventHandler*> handlers_;
    // std::vector<EventHandler*> handlers_;
    EventDemultiplexer * demutliplexer_;
};


class EventDemultiplexer { // 多路复用

    void select();

};


class EventHandler {  // 事件处理, 和句柄绑定的
// 事件处理程序仅在反应器模式的实例中序列化。因此,多个
// 多个线程中的事件处理程序可以并行运行。
public:
    virtual void handle_event();
    Handle get_handle();
private:
    Handle handle_;
};

class ConcreteEventHandler : public EventHandler {
// 具体的事件处理程序通常由应用程序开发人员创建，以执行特定的服务来响应事件
public:
    void handle_event(EventType et) override {
        // assert(et == READ_EVENT); // 特定的事件，如listen只会处理可读事件
        switch (et)
        {
        case WRITE_EVENT:
            ...
            break;
        case READ_EVENT:
            ...
            break;
        case ...:
        default:
            break;
        }
    }
};

class AcceptorHandler : public EventHandler {
public:
    AcceptorHandler(INET_ADDR addr) {
        Dispatcher::instance()->register_handler(this, ACCEPT_EVENT);
    }

    void handle_event(EventType et) {
        assert(et == READ_EVENT);
        SOCK_Stream new_connection;
        acceptor_.accept (new_connection);
        Logging_Handler *handler = new Logging_Handler (new_connection);
    }

private:
    SOCK_Accpetor acceptor_;
}

class LoggingHandler : public EventHandler {
public:
    Logging_Handler (SOCK_Stream &cs) cs(cs) {
        Dispatcher::instance ()->register_handler (this, READ_EVENT);
    }

    void handle_event(EventType et) {
        if (et == READ_EVENT) {
            Log_Record log_record;

            peer_stream_.recv ((void *) log_record, sizeof log_record);

            // Write logging record to standard output.
            log_record.write (STDOUT);

            }
            else if (et == CLOSE_EVENT) {
            peer_stream_.close ();
            delete (void *) this;
            }
    }

private:
    SOCK_Stream cs;
}


int main (void)
{
    // Logging server port number.
    INET_Addr server_addr (PORT);

    // Initialize logging server endpoint and
    // register with the Initiation_Dispatcher.
    Logging_Acceptor la (server_addr);

    // Main event loop that handles client
    // logging records and connection requests.
    for (;;)
    Initiation_Dispatcher::instance ()->
    handle_events ();

    /* NOTREACHED */
    return 0;

}