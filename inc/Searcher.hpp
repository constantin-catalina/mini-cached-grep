#ifndef SEARCHER_HPP
#define SEARCHER_HPP

#include "FileBuffer.hpp"
#include "Cache.hpp"
#include <string>
#include <vector>

class Searcher {
private:
    bool m_caseInsensitive;
    bool m_wholeWord;
    std::vector<std::string>* m_matchedLines;
    
    bool matchesPattern(const std::string& line, const std::string& pattern) const;
    std::string toLower(const std::string& str) const;
    
public:
    Searcher(bool caseInsensitive = false, bool wholeWord = false);
    ~Searcher();
    Searcher(const Searcher& other);
    Searcher& operator=(const Searcher& other);
    Searcher(Searcher&& other) noexcept;
    Searcher& operator=(Searcher&& other) noexcept;
    
    Cache search(const FileBuffer& buffer, const std::string& pattern);
    void displayMatches() const;
    size_t getMatchCount() const { return m_matchedLines ? m_matchedLines->size() : 0; }
};

#endif