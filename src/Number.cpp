#include "Number.h"

struct Pows {
public:
    static double getPow(int exp) {
        pows[MAX_EXP] = 1.;
        int idx = exp + MAX_EXP;
        if (idx < 0 || idx > pows.size()) { return 0.; }
        if (idx < MAX_EXP) {
            for (lb; lb >= idx; --lb) {
                pows[lb] = pows[lb + 1] * .1;
            }
        }
        else {
            for (ub; ub <= idx; ++ub) {
                pows[ub] = pows[ub - 1] * 10;
            }
        }
        return pows[idx];
    }
private:
    static std::array<double, MAX_EXP * 2 + 1> pows;
    static size_t lb, ub;
};
std::array<double, MAX_EXP * 2 + 1> Pows::pows;
size_t Pows::lb = MAX_EXP - 1, Pows::ub = MAX_EXP + 1;

Number::Number() {}
Number::Number(double val) { setValue(val); }
Number::Number(double val, int exp) { setValue(val, exp); }
Number::Number(const Number& num) { 
    value = num.value;
    exponent = num.exponent;
    zero = num.zero;
}
Number::~Number() {}

void Number::copy(const Number& other) {
    value = other.value;
    exponent = other.exponent;
    zero = other.zero;
}

void Number::setValue(double val) {
    value = val;
    balance();
}

void Number::setValue(double val, int exp) {
    value = val;
    exponent = exp;
    balance();
}

void Number::balance() {
    if (value == 0.) { zero = true; }
    if (zero) { return; }
    while (abs(value) >= 10) {
        value *= .1;
        exponent++;
    }
    while (abs(value) < 1) {
        value *= 10.;
        exponent--;
    }
    value = round(value * Pows::getPow(TOLERANCE)) * Pows::getPow(-TOLERANCE); 
    if (value == 0.) { zero = true; }
}

// Math Functions
Number& Number::operator+=(const Number& rhs) {
    int diff = exponent - rhs.exponent;
    if (zero || diff <= -TOLERANCE) { copy(rhs); }
    else if (!rhs.zero && diff < TOLERANCE) {
        if (diff > 0) {
            setValue(value + rhs.value * Pows::getPow(-diff));
        }
        else {
            setValue(rhs.value + value * Pows::getPow(diff), rhs.exponent);
        }
    }
    return *this;
}

Number& Number::operator-=(const Number& rhs) {
    return *this += (Number(-rhs.value, rhs.exponent));
}

Number& Number::operator*=(const Number& num) {
    if (zero || num.zero) { zero = true; }
    else {
         setValue(value * num.value, exponent + num.exponent);
    }
    return *this;
}

Number& Number::operator/=(const Number& num) {
    if (num.zero) {
        throw std::domain_error("Divide by zero error");
    }
    else if (!zero) {
        setValue(value / num.value, exponent - num.exponent);
    }
    return *this;
}

Number& Number::operator^=(double power) {
    if (zero) {
        if (power == 0) {
            throw std::domain_error("0^0 error");
        }
    }
    else {
        int exp = (int)(exponent * power);
        double expDecimal = exponent * power - exp;
        double val = 1;
        while (power >= 1) {
            val *= value;
            if (val >= 10) {
                val /= 10;
                exp++;
            }
            power--;
        }
        if (power != 0) {
            val *= pow(value, power) * pow(10, expDecimal);
        }
        setValue(val, exp);
    }
    return *this;
}

Number Number::logBase(double base) const {
    if (base == 1 || base < 1) {
        throw std::domain_error("Logb(x): Invalid base b for logarithm, must be b >= 0 and b != 1");
        return Number();
    }
    if (zero || value < 0) {
        throw std::domain_error("Logb(x): Invalid input x for logarithm, must be x > 0");
        return Number();
    }
    return Number((exponent + log10(value)) / log10(base));
}

// Comparison Operators
bool Number::operator ==(const Number& rhs) const {
    return (zero && rhs.zero) || (!zero && !rhs.zero &&
        abs(value - rhs.value) < Pows::getPow(-TOLERANCE) && exponent == rhs.exponent);
}

bool operator <(const Number& lhs, const Number& rhs) {
    if (lhs == rhs) { return false; }
    if (lhs.zero) { return rhs.value > 0; }
    if (rhs.zero) { return lhs.value < 0; }
    double v1 = lhs.value, v2 = rhs.value;
    int e1 = lhs.exponent, e2 = rhs.exponent;
    // Opposite signs, lhs is smaller if it is negative
    if (v1 * v2 < 0) { return v1 < 0; }
    // Exponents are equal, compare values
    if (e1 == e2) { return v1 < v2; }
    // Exponents are not equal, compare exponenet and sign
    return (e1 < e2) == (v1 >= 0);
}

std::ostream& operator <<(std::ostream& os, const Number& rhs) {
    if (rhs.zero) { os << "0"; return os; }
    int e = rhs.exponent;
    int idx = 0;
    int pointIdx = (e < 0 || e > 2) ? 1 : e + 1;
    // Get digits
    for (char ch : std::to_string(rhs.value)) {
        if (isdigit(ch)) {
            if (idx == pointIdx) { os << "."; }
            os << ch;
            idx++;
            if (idx == SIG_FIGS) { break; }
        }
        else if (ch == '-') { os << "-"; }
    }
    for (idx; idx < SIG_FIGS; ++idx) {
        if (idx == pointIdx) { os << "."; }
        os << "0";
    }
    if (e < 0 || e > 2) {
        os << "e" << e;
    }
    return os;
}