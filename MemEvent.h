#ifndef MEMEVENT
#define MEMEVENT
class MemoryEvent{
private:
    bool isWrite;
    int thread_identifier;
    int address;
    int index;
public:
    MemoryEvent(int thread_identifier, int address, bool isWrite, int index);
    MemoryEvent() = default;
    void DebugOutput();
    void ShortOutput();
    bool getWrite();
    int getAddr();
    int getIndex();
};

#endif