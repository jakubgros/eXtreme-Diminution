#include "Types.h"

Rgb operator+ (const Rgb &left, const Rgb &right)
{
	return Rgb{ left.r + right.r, left.g + right.g, left.b + right.b };
}

Rgb operator-(const Rgb &left, const Rgb &right)
{
	return Rgb{ left.r - right.r, left.g - right.g, left.b - right.b };
}

Rgb operator*(const double &left, const Rgb &right)
{
	return Rgb{ int(left*right.r), int(left*right.g), int(left*right.b) };
}

Rgb operator*(const Rgb& left, const double &right)
{
	return Rgb{ int(left.r*right), int(left.g*right), int(left.b*right) };
}

bool operator !=(const Rgb& left,const Rgb& right)
{
	if (left.r == right.r)
		if (left.g == right.g)
			if (left.b == right.b)
				return false;
	return true;

}

bool operator ==(const Rgb& left,const Rgb& right)
{
	if (left.r == right.r)
		if (left.g == right.g)
			if (left.b == right.b)
				return true;
	return false;
}