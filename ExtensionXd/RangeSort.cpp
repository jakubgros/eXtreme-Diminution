#include "RangeSort.h"

RangeSort::RangeSort(const Pixmap* const pixmap, const size_t numOfColors)
	: originalPixmap_(pixmap),
	pixmap_(),
	numOfColors_(numOfColors),
	rRange_(),
	gRange_(),
	bRange_(),
	isLastPartition_(false)
{

}

std::vector<Rgb> RangeSort::sort()
{
	checkResources();
	createNewPixmap(originalPixmap_);;
	sortBuckets(0, pixmap_.size()-1);
	return pixmap_;
}

void RangeSort::checkResources() const
{
	if (originalPixmap_ == nullptr)
		throw std::logic_error("RangeSort - nullptr resources");
}

void RangeSort::createNewPixmap(const Pixmap* const pixmap)
{
	for (size_t y = 0; y < (*pixmap)[0].size(); ++y)
		for (size_t x = 0; x < (*pixmap).size(); ++x)
			pixmap_.push_back((*pixmap)[x][y]);
}

void RangeSort::sortBuckets(const size_t left, const size_t right)
{
	findRanges(left, right);
	sortByMaxRange(left, right);
	callRecursionIfNecessary(left, right);
}

void RangeSort::findRanges(const size_t left, const size_t right)
{
	initRanges(left);
	for (int i = left; i <= right; ++i)
		updateRanges(i);
}

void RangeSort::initRanges(const size_t index)
{
	rRange_.setMin(pixmap_[index].r);
	rRange_.setMax(pixmap_[index].r);
	gRange_.setMin(pixmap_[index].g);
	gRange_.setMax(pixmap_[index].g);
	bRange_.setMin(pixmap_[index].b);
	bRange_.setMax(pixmap_[index].b);
}

void RangeSort::updateRanges(const size_t index)
{
	rRange_.update(pixmap_[index].r);
	gRange_.update(pixmap_[index].g);
	bRange_.update(pixmap_[index].b);
}

bool compareR(const Rgb& a, const Rgb& b)
{
	return a.r < b.r;
}
bool compareG(const Rgb& a, const Rgb& b)
{
	return a.g < b.g;
}
bool compareB(const Rgb& a, const Rgb& b)
{
	return a.b < b.b;
}

void RangeSort::sortByMaxRange(const size_t left, const size_t right)
{
	if (rRange_ == std::max(rRange_, std::max(bRange_, gRange_)))
		std::sort(pixmap_.begin() + left, pixmap_.begin() + right + 1, compareR);
	else
		if (gRange_ == std::max(gRange_, bRange_))
			std::sort(pixmap_.begin() + left, pixmap_.begin() + right + 1, compareG);
		else
			std::sort(pixmap_.begin() + left, pixmap_.begin() + right + 1, compareB);
}

void RangeSort::callRecursionIfNecessary(const size_t left, const size_t right)
{
	if (!isLastPartition(left, right))
		sortBuckets(left, left + (right - left) / 2);
	if (!isLastPartition(left, right))
		sortBuckets(left + (right - left) / 2 + 1, right);
}

bool RangeSort::isLastPartition(const size_t left, const size_t right) const
{
	return (right - left <= pixmap_.size() / numOfColors_);
}