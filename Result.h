#pragma once

struct Result
{
	Result() : value(0), slot(0) {}
	Result(int value) : value(value), slot(0) {}
	Result(int value, int slot) : value(value), slot(slot) {}
	Result(const Result& result, int slot) : slot(slot), value(result.value) {}

	Result operator-() const { return Result(-value, slot); }
	Result operator/(int value) { return Result(this->value / value, slot); }
	bool operator>=(const Result& other) const { return value >= other.value; }
	bool operator>(const Result& other) const { return value > other.value; }

	int value;
	int slot;
};

