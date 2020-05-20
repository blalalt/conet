/*
 * @Description: IO监听定时器
 * @version: 
 * @Author: blalalt
 * @Date: 2020-05-18 14:55:46
 */ 


namespace conet {

class TimerHandle {
public:
    TimerHandle();
    ~TimerHandle();
    void open();
    
private:
    int handle_;
}


}