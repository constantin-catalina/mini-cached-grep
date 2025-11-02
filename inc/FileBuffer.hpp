#ifndef FILEBUFFER_HPP
#define FILEBUFFER_HPP

#include <string>
#include <fstream>

class FileBuffer {
private:
    char* m_buffer;
    size_t m_size;
    std::string m_filename;
    
public:
    FileBuffer(const std::string& filename);
    ~FileBuffer();
    FileBuffer(const FileBuffer& other);
    FileBuffer& operator=(const FileBuffer& other);
    FileBuffer(FileBuffer&& other) noexcept;
    FileBuffer& operator=(FileBuffer&& other) noexcept;
    
    const char* getBuffer() const { return m_buffer; }
    size_t getSize() const { return m_size; }
    const std::string& getFilename() const { return m_filename; }
    bool isValid() const { return m_buffer != nullptr && m_size > 0; }
};

#endif