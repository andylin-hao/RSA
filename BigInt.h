//
// Created by yoda on 2020/11/10.
//

#ifndef RSA_CONSOLE_BIGINT_H
#define RSA_CONSOLE_BIGINT_H

#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <climits>
#include <cmath>
#include <string>
#include <tuple>
#include <sstream>
#include <random>
#include <stack>
#include <thread>
#include <future>
#include "Buffer.h"

typedef long long ll;
#define MAX_K 17
#define MAX_NTT 100010
#define BASE 100000000 // make sure that log10(BASE) is even
#define NUM_THREADS 5

using namespace std;

class BigInt {
private:
    int compare(const BigInt &right) const;
    static void divide_with_remainder(const BigInt& dividend, const BigInt& divisor, BigInt& quotient, BigInt& remainder);
    static void add_at(Buffer &buf, const ll &idx, const ll &datum);

    // prime table
    static const int prime_table[];

public:
    int sign = 1;
    Buffer nums;

    // constructors
    BigInt();
    explicit BigInt(const string &_val);
    explicit BigInt(const ll &_val);
    explicit BigInt(const Buffer &_val, int _sign = 1);
    BigInt(const BigInt &_val);
    BigInt(const Buffer &_val, const ll &_start, const ll &_len, int _sign = 1);

    // Unary arithmetic operators
    BigInt operator+()  const;
    BigInt operator-()  const;

    // Binary arithmetic operators
    BigInt operator+(const BigInt &right) const;
    BigInt operator-(const BigInt &right) const;
    BigInt operator*(const BigInt &right) const;
    BigInt operator/(const BigInt &right) const;
    BigInt operator%(const BigInt &right) const;

    BigInt operator+(const ll &right) const;
    BigInt operator-(const ll &right) const;
    BigInt operator*(const ll &right) const;
    BigInt operator/(const ll &right) const;
    BigInt operator%(const ll &right) const;

    BigInt operator+(const string &right) const;
    BigInt operator-(const string &right) const;
    BigInt operator*(const string &right) const;
    BigInt operator/(const string &right) const;
    BigInt operator%(const string &right) const;

    // Arithmetic-assignment operators:
    BigInt& operator+=(const BigInt &right);
    BigInt& operator-=(const BigInt &right);
    BigInt& operator*=(const BigInt &right);
    BigInt& operator/=(const BigInt &right);
    BigInt& operator%=(const BigInt &right);

    BigInt& operator+=(const long long &right);
    BigInt& operator-=(const long long &right);
    BigInt& operator*=(const long long &right);
    BigInt& operator/=(const long long &right);
    BigInt& operator%=(const long long &right);
    BigInt& operator>>=(const long long &right);

    BigInt& operator+=(const string &right);
    BigInt& operator-=(const string &right);
    BigInt& operator*=(const string &right);
    BigInt& operator/=(const string &right);
    BigInt& operator%=(const string &right);

    // Relational operators
    bool operator>(const BigInt &right) const;
    bool operator<(const BigInt &right) const;
    bool operator==(const BigInt &right) const;
    bool operator!=(const BigInt &right) const;
    bool operator>=(const BigInt &right) const;
    bool operator<=(const BigInt &right) const;

    bool operator>(const ll &right) const;
    bool operator<(const ll &right) const;
    bool operator>=(const ll &right) const;
    bool operator<=(const ll &right) const;
    bool operator==(const ll &right) const;
    bool operator!=(const ll &right) const;

    bool operator>(const string &right) const;
    bool operator<(const string &right) const;
    bool operator>=(const string &right) const;
    bool operator<=(const string &right) const;
    bool operator==(const string &right) const;
    bool operator!=(const string &right) const;

    // Assignment operators
    BigInt& operator=(const BigInt &right);
    BigInt& operator=(const ll &right);
    BigInt& operator=(const string &right);

    // Math
    BigInt abs()  const;
    static BigInt gen_odd(const unsigned &len);
    BigInt mod_pow(const BigInt& exp, const BigInt& mod) const;
    BigInt mod_pow(const vector<int>& exp_bits, const BigInt& mod) const;
    bool is_even() const;
    bool is_probable_prime(const unsigned &confidence) const;
    static BigInt gen_prime(const ll &len, const ll& confidence);
    static void gen_prime_thread(const ll &len, const ll& confidence, promise<BigInt> &promise);
    static void gen_prime_child(BigInt *random, BigInt* result, const ll &confidence, bool *found_prime);

    // Conversion
    string to_string() const;
    long long to_long_long() const;
    vector<int> to_bit_vec() const;

    // I/O
    friend ostream &operator<<(ostream &os, const BigInt &integer);
};

class NTT {
    static ll **alpha;
    static const ll modulo = 1325137921;

private:
    static void init_num(const Buffer &data, ll* num, int& num_len) ;
    void init_alpha();
    void convolution(ll *left, ll *right, int bit_len, ll *result) const;
    static void n_to_r(ll *_num, int num_bits) ;
    void r_to_n(ll *_num, int num_bits) const;
    void ext_gcd(ll a, ll b, ll &gcd, ll &x, ll &y) const;

public:
    NTT();
    BigInt mul(const BigInt& left, const BigInt& right);
};

#endif //RSA_CONSOLE_BIGINT_H
