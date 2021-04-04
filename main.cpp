#include <thread>
#include "MemEvent.h"
#include <vector>
#include <iostream>
#include <fstream>

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
    mem_1.push_back(MemoryEvent(0, 2, false, 1));
    mem_1.push_back(MemoryEvent(0, 1, false, 2));
    mem_1.push_back(MemoryEvent(0, 2, false, 3));
    
    mem_3.push_back(MemoryEvent(0, 1, false, 2));
    mem_3.push_back(MemoryEvent(0, 2, false, 3));

    mem_2.push_back(MemoryEvent(1, 1, true, 4));
    mem_2.push_back(MemoryEvent(1, 2, true, 5));
    mem_2.push_back(MemoryEvent(1, 1, true, 6)); 
    
    mem_4.push_back(MemoryEvent(1, 2, true, 5));
    mem_4.push_back(MemoryEvent(1, 1, true, 6));

    prefix_1.push_back(MemoryEvent(0, 2, false, 1));
    prefix_2.push_back(MemoryEvent(1, 1, true, 4));
}

void dfs2(std::vector<MemoryEvent>& target_list_1, std::vector<MemoryEvent>& prefix, void* data_location, bool* visited){
    bool all_visited = true;
    for(int i = 0; i < target_list_1.size(); ++i){
        if(!visited[i]) all_visited = false;
        if(!all_visited) break;
    }
    if (all_visited){
        std::vector<std::vector<MemoryEvent>> *p = reinterpret_cast<std::vector<std::vector<MemoryEvent>> *>(data_location);
        p->push_back(std::vector<MemoryEvent>(prefix));
        return;
    }

    for(int i = 0; i < target_list_1.size(); ++i){
        if(!visited[i]){
            prefix.push_back(target_list_1[i]);
            visited[i] = true;
            dfs2(target_list_1, prefix, data_location, visited);
            prefix.pop_back();
            visited[i] = false;
        }
    }

}
void doDispatch(){
    MemoryEventList.push_back(MemoryEvent(0, 2, false, 1));
    MemoryEventList.push_back(MemoryEvent(0, 1, false, 2));
    MemoryEventList.push_back(MemoryEvent(0, 2, false, 3));
    MemoryEventList.push_back(MemoryEvent(1, 1, true, 4));
    MemoryEventList.push_back(MemoryEvent(1, 2, true, 5));
    MemoryEventList.push_back(MemoryEvent(1, 1, true, 6)); 
    bool visited[6][6] = {
        {true, false, false, false, false, false}, 
        {false, true, false, false, false, false},
        {false, false, true, false, false, false},
        {false, false, false, true, false, false},
        {false, false, false, false, true, false},
        {false, false, false, false, false, true},
    };

    std::vector<std::vector<MemoryEvent>> *res1 = new std::vector<std::vector<MemoryEvent>>();    
    std::vector<std::vector<MemoryEvent>> *res2 = new std::vector<std::vector<MemoryEvent>>();
    std::vector<std::vector<MemoryEvent>> *res3 = new std::vector<std::vector<MemoryEvent>>();
    std::vector<std::vector<MemoryEvent>> *res4 = new std::vector<std::vector<MemoryEvent>>();
    std::vector<std::vector<MemoryEvent>> *res5 = new std::vector<std::vector<MemoryEvent>>();
    std::vector<std::vector<MemoryEvent>> *res6 = new std::vector<std::vector<MemoryEvent>>();

    std::vector<MemoryEvent> ref1, ref2, ref3, ref4, ref5, ref6;
    ref1.push_back(MemoryEventList[0]);
    ref2.push_back(MemoryEventList[1]);
    ref3.push_back(MemoryEventList[2]);
    ref4.push_back(MemoryEventList[3]);
    ref5.push_back(MemoryEventList[4]);
    ref6.push_back(MemoryEventList[5]);
    std::thread t1(dfs2, std::ref(MemoryEventList), std::ref(ref1), (void*) res1, visited[0]);
    std::thread t2(dfs2, std::ref(MemoryEventList), std::ref(ref2), (void*) res2, visited[1]);
    std::thread t3(dfs2, std::ref(MemoryEventList), std::ref(ref3), (void*) res3, visited[2]);
    std::thread t4(dfs2, std::ref(MemoryEventList), std::ref(ref4), (void*) res4, visited[3]);
    std::thread t5(dfs2, std::ref(MemoryEventList), std::ref(ref5), (void*) res5, visited[4]);
    std::thread t6(dfs2, std::ref(MemoryEventList), std::ref(ref6), (void*) res6, visited[5]);
    std::cout << "Calculating All result..." << std::endl;
    t1.join(); t2.join(); t3.join(); t4.join(); t5.join();
    t6.join();
    std::cout << "All result: " << std::endl;
    std::vector<std::vector<MemoryEvent>> allresult_sub;
    for(int i = 0; i < res1->size(); ++i) allresult_sub.push_back(res1->operator[](i));
    for(int i = 0; i < res2->size(); ++i) allresult_sub.push_back(res2->operator[](i));
    for(int i = 0; i < res3->size(); ++i) allresult_sub.push_back(res3->operator[](i));
    for(int i = 0; i < res4->size(); ++i) allresult_sub.push_back(res4->operator[](i));
    for(int i = 0; i < res5->size(); ++i) allresult_sub.push_back(res5->operator[](i));
    for(int i = 0; i < res6->size(); ++i) allresult_sub.push_back(res6->operator[](i));
    std::cout << "Total size: " << allresult_sub.size() << std::endl;
    std::ofstream rnd("RND.txt");
    for(int i = 0; i < allresult_sub.size(); ++i){
        std::cout << "#" << i << ": ";
        for(int j = 0; j < allresult_sub[i].size(); ++j) allresult_sub[i][j].ShortOutput();
        std::cout << " Result: ";
        int r[5];
        r[1] = r[2] = r[3] = 0;
        int watchlist[10];
        for(int i = 0; i < 10; ++i) watchlist[i] = -1;
        for(int j = 0; j < allresult_sub[i].size(); ++j) if(allresult_sub[i][j].getWrite()) r[allresult_sub[i][j].getAddr()]++; else {
            std::cout << allresult_sub[i][j].getAddr() << ": [" << (r[allresult_sub[i][j].getAddr()]) << "], ";
            watchlist[allresult_sub[i][j].getIndex()] = r[allresult_sub[i][j].getAddr()];
        }
        for(int i = 0; i < 10; ++i) rnd << watchlist[i] << " ";
        for(int j = 0; j < allresult_sub[i].size(); ++j) rnd << allresult_sub[i][j].getIndex() << (allresult_sub[i][j].getWrite() ? "W" : "L") << allresult_sub[i][j].getAddr() << " ";
        rnd << std::endl;
        std::cout << std::endl;
    }
    rnd.close();
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
    std::ofstream sc("SC.txt");
    std::cout << "SC Result: " << std::endl;
    for(int i = 0; i < AllLists.size(); ++i){
        std::cout << "#" << i << ": ";
        for(int j = 0; j < AllLists[i].size(); ++j) AllLists[i][j].ShortOutput();
        std::cout << " Result: ";
        int r[5];
        r[1] = r[2] = r[3] = 0;
        int watchlist[10];
        for(int i = 0; i < 10; ++i) watchlist[i] = -1;
        for(int j = 0; j < AllLists[i].size(); ++j) if(AllLists[i][j].getWrite()) r[AllLists[i][j].getAddr()]++; else {
            std::cout << AllLists[i][j].getAddr() << ": [" << (r[AllLists[i][j].getAddr()]) << "], ";
            watchlist[AllLists[i][j].getIndex()] = r[AllLists[i][j].getAddr()];
        }
        for(int i = 0; i < 10; ++i) sc << watchlist[i] << " ";
        for(int j = 0; j < AllLists[i].size(); ++j) sc << (AllLists[i][j].getWrite() ? "W" : "L") << AllLists[i][j].getAddr() << " ";
        sc << std::endl;
        std::cout << std::endl;
    }
    sc.close();
    doDispatch();
    return 0;
}