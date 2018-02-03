#pragma once

class Range
{
public:
	Range(const int min,const int max);
	Range();
	int getRange() const;
	void setMin(const int min);
	void setMax(const int max);
	void update(const int value);
	bool operator < (const Range &range) const;
	bool operator <= (const Range &range) const;
	bool operator > (const Range &range) const;
	bool operator >= (const Range &range) const;
	bool operator == (const Range &range) const;
	Range operator *(const double &value) const;
	Range& operator *=(const double &value);
private:
	int _min;
	int _max;
private:
};