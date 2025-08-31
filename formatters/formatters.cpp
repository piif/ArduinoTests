#include <iostream>
#include <iomanip>

using namespace std;

struct Bin {
    unsigned long long value;
    unsigned short len;
    char pad;
    explicit Bin(unsigned long long v, unsigned short _len = 0, char _pad = ' ') : value(v), len(_len), pad(_pad) {}
};

std::ostream& operator<<(std::ostream& os, Bin b) {
    unsigned long long mask;
    if (b.len != 0) {
        mask = 1UL << (b.len-1);
    } else {
        // compute "binary len" of value
        mask = 1UL << ((sizeof(mask) * __CHAR_BIT__)-1);
    }

    while (mask > 1 && (mask & b.value) == 0) {
        if (b.len != 0) {
            os << b.pad;
        }
        mask >>=1;
    }
    while(mask) {
        os << ( mask & b.value ? '1' : '0');
        mask >>=1;
    }
    return os;
}

Bin bin(unsigned long long value, unsigned short len = 0, char pad = ' ') {
    return Bin(value, len, pad);
}

#define bitlen(x) ((sizeof(x)<<3) - __builtin_clz(x) )

int main() {
    unsigned short s = 42;
    unsigned int   i = 42;
    unsigned long  l = 1234;
    unsigned long long ll = 12345678UL;

    cout << "sizeof  s = " << (sizeof( s) * __CHAR_BIT__) << " , clz/len(" << setbase(16) << s <<") = "  << setbase(10) << __builtin_clz( s) << '/' << bitlen( s) << endl;
    cout << "sizeof  i = " << (sizeof( i) * __CHAR_BIT__) << " , clz/len(" << setbase(16) << i <<") = "  << setbase(10) << __builtin_clz( i) << '/' << bitlen( i) << endl;
    cout << "sizeof  l = " << (sizeof( l) * __CHAR_BIT__) << " , clz/len(" << setbase(16) << l <<") = "  << setbase(10) << __builtin_clz( l) << '/' << bitlen( l) << endl;
    cout << "sizeof ll = " << (sizeof(ll) * __CHAR_BIT__) << " , clz/len(" << setbase(16) <<ll <<") = "  << setbase(10) << __builtin_clz(ll) << '/' << bitlen(ll) << endl;

    cout << "\ns = " << s << " = 0x" << setbase(16) << s << setbase(10)
        << " = '0b" << bin(s)
        << "' =(4/' ') '0b" << bin(s, 4)
        << "' =(4/'0') '0b" << bin(s, 4, '0') << "'\n\n";

    cout << "i = " << i << " = 0x" << setbase(16) << i << setbase(10)
        << " = '0b" << bin(i)
        << "' =(8/' ') '0b" << bin(i, 8)
        << "' =(8/'0') '0b" << bin(i, 8, '0') << "'\n\n";

    s = 1UL << ((sizeof(s) * __CHAR_BIT__)-1);
    cout << "s = " << s << " = 0x" << setbase(16) << s << setbase(10)
        << " = '0b" << bin(s) << "'\n\n";

    s = 0;
    cout << "s = " << s << " = 0x" << setbase(16) << s << setbase(10)
        << " = '0b" << bin(s)
        << "' =(4/' ') '0b" << bin(s, 4)
        << "' =(4/'0') '0b" << bin(s, 4, '0') << "'\n\n";

    cout << "ll = " << ll << " = 0x" << setbase(16) << ll << setbase(10)
        << "\n = '0b" << bin(ll)
        << "'\n =(60/' ') '0b" << bin(ll, 60)
        << "'\n =(60/'0') '0b" << bin(ll, 60, '0')
        << "'\n =(32/'0') '0b" << bin(ll, 32, '0') << "\'\n\n";

    ll = 1UL << ((sizeof(ll) * __CHAR_BIT__)-1);
    cout << "ll = " << ll << " = 0x" << setbase(16) << ll << setbase(10)
        << "\n = '0b" << bin(ll)
        << "'\n =(60/'.') '0b" << bin(ll, 60, '.')
        << "'\n =(60/'0') '0b" << bin(ll, 60, '0')
        << "'\n =(32/'0') '0b" << bin(ll, 32, '0') << "\'\n";
}