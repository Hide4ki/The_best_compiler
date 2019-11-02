#include <iostream>
#include <fstream>
#include <string>
#include "Scanner.h"

using namespace std;

int main(void)
{
  Scanner *testScanner = new Scanner;

  int cnt;
  int num;

  bool skip;
  bool allGood = true;

  string testValue;
  string expectedValue;

  ifstream fin("test sc #1");
  if(!fin)
    return 2;

  fin >> cnt;
  for(int i = 0; i<cnt; i++)
  {
    fin >> num;
    fin >> skip;
    fin >> testValue;
    fin >> expectedValue;

    if(!skip)
    {
      cout << "test #" << num << endl;
      if(expectedValue == testScanner->processing(testValue))
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
  
  delete testScanner;
  system("pause");
  return 0;
}