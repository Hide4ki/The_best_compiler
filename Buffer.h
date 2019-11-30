#pragma once
#include "Place.h"
#include "MyException.h"

template <class T>
class Scanner;

template <class T>
class Buffer
{
	T _it;
	T _end;
	long _line;
	long _pos;
	char _preSymb;
	bool _fRemSymb;
	Buffer() = delete;
	Buffer(T, T);
	~Buffer() = default;
public:
	Place nowPlace();
	void returnSymb();
	void skipLine();
	char gc();
	friend class Scanner<T>;
};

template<class T>
Buffer<T>::Buffer(T it, T end) : _it{ it }, _end{ end }
{
	_preSymb = *_it;
	_fRemSymb = false;
	_line = 1;
	_pos = 0;
}

template<class T>
Place Buffer<T>::nowPlace()
{
	return Place(_line, _pos);
}

template<class T>
void Buffer<T>::returnSymb()
{
	if (_fRemSymb)
		throw MyException("Error in buffer is twice symbol return!", Place(_line, _pos));
	_fRemSymb = true;
}

template<class T>
void Buffer<T>::skipLine()
{
	_line++;
	_pos = 0;
	if (_fRemSymb && (_preSymb == '\n'))
	{
		_fRemSymb = false;
		return;
	}
	_fRemSymb = false;
	while (*_it != '\n')
		_it++;
	++_it;
}

template<class T>
char Buffer<T>::gc()
{
	_pos++;
	if (_fRemSymb)
	{
		_pos--;
		_fRemSymb = false;
		return _preSymb;
	}
	if (_it == _end)
	{
		_preSymb = EOF;
		return _preSymb;
	}
	_preSymb = *_it++;
	return _preSymb;
}