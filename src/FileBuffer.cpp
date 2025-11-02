#include "FileBuffer.hpp"
#include <iostream>
#include <cstring>

FileBuffer::FileBuffer(const std::string& filename)
    : m_buffer(nullptr), m_size(0), m_filename(filename) {
    std::cout << "[FileBuffer] Constructor called for file: \"" 
              << filename << "\"" << std::endl;
    
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "[FileBuffer] Error: Cannot open file!" << std::endl;
        return;
    }
    
    m_size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    m_buffer = new char[m_size + 1];
    file.read(m_buffer, m_size);
    m_buffer[m_size] = '\0';
    
    file.close();
    std::cout << "[FileBuffer] Read " << m_size << " bytes" << std::endl;
}

FileBuffer::~FileBuffer() {
    std::cout << "[FileBuffer] Destructor called for \"" 
              << m_filename << "\" (releasing " << m_size << " bytes)" << std::endl;
    delete[] m_buffer;
    m_buffer = nullptr;
    m_size = 0;
}

FileBuffer::FileBuffer(const FileBuffer& other)
    : m_buffer(nullptr), m_size(other.m_size), m_filename(other.m_filename) {
    std::cout << "[FileBuffer] Copy constructor called for \"" 
              << m_filename << "\"" << std::endl;
    std::cout << "             -> DEEP COPY of " << m_size << " bytes" << std::endl;
    
    if (other.m_buffer && other.m_size > 0) {
        m_buffer = new char[m_size + 1];
        std::memcpy(m_buffer, other.m_buffer, m_size);
        m_buffer[m_size] = '\0';
    }
}

FileBuffer& FileBuffer::operator=(const FileBuffer& other) {
    std::cout << "[FileBuffer] Copy assignment operator called" << std::endl;
    
    if (this != &other) {
        delete[] m_buffer;
        
        m_size = other.m_size;
        m_filename = other.m_filename;
        
        if (other.m_buffer && other.m_size > 0) {
            m_buffer = new char[m_size + 1];
            std::memcpy(m_buffer, other.m_buffer, m_size);
            m_buffer[m_size] = '\0';
        } else {
            m_buffer = nullptr;
        }
    }
    return *this;
}

FileBuffer::FileBuffer(FileBuffer&& other) noexcept
    : m_buffer(other.m_buffer),
      m_size(other.m_size),
      m_filename(std::move(other.m_filename)) {
    std::cout << "[FileBuffer] Move constructor called for \"" 
              << m_filename << "\"" << std::endl;
    
    other.m_buffer = nullptr;
    other.m_size = 0;
}

FileBuffer& FileBuffer::operator=(FileBuffer&& other) noexcept {
    std::cout << "[FileBuffer] Move assignment operator called" << std::endl;
    
    if (this != &other) {
        delete[] m_buffer;
        
        m_buffer = other.m_buffer;
        m_size = other.m_size;
        m_filename = std::move(other.m_filename);
        
        other.m_buffer = nullptr;
        other.m_size = 0;
    }
    return *this;
}