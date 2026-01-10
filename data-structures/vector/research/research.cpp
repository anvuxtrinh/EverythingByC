#include <iostream>
#include <vector>
#include <string>
#include <fstream>

// ANSI color codes
constexpr const char* CLR_RESET   = "\x1b[0m";
constexpr const char* CLR_RED     = "\x1b[31;1m";
constexpr const char* CLR_GREEN   = "\x1b[32;1m";
constexpr const char* CLR_YELLOW  = "\x1b[33;1m";
constexpr const char* CLR_CYAN    = "\x1b[36;1m";

#define ELEMENT_NUMBER_TC1 1000000
#define ELEMENT_NUMBER_TC2 10

void TC1();
void TC2();
void pushElementsToVector(std::vector<int>& vec, int number);
void report();


void TC3();
void TC4();

int main(){
    std::cout << std::endl;

    std::cout << CLR_GREEN << "=============== TC1 ===============" << CLR_RESET << std::endl;
    TC1();

    std::cout << std::endl;

    std::cout << CLR_GREEN << "=============== TC2 ===============" << CLR_RESET << std::endl;
    TC2();

    return 0;
}

void TC1(){
    std::vector<int> vec;
    std::cout << CLR_YELLOW << "Large elements case:" << CLR_RESET << std::endl;
    pushElementsToVector(vec, ELEMENT_NUMBER_TC1);
    report();
    std::cout << std::endl;
    std::cout << CLR_YELLOW << "Small elements case:" << CLR_RESET << std::endl;
    vec.clear();
    pushElementsToVector(vec, ELEMENT_NUMBER_TC2);
    report();
}

void pushElementsToVector(std::vector<int>& vec, int number){
    for(int i = 0; i < number; i++){
        vec.push_back(i);
    }
}

void report(){
    bool heap_reported = false;
    bool stack_reported = false;
    std::string line;
    size_t line_size;

    std::ifstream smaps("/proc/self/smaps");
    if(!smaps.is_open()){
        std::cerr << "error opening smaps file" << std::endl;
        return;
    }

    while(std::getline(smaps, line)){
        if(line.find("stack") != std::string::npos){
            std::cout << "=====STACK REPORT=====" << std::endl;
            for(int i = 0; i < 22; i++){
                std::getline(smaps, line);
                std::cout << line << std::endl;
            }
            stack_reported = true;
        }

        if(line.find("heap") != std::string::npos){
            std::cout << "=====HEAP REPORT=====" << std::endl;
            for(int i = 0; i < 22; i++){
                std::getline(smaps, line);
                std::cout << line << std::endl;
            }
            heap_reported = true;
        }

        if(heap_reported && stack_reported){
            break;
        }
    }
}

void TC2(){
    std::vector<int> vec1(5, 0);
    std::vector<int> vec2(9, 0);
    auto cap1 = vec1.capacity();
    auto cap2 = vec2.capacity();
    std::cout << "Vector 1 capacity: " << cap1 << std::endl;
    std::cout << "Vector 2 capacity: " << cap2 << std::endl;

    for(int i = 0; i < 15; i++){
        vec1.push_back(i);
        if(vec1.capacity() != cap1){
            std::cout << "After pushing " << i << ", Vector 1 capacity: " << vec1.capacity() << std::endl;
            cap1 = vec1.capacity();
        }
    }

    for(int i = 0; i < 15; i++){
        vec2.push_back(i);
        if(vec2.capacity() != cap2){
            std::cout << "After pushing " << i << ", Vector 2 capacity: " << vec2.capacity() << std::endl;
            cap2 = vec2.capacity();
        }
    }
}