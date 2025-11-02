#ifndef CACHE_HPP
#define CACHE_HPP

#include <string>

class Cache {
private:
    std::string* m_pattern;
    int* m_results;
    size_t m_capacity;
    size_t m_size;
    
    void resize();

public:
    Cache(const std::string& pattern, size_t initialCapacity = 10);
    ~Cache();
    Cache(const Cache& other);
    Cache& operator=(const Cache& other);
    Cache(Cache&& other) noexcept;
    Cache& operator=(Cache&& other) noexcept;
    
    void addResult(int position);
    void displayResults() const;
    size_t getSize() const { return m_size; }
    const std::string& getPattern() const { return *m_pattern; }
    
    static Cache createOptimizedCache(const std::string& pattern);
};

#endif