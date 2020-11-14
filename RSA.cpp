//
// Created by yoda on 2020/11/12.
//

#include "RSA.h"

void RSA::gen_key(ll len) {
    promise<BigInt> promise_p, promise_q;
    thread t_p = thread(&BigInt::gen_prime_thread, len, 15, ref(promise_p));
    thread t_q = thread(&BigInt::gen_prime_thread, len, 15, ref(promise_q));
    future<BigInt> future_p = promise_p.get_future();
    future<BigInt> future_q = promise_q.get_future();
    BigInt p = future_p.get(), q = future_q.get();
    t_p.join();
    t_q.join();
    public_key_n = p * q;
    public_key_e = 65537;
    BigInt phi = (p - 1) * (q - 1);
    private_key = mod_inverse(BigInt(public_key_e), phi);
}

BigInt RSA::mod_inverse(const BigInt &to_invert, const BigInt &mod) const {
    BigInt gcd, x, y;
    ext_gcd(to_invert, mod, gcd, x, y);
    return (x + mod) % mod;
}

void RSA::ext_gcd(const BigInt &a, const BigInt &b, BigInt &gcd, BigInt &x, BigInt &y) const {
    if (b == 0) {
        x = 1;
        y = 0;
        gcd = a;
    } else {
        ext_gcd(b, a % b, gcd, y, x);
        y -= a / b * x;
    }
}

BigInt RSA::encrypt_by_public(const BigInt &content) const {
    return content.mod_pow(public_key_e, public_key_n);
}

BigInt RSA::decrypt_by_private(const BigInt &content) const {
    return content.mod_pow(private_key, public_key_n);
}

string RSA::encrypt_by_public(const string &content) const {
    ll cur = 0;
    ll size = content.size();
    string res;
    while (true) {
        if (cur >= size)
            break;
        ll block_len = min((unsigned long long) MAX_LEN, content.size() - cur);
        if (block_len == MAX_LEN - 1)
            block_len -= 10;
        string block = content.substr(cur, block_len);
        BigInt block_num(0);

        for (ll i = 0; i < MAX_LEN; i++) {
            if (i == block_len) {
                block_num = block_num * 256 + 1;
            } else if (i > block_len) {
                block_num = block_num * 256;
            } else
                block_num = block_num * 256 + block[i];
        }

        BigInt encrypted_block = encrypt_by_public(block_num);
        res += encrypted_block.to_string();
        res += "\n";

        cur += block_len;
    }

    return res;
}

string RSA::decrypt_by_private(const string &content) const {
    vector<string> encrypted_nums = split(content, "\n");
    string res;
    for (const auto& encrypted_num: encrypted_nums) {
        BigInt decrypted = decrypt_by_private(BigInt(encrypted_num));
        BigInt _decrypted = decrypted;
        string decrypted_str;

        bool padding = true;
        for (int i = 0; i < 10; i++) {
            if (!_decrypted.is_even()) {
                padding = false;
                break;
            }
            _decrypted >>= 1;
        }

        if (padding) {
            while (decrypted.is_even()) {
                decrypted >>= 8;
            }
            decrypted -= 1;
            decrypted >>= 8;
        }

        while (decrypted != 0) {
            char character = (char)(decrypted % 256).to_long_long();
            decrypted_str.insert(0, 1, character);
            decrypted >>= 8;
        }

        res += decrypted_str;
    }

    return res;
}

vector<string> RSA::split(const string &str, const string &delimiter) {
    vector<string> res;
    if (str.empty())
        return res;

    char *strs = new char[str.length() + 1];
    strcpy(strs, str.c_str());

    char *d = new char[delimiter.length() + 1];
    strcpy(d, delimiter.c_str());

    char *p = strtok(strs, d);
    while (p) {
        string s = p;
        res.push_back(s);
        p = strtok(nullptr, d);
    }

    return res;
}
