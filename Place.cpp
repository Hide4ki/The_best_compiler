#include "Place.h"

Place::Place() :_line{0}, _pos{0}
{
}

pair<long, long> Place::getPlace() const
{
	return pair<long, long>(_line, _pos);
}

Place::Place(long line, long pos) :_line{ line }, _pos{ pos }
{
}

Place::Place(const Place &copy) : _line{ copy._line }, _pos{ copy._pos }
{
}

Place Place::operator=(Place &o)
{
	_line = o._line;
	_pos = o._pos;
	return *this;
}

Place::~Place()
{
}
