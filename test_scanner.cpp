#include <iostream>
#include <fstream>
#include <string>
#include "Scanner.h"

using namespace std;

int main(void)
{
  Scanner *test_scanner = new Scanner;

  int cnt;
  int num;

  bool skip;
  bool allGood = true;

  string test_value;
  string expected_value;

  ifstream fin("test sc #1");
  if(!fin)
    return 2;

  fin >> cnt;
  for(int i = 0; i<cnt; i++)
  {
    fin >> num;
    fin >> skip;
    fin >> test_value;
    fin >> expected_value;

    if(!skip)
    {
      cout << "test #" << num << endl;
      if(expected_value == test_scanner->processing(test_value))
        cout << "YES";
      else
      {
        cout << "NO";
        allGood = false;
      }
      cout << endl;
    }
  }
  
  if(!allGood)
    cout << "tests is not done!" << endl;
  else
    cout << "tests is done!" << endl;
  
  delete test_scanner;
  system("pause");
  return 0;
}