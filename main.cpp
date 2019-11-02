#include <iostream>
#include <fstream>
#include <string>
#include "Scanner.h"
#include <windows.h>
#include "Compiler.h"
#include "Linker.h"
#include "Parser.h"
using namespace std;

int main(void)
{
  ifstream listFileOfSourceLanguage("listFiles.txt");
  if(!listFileOfSourceLanguage)
    return 2;

  ofstream fileOfTargetLanguage("result");
  if(!fileOfTargetLanguage)
    return 3;

  string fileName;

  while(getline(listFileOfSourceLanguage,fileName))
  {
    ifstream fileSourceLanguage(fileName);
    if(!fileSourceLanguage)
      return 4;
    //TODO
    fileSourceLanguage.close();
  }

  listFileOfSourceLanguage.close();
  fileOfTargetLanguage.close();

  system("pause");
  return 0;
}