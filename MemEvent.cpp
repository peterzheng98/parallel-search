#include "MemEvent.h"
#include <iostream>

MemoryEvent::MemoryEvent(int thread_identifier, int address, bool isWrite) : thread_identifier(thread_identifier), address(address), isWrite(isWrite) { }

void MemoryEvent::DebugOutput(){
    std::cout << "Memory access: " << address << "[" << thread_identifier << "]" << " Operation: " << (isWrite ? "Write" : "Read") << std::endl;
}

void MemoryEvent::ShortOutput(){
    std::cout << (isWrite ? "W" : "L") << address << " ";
}

bool MemoryEvent::getWrite(){ return this->isWrite; }

int MemoryEvent::getAddr(){ return this->address; }