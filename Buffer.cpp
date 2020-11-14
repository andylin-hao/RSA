//
// Created by yoda on 2020/11/10.
//

#include "Buffer.h"

ll Buffer::operator[](const ll &_idx) const {
    if (_idx >= len)
        throw std::overflow_error("index out of range");
    return data[_idx];
}

size_t Buffer::size() const {
    return len;
}

void Buffer::append(const ll &_datum) {
    if (len >= MAX_N)
        throw std::overflow_error("buffer overflow");
    data[len++] = _datum;
}

Buffer &Buffer::operator=(const Buffer &right) {
    if (&right != this) {
        memmove(data, right.data, right.len * sizeof(ll));
        len = right.len;
    }

    return *this;
}

Buffer::Buffer(size_t &_len) {
    len = _len;
}

void Buffer::insert(const ll &_idx, const ll &_datum) {
    data[_idx] = _datum;
    if (_idx >= len) len = _idx + 1;
}

ll *Buffer::get_ptr(const ll &_idx) {
    if (_idx >= len)
        throw std::range_error("access out of range");
    return data + _idx;
}

void Buffer::copy_from(const Buffer &_val, const ll &_start, const ll &_len) {
    memmove(this->data, _val.data + _start, _len * sizeof(*_val.data));
    len = _len;
}

ostream &operator<<(ostream &os, const Buffer &buf) {
    for (ll i = 0; i < buf.len; i++) {
        os << buf.data[i];
    }

    return os;
}

ll Buffer::get_or_zero(const ll &_idx) const {
    if (_idx >= len)
        return 0;
    return data[_idx];
}

void Buffer::reset_to(const ll &_len) {
    len = _len;
}

Buffer::Buffer() = default;
