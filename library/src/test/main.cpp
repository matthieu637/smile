#include <test/DraftTest.hpp>
#include "bib/Logger.hpp"
#include "simu/DTeacher.hpp"

// FNV-1a constants
static constexpr unsigned long long basis = 14695981039346656037ULL;
static constexpr unsigned long long prime = 1099511628211ULL;

// compile-time hash helper function
constexpr unsigned long long hash_one(char c, const char* remain, unsigned long long value)
{
    return c == 0 ? value : hash_one(remain[0], remain + 1, (value ^ c) * prime);
}

// compile-time hash
constexpr unsigned long long hash_(const char* str)
{
    return hash_one(str[0], str + 1, basis);
}

// run-time hash
unsigned long long hash_rt(const char* str) {
    unsigned long long hash = basis;
    while (*str != 0) {
        hash ^= str[0];
        hash *= prime;
        ++str;
    }
    return hash;
}

constexpr long long operator"" _hash( const char* str, size_t n ) {
    return hash_( str );
}


int main(int argc, char* argv[])
{

    DraftTest m;
    if(argc > 1 ) {
        switch(hash_(argv[1])) {
        case "FGQ1"_hash:
            m.FGQ1();
            break;
        case "FMQ1"_hash:
            m.FMQ1();
            break;
        case "mcar_qltable_teacher"_hash:
            m.mcar_qltable_teacher(atof(argv[2]));
            break;
        }
    }
    else {
//         m.FGQ1();
//         m.FMQ1();
      
        m.TGQ1Q2(3);
//         m.mcar_qltable_teacher(2);
    }

}



