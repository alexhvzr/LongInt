//
// Created by Alex Hayden van Zuiden-Rylander on 2019-03-08.
//

#ifndef VERYLONGINTEGER_LONGINT_H
#define VERYLONGINTEGER_LONGINT_H

#include <iostream>
#include "deque.h"

using namespace std;

class LongInt {
    friend istream &operator>>( istream &in, LongInt &rhs );  //        while in.good (skip garbage) if not digits or (check for sign)negative, if digits start, negative = false else true, keep reading digits, add back, remove zeros
    friend ostream &operator<<( ostream &out, const LongInt &rhs ); //  check sign, if true, then -, then repeat this** cout << removeFront(); don't apply to original object
    // deque<char> temp = rhs.digits;
public:
    // Constructors
    LongInt( const string str );
    LongInt( const LongInt &rhs );
    LongInt( );

    // Destructor
    ~LongInt( );

    // Arithmetic binary operators
    LongInt operator+( const LongInt &rhs ) const;
    LongInt operator-( const LongInt &rhs ) const;

    // assignment operators
    const LongInt &operator=( const LongInt &rhs );

    // Logical binary operators
    bool operator< ( const LongInt & rhs ) const;
    bool operator<=( const LongInt & rhs ) const;
    bool operator> ( const LongInt & rhs ) const;
    bool operator>=( const LongInt & rhs ) const;
    bool operator==( const LongInt & rhs ) const;
    bool operator!=( const LongInt & rhs ) const;

private:
    Deque<char> digits;
    bool negative;
    void remove0s( );
};


#endif //VERYLONGINTEGER_LONGINT_H
