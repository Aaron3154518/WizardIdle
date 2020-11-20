#ifndef NUMBER_h
#define NUMBER_h

#include <iostream>
#include <iomanip>
#include <cmath>
#include <array>
#include <string>
#include <limits>
#include <stdexcept>

#include "Definitions.h"

constexpr auto TOLERANCE = 10;
constexpr auto MAX_EXP = std::numeric_limits<double>::max_exponent10;
constexpr auto SIG_FIGS = 3;

class Number {
public:
	Number();
	Number(double val);
	Number(double val, int exp);
	Number(const Number& num);
	~Number();

	void copy(const Number& other);
	void setValue(double val);
	void setValue(double val, int exp);

	double getValue() const { return value; }
	int getExponent() const { return exponent; }

	void balance();
	bool isZero() const { return zero; }

	// Math Operators
	Number& operator +=(const Number& rhs);
	friend Number operator +(Number lhs, const Number& rhs) { return lhs += rhs; }
	Number& operator -=(const Number& rhs);
	friend Number operator -(Number lhs, const Number& rhs) { return lhs -= rhs; }
	Number& operator *=(const Number& rhs);
	friend Number operator *(Number lhs, const Number& rhs) { return lhs *= rhs; }
	Number& operator /=(const Number& rhs);
	friend Number operator /(Number lhs, const Number& rhs) { return lhs /= rhs; }
	Number& operator ^=(double rhs);
	friend Number operator ^(Number lhs, double rhs) { return lhs ^= rhs; }
	Number logBase(double base) const;

	// Comparison Operators
	bool operator ==(const Number& rhs) const;
	friend bool operator <(const Number& lhs, const Number& rhs);
	friend bool operator <=(const Number& lhs, const Number& rhs) { return lhs == rhs || lhs < rhs; }
	friend bool operator >(const Number& lhs, const Number& rhs) { return rhs < lhs; }
	friend bool operator >=(const Number& lhs, const Number& rhs) { return lhs == rhs || lhs > rhs; }
	static Number getMin(const Number& a, const Number& b) { return a <= b ? a : b; }
	static Number getMax(const Number& a, const Number& b) { return a >= b ? a : b; }

	friend std::ostream& operator <<(std::ostream& os, const Number& rhs);

private:
	int exponent = 0;
	double value = 0.;
	bool zero = false;
};

#endif /* Number */