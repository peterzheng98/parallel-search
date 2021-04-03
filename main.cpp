#include <thread>
#include "MemEvent.h"
#include <vector>
#include <iostream>

std::vector<MemoryEvent> MemoryEventList, mem_1, mem_2, mem_3, mem_4, prefix_1, prefix_2;
std::vector<std::vector<MemoryEvent>> AllLists;


void dfs(std::vector<MemoryEvent>& target_list_1, std::vector<MemoryEvent>& target_list_2, int idx1, int idx2, std::vector<MemoryEvent>& prefix, void* data_location){
    if(target_list_1.size() == idx1 && target_list_2.size() == idx2){
        std::vector<std::vector<MemoryEvent>> *p = reinterpret_cast<std::vector<std::vector<MemoryEvent>> *>(data_location);
        p->push_back(std::vector<MemoryEvent>(prefix));
        return;
    }
    if(target_list_1.size() != idx1) {
        prefix.push_back(target_list_1[idx1]);
        dfs(target_list_1, target_list_2, idx1 + 1, idx2, prefix, data_location);
        prefix.pop_back();
    }
    if(target_list_2.size() != idx2) {
        prefix.push_back(target_list_2[idx2]);
        dfs(target_list_1, target_list_2, idx1, idx2 + 1, prefix, data_location);
        prefix.pop_back();
    }
}

void doInitialize(){
    mem_1.push_back(MemoryEvent(0, 1, false));
    mem_1.push_back(MemoryEvent(0, 2, false));
    mem_1.push_back(MemoryEvent(0, 1, false));
    mem_1.push_back(MemoryEvent(0, 2, false));
    
    mem_3.push_back(MemoryEvent(0, 2, false));
    mem_3.push_back(MemoryEvent(0, 1, false));
    mem_1.push_back(MemoryEvent(0, 2, false));

    mem_2.push_back(MemoryEvent(1, 2, true));
    mem_2.push_back(MemoryEvent(1, 1, true));
    mem_2.push_back(MemoryEvent(1, 2, true)); 
    mem_2.push_back(MemoryEvent(1, 1, true));
    
    mem_4.push_back(MemoryEvent(1, 1, true));
    mem_4.push_back(MemoryEvent(1, 2, true));
    mem_2.push_back(MemoryEvent(1, 1, true));

    prefix_1.push_back(MemoryEvent(0, 1, false));
    prefix_2.push_back(MemoryEvent(1, 2, true));
}

int main(){
    doInitialize();
    std::vector<std::vector<MemoryEvent>> *p = new std::vector<std::vector<MemoryEvent>>();
    std::vector<std::vector<MemoryEvent>> *q = new std::vector<std::vector<MemoryEvent>>();
    std::thread t1(dfs, std::ref(mem_1), std::ref(mem_4), 0, 0, std::ref(prefix_2), (void*) p);
    std::thread t2(dfs, std::ref(mem_3), std::ref(mem_2), 0, 0, std::ref(prefix_1), (void*) q);
    std::cout << "Calculating..." << std::endl;
    t1.join();
    t2.join();
    for(int i = 0; i < p->size(); ++i) AllLists.push_back(p->operator[](i));
    for(int i = 0; i < q->size(); ++i) AllLists.push_back(q->operator[](i));
    for(int i = 0; i < AllLists.size(); ++i){
        std::cout << "#" << i << ": ";
        for(int j = 0; j < AllLists[i].size(); ++j) AllLists[i][j].ShortOutput();
        
        std::cout << std::endl;
    }
    return 0;
}