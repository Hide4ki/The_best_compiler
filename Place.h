#pragma once
#include <map>

using namespace std;

class Place
{
private:
	long _line;
	long _pos;
public:
	Place();
	pair<long, long> getPlace() const;
	Place(long, long);
	Place(const Place&);
	Place operator=( Place&);
	~Place();
};

