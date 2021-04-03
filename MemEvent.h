#ifndef MEMEVENT
#define MEMEVENT
class MemoryEvent{
private:
    bool isWrite;
    int thread_identifier;
    int address;
public:
    MemoryEvent(int thread_identifier, int address, bool isWrite);
    MemoryEvent() = default;
    void DebugOutput();
    void ShortOutput();
};

#endif