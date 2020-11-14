//
// Created by yoda on 2020/11/10.
//

#include "BigInt.h"

const int BigInt::prime_table[] = {
        3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41,
        43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 113,
        193, 241, 257, 337, 353, 401, 433, 449, 577, 593, 641,
        673, 769, 881, 929, 977, 1009, 1153, 1201, 1217, 1249,
        1297, 1361, 1409, 1489, 1553, 1601, 1697, 1777, 1873,
        1889, 2017, 2081, 2113, 2129, 2161, 2273, 2417, 2593,
        2609, 2657, 2689, 2753, 2801, 2833, 2897, 3041, 3089,
        3121, 3137, 3169, 3217, 3313, 3329, 3361, 3457, 3617,
        3697, 3761, 3793, 3889, 4001, 4049, 4129, 4177, 4241,
        4273, 4289, 4337, 4481, 4513, 4561, 4657, 4673, 4721,
        4801, 4817, 4993, 5009, 5153, 5233, 5281, 5297, 5393,
        5441, 5521, 5569, 5857, 5953, 6113, 6257, 6337, 6353,
        6449, 6481, 6529, 6577, 6673, 6689, 6737, 6833, 6961,
        6977, 7057, 7121, 7297, 7393, 7457, 7489, 7537, 7649,
        7681, 7793, 7841, 7873, 7937, 8017, 8081, 8161, 8209,
        8273, 8353, 8369, 8513, 8609, 8641, 8689, 8737, 8753,
        8849, 8929, 9041, 9137, 9281, 9377, 9473, 9521, 9601,
        9649, 9697, 9857
};


BigInt::BigInt() {
    nums.append(0);
}

BigInt::BigInt(const string &_val) {
    int shift = log10(BASE);
    size_t size = _val.size();
    size_t str_size = size;
    if (size == 0)
        return;

    ll i = 0;
    if (_val[0] == '-') {
        sign = -1;
        i++;
        size--;
    }
    size = ceil((double) size / shift);
    // the length of the highest digit
    int first_len = shift - (int) (size * shift - _val.size() + i);
    nums.append(stoll(_val.substr(i, first_len)));
    i += first_len;

    for (; i < str_size; i += shift) {
        ll num = stoll(_val.substr(i, shift));
        nums.append(num);
    }
}

BigInt::BigInt(const ll &_val) {
    ostringstream ss;
    ss << _val;
    new(this) BigInt(ss.str());
    if (_val < 0)
        sign = -1;
}

BigInt::BigInt(const Buffer &_val, int _sign) {
    nums = _val;
    sign = _sign;
}

BigInt::BigInt(const BigInt &_val) {
    nums = _val.nums;
    sign = _val.sign;
}


BigInt::BigInt(const Buffer &_val, const ll &_start, const ll &_len, int _sign) {
    nums.copy_from(_val, _start, _len);
    sign = _sign;
}

BigInt BigInt::operator+() const {
    BigInt res(nums);
    res.sign = 1;
    return res;
}

BigInt BigInt::operator-() const {
    BigInt res(nums);
    res.sign = -sign;
    return res;
}

BigInt BigInt::operator+(const BigInt &right) const {
    // if the operation is actually -
    if (sign == -1 && right.sign == 1) {
        BigInt tmp(*this);
        tmp.sign = 1;
        return right - tmp;
    } else if (sign == 1 && right.sign == -1) {
        BigInt tmp(right);
        tmp.sign = 1;
        return *this - tmp;
    }

    // swap if the right operator number is longer than the left
    const BigInt *left_p = this;
    const BigInt *right_p = &right;
    if (nums.size() < right.nums.size()) {
        const BigInt *tmp = left_p;
        left_p = right_p;
        right_p = tmp;
    }

    size_t res_len = left_p->nums.size() + 1;
    size_t left_len = left_p->nums.size();
    size_t right_len = right_p->nums.size();
    Buffer res(res_len);
    ll i = 1;
    for (; i <= right_len; i++) {
        ll added = res[res_len - i] + left_p->nums[left_len - i] + right_p->nums[right_len - i];
        res.insert(res_len - i - 1, res[res_len - i - 1] + added / BASE);
        res.insert(res_len - i, added % BASE);
    }
    for (; i <= left_len; i++) {
        ll added = res[res_len - i] + left_p->nums[left_len - i];
        res.insert(res_len - i - 1, res[res_len - i - 1] + added / BASE);
        res.insert(res_len - i, added % BASE);
    }

    ll id = 0;
    while (res[id] == 0) id++;
    BigInt result(res, id, res.size() - id);
    if (id == res.size())
        result = 0;
    if (sign == -1 && right.sign == -1)
        result.sign = -1;
    return result;
}

BigInt BigInt::operator-(const BigInt &right) const {
    // if the operation is actually +
    if (sign == -1) {
        BigInt tmp(*this);
        tmp.sign = 1;
        BigInt result = tmp + right;
        result.sign = -result.sign;
        return result;
    } else if (right.sign == -1) {
        BigInt tmp(right);
        tmp.sign = 1;
        return tmp + *this;
    }

    // swap if the right operator number is larger than the left
    const BigInt *left_p = this;
    const BigInt *right_p = &right;
    int res_sign = 1;
    if (*this < right) {
        const BigInt *tmp = left_p;
        left_p = right_p;
        right_p = tmp;
        res_sign = -1;
    }

    size_t res_len = left_p->nums.size() + 1;
    size_t left_len = left_p->nums.size();
    size_t right_len = right_p->nums.size();
    Buffer res(res_len);
    ll i = 1;
    for (; i <= right_len; i++) {
        ll minus = left_p->nums[left_len - i] - res[res_len - i] - right_p->nums[right_len - i];
        if (minus < 0) {
            res.insert(res_len - i - 1, 1);
            res.insert(res_len - i, (minus + BASE) % BASE);
        } else {
            res.insert(res_len - i - 1, 0);
            res.insert(res_len - i, minus);
        }
    }
    for (; i <= left_len; i++) {
        ll minus = left_p->nums[left_len - i] - res[res_len - i];
        if (minus < 0) {
            res.insert(res_len - i - 1, 1);
            res.insert(res_len - i, (minus + BASE) % BASE);
        } else {
            res.insert(res_len - i - 1, 0);
            res.insert(res_len - i, minus);
        }
    }

    int id = 0;
    while (res[id] == 0) id++;
    BigInt result(res, id, res.size() - id);
    if (id == res.size())
        result = 0;
    result.sign = res_sign;
    return result;
}

BigInt BigInt::operator*(const BigInt &right) const {
    NTT ntt;
    return ntt.mul(*this, right);
}

BigInt BigInt::operator/(const BigInt &right) const {
    BigInt quotient, remainder;
    divide_with_remainder(*this, right, quotient, remainder);
    return quotient;
}


BigInt BigInt::operator%(const BigInt &right) const {
    BigInt quotient, remainder;
    divide_with_remainder(*this, right, quotient, remainder);
    return remainder;
}


BigInt BigInt::operator+(const ll &right) const {
    BigInt _right(right);
    return *this + _right;
}

BigInt BigInt::operator-(const ll &right) const {
    BigInt _right(right);
    return *this - _right;
}

BigInt BigInt::operator*(const ll &right) const {
    BigInt _right(right);
    return *this * _right;
}

BigInt BigInt::operator/(const ll &right) const {
    BigInt _right(right);
    return *this / _right;
}

BigInt BigInt::operator%(const ll &right) const {
    BigInt _right(right);
    return *this % _right;
}

BigInt BigInt::operator+(const string &right) const {
    BigInt _right(right);
    return *this + _right;
}

BigInt BigInt::operator-(const string &right) const {
    BigInt _right(right);
    return *this - _right;
}

BigInt BigInt::operator*(const string &right) const {
    BigInt _right(right);
    return *this * _right;
}

BigInt BigInt::operator/(const string &right) const {
    BigInt _right(right);
    return *this / _right;
}

BigInt BigInt::operator%(const string &right) const {
    BigInt _right(right);
    return *this % _right;
}


BigInt &BigInt::operator+=(const BigInt &right) {
    *this = *this + right;
    return *this;
}

BigInt &BigInt::operator-=(const BigInt &right) {
    *this = *this - right;
    return *this;
}

BigInt &BigInt::operator*=(const BigInt &right) {
    *this = *this * right;
    return *this;
}

BigInt &BigInt::operator/=(const BigInt &right) {
    *this = *this / right;
    return *this;
}

BigInt &BigInt::operator%=(const BigInt &right) {
    *this = *this % right;
    return *this;
}

BigInt &BigInt::operator+=(const long long int &right) {
    *this = *this + BigInt(right);
    return *this;
}

BigInt &BigInt::operator-=(const long long int &right) {
    *this = *this - BigInt(right);
    return *this;
}

BigInt &BigInt::operator*=(const long long int &right) {
    *this = *this * BigInt(right);
    return *this;
}

BigInt &BigInt::operator/=(const long long int &right) {
    *this = *this / BigInt(right);
    return *this;
}

BigInt &BigInt::operator%=(const long long int &right) {
    *this = *this % BigInt(right);
    return *this;
}

BigInt &BigInt::operator+=(const string &right) {
    *this = *this + BigInt(right);
    return *this;
}

BigInt &BigInt::operator-=(const string &right) {
    *this = *this - BigInt(right);
    return *this;
}

BigInt &BigInt::operator*=(const string &right) {
    *this = *this * BigInt(right);
    return *this;
}

BigInt &BigInt::operator/=(const string &right) {
    *this = *this / BigInt(right);
    return *this;
}

BigInt &BigInt::operator>>=(const long long int &right) {
    ll size = nums.size();
    for (ll i = 0; i < right; i++) {
        ll remainder = 0;
        for (ll j = 0; j < size; j++) {
            nums.insert(j, remainder * BASE + nums[j]);
            remainder = nums[j] % 2;
            nums.insert(j, nums[j] >> 1);
        }
    }
    ll i = 0;
    while (i < size && nums[i] == 0) i++;
    if (i) {
        if (i == size)
            nums.reset_to(1);
        else
            nums.copy_from(nums, i, size - i);
    }
    return *this;
}

BigInt &BigInt::operator%=(const string &right) {
    *this = *this % BigInt(right);
    return *this;
}

int BigInt::compare(const BigInt &right) const {
    if (sign > right.sign)
        return 1;
    else if (sign < right.sign)
        return -1;

    int result = 0;
    if (nums.size() > right.nums.size())
        result = 1;
    else if (nums.size() < right.nums.size())
        result = -1;
    else if (nums.size() == right.nums.size()) {
        for (ll i = 0; i < nums.size(); i++) {
            if (nums[i] > right.nums[i]) {
                result = 1;
                break;
            } else if (nums[i] < right.nums[i]) {
                result = -1;
                break;
            }
        }
    }

    if (sign == -1 && right.sign == -1)
        result = -result;
    return result;
}

void BigInt::divide_with_remainder(const BigInt &dividend, const BigInt &divisor, BigInt &quotient, BigInt &remainder) {
    BigInt abs_dividend = dividend.abs();
    BigInt abs_divisor = divisor.abs();

    if (divisor == 0)
        throw logic_error("Attempted division by zero");
    if (dividend == 0) {
        quotient = 0;
        remainder = 0;
        return;
    }
    if (divisor == 1) {
        quotient = dividend;
        remainder = 0;
        return;
    }
    if (divisor == -1) {
        quotient = -dividend;
        remainder = 0;
        return;
    }
    if (abs_dividend < abs_divisor) {
        quotient = 0;
        remainder = dividend;
        return;
    }
    if (abs_dividend == abs_divisor) {
        quotient = BigInt((ll) dividend.sign == divisor.sign ? 1 : -1);
        remainder = 0;
        return;
    }

    if (abs_dividend <= LLONG_MAX and abs_divisor <= LLONG_MAX) {
        ll abs_dividend_ll = abs_dividend.to_long_long();
        ll abs_divisor_ll = abs_divisor.to_long_long();
        quotient = abs_dividend_ll / abs_divisor_ll;
        remainder = abs_dividend_ll % abs_divisor_ll;
    } else {
        quotient = 0;
        ll dividend_pos = 0;
        ll dividend_size = dividend.nums.size(), divisor_size = divisor.nums.size();
        Buffer remainder_buf;
        remainder_buf.copy_from(dividend.nums, 0, dividend.nums.size());
        while (dividend_pos + divisor_size <= dividend_size) {
            if (remainder_buf[dividend_pos] > divisor.nums[0]) {
                ll estimate_quotient = remainder_buf[dividend_pos] / (divisor.nums[0] + 1);
                for (ll i = divisor_size - 1; i >= 0; i--) {
                    ll remainder_pos = i + dividend_pos;
                    remainder_buf.insert(remainder_pos,
                                         remainder_buf[remainder_pos] - estimate_quotient * divisor.nums[i]);
                    if (remainder_buf[remainder_pos] < 0) {
                        ll borrow = (-remainder_buf[remainder_pos]) / BASE + 1;
                        remainder_buf.insert(remainder_pos, remainder_buf[remainder_pos] + borrow * BASE);
                        remainder_buf.insert(remainder_pos - 1, remainder_buf[remainder_pos - 1] - borrow);
                    }
                }
                add_at(quotient.nums, dividend_pos, estimate_quotient);
//                quotient.nums.insert(dividend_pos, quotient.nums.get_or_zero(dividend_pos) + estimate_quotient);
                while (remainder_buf[dividend_pos] == 0)
                    dividend_pos++;
            } else {
                if (dividend_size - dividend_pos == divisor_size) {
                    int is_remainder_larger = 0;
                    for (ll i = dividend_pos; i < dividend_size; i++) {
                        if (remainder_buf[i] > divisor.nums[i - dividend_pos]) {
                            is_remainder_larger = 1;
                            break;
                        } else if (remainder_buf[i] < divisor.nums[i - dividend_pos]) {
                            is_remainder_larger = -1;
                            break;
                        }
                    }

                    if (is_remainder_larger >= 0) {
                        ll buf_size = remainder_buf.size();
                        for (ll j = dividend_pos; j < buf_size; j++) {
                            remainder_buf.insert(j, remainder_buf[j] - divisor.nums[j - dividend_pos]);
                            if (remainder_buf[j] < 0) {
                                ll borrow = (-remainder_buf[j]) / BASE + 1;
                                remainder_buf.insert(j, remainder_buf[j] + borrow * BASE);
                                remainder_buf.insert(j - 1, remainder_buf[j - 1] - borrow);
                            }
                        }
                        add_at(quotient.nums, dividend_pos, 1);
                    }
                    break;
                }

                remainder_buf.insert(dividend_pos + 1,
                                     remainder_buf[dividend_pos] * BASE + remainder_buf[dividend_pos + 1]);
                remainder_buf.insert(dividend_pos, 0);
                dividend_pos++;
            }
        }

        ll j = 0, buf_size = remainder_buf.size();
        while (j < buf_size && remainder_buf[j] == 0) j++;
        if (j == buf_size)
            remainder = 0;
        else
            remainder.nums.copy_from(remainder_buf, j, buf_size - j);
    }

    ll i = 0;
    while (quotient.nums[i] == 0) i++;
    quotient.nums.copy_from(quotient.nums, i, quotient.nums.size() - i);
    quotient.sign = dividend.sign == divisor.sign ? 1 : -1;
    remainder.sign = divisor.sign;
    if (remainder == 0)
        remainder.sign = 1;
}

void BigInt::add_at(Buffer &buf, const ll &idx, const ll &datum) {
    buf.insert(idx, buf.get_or_zero(idx) + datum);
    for (ll i = idx; i >= 0; i--) {
        if (buf[i] >= BASE) {
            buf.insert(i - 1, buf.get_or_zero(i - 1) + buf[i] / BASE);
            buf.insert(i, buf[i] % BASE);
        } else break;
    }
}

bool BigInt::operator>(const BigInt &right) const {
    return compare(right) == 1;
}

bool BigInt::operator<(const BigInt &right) const {
    return compare(right) == -1;
}

bool BigInt::operator==(const BigInt &right) const {
    return compare(right) == 0;
}


bool BigInt::operator!=(const BigInt &right) const {
    return !(*this == right);
}

bool BigInt::operator>=(const BigInt &right) const {
    return !(*this < right);
}

bool BigInt::operator<=(const BigInt &right) const {
    return !(*this > right);
}

bool BigInt::operator>(const ll &right) const {
    BigInt _right(right);
    return *this > _right;
}

bool BigInt::operator<(const ll &right) const {
    BigInt _right(right);
    return *this < _right;
}

bool BigInt::operator>=(const ll &right) const {
    BigInt _right(right);
    return *this >= _right;
}

bool BigInt::operator<=(const ll &right) const {
    BigInt _right(right);
    return *this <= _right;
}

bool BigInt::operator==(const ll &right) const {
    BigInt _right(right);
    return *this == _right;
}

bool BigInt::operator!=(const ll &right) const {
    BigInt _right(right);
    return *this != _right;
}

bool BigInt::operator>(const string &right) const {
    BigInt _right(right);
    return *this > _right;
}

bool BigInt::operator<(const string &right) const {
    BigInt _right(right);
    return *this < _right;
}

bool BigInt::operator>=(const string &right) const {
    BigInt _right(right);
    return *this >= _right;
}

bool BigInt::operator<=(const string &right) const {
    BigInt _right(right);
    return *this <= _right;
}

bool BigInt::operator==(const string &right) const {
    BigInt _right(right);
    return *this == _right;
}

bool BigInt::operator!=(const string &right) const {
    BigInt _right(right);
    return *this != _right;
}

BigInt &BigInt::operator=(const BigInt &right) {
    if (&right != this) {
        nums = right.nums;
        sign = right.sign;
    }
    return *this;
}

BigInt &BigInt::operator=(const ll &right) {
    BigInt tmp(right);
    nums = tmp.nums;
    sign = tmp.sign;
    return *this;
}

BigInt &BigInt::operator=(const string &right) {
    BigInt tmp(right);
    nums = tmp.nums;
    sign = tmp.sign;
    return *this;
}

BigInt BigInt::abs() const {
    BigInt result = *this;
    result.sign = 1;
    return result;
}

BigInt BigInt::gen_odd(const unsigned int &len) {
    const char dec_table[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    // remove 5 as number ends with 5 is definitely not a prime
    const char odd_table[] = {'1', '3', '7', '9'};
    random_device rd;
    mt19937 mt(rd());
    uniform_int_distribution<int> dist_dec(0, 9);
    uniform_int_distribution<int> dist_dec_no_zero(1, 9);
    uniform_int_distribution<int> dist_odd(0, 3);

    if (len) {
        stringstream ss;
        ss << dec_table[dist_dec_no_zero(mt)];
        for (unsigned i = 1; i < len - 1; i++) {
            ss << dec_table[dist_dec(mt)];
        }
        // the last one is an odd
        ss << odd_table[dist_odd(mt)];
        return BigInt(ss.str());
    }

    return BigInt(dist_dec(mt));
}

BigInt BigInt::mod_pow(const BigInt &exp, const BigInt &mod) const {
    BigInt tmp = *this;
    BigInt _exp = exp;
    BigInt res(1);

    while (_exp > 0) {
        if (!_exp.is_even())
            res = (res * tmp) % mod;
        _exp >>= 1;
        tmp = (tmp * tmp) % mod;
    }
    return res;
}

BigInt BigInt::mod_pow(const vector<int> &exp_bits, const BigInt &mod) const {
    BigInt tmp = *this;
    BigInt res(1);
    BigInt quotient;

    ll i = exp_bits.size() - 1;
    while (i >= 0) {
        if (exp_bits[i] == 1) {
            divide_with_remainder((res * tmp), mod, quotient, res);
        }
        i--;
        divide_with_remainder((tmp * tmp), mod, quotient, tmp);
    }
    return res;
}

bool BigInt::is_even() const {
    return !(nums[nums.size() - 1] % 2);
}

bool BigInt::is_probable_prime(const unsigned int &confidence) const {
    if (*this == 0 || *this == 1)
        return false;
    if (*this == 2)
        return true;

    // check if I am even
    if (this->is_even())
        return false;

    ll prime_table_size = sizeof(prime_table) / sizeof(prime_table[0]);
    for (ll i = 0; i < prime_table_size; i++) {
        if (*this % prime_table[i] == 0)
            return false;
    }

    BigInt n_minus_one = *this - 1;
    // n - 1 = 2^s * d
    unsigned s = 0;
    BigInt d = n_minus_one;
    while (true) {
        d >>= 1;
        s++;
        if (!d.is_even()) break;
    }

    random_device rd;
    mt19937 mt(rd());
    std::uniform_int_distribution<ll> dist(0, LONG_LONG_MAX - 1);
    vector<int> d_bits = d.to_bit_vec();
    for (unsigned i = 0; i < confidence; ++i) {
        // generate a random number such that 1 <= random_less_than <= n - 1
        unsigned long long ran = dist(mt);
        BigInt random(ran);
        BigInt random_less_than = random % *this;
        if (random_less_than == 0)
            random_less_than = *this - 1;

        BigInt examine = random_less_than.mod_pow(d_bits, *this);

        if (examine == 1) // probably is prime
            continue;

        bool is_prime = false;
        // check for 0 <= j < s,a^(2^j * d) mod n != -1
        for (size_t j = 0; j < s && !is_prime; j++) {
            if (examine == n_minus_one)
                is_prime = true;
            examine = (examine * examine) % *this;
        }
        // not prime
        if (!is_prime) {
            return false;
        }
    }
    // probably is prime
    return true;
}

BigInt BigInt::gen_prime(const ll &len, const ll &confidence) {
    BigInt random = gen_odd(len);
    BigInt randoms[NUM_THREADS], results[NUM_THREADS];
    bool found_prime = false;
    for (int i = 0; i < NUM_THREADS; i++) {
        randoms[i] = random + 500 * i;
        results[i] = 0;
    }
    thread threads[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++) {
        threads[i] = thread(&BigInt::gen_prime_child, &randoms[i], &results[i], confidence, &found_prime);
    }
    int count = 0;
    for (auto &thread : threads) {
        thread.join();
        count++;
        if (count == NUM_THREADS)
            break;
    }
    for (const auto &result : results) {
        if (result != 0)
            return result;
    }
    return BigInt();
}

void BigInt::gen_prime_thread(const ll &len, const ll& confidence, promise<BigInt> &promise) {
    BigInt random = gen_odd(len);
    bool found_prime = false;
    BigInt randoms[NUM_THREADS], results[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++) {
        randoms[i] = random + 500 * i;
        results[i] = 0;
    }
    thread threads[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++) {
        threads[i] = thread(&BigInt::gen_prime_child, &randoms[i], &results[i], confidence, &found_prime);
    }
    int count = 0;
    for (auto &thread : threads) {
        thread.join();
        count++;
        if (count == NUM_THREADS)
            break;
    }
    for (const auto &result : results) {
        if (result != 0) {
            promise.set_value(result);
            break;
        }
    }
}

void BigInt::gen_prime_child(BigInt *random, BigInt *result, const ll &confidence, bool *found_prime) {
    while (!random->is_probable_prime(confidence)) {
        if (*found_prime)
            return;
        *random += 4;
    }
    *found_prime = true;
    *result = *random;
}


string BigInt::to_string() const {
    string res;
    ll shift = log10(BASE);
    if (sign == -1)
        res += "-";
    ll size = nums.size();
    for (ll i = 0; i < size; i++) {
        ostringstream ss;
        ss << nums[i];
        string cur = ss.str();
        if (i != 0 && cur.size() != shift) {
            string fill;
            fill.append(shift - cur.size(), '0');
            res += fill;
            res += cur;
        } else res += cur;
    }
    return res;
}

long long BigInt::to_long_long() const {
    ll res = 0;
    ll size = nums.size();
    for (ll i = 0; i < size; i++) {
        ll num = nums[i];
        for (ll j = 0; j < (size - i - 1); j++) {
            num *= BASE;
        }
        res += num;
    }
    return res;
}

vector<int> BigInt::to_bit_vec() const {
    BigInt n_10 = *this;
    vector<int> bits;
    while (true) {
        bits.push_back(n_10.is_even() ? 0 : 1);
        n_10 >>= 1;
        if (n_10 < 2) {
            if (n_10 == 1)
                bits.push_back(1);
            break;
        }
    }
    vector<int> res(bits.rbegin(), bits.rend());
    return res;
}

ostream &operator<<(ostream &os, const BigInt &integer) {
    os << integer.to_string();

    return os;
}


ll **NTT::alpha = nullptr;

void NTT::init_num(const Buffer &data, ll *num, int &num_len) {
    // transform BASE-radix digits into 100-radix digits
    num_len = 0;
    ll shift = log10(BASE) / 2;
    ll data_size = data.size();
    for (ll i = data_size - 1; i >= 0; i--) {
        ll cur_datum = data[i];
        if (i != 0) {
            for (ll j = 0; j < shift; j++) {
                num[num_len] = cur_datum % 100;
                cur_datum /= 100;
                num_len++;
            }
        } else {
            while (cur_datum){
                num[num_len] = cur_datum % 100;
                cur_datum /= 100;
                num_len++;
            }
        }
    }
}

void NTT::init_alpha() {
    if (alpha != nullptr) return;

    alpha = new ll *[2];
    alpha[0] = new ll[MAX_K + 1]();
    alpha[1] = new ll[MAX_K + 1]();

    ll gcd, y;
    ext_gcd(alpha[0][MAX_K] = 1101238606, modulo, gcd, alpha[1][MAX_K], y);

    for (int i = 16; i >= 0; --i) {
        alpha[0][i] = alpha[0][i + 1] * alpha[0][i + 1] % modulo;
        alpha[1][i] = alpha[1][i + 1] * alpha[1][i + 1] % modulo;
    }
}

void NTT::convolution(ll *left, ll *right, int bit_len, ll *result) const {
    int &k = bit_len;
    int len = 1 << k;

    n_to_r(left, k);
    n_to_r(right, k);

    for (int i = 0; i < len; ++i)
        result[i] = left[i] * right[i] % modulo;

    r_to_n(result, k);
}

void NTT::n_to_r(ll *_num, int num_bits) {
    int nLen = 1 << num_bits;

    // butterfly operation
    for (int layer = num_bits; layer > 0; --layer) {
        int group = 1 << layer, brother = group >> 1;
        ll kernel = alpha[0][layer];

        for (int k = 0; k < nLen; k += group) {

            ll w = 1;

            // find the brother to be butterflied with the current element
            for (int j = 0; j < brother; ++j) {

                int cur = k + j, next = cur + brother;
                ll u = _num[cur], v = _num[next];

                _num[cur] = u + v;
                _num[cur] = _num[cur] < modulo ? _num[cur] : _num[cur] - modulo;

                // we might reach at most 2 * modulo^2
                _num[next] = (u + modulo - v) * w % modulo;
                w = w * kernel % modulo;
            }
        }
    }
}

void NTT::r_to_n(ll *_num, int num_bits) const {
    int nLen = 1 << num_bits;

    // butterfly operation
    for (int layer = 1; layer <= num_bits; ++layer) {
        int group = 1 << layer, brother = group >> 1;
        ll kernel = alpha[1][layer], &half = alpha[1][1];

        for (int k = 0; k < nLen; k += group) {

            ll w = 1;

            // find the brother to be butterflied with the current element
            for (int j = 0; j < brother; ++j) {

                int cur = k + j, next = cur + brother;
                ll u = _num[cur], t = w * _num[next] % modulo;

                _num[cur] = u + t;
                _num[cur] = _num[cur] < modulo ? _num[cur] : _num[cur] - modulo;

                _num[next] = u - t;
                _num[next] = _num[next] < 0 ? _num[next] + modulo : _num[next];
                w = w * kernel % modulo;
            }
        }
    }


    ll gcd = 0, invN = 0, invM = 0;
    ext_gcd(nLen, modulo, gcd, invN, invM);
    invN = (invN % modulo + modulo) % modulo;
    for (int i = 0; i < nLen; ++i)
        _num[i] = _num[i] * invN % modulo;
}

void NTT::ext_gcd(ll a, ll b, ll &gcd, ll &x, ll &y) const {
    if (!b) {
        x = 1;
        y = 0;
        gcd = a;
    } else {
        ext_gcd(b, a % b, gcd, y, x);
        y -= a / b * x;
    }
}

NTT::NTT() {
    init_alpha();
}

BigInt NTT::mul(const BigInt &left, const BigInt &right) {
    ll *num[2];
    num[0] = new ll[MAX_N]();
    num[1] = new ll[MAX_N]();
    int left_len = 0, right_len = 0;
    init_num(left.nums, num[0], left_len);
    init_num(right.nums, num[1], right_len);
    // find a k so that 2^k is the closest one to the length of the longest number
    int k = 0;
    int max_len = left_len < right_len ? right_len : left_len;
    while ((1 << k) < max_len) ++k;

    ll res[MAX_N];
    convolution(num[0], num[1], k + 1, res);

    int end_pos = (1 << (k + 1)) - 1;
    for (int i = 0; i < end_pos; ++i) {
        res[i + 1] += res[i] / 100;
        res[i] %= 100;
    }

    for (; end_pos >= 0 && res[end_pos] == 0; --end_pos);

    char num_str_char[MAX_N + 1] = {0};
    if (end_pos == -1)
        sprintf(num_str_char, "0");
    else {
        if (left.sign * right.sign == -1) {
            sprintf(num_str_char, "-");
            sprintf(num_str_char + 1, "%lld", res[end_pos]);
        } else
            sprintf(num_str_char, "%lld", res[end_pos]);
        char *tmp = num_str_char + strlen(num_str_char);

        for (int i = end_pos - 1; i >= 0; --i) {
            sprintf(tmp, "%02lld", res[i]);
            tmp = tmp + strlen(tmp);
        }
    }

    return BigInt(num_str_char);
}
