#include "Range.h"
#include <stdexcept>

Range::Range(const int min,const int max) :
	_min(min),
	_max(max)
{
	if (min > max)
		throw new std::logic_error("Range::Range = min > max ");
}

Range::Range() :
	_min(0),
	_max(0)
{

}

int Range::getRange() const
{
	return _max - _min;
}

void Range::setMin(const int min)
{
	_min = min;
}

void Range::setMax(const int max)
{
	_max = max;
}

void Range::update(const int value)
{
	if (_min > value)
		_min = value;
	else
		if (_max < value)
			_max = value;
}


bool Range::operator < (const Range &range) const
{
	if (this->getRange() < range.getRange())
		return true;
	else
		return false;
}

bool Range::operator <= (const Range &range) const
{
	if (this->getRange() <= range.getRange())
		return true;
	else
		return false;
}

bool Range::operator > (const Range &range) const
{
	if (this->getRange() > range.getRange())
		return true;
	else
		return false;
}

bool Range::operator >= (const Range &range) const
{
	if (this->getRange() >= range.getRange())
		return true;
	else
		return false;
}

bool Range::operator == (const Range &range) const
{
	if (this->getRange() == range.getRange())
		return true;
	else
		return false;
}

Range Range::operator *(const double &value) const
{
	return Range(_min*value, _max*value);
}

Range& Range::operator *=(const double &value)
{
	this->_min*=value;
	this->_max*=value;
	return *this;
}