#include <iostream>
#include <string>

class BigInteger {
private:
	std::string s;
	bool sign;
	int SZ()const;
	std::string Reverse()const;
	BigInteger(bool sign, std::string s);
	void norm();
public:
	BigInteger();
	BigInteger(const int x);
	BigInteger& operator+=(const BigInteger & b);
	friend BigInteger operator+(const BigInteger & a, const BigInteger & b);
	BigInteger& operator-=(const BigInteger & b);
	friend BigInteger operator-(const BigInteger & a, const BigInteger & b);
	BigInteger& operator*=(const BigInteger & b);
	friend BigInteger operator*(const BigInteger & a, const BigInteger & b);
	BigInteger& operator/=(const BigInteger & b);
	friend BigInteger operator/(const BigInteger & a, const BigInteger & b);
	BigInteger& operator%=(const BigInteger & b);
	friend BigInteger operator%(const BigInteger & a, const BigInteger & b);
	BigInteger& operator++();
	BigInteger operator++(int);
	BigInteger& operator--();
	BigInteger operator--(int);
	BigInteger& operator=(const BigInteger & b);
	bool operator<(const BigInteger & b)const;
	bool operator>(const BigInteger & b)const;
	bool operator<=(const BigInteger & b)const;
	bool operator>=(const BigInteger & b)const;
	bool operator==(const BigInteger & b)const;
	bool operator!=(const BigInteger & b)const;
	BigInteger operator-()const;
	explicit operator bool()const;
	std::string toString()const;
	friend std::ostream& operator<<(std::ostream & in, const BigInteger & te);
	friend std::istream& operator >> (std::istream & fr, BigInteger & te);
};

int BigInteger::SZ()const {
	return (int)s.size();
}

std::string BigInteger::Reverse()const {
	std::string ret;
	for (int i = SZ() - 1; i >= 0; i--)
		ret.push_back(s[i]);
	return ret;
}

void BigInteger::norm() {
	if (SZ() == 0)
		s.push_back(0);
	while (SZ() > 1 && s.back() == '0')
		s.pop_back();
	if (SZ() == 1 && s[0] == '0')
		sign = false;
}

BigInteger::BigInteger() :s("0"), sign(false) {}

BigInteger::BigInteger(const int x) {
	int t = x;
	if (x >= 0)
		sign = false;
	else
		sign = true, t *= -1;
	while (t > 0) {
		s.push_back(t % 10 + '0');
		t /= 10;
	}
	if (SZ() == 0)
		s += "0";
}

BigInteger::BigInteger(bool sign_, std::string s_) {
	sign = sign_;
	s = s_;
	if (s == "0")
		sign = false;
}

BigInteger& BigInteger::operator+=(const BigInteger & b) {
	bool NewSign = (*this < -b);
	int carry = 0;
	int GoOver = 0;
	for (int i = 0; i < SZ() || i < b.SZ() || carry > 0; i++) {
		if (i >= SZ())s.push_back('0');
		int NewVal = (s[i] - '0') * (sign ^ NewSign ? -1 : 1) + carry - GoOver;
		if (i < b.SZ())
			NewVal += (b.s[i] - '0') * (b.sign ^ NewSign ? -1 : 1);
		GoOver = 0;
		if (NewVal < 0) { GoOver = 1; NewVal += 10; }
		s[i] = (NewVal % 10) + '0';
		carry = NewVal / 10;
	}
	sign = NewSign;
	norm();
	return *this;
}

BigInteger operator+(const BigInteger & a, const BigInteger & b) {
	BigInteger aa = a;
	return (aa += b);
}

BigInteger& BigInteger::operator-=(const BigInteger & b) {
	return (*this += -b);
}

BigInteger operator-(const BigInteger & a, const BigInteger & b) {
	BigInteger aa = a;
	return (aa -= b);
}

BigInteger& BigInteger::operator*=(const BigInteger & b) {
	BigInteger ret = 0;
	ret.sign = sign ^ b.sign;
	for (int i = 0; i < SZ(); i++) {
		int Add = 0;
		int posRet = i;
		for (int j = 0; j < b.SZ() || Add > 0; j++, posRet++) {
			if (posRet >= ret.SZ())ret.s.push_back('0');
			int val = s[i] - '0';
			if (j < b.SZ())
				val *= (b.s[j] - '0');
			else
				val = 0;
			val += Add;
			val += (ret.s[posRet] - '0');
			Add = val / 10;
			ret.s[posRet] = (val % 10) + '0';
		}
	}
	ret.norm();
	*this = ret;
	return *this;
}

BigInteger operator*(const BigInteger & a, const BigInteger & b) {
	BigInteger aa = a;
	return (aa *= b);
}

BigInteger& BigInteger::operator/=(const BigInteger & b) {
	BigInteger del = b;
	if (del < 0)del *= -1;
	BigInteger ret = 0;
	ret.sign = sign ^ b.sign;
	BigInteger R = 0;
	for (int i = SZ() - 1; i >= 0; i--) {
		R = R * 10 + (s[i] - '0');
		int x; for (x = 9; R < x * del; x--);
		R -= x * del;
		ret.s.push_back(x + '0');
	}
	ret.s = ret.Reverse();
	ret.norm();
	*this = ret;
	return *this;
}

BigInteger operator/(const BigInteger & a, const BigInteger & b) {
	BigInteger aa = a;
	return (aa /= b);
}

BigInteger& BigInteger::operator%=(const BigInteger & b) {
	return (*this = *this - (*this / b) * b);
}

BigInteger operator%(const BigInteger & a, const BigInteger & b) {
	BigInteger aa = a;
	return (aa %= b);
}

BigInteger& BigInteger::operator++() {
	return (*this += 1);
}

BigInteger BigInteger::operator++(int) {
	BigInteger oldValue = *this;
	++*this;
	return oldValue;
}

BigInteger& BigInteger::operator--() {
	return (*this -= 1);
}

BigInteger BigInteger::operator--(int) {
	BigInteger oldValue = *this;
	--*this;
	return oldValue;
}

BigInteger& BigInteger::operator=(const BigInteger & b) {
	if (this == &b)return *this;
	sign = b.sign;
	s.clear();
	for (int i = 0; i < b.SZ(); i++)
		s.push_back(b.s[i]);
	return *this;
}

bool BigInteger::operator<(const BigInteger & b)const {
	if (sign && !b.sign)return true;
	if (b.sign && !sign)return false;
	if (SZ() > b.SZ())return sign;
	if (SZ() < b.SZ())return !sign;
	for (int i = SZ() - 1; i >= 0; i--) {
		if (s[i] < b.s[i])return !sign;
		if (s[i] > b.s[i])return sign;
	}
	return false;
}

bool BigInteger::operator>(const BigInteger & b)const {
	return !(*this <= b);
}

bool BigInteger::operator<=(const BigInteger & b)const {
	return (*this < b || *this == b);
}

bool BigInteger::operator>=(const BigInteger & b)const {
	return (*this > b || *this == b);
}

bool BigInteger::operator==(const BigInteger & b)const {
	if (sign != b.sign || SZ() != b.SZ())return false;
	for (int i = 0; i < SZ(); i++)
		if (s[i] != b.s[i])return false;
	return true;
}

bool BigInteger::operator!=(const BigInteger & b)const {
	return !(*this == b);
}

BigInteger BigInteger::operator-()const {
	return BigInteger(sign ^ 1, s);
}

BigInteger::operator bool()const {
	return (*this != 0);
}

std::string BigInteger::toString()const {
	return ((sign ? "-" : "") + Reverse());
}

std::ostream& operator << (std::ostream & in, const BigInteger & te) {
	in << te.toString();
	return in;
}

std::istream& operator >> (std::istream & fr, BigInteger & te) {
	fr >> te.s;
	te.sign = false;
	if (te.s[0] == '-') {
		te.sign = true;
		te.s = te.s.substr(1, (int)te.s.size() - 1);
	}
	te.s = te.Reverse();
	te.norm();
	return fr;
}