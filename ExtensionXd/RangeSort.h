#pragma once
#include <vector>
#include "PixmapToStringConverter.h"
#include "Range.h"


class RangeSort
{
public:
	RangeSort(const Pixmap* const pixmap, const size_t numOfColors);
	~RangeSort() = default;
	std::vector<Rgb> sort();
private:
	const Pixmap * const originalPixmap_;
	std::vector<Rgb> pixmap_;
	size_t numOfColors_;
	Range rRange_;
	Range gRange_;
	Range bRange_;
	bool isLastPartition_;
private:
	void checkResources() const;
	void createNewPixmap(const Pixmap * const pixmap);
	void sortBuckets(const size_t left, const size_t right);
	void findRanges(const size_t left, const size_t right);
	void initRanges(const size_t index);
	void updateRanges(const size_t index);
	void sortByMaxRange(const size_t left, const size_t right);
	void callRecursionIfNecessary(const size_t left, const size_t right);
	bool isLastPartition(const size_t left, const size_t right) const;	
	RangeSort(const RangeSort&);
	RangeSort& operator=(const RangeSort&);
};
