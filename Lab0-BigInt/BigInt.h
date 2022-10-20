#ifndef iostream
#include <iostream>
#endif

#ifndef limits
#include <limits>
#endif

#ifndef string
#include <string>
#endif

#ifndef vector
#include <vector>
#endif

#define DEBUG


namespace LongMath
{
    typedef unsigned char uchar;

    class BigInt {
    public:

        // Constructors
        BigInt();
        explicit BigInt(int);
        explicit BigInt(std::string s);
        BigInt(const BigInt&);
        BigInt(BigInt&&) noexcept;

#ifdef DEBUG
        explicit BigInt(std::vector<uchar> numberV) :
            isNegative(numberV[numberV.size() - 1] > INT8_MAX),
            numberArr (numberV) {}
#endif

        // Destructor
        ~BigInt();

        // Math operators
        BigInt &operator=(const BigInt &);
        BigInt &operator=(      BigInt &&) noexcept;

        BigInt operator~() const;

        BigInt &operator++();
        BigInt &operator--();

        const BigInt  operator++(int);
        const BigInt  operator--(int);

        BigInt &operator+=(const BigInt &);
        BigInt &operator*=(const BigInt &);
        BigInt &operator-=(const BigInt &);
        BigInt &operator/=(const BigInt &);
        BigInt &operator^=(const BigInt &);
        BigInt &operator%=(const BigInt &);
        BigInt &operator&=(const BigInt &);
        BigInt &operator|=(const BigInt &);

        BigInt operator+() const; //unary
        BigInt operator-() const; //unary

        // Bool operators
        bool operator==(const BigInt &) const;
        bool operator!=(const BigInt &) const;
        bool operator< (const BigInt &) const;
        bool operator> (const BigInt &) const;
        bool operator<=(const BigInt &) const;
        bool operator>=(const BigInt &) const;

        // Convert operators
        explicit operator int() const;
        explicit operator std::string() const;
#ifdef DEBUG
        const std::vector<uchar>& getArray()
        {
            return numberArr;
        }
#endif

        // Size
        [[nodiscard]] size_t size() const; //size in bytes
#ifdef DEBUG
        [[nodiscard]] bool lessZero() const
        {
            return isNegative;
        }
#endif

    // Public Addons:
    // Operator for working with unsigned char elements
    uchar operator%(const uchar&);

    private:
        bool isNegative = false;        // Sign { 0 if number >= 0; 1 if < 0}
        std::vector <uchar> numberArr;  // Array of 1-byte elements,
                                        // every element means radix in 256-based system

        // Private Addons

#ifdef DEBUG
    public:
#endif
        // Byte shift, used in operator "/=" for quick division by 2
        BigInt &operator>>=(size_t);

        // Functions for manipulating void radixes
        void addRadix  ();
        void purgeRadix();
    };

    // Useful constants
    const BigInt ZERO(0);
    const BigInt ONE (1);
    const BigInt DEC (10);

    const size_t DECIMAL_SYSTEM_BASE = 10;

    // Binary operators
    BigInt operator+(const BigInt&, const BigInt&);
    BigInt operator-(const BigInt&, const BigInt&);
    BigInt operator*(const BigInt&, const BigInt&);
    BigInt operator/(const BigInt&, const BigInt&);
    BigInt operator^(const BigInt&, const BigInt&);
    BigInt operator%(const BigInt&, const BigInt&);
    BigInt operator&(const BigInt&, const BigInt&);
    BigInt operator|(const BigInt&, const BigInt&);

    // Streams functions
    std::ostream& operator<<(std::ostream&, const BigInt&);
    std::istream& operator>>(std::istream&,       BigInt&);
}
