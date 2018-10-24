#include <iostream>
#include <string>
#include "BigInteger.h"

class Rational {
private:
	BigInteger p, q;
	BigInteger gcd(const BigInteger & a, const BigInteger & b);
	void reduce();
	Rational(const BigInteger & p_, const BigInteger & q_);
public:
	Rational();
	Rational(const BigInteger& a);
	Rational(const int & a);
	Rational& operator=(const Rational & b);
	Rational& operator+=(const Rational & b);
	friend Rational operator+(const Rational & a, const Rational & b);
	Rational& operator-=(const Rational & b);
	friend Rational operator-(const Rational & a, const Rational & b);
	Rational& operator*=(const Rational & b);
	friend Rational operator*(const Rational & a, const Rational & b);
	Rational& operator/=(const Rational & b);
	friend Rational operator/(const Rational & a, const Rational & b);

	bool operator<(const Rational & b)const;
	bool operator>(const Rational & b)const;
	bool operator<=(const Rational & b)const;
	bool operator>=(const Rational & b)const;
	bool operator==(const Rational & b)const;
	bool operator!=(const Rational & b)const;
	Rational operator-()const;

	std::string asDecimal(size_t precision)const;
	std::string toString()const;
	explicit operator double()const;
};

BigInteger Rational::gcd(const BigInteger & a, const BigInteger & b) {
	BigInteger aa = a;
	if (aa < 0)aa *= -1;
	BigInteger bb = b;
	if (bb < 0)bb *= -1;
	while (bb) {
		aa %= bb;
		auto cc = aa;
		aa = bb;
		bb = cc;
	}
	return aa;
}

void Rational::reduce() {
	BigInteger g = gcd(p, q);
	p /= g;
	q /= g;
	if (q < 0) {
		q *= -1;
		p *= -1;
	}
}

Rational::Rational(const BigInteger & p_, const BigInteger & q_) {
	p = p_;
	q = q_;
	reduce();
}

Rational::Rational() :p(0), q(1) {}

Rational::Rational(const BigInteger & a) : p(a), q(1) {}

Rational::Rational(const int & a) : p(a), q(1) {}

Rational& Rational::operator=(const Rational & b) {
	if (this == &b)return *this;
	p = b.p;
	q = b.q;
	return *this;
}

Rational& Rational::operator+=(const Rational & b) {
	p = p * b.q + q * b.p;
	q *= b.q;
	reduce();
	return *this;
}

Rational operator+(const Rational & a, const Rational & b) {
	Rational aa = a;
	return (aa += b);
}

Rational& Rational::operator-=(const Rational & b) {
	p = p * b.q - q * b.p;
	q *= b.q;
	reduce();
	return *this;
}

Rational operator-(const Rational & a, const Rational & b) {
	Rational aa = a;
	return (aa -= b);
}

Rational& Rational::operator*=(const Rational & b) {
	p *= b.p;
	q *= b.q;
	reduce();
	return *this;
}

Rational operator*(const Rational & a, const Rational & b) {
	Rational aa = a;
	return (aa *= b);
}

Rational& Rational::operator/=(const Rational & b) {
	p *= b.q;
	q *= b.p;
	reduce();
	return *this;
}

Rational operator/(const Rational & a, const Rational & b) {
	Rational aa = a;
	return (aa /= b);
}

bool Rational::operator<(const Rational & b)const {
	return p * b.q < b.p * q;
}

bool Rational::operator>(const Rational & b)const {
	return p * b.q > b.p * q;
}

bool Rational::operator<=(const Rational & b)const {
	return p * b.q <= b.p * q;
}

bool Rational::operator>=(const Rational & b)const {
	return p * b.q >= b.p * q;
}

bool Rational::operator==(const Rational & b)const {
	return p * b.q == b.p * q;
}

bool Rational::operator!=(const Rational & b)const {
	return p * b.q != b.p * q;
}

Rational Rational::operator-()const {
	return Rational(-p, q);
}

std::string Rational::asDecimal(size_t precision = 0)const {
	BigInteger P = p, Q = q;
	bool sign = false;
	if (P < 0) { sign = true; P *= -1; }
	BigInteger whole = P / Q;
	std::string ret = (sign ? "-" : "") + whole.toString();
	if (P < 0) { P *= -1; whole *= -1; }
	P -= whole * Q;
	int startI = (int)ret.size() + 1;
	ret += ((precision == 0 ? "" : ".") + std::string(precision, ' '));
	for (size_t i = startI; i < startI + precision; i++) {
		P = 10 * P;
		int Let = (int)(P / Q);
		P %= Q;
		ret[i] = Let + '0';
	}
	return ret;
}

std::string Rational::toString()const {
	std::string ret = p.toString();
	if (q != 1)ret += "/" + q.toString();
	return ret;
}

Rational::operator double()const {
	BigInteger P = p, Q = q;
	bool sign = false;
	if (P < 0) { sign = true; P *= -1; }
	BigInteger whole = P / Q;
	double ans = (double)(int)whole;
	double pos = (double)1;
	P -= whole * Q;
	while (pos != 0) {
		P = 10 * P;
		pos /= 10;
		int Let = (int)(P / Q);
		P %= Q;
		ans += pos * Let;
	}
	if (sign)ans *= -1;
	return ans;
}