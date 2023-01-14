#pragma once

#include "Scanner.h"
#include "DFA.h"
#include <fstream>




class TableDrivenScanner :
    public Scanner
{
    
private:

    const DFA& tokenizer;
    std::ifstream source;
    std::string source_name;
    

    char get_char(); // read from file, handle trigraphs and line merging (phases 1 & 2)
    char next_char(); //manage buffer
    PPWord next_word() override;
    void move_back();
    char peek_next();
    std::vector<PPWord> retokenize_header(const PPWord& header_name_token); // retokenizes header names when found outside of an include directive

    std::vector<char> buffer;
    int buffer_size;

    int buffer_fence;
    int buffer_ptr; 

    int current_line_num = 1;

    bool from_file = true; //load buffer periodically if true, else buffer holds entire string

    void print_buffer();

    bool past_fill = false;

public:

    TableDrivenScanner(const std::string& file_name, const DFA& dfa, int buffer_sz);
    TableDrivenScanner(std::string s, const DFA& dfa);
    std::vector<PPWord> scan() override;

};

class FileNotFoundError : std::exception
{
private: 
    std::string out;
    

public:
    FileNotFoundError(std::string name) :
        out(name + " was not able to be opened")
    {}

    virtual const char* what() const throw()
    {
        return out.c_str();
    }
};

class InvalidLineMerge : std::exception
{
private:
    std::string out;

public:
    InvalidLineMerge(std::string name) :
        out(name + " has an invalid line merge at end of file")
    {}

    virtual const char* what() const throw()
    {
        return out.c_str();
    }
    
};

class BufferUnderflow : std::exception
{
public: 
    virtual const char* what() const throw()
    {
        return "Tried to roll back too far in scan buffer";
    }
};

class EndOfFileReached : std::exception
{
public:
    virtual const char* what() const throw()
    {
        return "End of file reached";
    }

};

