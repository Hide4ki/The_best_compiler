#pragma once
#include <functional>
#include "Scanner.h"
#include "TableSymbol.h"
#include "Parser.h"
#include "LexIterator.h"

using namespace std;

template <class T>
class Parser;

template<class T>
class Interpretator
{
private:
	Parser<T> *_myParser;
	TableSymbol *_myTableSymbol;
public:
	void execute();
	Interpretator(T, T);
	~Interpretator();
};
enum class Oper {OR, AND, EQ, NOTEQ, ASS, LARGE, LESS, INC, ADD, DIV, MUL, NOT, NEG};

template<class T>
void Interpretator<T>::execute()
{

	auto tree = _myParser->getSyntaxTree();
	/*SyntaxTreeIterator<T> it{ tree };
	SyntaxTreeIterator<T> end{};*/
	vector<Token> polis{ tree->GeneratePolis() };
	stack<int> st;
	auto push = [&](int a) { st.push(a); }; 
	function<void(int)> ipush{push};
	auto pop = [&]() { int tmp = st.top(); st.pop(); return tmp; };
	function<int(void)> ipop{pop};
	auto GetNum = [&](int a) {return _myTableSymbol->getTableLiterals().getNum(a); };
	auto GetVal = [&](int a) {return _myTableSymbol->getTableID().getValue(a); };
	int i;
	map < int, void(*)(function<void(int)>, function<int(void)>)>ops
	{
		{static_cast<int>(Oper::ADD), [](function<void(int)> push, function<int(void)> pop) {push(pop() + pop()); }},
		{static_cast<int>(Oper::NEG), [](function<void(int)> push, function<int(void)> pop) {push(pop() - pop()); }},
		{static_cast<int>(Oper::MUL), [](function<void(int)> push, function<int(void)> pop) {push(pop() * pop()); }},
		{static_cast<int>(Oper::DIV), [](function<void(int)> push, function<int(void)> pop) {push(pop() / pop()); }},
	};
	
	for (i = 0; i < polis.size(); i++)
	{
		auto token = polis[i];
		auto[tokenName, att] = token.getValue();
		switch (tokenName)
		{
		case TokenName::NULLTOKEN:
			ipush(att);
			break;
		case TokenName::WORD:
			if (att == static_cast<int>(ServWord::TRUE))
				ipush(1);
			else
				ipush(0);
			break; //OR_OP, AND_OP, EQ_OP, NOTEQ_OP, ASS_OP, LARGE_OP, LESS_OP, INC_OP, ADD_OP, DIV_OP, MUL_OP, NOT_OP, NEG_OP,
		case TokenName::DELIM:
			ops.at(att)(ipush, ipop);
			break;
		case TokenName::LITERAL:
			ipush(GetNum(att));
			break;
		case TokenName::ID:
			ipush(GetVal(att));
			break;
		case TokenName::ADDRESS:
			ipush(att);
			break;
		}
	}
	//_myProgram = _myParser->getProgram();
	//_myProgram = _myOptimizer(_myProgram->begin(), _myProgram->end());
	//_myCode = _myGeneratorCode(_myProgram->begin(), _myProgram->end());
	//_myCode->execute();
	cout << _myTableSymbol->getTableDelim() << endl;
	cout << _myTableSymbol->getTableID() << endl;
	cout << _myTableSymbol->getTableServWord() << endl;
	cout << _myTableSymbol->getTableLiterals() << endl;
}

template<class T>
Interpretator<T>::Interpretator(T it, T end)
{
	_myTableSymbol = new TableSymbol;
	Scanner<T> *myScanner = new Scanner<T>(it, end);
	myScanner->SetTable(_myTableSymbol);
	myScanner->nextToken();
	_myParser = new Parser<T>(LexIterator<T>{ myScanner }, LexIterator<T>{ new Scanner<T>{} });
	_myParser->SetTable(_myTableSymbol);
}

template<class T>
Interpretator<T>::~Interpretator()
{
	delete _myParser;
	delete _myTableSymbol;
}
