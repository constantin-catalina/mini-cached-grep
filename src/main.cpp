#include "Cache.hpp"
#include "FileBuffer.hpp"
#include "Searcher.hpp"
#include <iostream>
#include <string>
#include <cstring>
#include <iomanip>

void printUsage();
void demonstrateCopyConstructor();
void demonstrateMoveConstructor();
void demonstrateDestructor();
void demonstrateBadRegularConstructor();
void demonstrateBadCopyConstructor();
void demonstrateBadMoveConstructor();
void demonstrateBadDestructor();

int main(int argc, char* argv[]) {
    if (argc == 1) {        
        std::cout << std::string(60, '=') << std::endl;
        std::cout << "=" << std::string(23, ' ') << "MINI - GREP" << std::string(24, ' ') << "=" << std::endl;
        std::cout << std::string(60, '=') << std::endl;
        printUsage();
        std::cout << std::endl << std::string(60, '=') << std::endl;
        std::cout << "=" << std::string(20, ' ') << "DEMONSTRATION MENU" << std::string(20, ' ') << "=" << std::endl;
        std::cout << std::string(60, '=') << std::endl << std::endl;
        std::cout << "1. Destructor" << std::endl << "2. Copy constructor" << std::endl << "3. Move constructor" << std::endl << "4. [BAD] Regular constructor" << std::endl << "5. [BAD] Copy constructor" << std::endl << "6. [BAD] Move constructor" << std::endl << "7. [BAD] Destructor" << std::endl << "8. Exit" << std::endl;
        
        int option;
        std::cout << std::endl << "Enter your option: ";
        std::cin >> option;
        std::cout << std::endl;
        switch(option){
            case 1:
                demonstrateDestructor();
                break;
            case 2:
                demonstrateCopyConstructor();
                break;
            case 3:
                demonstrateMoveConstructor();
                break;
            case 4:
                demonstrateBadRegularConstructor();
                break;
            case 5:
                demonstrateBadCopyConstructor();
                break;
            case 6:
                demonstrateBadMoveConstructor();
                break;
            case 7:
                demonstrateBadDestructor();
                break;
            default:
                break;
        }
        
        std::cout << std::endl;
        return 0;
    }
    
    bool caseInsensitive = false;
    bool wholeWord = false;
    std::string pattern;
    std::string filename;
    
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-i") {
            caseInsensitive = true;
        } else if (arg == "-w") {
            wholeWord = true;
        } else if (pattern.empty()) {
            pattern = arg;
        } else if (filename.empty()) {
            filename = arg;
        }
    }
    
    std::cout << std::endl;
    if (pattern.empty() || filename.empty()) {
        std::cerr << "Error: Pattern and file are required!" << std::endl;
        printUsage();
        return 1;
    }
    
    std::cout << "Searching for \"" << pattern << "\" in \"" << filename << "\"" << std::endl << std::endl;
    
    FileBuffer buffer(filename);
    if (!buffer.isValid()) {
        std::cerr << "Error: Cannot read file!" << std::endl;
        return 1;
    }
    
    Searcher searcher(caseInsensitive, wholeWord);
    Cache results = searcher.search(buffer, pattern);
    
    searcher.displayMatches();
    
    return 0;
}


void printUsage() {
    std::cout << "\nUsage: ./build/mini-grep [options] <pattern> <file>" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -i    Case-insensitive search" << std::endl;
    std::cout << "  -w    Whole word matching" << std::endl;
    std::cout << "\nExamples:" << std::endl;
    std::cout << "  mini-grep error log.txt" << std::endl;
    std::cout << "  mini-grep -w test code.cpp" << std::endl;
}


void demonstrateCopyConstructor() {
    
    std::cout << "Creating original FileBuffer:" << std::endl;
    FileBuffer fb1("test.txt");
    
    std::cout << "\nCopying buffer (calls copy constructor):" << std::endl;
    FileBuffer fb2 = fb1;
    
    std::cout << "\nBoth objects are independent:" << std::endl;
    std::cout << "- fb1 valid: " << fb1.isValid() << std::endl;
    std::cout << "- fb2 valid: " << fb2.isValid() << std::endl;
    
    std::cout << "\nOn exit, each object will free its own buffer." << std::endl;
}
 
void demonstrateMoveConstructor() {

    std::cout << "Creating Cache through factory function:" << std::endl;
    Cache cache = Cache::createOptimizedCache("test");    
}


void demonstrateDestructor() {
    FileBuffer fb("test.txt");
    Cache cache("test", 5);
}


// ==========================
// ==== BAD DEMO CLASSES ====
// ==========================

#include <unordered_map>

static std::unordered_map<void*, int> g_refcount;

static void track_alloc(void* p){ 
    if (p) g_refcount[p] = 1;
}

static void track_alias(void* p){ 
    if (p) g_refcount[p] += 1; 
}

static int  current_refs(void* p){ 
    auto it=g_refcount.find(p); 
    return it==g_refcount.end()?0:it->second; 
}

static void track_free_begin(void* p){
    int refs = current_refs(p);
    if (p && refs > 1) {
        std::cout << "ERROR: double-ownership detected before delete (refs=" << refs
                  << ") at " << p << "\n";
    } else if (p && refs == 0) {
        std::cout << "ERROR: invalid/unknown pointer about to be freed at " << p << "\n";
    }
}

static void track_free_end(void* p){ 
    if (!p) return; 
        auto it=g_refcount.find(p);
    if (it!=g_refcount.end()) { 
        it->second -= 1; 
        if (it->second <= 0) g_refcount.erase(it);
    }
}


class BadRegularCtor {
    int*   data;
    size_t size;
    bool   initialized; 
public:
    BadRegularCtor() : initialized(false) {
        std::cout << "[BadRegularConstructor] Constructor: members uninitialized\n";
    }
    void use() {
        if (!initialized) {
            std::cout << "ERROR: using members before initialization (data/size have indeterminate values)\n";
            return; 
        }
    }
    ~BadRegularCtor() {
        std::cout << "[BadRegularConstructor] No cleanup performed\n";
    }
};


class BadCopyBuffer {
    char*  p;
    size_t n;
public:
    explicit BadCopyBuffer(const char* txt) : p(nullptr), n(0) {
        n = std::strlen(txt);
        p = new char[n + 1];
        std::memcpy(p, txt, n + 1);
        track_alloc(p);
        std::cout << "[BadCopyBuffer] ctor p=" << static_cast<void*>(p) << " n=" << n << "\n";
    }

    BadCopyBuffer(const BadCopyBuffer& other) : p(other.p), n(other.n) {
        track_alias(p);
        std::cout << "[BadCopyBuffer] copy-ctor (shallow) p=" << static_cast<void*>(p)
                  << " -> refs=" << current_refs(p) << "\n";
        if (current_refs(p) > 1) {
            std::cout << "ERROR: two objects now own the same heap pointer\n";
        }
    }

    ~BadCopyBuffer() {
        track_free_begin(p);
        std::cout << "[BadCopyBuffer] dtor delete p=" << static_cast<void*>(p) << "\n";
        delete[] p;           
        track_free_end(p);
        p = nullptr;
    }
    void neutralize() { p = nullptr; } 
};


class BadMoveBuffer {
    char*  p;
    size_t n;
public:
    explicit BadMoveBuffer(size_t bytes) : p(new char[bytes]{}), n(bytes) {
        track_alloc(p);
        std::cout << "[BadMoveBuffer] ctor p=" << static_cast<void*>(p) << " n=" << n << "\n";
    }

    BadMoveBuffer(BadMoveBuffer&& other) noexcept : p(other.p), n(other.n) {
        track_alias(p); 
        std::cout << "[BadMoveBuffer] move-ctor (source not nulled) p=" << static_cast<void*>(p)
                  << " -> refs=" << current_refs(p) << "\n";
        if (current_refs(p) > 1) {
            std::cout << "ERROR: after move, source still owns the pointer\n";
        }
    }

    ~BadMoveBuffer() {
        track_free_begin(p);
        std::cout << "[BadMoveBuffer] dtor delete p=" << static_cast<void*>(p) << "\n";
        delete[] p;
        track_free_end(p);
        p = nullptr;
    }

    void forceNull() { p = nullptr; } 
};


static std::size_t g_leak_bytes = 0;

class BadDestructor {
    char*  p;
    size_t n;
    static std::vector<char*> s_ptrs;
public:
    explicit BadDestructor(size_t bytes) : p(new char[bytes]), n(bytes) {
        g_leak_bytes += n;
        s_ptrs.push_back(p);
        std::cout << "[BadDestructor] ctor alloc " << n
                  << " at " << static_cast<void*>(p) << "\n";
    }

    ~BadDestructor() {
        std::cout << "[BadDestructor] No cleanup performed\n";
    }

    static void reportLeaks() {
        if (g_leak_bytes > 0)
            std::cout << "ERROR: memory leak detected: " << g_leak_bytes << " bytes\n";
        else
            std::cout << "[BadDestructor] no leaks detected\n";
    }

    static void cleanupAfterDemo() {
        for (char* q : s_ptrs) delete[] q;     
        s_ptrs.clear();
        g_leak_bytes = 0;
    }
};

std::vector<char*> BadDestructor::s_ptrs;

void demonstrateBadRegularConstructor() {

    BadRegularCtor x;
    x.use();
}

void demonstrateBadCopyConstructor() {
    
    BadCopyBuffer a("hello");
    BadCopyBuffer b = a;          
    b.neutralize();
    
}

void demonstrateBadMoveConstructor() {

    BadMoveBuffer src(4096);
    BadMoveBuffer dst = std::move(src); 

    dst.forceNull();                    
}

void demonstrateBadDestructor() {

    BadDestructor a(10 * 1024);
    BadDestructor b(20 * 1024);
    
    BadDestructor::reportLeaks();     
    BadDestructor::cleanupAfterDemo(); 
}
