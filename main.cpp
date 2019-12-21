#include <iostream>
#include <iterator>
#include <fstream>
#include "Scanner.h"
#include "TableDelim.h"
#include "operator.h"

using namespace std;

int main()
{
	ifstream fin("test.txt");

	if (!fin)
		return 1;

	auto myInterpretator = new Interpretator<istreambuf_iterator<char>>(istreambuf_iterator<char>(fin), istreambuf_iterator<char>{});

	myInterpretator->execute();

	system("pause");
	return 0;
}