#pragma once
#include "Program.h"

template <class T>
class SyntaxTreeIterator
{
private:
	Program<T>* _i;
public:
	SyntaxTreeIterator(Program<T>*);
	SyntaxTreeIterator() {};
	Token operator*();
	Program<T>* operator()() { return _i; };
	void operator++();
	bool operator!=(SyntaxTreeIterator<T>);
};

template<class T>
inline SyntaxTreeIterator<T>::SyntaxTreeIterator(Program<T>* i) :_i{ i }
{
	_i->_cntVisit = 0;
}

template<class T>
inline Token SyntaxTreeIterator<T>::operator*()
{
	return _i->_myPolisElem;
}

template<class T>
inline void SyntaxTreeIterator<T>::operator++()
{
	if (_i->_cntVisit == _i->_childs.size())
	{
		_i = _i->_parent;
		_i->_cntVisit++;
		++(*this);
	}
	else
	{
		_i = _i->_childs[_i->_cntVisit];
		_i->_cntVisit = 0;
	}
}

template<class T>
inline bool SyntaxTreeIterator<T>::operator!=(SyntaxTreeIterator<T>)
{
	return _i->_name == "END" ? false : true;
}
