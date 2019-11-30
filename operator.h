#pragma once
#include <iostream>
#include <map>
#include <string>
#include "Token.h"
#include "Place.h"

using namespace std;

ostream &operator <<(ostream &stream, pair<Token, Place> &myType);