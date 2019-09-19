#pragma once
#include <iostream>
#include <string>
/*
Scanner - 
*/
using namespace std;

class Scanner
{
public:
  explicit Scanner(void);
  Scanner(const Scanner&);
  ~Scanner(void);
  void operator = (Scanner&);
  string processing(string&);
private:
  
};

Scanner::Scanner(void)
{
  
}

Scanner::Scanner(const Scanner& myScanner)
{
  
}

Scanner::~Scanner(void)
{
  
}

void Scanner::operator = (Scanner&)
{
  
}

string Scanner::processing(string&a)
{
  return a;
}
