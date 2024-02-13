// base-2 and base-10 logarithms and exponents, à la C99/C++0x

#ifndef LOG2_H
#define LOG2_H

#include <cmath>

#ifdef log2
#	undef log2
#endif
#ifdef exp2
#	undef exp2
#endif
#ifndef M_LN2
#	define M_LN2 0.69314718055994530942 // log(2.0)
#endif
#ifdef log10
#	undef log10
#endif
#ifdef exp10
#	undef exp10
#endif
#ifndef M_LN10
#	define M_LN10 2.30258509299404568402 // log(10.0)
#endif

namespace transitional {
    inline double Log2(double x) { return std::log(x) / M_LN2; }
    inline double Log2(int x) { return std::log(x) / M_LN2; }
    inline float Log2(float x) { return std::log(x) / float(M_LN2); }
    inline double Exp2(double x) { return std::exp(x * M_LN2); }
    inline double Exp2(int x) { return std::exp(x * M_LN2); }
    inline float Exp2(float x) { return std::exp(x * float(M_LN2)); }
    inline double Log10(double x) { return std::log(x) / M_LN10; }
    inline double Log10(int x) { return std::log(x) / M_LN10; }
    inline float Log10(float x) { return std::log(x) / float(M_LN10); }
    inline double Exp10(double x) { return std::exp(x * M_LN10); }
    inline double Exp10(int x) { return std::exp(x * M_LN10); }
    inline float Exp10(float x) { return std::exp(x * float(M_LN10)); }
}
#define log2 transitional::Log2
#define exp2 transitional::Exp2
#define log10 transitional::Log10
#define exp10 transitional::Exp10

inline double base2log(double x) { return transitional::Log2(x); }

#endif // LOG2_H
