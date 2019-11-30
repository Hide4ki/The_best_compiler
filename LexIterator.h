#pragma once
#include <map>
#include "Scanner.h"
#include "Parser.h"
#include "Interpretator.h"
#include "Token.h"
#include "Place.h"

template <class T>
class Parser;

template <class T>
class Scanner;

template <class T>
class Interpretator;

template <class T>
class LexIterator
{
private:
	Scanner<T>* _i;
	LexIterator() = delete;
	LexIterator(Scanner<T>*);
	LexIterator(const LexIterator&) = delete;
	LexIterator(LexIterator&&)  noexcept;
	LexIterator& operator=(const LexIterator&) = delete;
	~LexIterator();
public:
	bool operator!=(const LexIterator<T>&)const;
	pair<Token, Place> operator*();
	LexIterator<T> &operator++();
	void push(Token);
	friend class Parser<T>;
	friend class Interpretator<T>;
};

template<class T>
LexIterator<T>::LexIterator(LexIterator &&a) noexcept:_i{a._i}
{
	a._i = 0;
}

template<class T>
LexIterator<T>::~LexIterator()
{
	delete _i;
}

template<class T>
bool LexIterator<T>::operator!=(const LexIterator<T>&r) const
{
	return *_i != *r._i;
}

template<class T>
pair<Token, Place> LexIterator<T>::operator*()
{
	return _i->getToken();
}

template<class T>
LexIterator<T>& LexIterator<T>::operator++()
{
	_i->nextToken();
	return *this;
}

template<class T>
void LexIterator<T>::push(Token _token)
{
	_i->returnToken(_token);
}

template<class T>
LexIterator<T>::LexIterator(Scanner<T>* i)
{
	_i = i;
}
