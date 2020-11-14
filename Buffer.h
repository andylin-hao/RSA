//
// Created by yoda on 2020/11/10.
//

#ifndef RSA_CONSOLE_BUFFER_H
#define RSA_CONSOLE_BUFFER_H

#include <string>
#include <iostream>
#include <cstdlib>
#include <cstring>

#define MAX_N 1000
typedef long long ll;

using namespace std;

class Buffer {
private:
    ll data[MAX_N] = {0};
    ll len = 0;
public:

    // constructors
    Buffer();
    explicit Buffer(size_t &_len);

    // operators
    ll operator[](const ll &_idx) const;
    Buffer &operator=(const Buffer &right);
    friend ostream& operator<<(ostream &os, const Buffer &buf);

    // attributes
    size_t size() const;

    // operations
    void append(const ll &_datum);
    void insert(const ll &_idx, const ll &_datum);
    ll* get_ptr(const ll&_idx);
    void copy_from(const Buffer &_val, const ll &_start, const ll &_len);
    ll get_or_zero(const ll &_idx) const;
    void reset_to(const ll &_len);
};


#endif //RSA_CONSOLE_BUFFER_H
