#include "Range.h"
#include <stdexcept>

Range::Range(const int min,const int max) :
	min_(min),
	max_(max)
{
	checkResources(min, max);
}

void Range::checkResources(const int min, const int max)
{
	if (min > max)
		throw new std::invalid_argument("Range::Range = min > max ");
}

Range::Range() :
	min_(INT_MAX),
	max_(INT_MIN)
{

}

int Range::getRange() const
{
	return max_ - min_;
}

void Range::setMin(const int min)
{
	min_ = min;
}

void Range::setMax(const int max)
{
	max_ = max;
}

void Range::update(const int value)
{
	if (min_ > value)
		min_ = value;
	else
		if (max_ < value)
			max_ = value;
}


bool Range::operator < (const Range &range) const
{
	return this->getRange() < range.getRange();
}

bool Range::operator <= (const Range &range) const
{
	return this->getRange() <= range.getRange();
}

bool Range::operator > (const Range &range) const
{
	return this->getRange() > range.getRange();
}

bool Range::operator >= (const Range &range) const
{
	return this->getRange() >= range.getRange();
}

bool Range::operator == (const Range &range) const
{
	return this->getRange() == range.getRange();
}

Range Range::operator *(const double &value) const
{
	return { static_cast<int>(min_*value), static_cast<int>(max_*value) };
}

Range& Range::operator *=(const double &value)
{
	this->min_*=value;
	this->max_*=value;
	return *this;
}