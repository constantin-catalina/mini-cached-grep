#include "Cache.hpp"
#include <iostream>
#include <cstring>

Cache::Cache(const std::string& pattern, size_t initialCapacity)
    : m_pattern(new std::string(pattern)),
      m_results(new int[initialCapacity]),
      m_capacity(initialCapacity),
      m_size(0) {
    std::cout << "[Cache] Constructor called for pattern: \"" 
              << pattern << "\" (capacity: " << initialCapacity << ")" << std::endl;
}

Cache::~Cache() {
    std::cout << "[Cache] Destructor called for pattern: \"" 
              << (m_pattern ? *m_pattern : "null") << "\" (releasing resources)" << std::endl;
    delete m_pattern;
    delete[] m_results;
    m_pattern = nullptr;
    m_results = nullptr;
}

Cache::Cache(const Cache& other)
    : m_pattern(new std::string(*other.m_pattern)),
      m_results(new int[other.m_capacity]),
      m_capacity(other.m_capacity),
      m_size(other.m_size) {
    std::cout << "[Cache] Copy constructor called for pattern: \"" 
              << *m_pattern << "\"" << std::endl;
    
    for (size_t i = 0; i < m_size; ++i) {
        m_results[i] = other.m_results[i];
    }
}

Cache& Cache::operator=(const Cache& other) {
    std::cout << "[Cache] Copy assignment operator called" << std::endl;
    
    if (this != &other) {
        delete m_pattern;
        delete[] m_results;
        
        m_pattern = new std::string(*other.m_pattern);
        m_results = new int[other.m_capacity];
        m_capacity = other.m_capacity;
        m_size = other.m_size;
        
        for (size_t i = 0; i < m_size; ++i) {
            m_results[i] = other.m_results[i];
        }
    }
    return *this;
}

Cache::Cache(Cache&& other) noexcept
    : m_pattern(other.m_pattern),
      m_results(other.m_results),
      m_capacity(other.m_capacity),
      m_size(other.m_size) {
    std::cout << "[Cache] Move constructor called for pattern: \"" 
              << *m_pattern << "\"" << std::endl;
    
    other.m_pattern = nullptr;
    other.m_results = nullptr;
    other.m_capacity = 0;
    other.m_size = 0;
}

Cache& Cache::operator=(Cache&& other) noexcept {
    std::cout << "[Cache] Move assignment operator called" << std::endl;
    
    if (this != &other) {
        delete m_pattern;
        delete[] m_results;
        
        m_pattern = other.m_pattern;
        m_results = other.m_results;
        m_capacity = other.m_capacity;
        m_size = other.m_size;
        
        other.m_pattern = nullptr;
        other.m_results = nullptr;
        other.m_capacity = 0;
        other.m_size = 0;
    }
    return *this;
}

void Cache::resize() {
    size_t newCapacity = m_capacity * 2;
    int* newResults = new int[newCapacity];
    
    for (size_t i = 0; i < m_size; ++i) {
        newResults[i] = m_results[i];
    }
    
    delete[] m_results;
    m_results = newResults;
    m_capacity = newCapacity;
}

void Cache::addResult(int position) {
    if (m_size >= m_capacity) {
        resize();
    }
    m_results[m_size++] = position;
}

void Cache::displayResults() const {
    std::cout << "Results for \"" << *m_pattern << "\": " 
              << m_size << " found" << std::endl;
}