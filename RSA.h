//
// Created by yoda on 2020/11/12.
//

#ifndef RSA_CONSOLE_RSA_H
#define RSA_CONSOLE_RSA_H

#include "BigInt.h"

#define MAX_LEN 80

using namespace std;

class RSA {
private:
    BigInt mod_inverse(const BigInt &to_invert, const BigInt &mod) const;
    void ext_gcd(const BigInt& a, const BigInt& b, BigInt &gcd, BigInt &x, BigInt &y) const;
    static vector<string> split(const string& str, const string& delimiter);

public:
    BigInt public_key_n;
    BigInt public_key_e;
    BigInt private_key;

    void gen_key(ll len);
    BigInt encrypt_by_public(const BigInt &content) const;
    BigInt decrypt_by_private(const BigInt &content) const;
    string encrypt_by_public(const string &content) const;
    string decrypt_by_private(const string &content) const;
};


#endif //RSA_CONSOLE_RSA_H
