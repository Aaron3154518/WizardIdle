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

class Number {
public:
    Number() = default;
    Number(int layer, double exp, int sign); // From tetration notation
    Number(double val); // From a double
    Number(double val, int exp); // From scientific notation
    Number(std::string str); // From string
    ~Number() = default;

    double toDouble() const;

    Number copy() const;
    void copy(const Number& n);

    void balance();
    bool isZero() const { return mSign == 0; }

    // Math operators
    // -n
    Number& negate() { mSign *= -1; return *this; }
    Number operator -() const { return copy().negate(); }
    // n + m
    Number& add(const Number& num);
    Number& operator +=(const Number& rhs) { add(rhs); return *this; }
    // n - m
    Number& subtract(const Number& num);
    Number& operator -=(const Number& rhs) { subtract(rhs); return *this; }
    // n * m
    Number& multiply(const Number& num);
    Number& operator *=(const Number& rhs) { multiply(rhs); return *this; }
    // n / m
    Number& divide(const Number& num);
    Number& operator /=(const Number& rhs) { divide(rhs); return *this; }
    // n ^ m
    Number& power(const Number& num);
    Number& operator ^=(const Number& rhs) { power(rhs); return *this; }

    // Math functions - fooIP() writes the result to the caller and returns the caller
    //                - foo() returns the result as a separate object and returns that
    // returns x given n = 10^x (e.g. 10^10^10^9 would return 10^10^9)
    Number& getExponentIP();
    Number getExponent() const { return copy().getExponentIP(); }
    // 1/n
    Number& getReciprocalIP();
    Number getReciprocal() const { return copy().getReciprocalIP(); }
    // |n|
    Number& absValIP() { mSign *= mSign; return *this; }
    Number absVal() const { return copy().absValIP(); }
    // 10^n
    Number& powTenIP();
    Number powTen() const { return copy().powTenIP(); }
    // log(n)
    Number& logTenIP();
    Number logTen() const { return copy().logTenIP(); }
    // logb(n)
    Number& logBaseIP(Number base);
    Number logBase(const Number& base) const { return copy().logBaseIP(base); }

    // Comparison Functions
    bool equal(const Number& other) const;
    bool less(const Number& other) const;

    void printAll() const;

    friend std::ostream& operator <<(std::ostream& os, const Number& rhs);

private:
    // Default value = 0
    double mExp = 0.;
    int mLayer = 0, mSign = 0;
};

// Binary operators
static Number operator +(const Number& lhs, const Number& rhs) { return lhs.copy() += rhs; }
static Number operator -(const Number& lhs, const Number& rhs) { return lhs.copy() -= rhs; }
static Number operator *(const Number& lhs, const Number& rhs) { return lhs.copy() *= rhs; }
static Number operator /(const Number& lhs, const Number& rhs) { return lhs.copy() /= rhs; }
static Number operator ^(const Number& lhs, const Number& rhs) { return lhs.copy() ^= rhs; }
static bool operator ==(const Number& lhs, const Number& rhs) { return lhs.equal(rhs); }
static bool operator !=(const Number& lhs, const Number& rhs) { return !lhs.equal(rhs); }
static bool operator <(const Number& lhs, const Number& rhs) { return lhs.less(rhs); }
static bool operator <=(const Number& lhs, const Number& rhs) { return !rhs.less(lhs); }
static bool operator >(const Number& lhs, const Number& rhs) { return rhs.less(lhs); }
static bool operator >=(const Number& lhs, const Number& rhs) { return !lhs.less(rhs); }
static Number min(const Number& a, const Number& b) { return a <= b ? a : b; }
static Number max(const Number& a, const Number& b) { return a >= b ? a : b; }

static double round(double val, double precision);

#endif
