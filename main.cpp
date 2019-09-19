#include <iostream>
#include <fstream>
#include <string>
#include "Scanner.h"
#include <windows.h>

using namespace std;

int main(void)
{
  ifstream list_file_of_source_language("list files for translation");
  if(!list_file_of_source_language)
    return 2;

  ofstream file_of_target_language("translation result");
  if(!file_of_target_language)
    return 3;

  string file_name;

  while(getline(list_file_of_source_language,file_name))
  {
    ifstream file_source_language(file_name);
    //TODO
    file_source_language.close();
  }

  list_file_of_source_language.close();
  file_of_target_language.close();

  system("pause");
  return 0;
}