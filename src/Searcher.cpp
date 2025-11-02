#include "Searcher.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>

Searcher::Searcher(bool caseInsensitive, bool wholeWord)
    : m_caseInsensitive(caseInsensitive),
      m_wholeWord(wholeWord),
      m_matchedLines(new std::vector<std::string>()) {
    std::cout << "[Searcher] Constructor called (case-insensitive: " 
              << (m_caseInsensitive ? "YES" : "NO") 
              << ", whole-word: " << (m_wholeWord ? "YES" : "NO") << ")" << std::endl;
}

Searcher::~Searcher() {
    std::cout << "[Searcher] Destructor called (releasing vector with " 
              << (m_matchedLines ? m_matchedLines->size() : 0) << " results)" << std::endl;
    delete m_matchedLines;
    m_matchedLines = nullptr;
}

Searcher::Searcher(const Searcher& other)
    : m_caseInsensitive(other.m_caseInsensitive),
      m_wholeWord(other.m_wholeWord),
      m_matchedLines(new std::vector<std::string>(*other.m_matchedLines)) {
    std::cout << "[Searcher] Copy constructor called" << std::endl;
}

Searcher& Searcher::operator=(const Searcher& other) {
    std::cout << "[Searcher] Copy assignment operator called" << std::endl;
    
    if (this != &other) {
        delete m_matchedLines;
        
        m_caseInsensitive = other.m_caseInsensitive;
        m_wholeWord = other.m_wholeWord;
        m_matchedLines = new std::vector<std::string>(*other.m_matchedLines);
    }
    return *this;
}

Searcher::Searcher(Searcher&& other) noexcept
    : m_caseInsensitive(other.m_caseInsensitive),
      m_wholeWord(other.m_wholeWord),
      m_matchedLines(other.m_matchedLines) {
    std::cout << "[Searcher] Move constructor called" << std::endl;
    
    other.m_matchedLines = nullptr;
}

Searcher& Searcher::operator=(Searcher&& other) noexcept {
    std::cout << "[Searcher] Move assignment operator called" << std::endl;
    
    if (this != &other) {
        delete m_matchedLines;
        
        m_caseInsensitive = other.m_caseInsensitive;
        m_wholeWord = other.m_wholeWord;
        m_matchedLines = other.m_matchedLines;
        
        other.m_matchedLines = nullptr;
    }
    return *this;
}

std::string Searcher::toLower(const std::string& str) const {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return result;
}

bool Searcher::matchesPattern(const std::string& line, const std::string& pattern) const {
    std::string searchLine = m_caseInsensitive ? toLower(line) : line;
    std::string searchPattern = m_caseInsensitive ? toLower(pattern) : pattern;
    
    if (!m_wholeWord) {
        return searchLine.find(searchPattern) != std::string::npos;
    }
    
    size_t pos = 0;
    while ((pos = searchLine.find(searchPattern, pos)) != std::string::npos) {
        bool validStart = (pos == 0 || !std::isalnum(searchLine[pos - 1]));
        bool validEnd = (pos + searchPattern.length() >= searchLine.length() ||
                        !std::isalnum(searchLine[pos + searchPattern.length()]));
        
        if (validStart && validEnd) {
            return true;
        }
        pos++;
    }
    return false;
}

Cache Searcher::search(const FileBuffer& buffer, const std::string& pattern) {
    std::cout << "\n[Searcher] Searching for pattern: \"" << pattern << "\"" << std::endl;
    
    Cache cache(pattern);
    m_matchedLines->clear();
    
    if (!buffer.isValid()) {
        std::cerr << "[Searcher] Invalid buffer!" << std::endl;
        return cache;
    }
    
    std::string content(buffer.getBuffer());
    std::istringstream stream(content);
    std::string line;
    int lineNumber = 0;
    int matchCount = 0;
    
    while (std::getline(stream, line)) {
        lineNumber++;
        if (matchesPattern(line, pattern)) {
            cache.addResult(lineNumber);
            m_matchedLines->push_back(line);
            matchCount++;
        }
    }
    
    std::cout << "[Searcher] Found " << matchCount << " matches" << std::endl;
    return cache;
}

void Searcher::displayMatches() const {
    if (!m_matchedLines || m_matchedLines->empty()) {
        std::cout << "No lines found." << std::endl;
        return;
    }
    
    std::cout << "\n=== MATCHES ===" << std::endl;
    for (size_t i = 0; i < m_matchedLines->size() && i < 10; ++i) {
        std::cout << (*m_matchedLines)[i] << std::endl;
    }
    std::cout << std::endl;
    
    if (m_matchedLines->size() > 10) {
        std::cout << "... and " << (m_matchedLines->size() - 10) << " more lines" << std::endl;
    }
}