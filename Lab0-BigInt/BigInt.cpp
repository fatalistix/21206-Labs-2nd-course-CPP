#include "BigInt.h"


namespace LongMath {
    // Realisation of private methods
    void BigInt::addRadix() {
        numberArr.push_back(isNegative ? UINT8_MAX : 0);
    }

    void BigInt::purgeRadix() {
        for (size_t i = numberArr.size() - 1; i > 0; i--) {
            if (!numberArr[i] && !isNegative || numberArr[i] == UINT8_MAX && isNegative){
                numberArr.pop_back();
            } else {
                break;
            }
        }
    }

    BigInt &BigInt::operator>>=(size_t shift) {
        const size_t j(shift / UINT8_WIDTH);
        const size_t k(shift % UINT8_WIDTH);
        for (size_t i = 0; i < numberArr.size(); i++) {
            const int buf = (i + j < numberArr.size() ? numberArr[i + j] : isNegative ? UINT8_MAX : 0) +
                            ((i + j + 1 < numberArr.size() ? numberArr[i + j + 1] : isNegative ? UINT8_MAX : 0)
                                    << UINT8_WIDTH);
            numberArr[i] = (buf >> int(k)) & UINT8_MAX;
        }
        purgeRadix();
        return *this;
    }

    //
    // Realisation of public members
    //

    // Constructors
    BigInt::BigInt() = default;

    BigInt::BigInt(int numberInt) {
        isNegative = (numberInt >> (INT32_WIDTH - 1)) & 1;

        for (size_t i = 0; i < sizeof(int); i++){
            numberArr.push_back((numberInt >> (i * UINT8_WIDTH)) & UINT8_MAX);
        }

        this->purgeRadix();
    }

    BigInt::BigInt(std::string s) {
        const size_t haveSign = (s[0] == '+' || s[0] == '-');
#ifndef DEBUG
        try
#endif
        {
            {
                if (s.empty()) {
                    throw std::invalid_argument("expected number, got nothing");
                }
                if (haveSign && s.size() == 1) {
                    throw std::invalid_argument("expected number, got only sign");
                }
                if (!std::isdigit(s[haveSign])) {
                    throw std::invalid_argument("expected digit, got \"" +
                                                std::string(1, s[haveSign]) +
                                                "\" in pos " +
                                                std::to_string(haveSign) +
                                                ", which is not a digit");
                }
            }
            numberArr.push_back(s[haveSign] - '0');

            for (size_t i = haveSign + 1; i < s.size(); i++) {
                if (!std::isdigit(s[i])) {
                    throw std::invalid_argument("expected digit, got \"" +
                                                std::string(1, s[i]) +
                                                "\" in pos " +
                                                std::to_string(i) +
                                                ", which is not a digit");
                }
                (*this) *= DEC;
                (*this) += BigInt(s[i] - '0');
            }
        }
#ifndef DEBUG
        catch (const std::invalid_argument& e)
        {
            std::cerr << e.what() << std::endl;
            numberArr   = ZERO.numberArr;
            isNegative ^= isNegative;
        }
#endif

        if (haveSign && s[0] == '-') {
            *this = -(*this);
        }

        purgeRadix();
    }

    BigInt::BigInt(const BigInt &numberBI) :
            isNegative(numberBI.isNegative) {
        numberArr = numberBI.numberArr;
    }

    BigInt::BigInt(BigInt &&numberBI) noexcept:
            isNegative(numberBI.isNegative),
            numberArr (std::move(numberBI.numberArr)) {}

    // Destructor
    BigInt::~BigInt() = default;

    //
    // Operators
    //

    // Assign operators
    BigInt &BigInt::operator=(const BigInt &numberBI) = default;

    BigInt &BigInt::operator=(BigInt &&numberBI) noexcept {
        isNegative = numberBI.isNegative;
        numberArr  = std::move(numberBI.numberArr);
        return *this;
    }

    // Invert bytes operator
    BigInt BigInt::operator~() const {
        BigInt forRet;

        for (uchar c: this->numberArr) {
            forRet.numberArr.push_back(~c);
        }

        forRet.isNegative = !isNegative;

        forRet.purgeRadix();
        return forRet;
    }

    // Increment operators
    BigInt &BigInt::operator++() {
        *this += ONE;
        return *this;
    }

    const BigInt BigInt::operator++(int) {
        BigInt forRet(*this);
        ++(*this);
        return forRet;
    }

    // Decrement operators
    BigInt &BigInt::operator--() {
        *this -= ONE;
        return *this;
    }

    const BigInt BigInt::operator--(int) {
        BigInt forRet(*this);
        --(*this);
        return forRet;
    }

    // Operators with "="
    BigInt &BigInt::operator+=(const BigInt &numberBI) {
        for (size_t i = numberArr.size(); i < numberBI.numberArr.size(); i++) {
            addRadix();
        }
        addRadix();

        unsigned short carry = 0;
        for (size_t i = 0; i < numberArr.size(); i++) {
            carry += numberArr[i] + (i < numberBI.numberArr.size() ?
                                     numberBI.numberArr[i] : numberBI.isNegative ?
                                                             UINT8_MAX : 0);
            numberArr[i] = uchar(carry & UINT8_MAX);
            carry >>= UINT8_WIDTH;
        }

        if (((numberArr[numberArr.size() - 1] >> (UINT8_WIDTH - 1)) & 1) != isNegative) {
            isNegative = !isNegative;
        }

        purgeRadix();

        return *this;
    }

    BigInt &BigInt::operator*=(const BigInt &numberBI) {
        const BigInt a =          isNegative ? -(*this)  : *this;
        const BigInt b = numberBI.isNegative ? -numberBI : numberBI;

        BigInt answer(0);

        for (size_t i = 1; i < b.numberArr.size(); i++) {
            answer.numberArr.push_back(0);
        }

        for (size_t i = answer.numberArr.size(); i < a.size(); i++) {
            answer.numberArr.push_back(0);
        }

        for (size_t i = 0; i < b.numberArr.size(); i++) {
            unsigned long long carry = 0;
            answer.numberArr.push_back(0);
            for (size_t j = 0; j < a.numberArr.size(); j++) {
                carry += answer.numberArr[i + j] + a.numberArr[j] * b.numberArr[i];
                answer.numberArr[i + j] = (uchar) (carry & UINT8_MAX);
                carry >>= 8;
            }
            answer.numberArr[i + numberArr.size()] += carry;
        }

        answer.purgeRadix();

        isNegative ^= numberBI.isNegative;

        if (isNegative) {
            *this = -answer;
        } else {
            *this = answer;
        }

        return *this;
    }

    BigInt &BigInt::operator-=(const BigInt &numberBI) {
        const BigInt inverted(-numberBI);
        *this += inverted;
        return *this;
    }

    BigInt &BigInt::operator/=(const BigInt &numberBI) {
        if (numberBI == ZERO) {
            throw std::invalid_argument("division by zero");
        }
        const BigInt a(         isNegative ? -(*this)  : *this);
        const BigInt b(numberBI.isNegative ? -numberBI : numberBI);

        BigInt l(0);
        BigInt r(a);
        ++r;
        BigInt m;
        BigInt answer;

        while (l < r) {
            m = r + l;
            m >>= 1;
            if (m * b > a) {
                r = m;
            } else if (m * b < a - b + ONE) {
                l = m + ONE;
            } else {
                answer = m;
                break;
            }
        }
        if (l == r) {
            answer = l;
        }

        isNegative ^= numberBI.isNegative;

        if (isNegative) {
            *this = -answer;
        } else {
            *this = answer;
        }

        return *this;
    }

    BigInt &BigInt::operator^=(const BigInt &numberBI) {
        for (size_t i = numberArr.size(); i < numberBI.numberArr.size(); i++) {
            addRadix();
        }

        for (size_t i = 0; i < numberArr.size(); i++) {
            numberArr[i] ^= (i < numberBI.numberArr.size() ?
                             numberBI.numberArr[i] : isNegative ? UINT8_MAX : 0);
        }

        isNegative ^= numberBI.isNegative;

        purgeRadix();

        return *this;
    }

    BigInt &BigInt::operator%=(const BigInt &numberBI) {
        BigInt buf(*this);
        buf /= numberBI;
        buf *= numberBI;
        *this -= buf;
        return *this;
    }

    BigInt &BigInt::operator&=(const BigInt &numberBI) {
        for (size_t i = numberArr.size(); i < numberBI.numberArr.size(); i++) {
            addRadix();
        }

        for (size_t i = 0; i < numberArr.size(); i++) {
            numberArr[i] &= (i < numberBI.numberArr.size() ?
                             numberBI.numberArr[i] : isNegative ? UINT8_MAX : 0);
        }

        isNegative &= numberBI.isNegative;

        purgeRadix();

        return *this;
    }

    BigInt &BigInt::operator|=(const BigInt &numberBI) {
        for (size_t i = numberArr.size(); i < numberBI.numberArr.size(); i++) {
            addRadix();
        }

        for (size_t i = 0; i < numberArr.size(); i++) {
            numberArr[i] |= (i < numberBI.numberArr.size() ?
                             numberBI.numberArr[i] : isNegative ? UINT8_MAX : 0);
        }

        isNegative |= numberBI.isNegative;

        purgeRadix();

        return *this;
    }

    // Addon

    uchar BigInt::operator%(const uchar& numberUC)
    {
        const BigInt a(numberUC);
        BigInt forRet(*this);
        forRet %= a;
        return forRet.numberArr[0];
    }

    // Unary sign operators
    BigInt BigInt::operator+() const {
        return *this;
    }

    BigInt BigInt::operator-() const {
        BigInt forRet(~(*this));
        ++forRet;
        return forRet;
    }

    // Bool operators
    bool BigInt::operator==(const BigInt &numberBI) const {
        if (!(isNegative ^ numberBI.isNegative) &&
            numberArr == numberBI.numberArr) {
            return true;
        }
        return false;
    }

    bool BigInt::operator!=(const BigInt &numberBI) const {
        return !((*this) == numberBI);
    }

    bool BigInt::operator<(const BigInt &numberBI) const {
        if (isNegative != numberBI.isNegative) {
            if (isNegative) {
                return true;
            }
            return false;
        }

        for (size_t j = (numberArr.size() > numberBI.numberArr.size() ?
                         numberArr.size() : numberBI.numberArr.size()); j > 0; j--) {
            const size_t i(j - 1);

            if ((i < numberArr.size() ? numberArr[i] : isNegative ? UINT8_MAX : 0) >
                (i < numberBI.numberArr.size() ? numberBI.numberArr[i] : numberBI.isNegative ? UINT8_MAX : 0)) {
                return false;
            }

            if ((i < numberArr.size() ? numberArr[i] : isNegative ? UINT8_MAX : 0) <
                (i < numberBI.numberArr.size() ? numberBI.numberArr[i] : numberBI.isNegative ? UINT8_MAX : 0)) {
                return true;
            }
        }
        return false;
    }

    bool BigInt::operator>(const BigInt &numberBI) const {
        if (isNegative != numberBI.isNegative) {
            if (isNegative) {
                return false;
            }
            return true;
        }

        for (size_t j = (numberArr.size() > numberBI.numberArr.size() ?
                         numberArr.size() : numberBI.numberArr.size()); j > 0; j--) {
            const size_t i(j - 1);

            if ((i < numberArr.size() ? numberArr[i] : isNegative ? UINT8_MAX : 0) <
                (i < numberBI.numberArr.size() ? numberBI.numberArr[i] : numberBI.isNegative ? UINT8_MAX : 0)) {
                return false;
            }

            if ((i < numberArr.size() ? numberArr[i] : isNegative ? UINT8_MAX : 0) >
                (i < numberBI.numberArr.size() ? numberBI.numberArr[i] : numberBI.isNegative ? UINT8_MAX : 0)) {
                return true;
            }
        }
        return false;
    }

    bool BigInt::operator<=(const BigInt &numberBI) const {
        return !(*this > numberBI);
    }

    bool BigInt::operator>=(const BigInt &numberBI) const {
        return !(*this < numberBI);
    }

    // Different object's convertors
    BigInt::operator int() const {
        int forRet(0);
        for (size_t i = 0; i < sizeof(int); i++) {
            forRet |= (i < numberArr.size() ?
                       numberArr[i] : isNegative ? UINT8_MAX : 0) << (i * UINT8_WIDTH);
        }
        return forRet;
    }

    BigInt::operator std::string() const {
        std::string forRet;
        std::vector<char> answer;
        BigInt num(*this > ZERO ? *this : -(*this));

        if (num == ZERO) {
            forRet.push_back('0');
            return forRet;
        }

        while (num > ZERO) {
            answer.push_back(char((num % uchar(DECIMAL_SYSTEM_BASE)) + '0'));
            num /= DEC;
        }

        if (*this < ZERO) {
            answer.push_back('-');
        }

        for (size_t i = answer.size(); i > 0; i--) {
            forRet.push_back(answer[i - 1]);
        }

        return forRet;
    }

    // Size of BigInt with sign
    size_t BigInt::size() const {
        return numberArr.size() + sizeof(isNegative);
    }

    // Binary operators
    BigInt operator+(const BigInt &a, const BigInt &b) {
        BigInt forRet(a);
        forRet += b;
        return forRet;
    }

    BigInt operator-(const BigInt &a, const BigInt &b) {
        BigInt forRet(a);
        forRet -= b;
        return forRet;
    }

    BigInt operator*(const BigInt &a, const BigInt &b) {
        BigInt forRet(a);
        forRet *= b;
        return forRet;
    }

    BigInt operator/(const BigInt &a, const BigInt &b) {
        BigInt forRet(a);
        forRet /= b;
        return forRet;
    }

    BigInt operator^(const BigInt &a, const BigInt &b) {
        BigInt forRet(a);
        forRet ^= b;
        return forRet;
    }

    BigInt operator%(const BigInt &a, const BigInt &b) {
        BigInt forRet(a);
        forRet %= b;
        return forRet;
    }

    BigInt operator&(const BigInt &a, const BigInt &b) {
        BigInt forRet(a);
        forRet &= b;
        return forRet;
    }

    BigInt operator|(const BigInt &a, const BigInt &b) {
        BigInt forRet(a);
        forRet |= b;
        return forRet;
    }

    //

    // Stream operators
    std::ostream &operator<<(std::ostream &out, const BigInt &numberBI) {
        std::vector<char> answer;
        BigInt num(numberBI > ZERO ? numberBI : -numberBI);

        if (num == ZERO) {
            return out << 0;
        }

        while (num > ZERO) {
            answer.push_back(char((num % uchar(DECIMAL_SYSTEM_BASE)) + '0'));
            num /= DEC;
        }

        if (numberBI < ZERO) {
            answer.push_back('-');
        }

        for (size_t i = answer.size(); i > 0; i--) {
            out << answer[i - 1];
        }

        return out;
    }

    std::istream &operator>>(std::istream &in, BigInt &numberBI) {
        std::string s;
        in >> s;
        numberBI = BigInt(s);
        return in;
    }
}
