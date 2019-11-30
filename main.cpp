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

	//istreambuf_iterator<char>(fin), istreambuf_iterator<char>());

	//try
	//{
	//	++a;
	//	for (; a != c; ++a)
	//		cout << *a << endl;
	//}
	//catch (MyException *e)
	//{
	//	cout << e->what() << endl;
	//	delete e;
	//}
	//cout << b.getTableDelim() << endl;
	//cout << b.getTableID() << endl;
	//cout << b.getTableServWord() << endl;
	//cout << b.getTableLitteral() << endl;
	system("pause");
	return 0;
}