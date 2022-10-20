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

// BigInt is a part of namespace LongMath
namespace LongMath
{
    // In this realisation the basic type of BigInt is unsigned char,
    // which is easier to typedef for shorter name
    typedef unsigned char uchar;

    class BigInt {
    public:
        // Constructors

        // Default constructor makes uninitialized positive number with no meaning numbers
        // Not recommended for manipulate with
        BigInt();
        // Converts signed int number to BigInt by copying all numbers in binary from int to BigInt
        // Sign is setting by lead bit
        explicit BigInt(int);
        // Converts std::string, which consists number with sign in decimal based system
        // to BigInt using Horner's method
        // Throws std::invalid argument when got not a number in decimal based system
        explicit BigInt(std::string s);
        // Default copy constructor
        BigInt(const BigInt&);
        // Default move constructor
        BigInt(BigInt&&) noexcept;

        // Marker DEBUG is used for GTests
        // std::vector<uchar> constructor helps to set number by digits easily
#ifdef DEBUG
        explicit BigInt(std::vector<uchar> numberV) :
            isNegative(numberV[numberV.size() - 1] > INT8_MAX),
            numberArr (numberV) {}
#endif

        // Default destructor
        ~BigInt();

        //
        // Math operators
        //

        // Default copy assignment
        BigInt &operator=(const BigInt &);
        // Default move assignment
        BigInt &operator=(      BigInt &&) noexcept;

        // Inverts bits and sign in number
        BigInt operator~() const;

        // Prefix increment/decrement returns reference to object,
        // which was incremented/decremented
        BigInt &operator++();
        BigInt &operator--();

        // Postfix increment/decrement returns copy of object,
        // which will be modified and then increment/decrement original one
        const BigInt  operator++(int);
        const BigInt  operator--(int);

        // Operator+= aligns left number to right's number size if necessary
        // and then adds another radix to avoid unexpected sign changing or overflowing
        // Then makes default strikingly addition with writing result in left number
        BigInt &operator+=(const BigInt &);

        // Operator*= makes copy of arguments with absolute value (both copies are positive)
        // Then makes special BigInt answer, which is aligned to max size of arguments
        // Then makes default strikingly multiplication with adding radix to answer on every step
        // to avoid overflowing and unexpected sign changing
        // After multiplication is done it changes sign of answer if necessary and write it to left argument
        BigInt &operator*=(const BigInt &);

        // This operator makes copy of right argument with changing its sign
        // and then calls operator+=, but with inverted copy of argument
        BigInt &operator-=(const BigInt &);

        // Operator/= makes copy of arguments with absolute value (both copies are positive)
        // And then it uses binary search on BigInt for quick division
        // It uses fact that the result of division multiplied with divider can be in
        // interval (divisible - divider, divider]
        // And using this fact we move right and left border
        // After division is done it changes sign of answer if necessary and write it to left argument
        // Division by zero calls std::invalid_argument
        BigInt &operator/=(const BigInt &);

        // This 3 operators do same:
        // Align left number to right number
        // Do bitwise operation
        // Modify sign
        BigInt &operator^=(const BigInt &);
        BigInt &operator&=(const BigInt &);
        BigInt &operator|=(const BigInt &);

        // Operator%= consist of making copy of left argument
        // Then this copy divides by right argument (/=)
        // Then it multiplies by right argument     (*=)
        // The result will be difference between original number and modified
        BigInt &operator%=(const BigInt &);

        // Unary operator+ returns *this (does nothing with number)
        BigInt operator+() const;

        // Unary operator- makes copy of number with inverted bits (~)
        // and then adds 1 to copy
        // returns copy
        // Same method is used in default signed types in C++
        BigInt operator-() const; //unary

        // Default equal operator
        // Uses built in methods for comparing
        bool operator==(const BigInt &) const;

        // Calls !(.. == ..)
        bool operator!=(const BigInt &) const;

        // These 2 operators first check sign of arguments
        // If signs are different obviously
        // If signs are same they compare numbers from last radix to first
        bool operator< (const BigInt &) const;
        bool operator> (const BigInt &) const;

        // These operators call !(.. > ..) and !(.. < ..) accordingly
        bool operator<=(const BigInt &) const;
        bool operator>=(const BigInt &) const;

        // Turns first 4 bytes to int number
        explicit operator int() const;

        // Pushes back (remainder of division by 10 + '0') in std::vector<char>
        // Divide number by 10
        // Does it while number > 0
        // if number < 0 puts '-' to std::string
        // Then reverse std::vector and put it elements to std::string
        explicit operator std::string() const;

        // This method is used for GTest
        // Returns number in 256-based system
#ifdef DEBUG
        const std::vector<uchar>& getArray()
        {
            return numberArr;
        }
#endif

        // Returns size in bytes
        [[nodiscard]] size_t size() const;

        // Is used for GTest
#ifdef DEBUG
        [[nodiscard]] bool lessZero() const
        {
            return isNegative;
        }
#endif

    // Public Addons:
    // Do same as operator% for BigInts, but returns forRet[0]
    uchar operator%(const uchar&);

    private:
        bool isNegative = false;        // Sign = { 0 if number >= 0; 1 if < 0}
        std::vector <uchar> numberArr;  // Array of 1-byte elements,
                                        // every i element means i+1 radix in 256-based system

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

    // These binary operators works same:
    // Make copy of left operand
    // call operator+= for copy and right operand
    // returns copy
    BigInt operator+(const BigInt&, const BigInt&);
    BigInt operator-(const BigInt&, const BigInt&);
    BigInt operator*(const BigInt&, const BigInt&);
    BigInt operator/(const BigInt&, const BigInt&);
    BigInt operator^(const BigInt&, const BigInt&);
    BigInt operator%(const BigInt&, const BigInt&);
    BigInt operator&(const BigInt&, const BigInt&);
    BigInt operator|(const BigInt&, const BigInt&);

    // Ostream operator<< calls std::string(BigInt) and puts std::string to ostream
    std::ostream& operator<<(std::ostream&, const BigInt&);

    // Istream operator>> calls BigInt(std::string) and puts it to second argument
    std::istream& operator>>(std::istream&,       BigInt&);
}
