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

template<class T>
void Interpretator<T>::execute()
{

	auto tree = _myParser->getSyntaxTree();
	/*SyntaxTreeIterator<T> it{ tree };
	SyntaxTreeIterator<T> end{};*/
	if (tree == 0)
		return;
	vector<Token> polis{ tree->GeneratePolis() };
	stack<int> st;
	auto push = [&](int a) { st.push(a); }; 
	function<void(int)> ipush{push};
	auto pop = [&]() { int tmp = st.top(); st.pop(); return tmp; };
	function<int(void)> ipop{pop};
	auto GetNum = [&](int a) {return _myTableSymbol->getTableLiterals().getNum(a); };
	auto GetVal = [&](int a) {return _myTableSymbol->getTableID().getValue(a); };
	
	map < int, void(*)(function<void(int)>, function<int(void)>, TableSymbol*, int&)>ops
	{
		{ static_cast<int>(ExtraType::ASS_OP), [](function<void(int)> push, function<int(void)> pop, TableSymbol *table, int &r) { int b = pop(); int a = pop(); table->getTableID().setValue(a,b); } },
		{ static_cast<int>(ExtraType::GOTO_OP), [](function<void(int)> push, function<int(void)> pop, TableSymbol *table, int &r) { int i = pop(); r = i - 1; } },
		{ static_cast<int>(ExtraType::IFGOTO_OP), [](function<void(int)> push, function<int(void)> pop, TableSymbol *table, int &r) { int i = pop(); int arg = pop(); if (!arg) { r = i - 1; } }},
		{ static_cast<int>(ExtraType::INC_OP), [](function<void(int)> push, function<int(void)> pop, TableSymbol *table, int &r) { int a = pop(); int b = table->getTableID().getValue(a); table->getTableID().setValue(a,++b); push(b); } },
		{ static_cast<int>(ExtraType::WRITE_OP), [](function<void(int)> push, function<int(void)> pop, TableSymbol *table, int &r) { cout << pop() << endl; } },
		{ static_cast<int>(ExtraType::READ_OP), [](function<void(int)> push, function<int(void)> pop,TableSymbol *table, int &r) { int a; cin >> a;	table->getTableID().setValue(pop(), a); } },
		{ static_cast<int>(ExtraType::OR_OP), [](function<void(int)> push, function<int(void)> pop,TableSymbol *table, int &r) {int a = pop(); int b = pop(); push(b || a); } },
		{ static_cast<int>(ExtraType::AND_OP), [](function<void(int)> push, function<int(void)> pop,TableSymbol *table, int &r) {int a = pop(); int b = pop(); push(b && a); }},
		{ static_cast<int>(ExtraType::EQ_OP), [](function<void(int)> push, function<int(void)> pop,TableSymbol *table, int &r) {int a = pop(); int b = pop(); push(b == a); }},
		{ static_cast<int>(ExtraType::NEG_OP), [](function<void(int)> push, function<int(void)> pop,TableSymbol *table, int &r) {int a = pop(); int b = pop(); push(b - a); }},
		{ static_cast<int>(ExtraType::NEGU_OP), [](function<void(int)> push, function<int(void)> pop,TableSymbol *table, int &r) {push(-pop()); } },
		{static_cast<int>(ExtraType::NOTEQ_OP), [](function<void(int)> push, function<int(void)> pop,TableSymbol *table, int &r) {int a = pop(); int b = pop(); push(b != a); }},
		{ static_cast<int>(ExtraType::LARGE_OP), [](function<void(int)> push, function<int(void)> pop,TableSymbol *table, int &r) {int a = pop(); int b = pop(); push(b > a); }},
		{static_cast<int>(ExtraType::LESS_OP), [](function<void(int)> push, function<int(void)> pop,TableSymbol *table, int &r) {int a = pop(); int b = pop(); push(b < a); }},
		{static_cast<int>(ExtraType::NOT_OP), [](function<void(int)> push, function<int(void)> pop,TableSymbol *table, int &r) {push(!pop()); }},
		{static_cast<int>(ExtraType::MUL_OP), [](function<void(int)> push, function<int(void)> pop,TableSymbol *table, int &r) {int a = pop(); int b = pop(); push(b * a); }},
		{static_cast<int>(ExtraType::DIV_OP), [](function<void(int)> push, function<int(void)> pop ,TableSymbol *table, int &r) {int a = pop(); int b = pop(); push(b / a); }},
		{static_cast<int>(ExtraType::ADD_OP), [](function<void(int)> push, function<int(void)> pop ,TableSymbol *table, int &r) {int a = pop(); int b = pop(); push(b + a); }},
	};
	int i;// ASS_OP, , GOTO_OP
	for (i = 0; i < polis.size(); i++)
	{
		int l;
		auto token = polis[i];
		auto[tokenName, att] = token.getValue();
		switch (tokenName)
		{
		case TokenName::NULLTOKEN:
			ipush(att + 1);
			break;
		case TokenName::WORD:
			if (att == static_cast<int>(ServWord::TRUE))
				ipush(1);
			else
				ipush(0);
			break;
		case TokenName::DELIM:
			l = i;
			ops.at(att+1)(ipush, ipop, _myTableSymbol, l);
			i = l;
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
		case TokenName::END:
			break;
		}
	}
	//_myProgram = _myParser->getProgram();
	//_myProgram = _myOptimizer(_myProgram->begin(), _myProgram->end());
	//_myCode = _myGeneratorCode(_myProgram->begin(), _myProgram->end());
	//_myCode->execute();
	//cout << _myTableSymbol->getTableDelim() << endl;
	//cout << _myTableSymbol->getTableID() << endl;
	//cout << _myTableSymbol->getTableServWord() << endl;
	//cout << _myTableSymbol->getTableLiterals() << endl;
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
