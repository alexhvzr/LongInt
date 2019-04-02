//
// Created by Alex Hayden van Zuiden-Rylander on 2019-03-18.
//

#include "longint.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>

istream &operator>>(istream &in, LongInt &rhs) {
    string input;
    in >> input;
    LongInt val(input);
    rhs = val;
    return in;
};

ostream &operator<<(ostream &out, const LongInt &rhs) {
    LongInt temp = rhs;
    if (rhs.digits.isEmpty()) {
        out << '0';
        return out;
    }
    if(rhs.digits.getFront() == '0'){
        out << '0';
        return out;
    }

    if (rhs.negative)
        out << '-';

    while (!temp.digits.isEmpty())
        out << temp.digits.removeFront();

    return out;
};

inline bool not_digit(char ch) {
    return '0' <= ch && ch <= '9';
}

string &stripNonNumericVals(const string str) {
    string temp;
    copy_if(str.begin(), str.end(), back_inserter(temp), not_digit);
    return temp;
}


LongInt::LongInt(const string str) {
    if (str[0] == '-')       // Check if the integer passed was negative.
        negative = true;

    if (str.size() == 0)     // Check if the sting passed is empty and return zero if so.
        digits.addFront('0');
    else {
        string numeric = stripNonNumericVals(str);  // Fill the digits deque with the char values.
        for (int i = 0; i < numeric.size(); i++) {
            digits.addBack(numeric[i]);
        }
        remove0s();
    }
}

LongInt::LongInt(const LongInt &rhs) {
    digits = rhs.digits;
    negative = rhs.negative;
}

LongInt::LongInt() {
    negative = false;
}

LongInt::~LongInt() {
    digits.clear();
}


LongInt LongInt::operator+(const LongInt &rhs) const {
    LongInt sum;

    // A + - B = A - B
    if (!negative && rhs.negative) {
        LongInt lhs, tempRhs;

        tempRhs.digits = rhs.digits;
        lhs.digits = digits;

        lhs.negative = tempRhs.negative = false;

        sum = lhs - tempRhs;
        return sum;
    }

        // -A + B = B - A
    else if (negative && !rhs.negative) {
        LongInt lhs, tempRhs;

        tempRhs.digits = rhs.digits;
        lhs.digits = digits;

        lhs.negative = tempRhs.negative = false;

        sum = tempRhs - lhs;
        return sum;
    }
        // -A + -B = -(A + B)
    else if (negative && rhs.negative) {
        LongInt lhs, tempRhs;

        tempRhs.digits = rhs.digits;
        lhs.digits = digits;

        lhs.negative = tempRhs.negative = false;

        sum = lhs + tempRhs;
        sum.negative = true;
        return sum;
    }
    // A + B
    Deque<char> leftVals = digits;
    Deque<char> rightVals = rhs.digits;

    // Make the numbers equal in length.
    for (int i = rightVals.size(); i < leftVals.size(); i++)
        rightVals.addFront('0');

    for (int j = leftVals.size(); j < rightVals.size(); j++)
        leftVals.addFront('0');

    Deque<char> itr = leftVals;

    int carry = 0;
    string finalSum;

    for (int k = 0; k < itr.size(); k++) {
        int left = leftVals.removeBack() - '0';
        int right = rightVals.removeBack() - '0';

        int summation = left + right + carry;
        carry = summation / 10;
        summation %= 10;

        finalSum = string(1, (char) (((int) '0') + summation)) + finalSum;
    }

    if (carry > 0)
        finalSum = "1" + finalSum;

    Deque<char> dequeSumAns;
    for (int l = 0; l < finalSum.length(); l++)
        dequeSumAns.addBack(finalSum[l]);

    sum.digits = dequeSumAns;
    sum.negative = false;

    return sum;

}

LongInt LongInt::operator-(const LongInt &rhs) const {
    LongInt sum;
    Deque<char> left;
    Deque<char> right;
    // A - (-B) = A + B
    if (!negative && rhs.negative) {
        LongInt lhs, tempRhs;

        lhs.digits = digits;
        tempRhs.digits = rhs.digits;

        lhs.negative = tempRhs.negative = false;

        sum = lhs + rhs;
        return sum;
    }
        // -A - B = - (A + B)
    else if (negative && !rhs.negative) {
        LongInt lhs, tempRhs;

        lhs.digits = digits;
        tempRhs.digits = rhs.digits;

        lhs.negative = tempRhs.negative = false;

        sum = lhs + rhs;
        sum.negative = true;

        return sum;
    }
        // -A - (-B) = B - A
    else if (negative && rhs.negative) {
        LongInt lhs, tempRhs;

        lhs.digits = digits;
        tempRhs.digits = rhs.digits;

        lhs.negative = tempRhs.negative = false;

        if (lhs > rhs) {
            sum = lhs - tempRhs;
            sum.negative = true;
            return sum;
        } else if (lhs < rhs) {
            sum = tempRhs - lhs;
            sum.negative = false;
            return sum;
        } else {
            // The return value is zero.
            Deque<char> zero;
            zero.addBack('0');
            sum.digits = zero;
            sum.negative = false;
            return sum;
        }
    }
        // A - B
    else if (!negative && !rhs.negative) {
        LongInt lhs, tempRhs;

        lhs.digits = digits;
        tempRhs.digits = rhs.digits;

        lhs.negative = tempRhs.negative = false;

        if (lhs > rhs) {
            left = lhs.digits;
            right = tempRhs.digits;
            sum.negative = false;
        } else if (lhs < rhs) {
            left = tempRhs.digits;
            right = lhs.digits;
            sum.negative = true;
        } else {
            // The return value is zero.
            Deque<char> zero;
            zero.addBack('0');
            sum.digits = zero;
            sum.negative = false;
            return sum;
        }

    }
    string finalSumString;
    Deque<char> itr = left;
    // Make the numbers equal in length.
    for (int i = right.size(); i < left.size(); i++)
        right.addFront('0');

    for (int j = left.size(); j < right.size(); j++)
        left.addFront('0');

    int borrow = 0;
    for (int i = 0; i < itr.size(); i++) {
        int leftInt = left.removeBack() - '0';
        int rightInt = right.removeBack() - '0';

        int sumInt = leftInt - rightInt - borrow;
        borrow = 0;

        if (sumInt < 0) {
            sumInt += 10;
            borrow = 1;
        }
        finalSumString = string(1, (char) (((int) '0') + sumInt)) + finalSumString;
    }

    Deque<char> dequeSumAns;
    for (int j = 0; j < finalSumString.length(); j++)
        dequeSumAns.addBack(finalSumString[j]);

    sum.digits = dequeSumAns;
    sum.remove0s();
    return sum;
}

const LongInt &LongInt::operator=(const LongInt &rhs) {
    digits = rhs.digits;
    negative = rhs.negative;
    return *this;
}

bool LongInt::operator<(const LongInt &rhs) const {
    if (negative && !rhs.negative)
        return true;

    if (negative && rhs.negative && this->digits.size() > rhs.digits.size())
        return true;

    if (digits.size() < rhs.digits.size())
        return true;

    if (this->digits.size() == rhs.digits.size()) {
        Deque<char> left = digits;
        Deque<char> right = rhs.digits;
        int lhsInt = left.removeFront() - '0';
        int rhsInt = right.removeFront() - '0';

        if (left.isEmpty()) {
            return (lhsInt < rhsInt);
        } else {
            while (lhsInt == rhsInt) {
                lhsInt = left.removeFront();
                rhsInt = right.removeFront();
            }
            return (lhsInt < rhsInt);
        }
    }
}

bool LongInt::operator<=(const LongInt &rhs) const {
    return (*this < rhs || *this == rhs);
}

bool LongInt::operator>(const LongInt &rhs) const {
    return !(*this < rhs);
}

bool LongInt::operator>=(const LongInt &rhs) const {
    return (!(*this < rhs) || *this == rhs);
}

bool LongInt::operator==(const LongInt &rhs) const {
    if (negative && !rhs.negative | !negative && rhs.negative)
        return false;
    if (digits.size() != rhs.digits.size())
        return false;

    Deque<char> left = digits;
    Deque<char> right = rhs.digits;
    int lhsInt = left.removeFront() - '0';
    int rhsInt = right.removeFront() - '0';

    while (lhsInt == rhsInt) {
        lhsInt = left.removeFront() - '0';
        rhsInt = right.removeFront() - '0';
        if (lhsInt != rhsInt)
            return false;
    }
    return true;
}

bool LongInt::operator!=(const LongInt &rhs) const {
    return !(*this == rhs);
}

void LongInt::remove0s() {
    if (digits.size() == 1)
        return;
    int count = digits.size();
    while (digits.getFront() == '0' && count > 1) {
        digits.removeFront();
        count--;
    }
}

