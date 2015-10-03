#ifndef SPACEMATH_H
#define SPACEMATH_H

#include <iostream>

template<typename T>
class vec2;
template<typename T>
class vec3;
template<typename T>
class vec4;

template<typename T>
class ArraySubscriptOperators
{
public:
    T& operator[] (int _index)
    {
        return *((T*)this + _index);
    }

    T& operator[] (int _index) const
    {
        return *((T*)this + _index);
    }
};

template<class vT, typename T, int A, int B, int C, int D>
class vProxy : public ArraySubscriptOperators<T>
{
public:
    static const bool isWritable = (A != B) &&
                                   ((std::is_same<vT, vec3<T>>::value) ?
                                   (B != C) && (A != C) : true) &&
                                   ((std::is_same<vT, vec4<T>>::value) ?
                                   (C != D) && (B != D) && (A != D) : true);
    typedef typename std::conditional<isWritable,
        vProxy, struct OperationNotValid>::type assignmentType;

#   define CREATE_ASSIGNMENT_OPERATOR(Op)                                           \
    template<class rhsVT, typename rhsT, int rhsA, int rhsB, int rhsC, int rhsD>    \
    assignmentType& operator##Op##(                                                 \
                    const vProxy<rhsVT, rhsT, rhsA, rhsB, rhsC, rhsD>& _rhs)        \
    {                                                                               \
        ((T*)this)[A] Op ((const rhsT*)&_rhs)[rhsA];                                \
        ((T*)this)[B] Op ((const rhsT*)&_rhs)[rhsB];                                \
        if ((sizeof(rhsVT) / sizeof(rhsT) > 2) && (sizeof(vT) / sizeof(T) > 2))     \
            ((T*)this)[C] Op ((const rhsT*)&_rhs)[rhsC];                            \
        if ((sizeof(rhsVT) / sizeof(rhsT) > 3) && (sizeof(vT) / sizeof(T) > 3))     \
            ((T*)this)[D] Op((const rhsT*)&_rhs)[rhsD];                             \
        return *this;                                                               \
    }                                                                               \
    assignmentType& operator##Op##(const T _rhs)                                    \
    {                                                                               \
        ((T*)this)[A] Op _rhs;                                                      \
        ((T*)this)[B] Op _rhs;                                                      \
        if ((sizeof(vT) / sizeof(T)) > 2)                                           \
            ((T*)this)[C] Op _rhs;                                                  \
        if ((sizeof(vT) / sizeof(T)) > 3)                                           \
            ((T*)this)[D] Op _rhs;                                                  \
        return *this;                                                               \
    }                                                                               \
    template<template<typename> class rhsVT, typename rhsT>                         \
    assignmentType& operator##Op##(const rhsVT<rhsT>& _rhs)                         \
    {                                                                               \
        ((T*)this)[A] Op _rhs[0];                                                   \
        ((T*)this)[B] Op _rhs[1];                                                   \
        if ((sizeof(vT) / sizeof(T) > 2) &&                                         \
           (std::is_same<rhsVT<rhsT>, vec3<rhsT>>::value ||                         \
            std::is_same<rhsVT<rhsT>, vec4<rhsT>>::value))                          \
            ((T*)this)[C] Op _rhs[2];                                               \
        if (std::is_same<rhsVT<rhsT>, vec4<rhsT>>::value &&                         \
           (sizeof(vT) / sizeof(T) > 3))                                            \
            ((T*)this)[D] Op _rhs[3];                                               \
        return *this;                                                               \
    }

    CREATE_ASSIGNMENT_OPERATOR(= );

    CREATE_ASSIGNMENT_OPERATOR(+= );
    CREATE_ASSIGNMENT_OPERATOR(-= );
    CREATE_ASSIGNMENT_OPERATOR(*= );
    CREATE_ASSIGNMENT_OPERATOR(/= );

    CREATE_ASSIGNMENT_OPERATOR(|= );
    CREATE_ASSIGNMENT_OPERATOR(&= );
    CREATE_ASSIGNMENT_OPERATOR(^= );
    CREATE_ASSIGNMENT_OPERATOR(%= );
    CREATE_ASSIGNMENT_OPERATOR(<<= );
    CREATE_ASSIGNMENT_OPERATOR(>>= );

#   undef CREATE_ASSIGNMENT_OPERATOR

#   define CREATE_ARITHMETIC_OPERATOR(Op)                                           \
    template<class rhsVT, typename rhsT, int rhsA, int rhsB, int rhsC, int rhsD>    \
    vT operator##Op##(                                                              \
       const vProxy<rhsVT, rhsT, rhsA, rhsB, rhsC, rhsD>& _rhs) const               \
    {                                                                               \
        vT result;                                                                  \
        result[0] = ((const T*)this)[A] Op ((const rhsT*)&_rhs)[rhsA];              \
        result[1] = ((const T*)this)[B] Op ((const rhsT*)&_rhs)[rhsB];              \
        if ((sizeof(vT) / sizeof(T) > 2) && (sizeof(rhsVT) / sizeof(rhsT) > 2))     \
            result[2] = ((const T*)this)[C] Op ((const rhsT*)&_rhs)[rhsC];          \
        if ((sizeof(vT) / sizeof(T) > 3) && (sizeof(rhsVT) / sizeof(rhsT) > 3))     \
            result[3] = ((const T*)this)[D] Op ((const rhsT*)&_rhs)[rhsD];          \
        return result;                                                              \
    }                                                                               \
    vT operator##Op##(const T _rhs) const                                           \
    {                                                                               \
        vT result;                                                                  \
        result[0] = ((const T*)this)[A] Op _rhs;                                    \
        result[1] = ((const T*)this)[B] Op _rhs;                                    \
        if ((sizeof(vT) / sizeof(T)) > 2)                                           \
            result[2] = ((const T*)this)[C] Op _rhs;                                \
        if ((sizeof(vT) / sizeof(T)) > 3)                                           \
            result[3] = ((const T*)this)[D] Op _rhs;                                \
        return result;                                                              \
    }                                                                               \
    friend vT operator##Op##(const T _lhs, const vProxy& _rhs)                      \
    {                                                                               \
        vT result;                                                                  \
        result[0] = _lhs Op ((const T*)&_rhs)[A];                                   \
        result[1] = _lhs Op ((const T*)&_rhs)[B];                                   \
        if ((sizeof(vT) / sizeof(T)) > 2)                                           \
            result[2] = _lhs Op ((const T*)&_rhs)[C];                               \
        if ((sizeof(vT) / sizeof(T)) > 3)                                           \
            result[3] = _lhs Op ((const T*)&_rhs)[D];                               \
        return result;                                                              \
    }                                                                               \
    template<template<typename> class lhsVT, typename lhsT>                         \
    friend vT operator##Op##(const lhsVT<lhsT>& _lhs, const vProxy& _rhs)           \
    {                                                                               \
        vT result;                                                                  \
        result[0] = ((const lhsT*)&_lhs)[0] Op ((const T*)&_rhs)[A];                \
        result[1] = ((const lhsT*)&_lhs)[1] Op ((const T*)&_rhs)[B];                \
        if ((sizeof(vT) / sizeof(T) > 2) &&                                         \
           (std::is_same<lhsVT<lhsT>, vec3<lhsT>>::value ||                         \
            std::is_same<lhsVT<lhsT>, vec4<lhsT>>::value))                          \
            result[2] = ((const lhsT*)&_lhs)[2] Op ((const T*)&_rhs)[C];            \
        if ((sizeof(vT) / sizeof(T) > 3) &&                                         \
            std::is_same<lhsVT<lhsT>, vec4<lhsT>>::value)                           \
            result[3] = ((const lhsT*)&_lhs)[3] Op ((const T*)&_rhs)[D];            \
        return result;                                                              \
    }                                                                               \
    template<template<typename> class rhsVT, typename rhsT>                         \
    vT operator##Op##(const rhsVT<rhsT>& _rhs) const                                \
    {                                                                               \
        vT result;                                                                  \
        result[0] = ((const T*)this)[A] Op ((const rhsT*)&_rhs)[0];                 \
        result[1] = ((const T*)this)[B] Op ((const rhsT*)&_rhs)[1];                 \
        if ((sizeof(vT) / sizeof(T) > 2) &&                                         \
           (std::is_same<rhsVT<rhsT>, vec3<rhsT>>::value ||                         \
            std::is_same<rhsVT<rhsT>, vec4<rhsT>>::value))                          \
            result[2] = ((const T*)this)[C] Op ((const rhsT*)&_rhs)[2];             \
        if ((sizeof(vT) / sizeof(T) > 3) &&                                         \
            std::is_same<rhsVT<rhsT>, vec4<rhsT>>::value)                           \
            result[2] = ((const T*)this)[D] Op ((const rhsT*)&_rhs)[3];             \
        return result;                                                              \
    }

    CREATE_ARITHMETIC_OPERATOR(+);
    CREATE_ARITHMETIC_OPERATOR(-);
    CREATE_ARITHMETIC_OPERATOR(*);
    CREATE_ARITHMETIC_OPERATOR(/ );

    CREATE_ARITHMETIC_OPERATOR(| );
    CREATE_ARITHMETIC_OPERATOR(&);
    CREATE_ARITHMETIC_OPERATOR(^);
    CREATE_ARITHMETIC_OPERATOR(%);
    CREATE_ARITHMETIC_OPERATOR(<< );
    CREATE_ARITHMETIC_OPERATOR(>> );

#   undef CREATE_ARITHMETIC_OPERATOR

#   define CREATE_COMPARISON_OPERATOR(Op)                                           \
    template<class rhsVT, typename rhsT, int rhsA, int rhsB, int rhsC, int rhsD>    \
    bool operator##Op##(                                                            \
         const vProxy<rhsVT, rhsT, rhsA, rhsB, rhsC, rhsD>& _rhs) const             \
    {                                                                               \
        return (((const T*)this)[A] Op ((const rhsT*)&_rhs)[rhsA]) &&               \
               (((const T*)this)[B] Op ((const rhsT*)&_rhs)[rhsB]) &&               \
               ((sizeof(vT) / sizeof(T) > 2) && (sizeof(rhsVT) / sizeof(rhsT) > 2) ?\
               (((const T*)this)[C] Op ((const rhsT*)&_rhs)[rhsC]) : true) &&       \
               ((sizeof(vT) / sizeof(T) > 3) && (sizeof(rhsVT) / sizeof(rhsT) > 3) ?\
               (((const T*)this)[D] Op ((const rhsT*)&_rhs)[rhsD]) : true);         \
    }                                                                               \
    bool operator##Op##(const T _rhs) const                                         \
    {                                                                               \
        return (((const T*)this)[A] Op _rhs) &&                                     \
               (((const T*)this)[B] Op _rhs) &&                                     \
               (((sizeof(vT) / sizeof(T)) > 2) ?                                    \
               (((const T*)this)[C] Op _rhs) : true) &&                             \
               (((sizeof(vT) / sizeof(T)) > 3) ?                                    \
               (((const T*)this)[D] Op _rhs) : true);                               \
    }                                                                               \
    friend bool operator##Op##(const T _lhs, const vProxy& _rhs)                    \
    {                                                                               \
        return (_lhs Op ((const T*)&_rhs)[A]) &&                                    \
               (_lhs Op ((const T*)&_rhs)[B]) &&                                    \
               (((sizeof(vT) / sizeof(T)) > 2) ?                                    \
               (_lhs Op ((const T*)&_rhs)[C]) : true) &&                            \
               (((sizeof(vT) / sizeof(T)) > 3) ?                                    \
               (_lhs Op ((const T*)&_rhs)[C]) : true);                              \
    }                                                                               \
    template<template<typename> class lhsVT, typename lhsT>                         \
    friend bool operator##Op##(const lhsVT<lhsT>& _lhs, const vProxy& _rhs)         \
    {                                                                               \
        return (((const lhsT*)&_lhs)[0] Op ((const T*)&_rhs)[A]) &&                 \
               (((const lhsT*)&_lhs)[1] Op ((const T*)&_rhs)[B]) &&                 \
               (((sizeof(vT) / sizeof(T) > 2) &&                                    \
               (std::is_same<lhsVT<lhsT>, vec3<lhsT>>::value ||                     \
                std::is_same<lhsVT<lhsT>, vec4<lhsT>>::value)) ?                    \
               (((const lhsT*)&_lhs)[2] Op ((const T*)&_rhs)[C]) : true) &&         \
               (((sizeof(vT) / sizeof(T) > 3) &&                                    \
                std::is_same<lhsVT<lhsT>, vec4<lhsT>>::value) ?                     \
               (((const lhsT*)&_lhs)[3] Op ((const T*)&_rhs)[D]) : true);           \
    }                                                                               \
    template<template<typename> class rhsVT, typename rhsT>                         \
    bool operator##Op##(const rhsVT<rhsT>& _rhs) const                              \
    {                                                                               \
        return (((const T*)this)[A] Op ((const rhsT*)&_rhs)[0]) &&                  \
               (((const T*)this)[B] Op ((const rhsT*)&_rhs)[1]) &&                  \
               (((sizeof(vT) / sizeof(T) > 2) &&                                    \
               (std::is_same<rhsVT<rhsT>, vec3<rhsT>>::value ||                     \
                std::is_same<rhsVT<rhsT>, vec4<rhsT>>::value)) ?                    \
               (((const T*)this)[C] Op ((const rhsT*)&_rhs)[2]) : true) &&          \
               (((sizeof(vT) / sizeof(T) > 3) &&                                    \
                std::is_same<rhsVT<rhsT>, vec4<rhsT>>::value) ?                     \
               (((const T*)this)[D] Op ((const rhsT*)&_rhs)[3]) : true);            \
    }

    CREATE_COMPARISON_OPERATOR(== );
    CREATE_COMPARISON_OPERATOR(!= );
    CREATE_COMPARISON_OPERATOR(<= );
    CREATE_COMPARISON_OPERATOR(>= );
    CREATE_COMPARISON_OPERATOR(< );
    CREATE_COMPARISON_OPERATOR(> );

#   undef CREATE_COMPARISON_OPERATOR
};

template<class vT, typename T>
class VectorOperators : public ArraySubscriptOperators<T>
{
public:
#   define CREATE_ASSIGNMENT_OPERATOR(Op)                                           \
    template<template<typename> class rhsVT, typename rhsT>                         \
    VectorOperators& operator##Op##(const rhsVT<rhsT>& _rhs)                        \
    {                                                                               \
        ((T*)this)[0] Op ((const rhsT*)&_rhs)[0];                                   \
        ((T*)this)[1] Op ((const rhsT*)&_rhs)[1];                                   \
        if ((sizeof(vT) / sizeof(T)) > 2 &&                                         \
           (std::is_same<rhsVT<rhsT>, vec3<rhsT>>::value ||                         \
            std::is_same<rhsVT<rhsT>, vec4<rhsT>>::value))                          \
            ((T*)this)[2] Op ((const rhsT*)&_rhs)[2];                               \
        if ((sizeof(vT) / sizeof(T)) > 3 &&                                         \
            std::is_same<rhsVT<rhsT>, vec4<rhsT>>::value)                           \
            ((T*)this)[3] Op ((const rhsT*)&_rhs)[3];                               \
        return *this;                                                               \
    }                                                                               \
    VectorOperators& operator##Op##(const T _rhs)                                   \
    {                                                                               \
        ((T*)this)[0] Op _rhs;                                                      \
        ((T*)this)[1] Op _rhs;                                                      \
        if ((sizeof(vT) / sizeof(T)) > 2)                                           \
            ((T*)this)[2] Op _rhs;                                                  \
        if ((sizeof(vT) / sizeof(T)) > 3)                                           \
            ((T*)this)[3] Op _rhs;                                                  \
        return *this;                                                               \
    }                                                                               \
    template<class rhsVT, typename rhsT, int rhsA, int rhsB, int rhsC, int rhsD>    \
    VectorOperators& operator##Op##(                                                \
                     const vProxy<rhsVT, rhsT, rhsA, rhsB, rhsC, rhsD>& _rhs)       \
    {                                                                               \
        ((T*)this)[0] Op ((const rhsT*)&_rhs)[rhsA];                                \
        ((T*)this)[1] Op ((const rhsT*)&_rhs)[rhsB];                                \
        if ((sizeof(vT) / sizeof(T)) > 2 && (sizeof(rhsVT) / sizeof(rhsT)) > 2)     \
            ((T*)this)[2] Op ((const rhsT*)&_rhs)[rhsC];                            \
        if ((sizeof(vT) / sizeof(T)) > 3 && (sizeof(rhsVT) / sizeof(rhsT)) > 3)     \
            ((T*)this)[3] Op ((const rhsT*)&_rhs)[rhsD];                            \
        return *this;                                                               \
    }

    CREATE_ASSIGNMENT_OPERATOR(= );

    CREATE_ASSIGNMENT_OPERATOR(+= );
    CREATE_ASSIGNMENT_OPERATOR(-= );
    CREATE_ASSIGNMENT_OPERATOR(*= );
    CREATE_ASSIGNMENT_OPERATOR(/= );

    CREATE_ASSIGNMENT_OPERATOR(|= );
    CREATE_ASSIGNMENT_OPERATOR(&= );
    CREATE_ASSIGNMENT_OPERATOR(^= );
    CREATE_ASSIGNMENT_OPERATOR(%= );
    CREATE_ASSIGNMENT_OPERATOR(<<= );
    CREATE_ASSIGNMENT_OPERATOR(>>= );

#   undef CREATE_ASSIGNMENT_OPERATOR

#   define CREATE_ARITHMETIC_OPERATOR(Op)                                           \
    template<template<typename> class rhsVT, typename rhsT>                         \
    vT operator##Op##(const rhsVT<rhsT>& _rhs) const                                \
    {                                                                               \
        vT result;                                                                  \
        result[0] = ((const T*)this)[0] Op ((const rhsT*)&_rhs)[0];                 \
        result[1] = ((const T*)this)[1] Op ((const rhsT*)&_rhs)[1];                 \
        if ((sizeof(vT) / sizeof(T)) > 2 &&                                         \
           (std::is_same<rhsVT<rhsT>, vec3<rhsT>>::value ||                         \
            std::is_same<rhsVT<rhsT>, vec4<rhsT>>::value))                          \
            result[2] = ((const T*)this)[2] Op ((const rhsT*)&_rhs)[2];             \
        if ((sizeof(vT) / sizeof(T)) > 2 &&                                         \
            std::is_same<rhsVT<rhsT>, vec4<rhsT>>::value)                           \
            result[3] = ((const T*)this)[3] Op ((const rhsT*)&_rhs)[3];             \
        return result;                                                              \
    }                                                                               \
    vT operator##Op##(const T _rhs) const                                           \
    {                                                                               \
        vT result;                                                                  \
        result[0] = ((const T*)this)[0] Op _rhs;                                    \
        result[1] = ((const T*)this)[1] Op _rhs;                                    \
        if ((sizeof(vT) / sizeof(T)) > 2)                                           \
            result[2] = ((const T*)this)[2] Op _rhs;                                \
        if ((sizeof(vT) / sizeof(T)) > 3)                                           \
            result[3] = ((const T*)this)[3] Op _rhs;                                \
        return result;                                                              \
    }                                                                               \
    friend vT operator##Op##(const T _lhs, const vT& _rhs)                          \
    {                                                                               \
        vT result;                                                                  \
        result[0] = _lhs Op ((const T*)&_rhs)[0];                                   \
        result[1] = _lhs Op ((const T*)&_rhs)[1];                                   \
        if ((sizeof(vT) / sizeof(T)) > 2)                                           \
            result[2] = _lhs Op ((const T*)&_rhs)[2];                               \
        if ((sizeof(vT) / sizeof(T)) > 3)                                           \
            result[3] = _lhs Op ((const T*)&_rhs)[3];                               \
        return result;                                                              \
    }

    CREATE_ARITHMETIC_OPERATOR(+);
    CREATE_ARITHMETIC_OPERATOR(-);
    CREATE_ARITHMETIC_OPERATOR(*);
    CREATE_ARITHMETIC_OPERATOR(/ );

    CREATE_ARITHMETIC_OPERATOR(| );
    CREATE_ARITHMETIC_OPERATOR(&);
    CREATE_ARITHMETIC_OPERATOR(^);
    CREATE_ARITHMETIC_OPERATOR(%);
    CREATE_ARITHMETIC_OPERATOR(<< );
    CREATE_ARITHMETIC_OPERATOR(>> );

#   undef CREATE_ARITHMETIC_OPERATOR

#   define CREATE_COMPARISON_OPERATOR(Op)                                           \
    template<template<typename> class rhsVT, typename rhsT>                         \
    bool operator##Op##(const rhsVT<rhsT>& _rhs) const                              \
    {                                                                               \
        return (((const T*)this)[0] Op ((const rhsT*)&_rhs)[0]) &&                  \
               (((const T*)this)[1] Op ((const rhsT*)&_rhs)[1]) &&                  \
               (((sizeof(vT) / sizeof(T) > 2) &&                                    \
               (std::is_same<rhsVT<rhsT>, vec3<rhsT>>::value ||                     \
                std::is_same<rhsVT<rhsT>, vec4<rhsT>>::value)) ?                    \
               (((const T*)this)[2] Op ((const rhsT*)&_rhs)[2]) : true) &&          \
               (((sizeof(vT) / sizeof(T) > 3) &&                                    \
                std::is_same<rhsVT<rhsT>, vec4<rhsT>>::value) ?                     \
               (((const T*)this)[3] Op ((const rhsT*)&_rhs)[3]) : true);            \
    }                                                                               \
    bool operator##Op##(const T _rhs) const                                         \
    {                                                                               \
        return (((const T*)this)[0] Op _rhs) &&                                     \
               (((const T*)this)[1] Op _rhs) &&                                     \
               ((sizeof(vT) / sizeof(T)) > 2 ?                                      \
               ((const T*)this)[2] Op _rhs : true) &&                               \
               ((sizeof(vT) / sizeof(T)) > 3 ?                                      \
               ((const T*)this)[3] Op _rhs : true);                                 \
    }                                                                               \
    friend bool operator##Op##(const T  _lhs, const vT& _rhs)                       \
    {                                                                               \
        return (_lhs Op ((const T*)&_rhs)[0]) &&                                    \
               (_lhs Op ((const T*)&_rhs)[1]) &&                                    \
               ((sizeof(vT) / sizeof(T)) > 2 ?                                      \
               (_lhs Op ((const T*)&_rhs)[2]) : true) &&                            \
               ((sizeof(vT) / sizeof(T)) > 3 ?                                      \
               (_lhs Op ((const T*)&_rhs)[3]) : true);                              \
    }

    CREATE_COMPARISON_OPERATOR(== );
    CREATE_COMPARISON_OPERATOR(!= );
    CREATE_COMPARISON_OPERATOR(<= );
    CREATE_COMPARISON_OPERATOR(>= );
    CREATE_COMPARISON_OPERATOR(<);
    CREATE_COMPARISON_OPERATOR(>);


#   undef CREATE_COMPARISON_OPERATOR
};

template<typename T>
class vec2 : public VectorOperators<vec2<T>, T>
{
public:
    union
    {
        struct { T x, y; };
        struct { T r, g; };
        struct { T s, t; };

        vProxy<vec2<T>, T, 0, 0, -1, -1> xx, rr, ss;
        vProxy<vec2<T>, T, 0, 1, -1, -1> xy, rg, st;
        vProxy<vec2<T>, T, 1, 0, -1, -1> yx, gr, ts;
        vProxy<vec2<T>, T, 1, 1, -1, -1> yy, gg, tt;
        vProxy<vec3<T>, T, 0, 0, 0, -1> xxx, rrr, sss;
        vProxy<vec3<T>, T, 0, 0, 1, -1> xxy, rrg, sst;
        vProxy<vec3<T>, T, 0, 1, 0, -1> xyx, rgr, sts;
        vProxy<vec3<T>, T, 0, 1, 1, -1> xyy, rgg, stt;
        vProxy<vec3<T>, T, 1, 0, 0, -1> yxx, grr, tss;
        vProxy<vec3<T>, T, 1, 0, 1, -1> yxy, grg, tst;
        vProxy<vec3<T>, T, 1, 1, 0, -1> yyx, ggr, tts;
        vProxy<vec3<T>, T, 1, 1, 1, -1> yyy, ggg, ttt;
        vProxy<vec4<T>, T, 0, 0, 0, 0> xxxx, rrrr, ssss;
        vProxy<vec4<T>, T, 0, 0, 0, 1> xxxy, rrrg, ssst;
        vProxy<vec4<T>, T, 0, 0, 1, 0> xxyx, rrgr, ssts;
        vProxy<vec4<T>, T, 0, 0, 1, 1> xxyy, rrgg, sstt;
        vProxy<vec4<T>, T, 0, 1, 0, 0> xyxx, rgrr, stss;
        vProxy<vec4<T>, T, 0, 1, 0, 1> xyxy, rgrg, stst;
        vProxy<vec4<T>, T, 0, 1, 1, 0> xyyx, rggr, stts;
        vProxy<vec4<T>, T, 0, 1, 1, 1> xyyy, rggg, sttt;
        vProxy<vec4<T>, T, 1, 0, 0, 0> yxxx, grrr, tsss;
        vProxy<vec4<T>, T, 1, 0, 0, 1> yxxy, grrg, tsst;
        vProxy<vec4<T>, T, 1, 0, 1, 0> yxyx, grgr, tsts;
        vProxy<vec4<T>, T, 1, 0, 1, 1> yxyy, grgg, tstt;
        vProxy<vec4<T>, T, 1, 1, 0, 0> yyxx, ggrr, ttss;
        vProxy<vec4<T>, T, 1, 1, 0, 1> yyxy, ggrg, ttst;
        vProxy<vec4<T>, T, 1, 1, 1, 0> yyyx, gggr, ttts;
        vProxy<vec4<T>, T, 1, 1, 1, 1> yyyy, gggg, tttt;

    };

    vec2() { }
    vec2(T _x, T _y) { ((T*)this)[0] = _x; ((T*)this)[1] = _y; }
    vec2(T _x) { vec2(_x, _x); }
    template<template<typename> class vT, typename iT>
    vec2(vT<iT>& _v)
    {
        ((T*)this)[0] = ((const iT*)&_v)[0];
        ((T*)this)[1] = ((const iT*)&_v)[1];
    }
    template<class vT, typename pT, int A, int B, int C, int D>
    vec2(vProxy<vT, pT, A, B, C, D>& _v)
    {
        ((T*)this)[0] = ((const pT*)&_v)[A];
        ((T*)this)[1] = ((const pT*)&_v)[B];
    }
};

template<typename T>
class vec3 : public VectorOperators<vec3<T>, T>
{
public:
    union
    {
        struct { T x, y, z; };
        struct { T r, g, b; };
        struct { T s, t, p; };

        vProxy<vec2<T>, T, 0, 0, -1, -1> xx, rr, ss;
        vProxy<vec2<T>, T, 0, 1, -1, -1> xy, rg, st;
        vProxy<vec2<T>, T, 0, 2, -1, -1> xz, rb, sp;
        vProxy<vec2<T>, T, 1, 0, -1, -1> yx, gr, ts;
        vProxy<vec2<T>, T, 1, 1, -1, -1> yy, gg, tt;
        vProxy<vec2<T>, T, 1, 2, -1, -1> yz, gb, tp;
        vProxy<vec2<T>, T, 2, 0, -1, -1> zx, br, ps;
        vProxy<vec2<T>, T, 2, 1, -1, -1> zy, bg, pt;
        vProxy<vec2<T>, T, 2, 2, -1, -1> zz, bb, pp;
        vProxy<vec3<T>, T, 0, 0, 0, -1> xxx, rrr, sss;
        vProxy<vec3<T>, T, 0, 0, 1, -1> xxy, rrg, sst;
        vProxy<vec3<T>, T, 0, 0, 2, -1> xxz, rrb, ssp;
        vProxy<vec3<T>, T, 0, 1, 0, -1> xyx, rgr, sts;
        vProxy<vec3<T>, T, 0, 1, 1, -1> xyy, rgg, stt;
        vProxy<vec3<T>, T, 0, 1, 2, -1> xyz, rgb, stp;
        vProxy<vec3<T>, T, 0, 2, 0, -1> xzx, rbr, sps;
        vProxy<vec3<T>, T, 0, 2, 1, -1> xzy, rbg, spt;
        vProxy<vec3<T>, T, 0, 2, 2, -1> xzz, rbb, spp;
        vProxy<vec3<T>, T, 1, 0, 0, -1> yxx, grr, tss;
        vProxy<vec3<T>, T, 1, 0, 1, -1> yxy, grg, tst;
        vProxy<vec3<T>, T, 1, 0, 2, -1> yxz, grb, tsp;
        vProxy<vec3<T>, T, 1, 1, 0, -1> yyx, ggr, tts;
        vProxy<vec3<T>, T, 1, 1, 1, -1> yyy, ggg, ttt;
        vProxy<vec3<T>, T, 1, 1, 2, -1> yyz, ggb, ttp;
        vProxy<vec3<T>, T, 1, 2, 0, -1> yzx, gbr, tps;
        vProxy<vec3<T>, T, 1, 2, 1, -1> yzy, gbg, tpt;
        vProxy<vec3<T>, T, 1, 2, 2, -1> yzz, gbb, tpp;
        vProxy<vec3<T>, T, 2, 0, 0, -1> zxx, brr, pss;
        vProxy<vec3<T>, T, 2, 0, 1, -1> zxy, brg, pst;
        vProxy<vec3<T>, T, 2, 0, 2, -1> zxz, brb, psp;
        vProxy<vec3<T>, T, 2, 1, 0, -1> zyx, bgr, pts;
        vProxy<vec3<T>, T, 2, 1, 1, -1> zyy, bgg, ptt;
        vProxy<vec3<T>, T, 2, 1, 2, -1> zyz, bgb, ptp;
        vProxy<vec3<T>, T, 2, 2, 0, -1> zzx, bbr, pps;
        vProxy<vec3<T>, T, 2, 2, 1, -1> zzy, bbg, ppt;
        vProxy<vec3<T>, T, 2, 2, 2, -1> zzz, bbb, ppp;
        vProxy<vec4<T>, T, 0, 0, 0, 0> xxxx, rrrr, ssss;
        vProxy<vec4<T>, T, 0, 0, 0, 1> xxxy, rrrg, ssst;
        vProxy<vec4<T>, T, 0, 0, 0, 2> xxxz, rrrb, sssp;
        vProxy<vec4<T>, T, 0, 0, 1, 0> xxyx, rrgr, ssts;
        vProxy<vec4<T>, T, 0, 0, 1, 1> xxyy, rrgg, sstt;
        vProxy<vec4<T>, T, 0, 0, 1, 2> xxyz, rrgb, sstp;
        vProxy<vec4<T>, T, 0, 0, 2, 0> xxzx, rrbr, ssps;
        vProxy<vec4<T>, T, 0, 0, 2, 1> xxzy, rrbg, sspt;
        vProxy<vec4<T>, T, 0, 0, 2, 2> xxzz, rrbb, sspp;
        vProxy<vec4<T>, T, 0, 1, 0, 0> xyxx, rgrr, stss;
        vProxy<vec4<T>, T, 0, 1, 0, 1> xyxy, rgrg, stst;
        vProxy<vec4<T>, T, 0, 1, 0, 2> xyxz, rgrb, stsp;
        vProxy<vec4<T>, T, 0, 1, 1, 0> xyyx, rggr, stts;
        vProxy<vec4<T>, T, 0, 1, 1, 1> xyyy, rggg, sttt;
        vProxy<vec4<T>, T, 0, 1, 1, 2> xyyz, rggb, sttp;
        vProxy<vec4<T>, T, 0, 1, 2, 0> xyzx, rgbr, stps;
        vProxy<vec4<T>, T, 0, 1, 2, 1> xyzy, rgbg, stpt;
        vProxy<vec4<T>, T, 0, 1, 2, 2> xyzz, rgbb, stpp;
        vProxy<vec4<T>, T, 0, 2, 0, 0> xzxx, rbrr, spss;
        vProxy<vec4<T>, T, 0, 2, 0, 1> xzxy, rbrg, spst;
        vProxy<vec4<T>, T, 0, 2, 0, 2> xzxz, rbrb, spsp;
        vProxy<vec4<T>, T, 0, 2, 1, 0> xzyx, rbgr, spts;
        vProxy<vec4<T>, T, 0, 2, 1, 1> xzyy, rbgg, sptt;
        vProxy<vec4<T>, T, 0, 2, 1, 2> xzyz, rbgb, sptp;
        vProxy<vec4<T>, T, 0, 2, 2, 0> xzzx, rbbr, spps;
        vProxy<vec4<T>, T, 0, 2, 2, 1> xzzy, rbbg, sppt;
        vProxy<vec4<T>, T, 0, 2, 2, 2> xzzz, rbbb, sppp;
        vProxy<vec4<T>, T, 1, 0, 0, 0> yxxx, grrr, tsss;
        vProxy<vec4<T>, T, 1, 0, 0, 1> yxxy, grrg, tsst;
        vProxy<vec4<T>, T, 1, 0, 0, 2> yxxz, grrb, tssp;
        vProxy<vec4<T>, T, 1, 0, 1, 0> yxyx, grgr, tsts;
        vProxy<vec4<T>, T, 1, 0, 1, 1> yxyy, grgg, tstt;
        vProxy<vec4<T>, T, 1, 0, 1, 2> yxyz, grgb, tstp;
        vProxy<vec4<T>, T, 1, 0, 2, 0> yxzx, grbr, tsps;
        vProxy<vec4<T>, T, 1, 0, 2, 1> yxzy, grbg, tspt;
        vProxy<vec4<T>, T, 1, 0, 2, 2> yxzz, grbb, tspp;
        vProxy<vec4<T>, T, 1, 1, 0, 0> yyxx, ggrr, ttss;
        vProxy<vec4<T>, T, 1, 1, 0, 1> yyxy, ggrg, ttst;
        vProxy<vec4<T>, T, 1, 1, 0, 2> yyxz, ggrb, ttsp;
        vProxy<vec4<T>, T, 1, 1, 1, 0> yyyx, gggr, ttts;
        vProxy<vec4<T>, T, 1, 1, 1, 1> yyyy, gggg, tttt;
        vProxy<vec4<T>, T, 1, 1, 1, 2> yyyz, gggb, tttp;
        vProxy<vec4<T>, T, 1, 1, 2, 0> yyzx, ggbr, ttps;
        vProxy<vec4<T>, T, 1, 1, 2, 1> yyzy, ggbg, ttpt;
        vProxy<vec4<T>, T, 1, 1, 2, 2> yyzz, ggbb, ttpp;
        vProxy<vec4<T>, T, 1, 2, 0, 0> yzxx, gbrr, tpss;
        vProxy<vec4<T>, T, 1, 2, 0, 1> yzxy, gbrg, tpst;
        vProxy<vec4<T>, T, 1, 2, 0, 2> yzxz, gbrb, tpsp;
        vProxy<vec4<T>, T, 1, 2, 1, 0> yzyx, gbgr, tpts;
        vProxy<vec4<T>, T, 1, 2, 1, 1> yzyy, gbgg, tptt;
        vProxy<vec4<T>, T, 1, 2, 1, 2> yzyz, gbgb, tptp;
        vProxy<vec4<T>, T, 1, 2, 2, 0> yzzx, gbbr, tpps;
        vProxy<vec4<T>, T, 1, 2, 2, 1> yzzy, gbbg, tppt;
        vProxy<vec4<T>, T, 1, 2, 2, 2> yzzz, gbbb, tppp;
        vProxy<vec4<T>, T, 2, 0, 0, 0> zxxx, brrr, psss;
        vProxy<vec4<T>, T, 2, 0, 0, 1> zxxy, brrg, psst;
        vProxy<vec4<T>, T, 2, 0, 0, 2> zxxz, brrb, pssp;
        vProxy<vec4<T>, T, 2, 0, 1, 0> zxyx, brgr, psts;
        vProxy<vec4<T>, T, 2, 0, 1, 1> zxyy, brgg, pstt;
        vProxy<vec4<T>, T, 2, 0, 1, 2> zxyz, brgb, pstp;
        vProxy<vec4<T>, T, 2, 0, 2, 0> zxzx, brbr, psps;
        vProxy<vec4<T>, T, 2, 0, 2, 1> zxzy, brbg, pspt;
        vProxy<vec4<T>, T, 2, 0, 2, 2> zxzz, brbb, pspp;
        vProxy<vec4<T>, T, 2, 1, 0, 0> zyxx, bgrr, ptss;
        vProxy<vec4<T>, T, 2, 1, 0, 1> zyxy, bgrg, ptst;
        vProxy<vec4<T>, T, 2, 1, 0, 2> zyxz, bgrb, ptsp;
        vProxy<vec4<T>, T, 2, 1, 1, 0> zyyx, bggr, ptts;
        vProxy<vec4<T>, T, 2, 1, 1, 1> zyyy, bggg, pttt;
        vProxy<vec4<T>, T, 2, 1, 1, 2> zyyz, bggb, pttp;
        vProxy<vec4<T>, T, 2, 1, 2, 0> zyzx, bgbr, ptps;
        vProxy<vec4<T>, T, 2, 1, 2, 1> zyzy, bgbg, ptpt;
        vProxy<vec4<T>, T, 2, 1, 2, 2> zyzz, bgbb, ptpp;
        vProxy<vec4<T>, T, 2, 2, 0, 0> zzxx, bbrr, ppss;
        vProxy<vec4<T>, T, 2, 2, 0, 1> zzxy, bbrg, ppst;
        vProxy<vec4<T>, T, 2, 2, 0, 2> zzxz, bbrb, ppsp;
        vProxy<vec4<T>, T, 2, 2, 1, 0> zzyx, bbgr, ppts;
        vProxy<vec4<T>, T, 2, 2, 1, 1> zzyy, bbgg, pptt;
        vProxy<vec4<T>, T, 2, 2, 1, 2> zzyz, bbgb, pptp;
        vProxy<vec4<T>, T, 2, 2, 2, 0> zzzx, bbbr, ppps;
        vProxy<vec4<T>, T, 2, 2, 2, 1> zzzy, bbbg, pppt;
        vProxy<vec4<T>, T, 2, 2, 2, 2> zzzz, bbbb, pppp;

    };
    vec3() { }
    vec3(T _x, T _y, T _z)
    {
        ((T*)this)[0] = _x;
        ((T*)this)[1] = _y;
        ((T*)this)[2] = _z;
    }
    vec3(T _x) { vec3(_x, _x, _x); }
    
    template<template<typename> class vT, typename pT>
    vec3(vT<pT>& _v)
    {
        ((T*)this)[0] = ((const pT*)&_v)[0];
        ((T*)this)[1] = ((const pT*)&_v)[1];
        if (std::is_same<vT<pT>, vec3<pT>>::value ||
            std::is_same<vT<pT>, vec4<pT>>::value)
            ((T*)this)[2] = ((const pT*)&_v)[2];
        else
            ((T*)this)[2] = 0;
    }
    template<template<typename> class vT, typename pT>
    vec3(vT<pT>& _v, T _x)
    {
        ((T*)this)[0] = ((const pT*)&_v)[0];
        ((T*)this)[1] = ((const pT*)&_v)[1];
        ((T*)this)[2] = _x;
    }

    template<class vT, typename pT, int A, int B, int C, int D>
    vec3(vProxy<vT, pT, A, B, C, D>& _v)
    {
        ((T*)this)[0] = ((const pT*)&_v)[A];
        ((T*)this)[1] = ((const pT*)&_v)[B];
        if (sizeof(vT) / sizeof(pT) > 2)
            ((T*)this)[2] = ((const pT*)this)[C];
        else
            ((T*)this)[2] = 0;
    }
    template<class vT, typename pT, int A, int B, int C, int D>
    vec3(vProxy<vT, pT, A, B, C, D>& _v, T _x)
    {
        ((T*)this)[0] = ((const pT*)&_v)[A];
        ((T*)this)[1] = ((const pT*)&_v)[B];
        ((T*)this)[2] = _x;
    }
};

template<typename T>
class vec4 : public VectorOperators<vec4<T>, T>
{
public:
    union
    {
        struct { T x, y, z, w; };
        struct { T r, g, b, a; };
        struct { T s, t, p, q; };

        vProxy<vec2<T>, T, 0, 0, -1, -1> xx, rr, ss;
        vProxy<vec2<T>, T, 0, 1, -1, -1> xy, rg, st;
        vProxy<vec2<T>, T, 0, 2, -1, -1> xz, rb, sp;
        vProxy<vec2<T>, T, 0, 3, -1, -1> xw, ra, sq;
        vProxy<vec2<T>, T, 1, 0, -1, -1> yx, gr, ts;
        vProxy<vec2<T>, T, 1, 1, -1, -1> yy, gg, tt;
        vProxy<vec2<T>, T, 1, 2, -1, -1> yz, gb, tp;
        vProxy<vec2<T>, T, 1, 3, -1, -1> yw, ga, tq;
        vProxy<vec2<T>, T, 2, 0, -1, -1> zx, br, ps;
        vProxy<vec2<T>, T, 2, 1, -1, -1> zy, bg, pt;
        vProxy<vec2<T>, T, 2, 2, -1, -1> zz, bb, pp;
        vProxy<vec2<T>, T, 2, 3, -1, -1> zw, ba, pq;
        vProxy<vec2<T>, T, 3, 0, -1, -1> wx, ar, qs;
        vProxy<vec2<T>, T, 3, 1, -1, -1> wy, ag, qt;
        vProxy<vec2<T>, T, 3, 2, -1, -1> wz, ab, qp;
        vProxy<vec2<T>, T, 3, 3, -1, -1> ww, aa, qq;
        vProxy<vec3<T>, T, 0, 0, 0, -1> xxx, rrr, sss;
        vProxy<vec3<T>, T, 0, 0, 1, -1> xxy, rrg, sst;
        vProxy<vec3<T>, T, 0, 0, 2, -1> xxz, rrb, ssp;
        vProxy<vec3<T>, T, 0, 0, 3, -1> xxw, rra, ssq;
        vProxy<vec3<T>, T, 0, 1, 0, -1> xyx, rgr, sts;
        vProxy<vec3<T>, T, 0, 1, 1, -1> xyy, rgg, stt;
        vProxy<vec3<T>, T, 0, 1, 2, -1> xyz, rgb, stp;
        vProxy<vec3<T>, T, 0, 1, 3, -1> xyw, rga, stq;
        vProxy<vec3<T>, T, 0, 2, 0, -1> xzx, rbr, sps;
        vProxy<vec3<T>, T, 0, 2, 1, -1> xzy, rbg, spt;
        vProxy<vec3<T>, T, 0, 2, 2, -1> xzz, rbb, spp;
        vProxy<vec3<T>, T, 0, 2, 3, -1> xzw, rba, spq;
        vProxy<vec3<T>, T, 0, 3, 0, -1> xwx, rar, sqs;
        vProxy<vec3<T>, T, 0, 3, 1, -1> xwy, rag, sqt;
        vProxy<vec3<T>, T, 0, 3, 2, -1> xwz, rab, sqp;
        vProxy<vec3<T>, T, 0, 3, 3, -1> xww, raa, sqq;
        vProxy<vec3<T>, T, 1, 0, 0, -1> yxx, grr, tss;
        vProxy<vec3<T>, T, 1, 0, 1, -1> yxy, grg, tst;
        vProxy<vec3<T>, T, 1, 0, 2, -1> yxz, grb, tsp;
        vProxy<vec3<T>, T, 1, 0, 3, -1> yxw, gra, tsq;
        vProxy<vec3<T>, T, 1, 1, 0, -1> yyx, ggr, tts;
        vProxy<vec3<T>, T, 1, 1, 1, -1> yyy, ggg, ttt;
        vProxy<vec3<T>, T, 1, 1, 2, -1> yyz, ggb, ttp;
        vProxy<vec3<T>, T, 1, 1, 3, -1> yyw, gga, ttq;
        vProxy<vec3<T>, T, 1, 2, 0, -1> yzx, gbr, tps;
        vProxy<vec3<T>, T, 1, 2, 1, -1> yzy, gbg, tpt;
        vProxy<vec3<T>, T, 1, 2, 2, -1> yzz, gbb, tpp;
        vProxy<vec3<T>, T, 1, 2, 3, -1> yzw, gba, tpq;
        vProxy<vec3<T>, T, 1, 3, 0, -1> ywx, gar, tqs;
        vProxy<vec3<T>, T, 1, 3, 1, -1> ywy, gag, tqt;
        vProxy<vec3<T>, T, 1, 3, 2, -1> ywz, gab, tqp;
        vProxy<vec3<T>, T, 1, 3, 3, -1> yww, gaa, tqq;
        vProxy<vec3<T>, T, 2, 0, 0, -1> zxx, brr, pss;
        vProxy<vec3<T>, T, 2, 0, 1, -1> zxy, brg, pst;
        vProxy<vec3<T>, T, 2, 0, 2, -1> zxz, brb, psp;
        vProxy<vec3<T>, T, 2, 0, 3, -1> zxw, bra, psq;
        vProxy<vec3<T>, T, 2, 1, 0, -1> zyx, bgr, pts;
        vProxy<vec3<T>, T, 2, 1, 1, -1> zyy, bgg, ptt;
        vProxy<vec3<T>, T, 2, 1, 2, -1> zyz, bgb, ptp;
        vProxy<vec3<T>, T, 2, 1, 3, -1> zyw, bga, ptq;
        vProxy<vec3<T>, T, 2, 2, 0, -1> zzx, bbr, pps;
        vProxy<vec3<T>, T, 2, 2, 1, -1> zzy, bbg, ppt;
        vProxy<vec3<T>, T, 2, 2, 2, -1> zzz, bbb, ppp;
        vProxy<vec3<T>, T, 2, 2, 3, -1> zzw, bba, ppq;
        vProxy<vec3<T>, T, 2, 3, 0, -1> zwx, bar, pqs;
        vProxy<vec3<T>, T, 2, 3, 1, -1> zwy, bag, pqt;
        vProxy<vec3<T>, T, 2, 3, 2, -1> zwz, bab, pqp;
        vProxy<vec3<T>, T, 2, 3, 3, -1> zww, baa, pqq;
        vProxy<vec3<T>, T, 3, 0, 0, -1> wxx, arr, qss;
        vProxy<vec3<T>, T, 3, 0, 1, -1> wxy, arg, qst;
        vProxy<vec3<T>, T, 3, 0, 2, -1> wxz, arb, qsp;
        vProxy<vec3<T>, T, 3, 0, 3, -1> wxw, ara, qsq;
        vProxy<vec3<T>, T, 3, 1, 0, -1> wyx, agr, qts;
        vProxy<vec3<T>, T, 3, 1, 1, -1> wyy, agg, qtt;
        vProxy<vec3<T>, T, 3, 1, 2, -1> wyz, agb, qtp;
        vProxy<vec3<T>, T, 3, 1, 3, -1> wyw, aga, qtq;
        vProxy<vec3<T>, T, 3, 2, 0, -1> wzx, abr, qps;
        vProxy<vec3<T>, T, 3, 2, 1, -1> wzy, abg, qpt;
        vProxy<vec3<T>, T, 3, 2, 2, -1> wzz, abb, qpp;
        vProxy<vec3<T>, T, 3, 2, 3, -1> wzw, aba, qpq;
        vProxy<vec3<T>, T, 3, 3, 0, -1> wwx, aar, qqs;
        vProxy<vec3<T>, T, 3, 3, 1, -1> wwy, aag, qqt;
        vProxy<vec3<T>, T, 3, 3, 2, -1> wwz, aab, qqp;
        vProxy<vec3<T>, T, 3, 3, 3, -1> www, aaa, qqq;
        vProxy<vec4<T>, T, 0, 0, 0, 0> xxxx, rrrr, ssss;
        vProxy<vec4<T>, T, 0, 0, 0, 1> xxxy, rrrg, ssst;
        vProxy<vec4<T>, T, 0, 0, 0, 2> xxxz, rrrb, sssp;
        vProxy<vec4<T>, T, 0, 0, 0, 3> xxxw, rrra, sssq;
        vProxy<vec4<T>, T, 0, 0, 1, 0> xxyx, rrgr, ssts;
        vProxy<vec4<T>, T, 0, 0, 1, 1> xxyy, rrgg, sstt;
        vProxy<vec4<T>, T, 0, 0, 1, 2> xxyz, rrgb, sstp;
        vProxy<vec4<T>, T, 0, 0, 1, 3> xxyw, rrga, sstq;
        vProxy<vec4<T>, T, 0, 0, 2, 0> xxzx, rrbr, ssps;
        vProxy<vec4<T>, T, 0, 0, 2, 1> xxzy, rrbg, sspt;
        vProxy<vec4<T>, T, 0, 0, 2, 2> xxzz, rrbb, sspp;
        vProxy<vec4<T>, T, 0, 0, 2, 3> xxzw, rrba, sspq;
        vProxy<vec4<T>, T, 0, 0, 3, 0> xxwx, rrar, ssqs;
        vProxy<vec4<T>, T, 0, 0, 3, 1> xxwy, rrag, ssqt;
        vProxy<vec4<T>, T, 0, 0, 3, 2> xxwz, rrab, ssqp;
        vProxy<vec4<T>, T, 0, 0, 3, 3> xxww, rraa, ssqq;
        vProxy<vec4<T>, T, 0, 1, 0, 0> xyxx, rgrr, stss;
        vProxy<vec4<T>, T, 0, 1, 0, 1> xyxy, rgrg, stst;
        vProxy<vec4<T>, T, 0, 1, 0, 2> xyxz, rgrb, stsp;
        vProxy<vec4<T>, T, 0, 1, 0, 3> xyxw, rgra, stsq;
        vProxy<vec4<T>, T, 0, 1, 1, 0> xyyx, rggr, stts;
        vProxy<vec4<T>, T, 0, 1, 1, 1> xyyy, rggg, sttt;
        vProxy<vec4<T>, T, 0, 1, 1, 2> xyyz, rggb, sttp;
        vProxy<vec4<T>, T, 0, 1, 1, 3> xyyw, rgga, sttq;
        vProxy<vec4<T>, T, 0, 1, 2, 0> xyzx, rgbr, stps;
        vProxy<vec4<T>, T, 0, 1, 2, 1> xyzy, rgbg, stpt;
        vProxy<vec4<T>, T, 0, 1, 2, 2> xyzz, rgbb, stpp;
        vProxy<vec4<T>, T, 0, 1, 2, 3> xyzw, rgba, stpq;
        vProxy<vec4<T>, T, 0, 1, 3, 0> xywx, rgar, stqs;
        vProxy<vec4<T>, T, 0, 1, 3, 1> xywy, rgag, stqt;
        vProxy<vec4<T>, T, 0, 1, 3, 2> xywz, rgab, stqp;
        vProxy<vec4<T>, T, 0, 1, 3, 3> xyww, rgaa, stqq;
        vProxy<vec4<T>, T, 0, 2, 0, 0> xzxx, rbrr, spss;
        vProxy<vec4<T>, T, 0, 2, 0, 1> xzxy, rbrg, spst;
        vProxy<vec4<T>, T, 0, 2, 0, 2> xzxz, rbrb, spsp;
        vProxy<vec4<T>, T, 0, 2, 0, 3> xzxw, rbra, spsq;
        vProxy<vec4<T>, T, 0, 2, 1, 0> xzyx, rbgr, spts;
        vProxy<vec4<T>, T, 0, 2, 1, 1> xzyy, rbgg, sptt;
        vProxy<vec4<T>, T, 0, 2, 1, 2> xzyz, rbgb, sptp;
        vProxy<vec4<T>, T, 0, 2, 1, 3> xzyw, rbga, sptq;
        vProxy<vec4<T>, T, 0, 2, 2, 0> xzzx, rbbr, spps;
        vProxy<vec4<T>, T, 0, 2, 2, 1> xzzy, rbbg, sppt;
        vProxy<vec4<T>, T, 0, 2, 2, 2> xzzz, rbbb, sppp;
        vProxy<vec4<T>, T, 0, 2, 2, 3> xzzw, rbba, sppq;
        vProxy<vec4<T>, T, 0, 2, 3, 0> xzwx, rbar, spqs;
        vProxy<vec4<T>, T, 0, 2, 3, 1> xzwy, rbag, spqt;
        vProxy<vec4<T>, T, 0, 2, 3, 2> xzwz, rbab, spqp;
        vProxy<vec4<T>, T, 0, 2, 3, 3> xzww, rbaa, spqq;
        vProxy<vec4<T>, T, 0, 3, 0, 0> xwxx, rarr, sqss;
        vProxy<vec4<T>, T, 0, 3, 0, 1> xwxy, rarg, sqst;
        vProxy<vec4<T>, T, 0, 3, 0, 2> xwxz, rarb, sqsp;
        vProxy<vec4<T>, T, 0, 3, 0, 3> xwxw, rara, sqsq;
        vProxy<vec4<T>, T, 0, 3, 1, 0> xwyx, ragr, sqts;
        vProxy<vec4<T>, T, 0, 3, 1, 1> xwyy, ragg, sqtt;
        vProxy<vec4<T>, T, 0, 3, 1, 2> xwyz, ragb, sqtp;
        vProxy<vec4<T>, T, 0, 3, 1, 3> xwyw, raga, sqtq;
        vProxy<vec4<T>, T, 0, 3, 2, 0> xwzx, rabr, sqps;
        vProxy<vec4<T>, T, 0, 3, 2, 1> xwzy, rabg, sqpt;
        vProxy<vec4<T>, T, 0, 3, 2, 2> xwzz, rabb, sqpp;
        vProxy<vec4<T>, T, 0, 3, 2, 3> xwzw, raba, sqpq;
        vProxy<vec4<T>, T, 0, 3, 3, 0> xwwx, raar, sqqs;
        vProxy<vec4<T>, T, 0, 3, 3, 1> xwwy, raag, sqqt;
        vProxy<vec4<T>, T, 0, 3, 3, 2> xwwz, raab, sqqp;
        vProxy<vec4<T>, T, 0, 3, 3, 3> xwww, raaa, sqqq;
        vProxy<vec4<T>, T, 1, 0, 0, 0> yxxx, grrr, tsss;
        vProxy<vec4<T>, T, 1, 0, 0, 1> yxxy, grrg, tsst;
        vProxy<vec4<T>, T, 1, 0, 0, 2> yxxz, grrb, tssp;
        vProxy<vec4<T>, T, 1, 0, 0, 3> yxxw, grra, tssq;
        vProxy<vec4<T>, T, 1, 0, 1, 0> yxyx, grgr, tsts;
        vProxy<vec4<T>, T, 1, 0, 1, 1> yxyy, grgg, tstt;
        vProxy<vec4<T>, T, 1, 0, 1, 2> yxyz, grgb, tstp;
        vProxy<vec4<T>, T, 1, 0, 1, 3> yxyw, grga, tstq;
        vProxy<vec4<T>, T, 1, 0, 2, 0> yxzx, grbr, tsps;
        vProxy<vec4<T>, T, 1, 0, 2, 1> yxzy, grbg, tspt;
        vProxy<vec4<T>, T, 1, 0, 2, 2> yxzz, grbb, tspp;
        vProxy<vec4<T>, T, 1, 0, 2, 3> yxzw, grba, tspq;
        vProxy<vec4<T>, T, 1, 0, 3, 0> yxwx, grar, tsqs;
        vProxy<vec4<T>, T, 1, 0, 3, 1> yxwy, grag, tsqt;
        vProxy<vec4<T>, T, 1, 0, 3, 2> yxwz, grab, tsqp;
        vProxy<vec4<T>, T, 1, 0, 3, 3> yxww, graa, tsqq;
        vProxy<vec4<T>, T, 1, 1, 0, 0> yyxx, ggrr, ttss;
        vProxy<vec4<T>, T, 1, 1, 0, 1> yyxy, ggrg, ttst;
        vProxy<vec4<T>, T, 1, 1, 0, 2> yyxz, ggrb, ttsp;
        vProxy<vec4<T>, T, 1, 1, 0, 3> yyxw, ggra, ttsq;
        vProxy<vec4<T>, T, 1, 1, 1, 0> yyyx, gggr, ttts;
        vProxy<vec4<T>, T, 1, 1, 1, 1> yyyy, gggg, tttt;
        vProxy<vec4<T>, T, 1, 1, 1, 2> yyyz, gggb, tttp;
        vProxy<vec4<T>, T, 1, 1, 1, 3> yyyw, ggga, tttq;
        vProxy<vec4<T>, T, 1, 1, 2, 0> yyzx, ggbr, ttps;
        vProxy<vec4<T>, T, 1, 1, 2, 1> yyzy, ggbg, ttpt;
        vProxy<vec4<T>, T, 1, 1, 2, 2> yyzz, ggbb, ttpp;
        vProxy<vec4<T>, T, 1, 1, 2, 3> yyzw, ggba, ttpq;
        vProxy<vec4<T>, T, 1, 1, 3, 0> yywx, ggar, ttqs;
        vProxy<vec4<T>, T, 1, 1, 3, 1> yywy, ggag, ttqt;
        vProxy<vec4<T>, T, 1, 1, 3, 2> yywz, ggab, ttqp;
        vProxy<vec4<T>, T, 1, 1, 3, 3> yyww, ggaa, ttqq;
        vProxy<vec4<T>, T, 1, 2, 0, 0> yzxx, gbrr, tpss;
        vProxy<vec4<T>, T, 1, 2, 0, 1> yzxy, gbrg, tpst;
        vProxy<vec4<T>, T, 1, 2, 0, 2> yzxz, gbrb, tpsp;
        vProxy<vec4<T>, T, 1, 2, 0, 3> yzxw, gbra, tpsq;
        vProxy<vec4<T>, T, 1, 2, 1, 0> yzyx, gbgr, tpts;
        vProxy<vec4<T>, T, 1, 2, 1, 1> yzyy, gbgg, tptt;
        vProxy<vec4<T>, T, 1, 2, 1, 2> yzyz, gbgb, tptp;
        vProxy<vec4<T>, T, 1, 2, 1, 3> yzyw, gbga, tptq;
        vProxy<vec4<T>, T, 1, 2, 2, 0> yzzx, gbbr, tpps;
        vProxy<vec4<T>, T, 1, 2, 2, 1> yzzy, gbbg, tppt;
        vProxy<vec4<T>, T, 1, 2, 2, 2> yzzz, gbbb, tppp;
        vProxy<vec4<T>, T, 1, 2, 2, 3> yzzw, gbba, tppq;
        vProxy<vec4<T>, T, 1, 2, 3, 0> yzwx, gbar, tpqs;
        vProxy<vec4<T>, T, 1, 2, 3, 1> yzwy, gbag, tpqt;
        vProxy<vec4<T>, T, 1, 2, 3, 2> yzwz, gbab, tpqp;
        vProxy<vec4<T>, T, 1, 2, 3, 3> yzww, gbaa, tpqq;
        vProxy<vec4<T>, T, 1, 3, 0, 0> ywxx, garr, tqss;
        vProxy<vec4<T>, T, 1, 3, 0, 1> ywxy, garg, tqst;
        vProxy<vec4<T>, T, 1, 3, 0, 2> ywxz, garb, tqsp;
        vProxy<vec4<T>, T, 1, 3, 0, 3> ywxw, gara, tqsq;
        vProxy<vec4<T>, T, 1, 3, 1, 0> ywyx, gagr, tqts;
        vProxy<vec4<T>, T, 1, 3, 1, 1> ywyy, gagg, tqtt;
        vProxy<vec4<T>, T, 1, 3, 1, 2> ywyz, gagb, tqtp;
        vProxy<vec4<T>, T, 1, 3, 1, 3> ywyw, gaga, tqtq;
        vProxy<vec4<T>, T, 1, 3, 2, 0> ywzx, gabr, tqps;
        vProxy<vec4<T>, T, 1, 3, 2, 1> ywzy, gabg, tqpt;
        vProxy<vec4<T>, T, 1, 3, 2, 2> ywzz, gabb, tqpp;
        vProxy<vec4<T>, T, 1, 3, 2, 3> ywzw, gaba, tqpq;
        vProxy<vec4<T>, T, 1, 3, 3, 0> ywwx, gaar, tqqs;
        vProxy<vec4<T>, T, 1, 3, 3, 1> ywwy, gaag, tqqt;
        vProxy<vec4<T>, T, 1, 3, 3, 2> ywwz, gaab, tqqp;
        vProxy<vec4<T>, T, 1, 3, 3, 3> ywww, gaaa, tqqq;
        vProxy<vec4<T>, T, 2, 0, 0, 0> zxxx, brrr, psss;
        vProxy<vec4<T>, T, 2, 0, 0, 1> zxxy, brrg, psst;
        vProxy<vec4<T>, T, 2, 0, 0, 2> zxxz, brrb, pssp;
        vProxy<vec4<T>, T, 2, 0, 0, 3> zxxw, brra, pssq;
        vProxy<vec4<T>, T, 2, 0, 1, 0> zxyx, brgr, psts;
        vProxy<vec4<T>, T, 2, 0, 1, 1> zxyy, brgg, pstt;
        vProxy<vec4<T>, T, 2, 0, 1, 2> zxyz, brgb, pstp;
        vProxy<vec4<T>, T, 2, 0, 1, 3> zxyw, brga, pstq;
        vProxy<vec4<T>, T, 2, 0, 2, 0> zxzx, brbr, psps;
        vProxy<vec4<T>, T, 2, 0, 2, 1> zxzy, brbg, pspt;
        vProxy<vec4<T>, T, 2, 0, 2, 2> zxzz, brbb, pspp;
        vProxy<vec4<T>, T, 2, 0, 2, 3> zxzw, brba, pspq;
        vProxy<vec4<T>, T, 2, 0, 3, 0> zxwx, brar, psqs;
        vProxy<vec4<T>, T, 2, 0, 3, 1> zxwy, brag, psqt;
        vProxy<vec4<T>, T, 2, 0, 3, 2> zxwz, brab, psqp;
        vProxy<vec4<T>, T, 2, 0, 3, 3> zxww, braa, psqq;
        vProxy<vec4<T>, T, 2, 1, 0, 0> zyxx, bgrr, ptss;
        vProxy<vec4<T>, T, 2, 1, 0, 1> zyxy, bgrg, ptst;
        vProxy<vec4<T>, T, 2, 1, 0, 2> zyxz, bgrb, ptsp;
        vProxy<vec4<T>, T, 2, 1, 0, 3> zyxw, bgra, ptsq;
        vProxy<vec4<T>, T, 2, 1, 1, 0> zyyx, bggr, ptts;
        vProxy<vec4<T>, T, 2, 1, 1, 1> zyyy, bggg, pttt;
        vProxy<vec4<T>, T, 2, 1, 1, 2> zyyz, bggb, pttp;
        vProxy<vec4<T>, T, 2, 1, 1, 3> zyyw, bgga, pttq;
        vProxy<vec4<T>, T, 2, 1, 2, 0> zyzx, bgbr, ptps;
        vProxy<vec4<T>, T, 2, 1, 2, 1> zyzy, bgbg, ptpt;
        vProxy<vec4<T>, T, 2, 1, 2, 2> zyzz, bgbb, ptpp;
        vProxy<vec4<T>, T, 2, 1, 2, 3> zyzw, bgba, ptpq;
        vProxy<vec4<T>, T, 2, 1, 3, 0> zywx, bgar, ptqs;
        vProxy<vec4<T>, T, 2, 1, 3, 1> zywy, bgag, ptqt;
        vProxy<vec4<T>, T, 2, 1, 3, 2> zywz, bgab, ptqp;
        vProxy<vec4<T>, T, 2, 1, 3, 3> zyww, bgaa, ptqq;
        vProxy<vec4<T>, T, 2, 2, 0, 0> zzxx, bbrr, ppss;
        vProxy<vec4<T>, T, 2, 2, 0, 1> zzxy, bbrg, ppst;
        vProxy<vec4<T>, T, 2, 2, 0, 2> zzxz, bbrb, ppsp;
        vProxy<vec4<T>, T, 2, 2, 0, 3> zzxw, bbra, ppsq;
        vProxy<vec4<T>, T, 2, 2, 1, 0> zzyx, bbgr, ppts;
        vProxy<vec4<T>, T, 2, 2, 1, 1> zzyy, bbgg, pptt;
        vProxy<vec4<T>, T, 2, 2, 1, 2> zzyz, bbgb, pptp;
        vProxy<vec4<T>, T, 2, 2, 1, 3> zzyw, bbga, pptq;
        vProxy<vec4<T>, T, 2, 2, 2, 0> zzzx, bbbr, ppps;
        vProxy<vec4<T>, T, 2, 2, 2, 1> zzzy, bbbg, pppt;
        vProxy<vec4<T>, T, 2, 2, 2, 2> zzzz, bbbb, pppp;
        vProxy<vec4<T>, T, 2, 2, 2, 3> zzzw, bbba, pppq;
        vProxy<vec4<T>, T, 2, 2, 3, 0> zzwx, bbar, ppqs;
        vProxy<vec4<T>, T, 2, 2, 3, 1> zzwy, bbag, ppqt;
        vProxy<vec4<T>, T, 2, 2, 3, 2> zzwz, bbab, ppqp;
        vProxy<vec4<T>, T, 2, 2, 3, 3> zzww, bbaa, ppqq;
        vProxy<vec4<T>, T, 2, 3, 0, 0> zwxx, barr, pqss;
        vProxy<vec4<T>, T, 2, 3, 0, 1> zwxy, barg, pqst;
        vProxy<vec4<T>, T, 2, 3, 0, 2> zwxz, barb, pqsp;
        vProxy<vec4<T>, T, 2, 3, 0, 3> zwxw, bara, pqsq;
        vProxy<vec4<T>, T, 2, 3, 1, 0> zwyx, bagr, pqts;
        vProxy<vec4<T>, T, 2, 3, 1, 1> zwyy, bagg, pqtt;
        vProxy<vec4<T>, T, 2, 3, 1, 2> zwyz, bagb, pqtp;
        vProxy<vec4<T>, T, 2, 3, 1, 3> zwyw, baga, pqtq;
        vProxy<vec4<T>, T, 2, 3, 2, 0> zwzx, babr, pqps;
        vProxy<vec4<T>, T, 2, 3, 2, 1> zwzy, babg, pqpt;
        vProxy<vec4<T>, T, 2, 3, 2, 2> zwzz, babb, pqpp;
        vProxy<vec4<T>, T, 2, 3, 2, 3> zwzw, baba, pqpq;
        vProxy<vec4<T>, T, 2, 3, 3, 0> zwwx, baar, pqqs;
        vProxy<vec4<T>, T, 2, 3, 3, 1> zwwy, baag, pqqt;
        vProxy<vec4<T>, T, 2, 3, 3, 2> zwwz, baab, pqqp;
        vProxy<vec4<T>, T, 2, 3, 3, 3> zwww, baaa, pqqq;
        vProxy<vec4<T>, T, 3, 0, 0, 0> wxxx, arrr, qsss;
        vProxy<vec4<T>, T, 3, 0, 0, 1> wxxy, arrg, qsst;
        vProxy<vec4<T>, T, 3, 0, 0, 2> wxxz, arrb, qssp;
        vProxy<vec4<T>, T, 3, 0, 0, 3> wxxw, arra, qssq;
        vProxy<vec4<T>, T, 3, 0, 1, 0> wxyx, argr, qsts;
        vProxy<vec4<T>, T, 3, 0, 1, 1> wxyy, argg, qstt;
        vProxy<vec4<T>, T, 3, 0, 1, 2> wxyz, argb, qstp;
        vProxy<vec4<T>, T, 3, 0, 1, 3> wxyw, arga, qstq;
        vProxy<vec4<T>, T, 3, 0, 2, 0> wxzx, arbr, qsps;
        vProxy<vec4<T>, T, 3, 0, 2, 1> wxzy, arbg, qspt;
        vProxy<vec4<T>, T, 3, 0, 2, 2> wxzz, arbb, qspp;
        vProxy<vec4<T>, T, 3, 0, 2, 3> wxzw, arba, qspq;
        vProxy<vec4<T>, T, 3, 0, 3, 0> wxwx, arar, qsqs;
        vProxy<vec4<T>, T, 3, 0, 3, 1> wxwy, arag, qsqt;
        vProxy<vec4<T>, T, 3, 0, 3, 2> wxwz, arab, qsqp;
        vProxy<vec4<T>, T, 3, 0, 3, 3> wxww, araa, qsqq;
        vProxy<vec4<T>, T, 3, 1, 0, 0> wyxx, agrr, qtss;
        vProxy<vec4<T>, T, 3, 1, 0, 1> wyxy, agrg, qtst;
        vProxy<vec4<T>, T, 3, 1, 0, 2> wyxz, agrb, qtsp;
        vProxy<vec4<T>, T, 3, 1, 0, 3> wyxw, agra, qtsq;
        vProxy<vec4<T>, T, 3, 1, 1, 0> wyyx, aggr, qtts;
        vProxy<vec4<T>, T, 3, 1, 1, 1> wyyy, aggg, qttt;
        vProxy<vec4<T>, T, 3, 1, 1, 2> wyyz, aggb, qttp;
        vProxy<vec4<T>, T, 3, 1, 1, 3> wyyw, agga, qttq;
        vProxy<vec4<T>, T, 3, 1, 2, 0> wyzx, agbr, qtps;
        vProxy<vec4<T>, T, 3, 1, 2, 1> wyzy, agbg, qtpt;
        vProxy<vec4<T>, T, 3, 1, 2, 2> wyzz, agbb, qtpp;
        vProxy<vec4<T>, T, 3, 1, 2, 3> wyzw, agba, qtpq;
        vProxy<vec4<T>, T, 3, 1, 3, 0> wywx, agar, qtqs;
        vProxy<vec4<T>, T, 3, 1, 3, 1> wywy, agag, qtqt;
        vProxy<vec4<T>, T, 3, 1, 3, 2> wywz, agab, qtqp;
        vProxy<vec4<T>, T, 3, 1, 3, 3> wyww, agaa, qtqq;
        vProxy<vec4<T>, T, 3, 2, 0, 0> wzxx, abrr, qpss;
        vProxy<vec4<T>, T, 3, 2, 0, 1> wzxy, abrg, qpst;
        vProxy<vec4<T>, T, 3, 2, 0, 2> wzxz, abrb, qpsp;
        vProxy<vec4<T>, T, 3, 2, 0, 3> wzxw, abra, qpsq;
        vProxy<vec4<T>, T, 3, 2, 1, 0> wzyx, abgr, qpts;
        vProxy<vec4<T>, T, 3, 2, 1, 1> wzyy, abgg, qptt;
        vProxy<vec4<T>, T, 3, 2, 1, 2> wzyz, abgb, qptp;
        vProxy<vec4<T>, T, 3, 2, 1, 3> wzyw, abga, qptq;
        vProxy<vec4<T>, T, 3, 2, 2, 0> wzzx, abbr, qpps;
        vProxy<vec4<T>, T, 3, 2, 2, 1> wzzy, abbg, qppt;
        vProxy<vec4<T>, T, 3, 2, 2, 2> wzzz, abbb, qppp;
        vProxy<vec4<T>, T, 3, 2, 2, 3> wzzw, abba, qppq;
        vProxy<vec4<T>, T, 3, 2, 3, 0> wzwx, abar, qpqs;
        vProxy<vec4<T>, T, 3, 2, 3, 1> wzwy, abag, qpqt;
        vProxy<vec4<T>, T, 3, 2, 3, 2> wzwz, abab, qpqp;
        vProxy<vec4<T>, T, 3, 2, 3, 3> wzww, abaa, qpqq;
        vProxy<vec4<T>, T, 3, 3, 0, 0> wwxx, aarr, qqss;
        vProxy<vec4<T>, T, 3, 3, 0, 1> wwxy, aarg, qqst;
        vProxy<vec4<T>, T, 3, 3, 0, 2> wwxz, aarb, qqsp;
        vProxy<vec4<T>, T, 3, 3, 0, 3> wwxw, aara, qqsq;
        vProxy<vec4<T>, T, 3, 3, 1, 0> wwyx, aagr, qqts;
        vProxy<vec4<T>, T, 3, 3, 1, 1> wwyy, aagg, qqtt;
        vProxy<vec4<T>, T, 3, 3, 1, 2> wwyz, aagb, qqtp;
        vProxy<vec4<T>, T, 3, 3, 1, 3> wwyw, aaga, qqtq;
        vProxy<vec4<T>, T, 3, 3, 2, 0> wwzx, aabr, qqps;
        vProxy<vec4<T>, T, 3, 3, 2, 1> wwzy, aabg, qqpt;
        vProxy<vec4<T>, T, 3, 3, 2, 2> wwzz, aabb, qqpp;
        vProxy<vec4<T>, T, 3, 3, 2, 3> wwzw, aaba, qqpq;
        vProxy<vec4<T>, T, 3, 3, 3, 0> wwwx, aaar, qqqs;
        vProxy<vec4<T>, T, 3, 3, 3, 1> wwwy, aaag, qqqt;
        vProxy<vec4<T>, T, 3, 3, 3, 2> wwwz, aaab, qqqp;
        vProxy<vec4<T>, T, 3, 3, 3, 3> wwww, aaaa, qqqq;

    };
    vec4() { }
    vec4(T _x, T _y, T _z, T _w)
    {
        ((T*)this)[0] = _x;
        ((T*)this)[1] = _y;
        ((T*)this)[2] = _z;
        ((T*)this)[3] = _w;
    }
    vec4(T _x) { vec4(_x, _x, _x, _x); }

    template<template<class> class vT, typename pT>
    vec4(vT<pT>& _v)
    {
        ((T*)this)[0] = ((const pT*)&_v)[0];
        ((T*)this)[1] = ((const pT*)&_v)[1];
        if (std::is_same<vT<pT>, vec3<pT>>::value)
        {
            ((T*)this)[2] = ((const pT*)&_v)[2];
            ((T*)this)[3] = 0;
        }
        else if (std::is_same<vT<pT>, vec4<pT>>::value)
        {
            ((T*)this)[2] = ((const pT*)&_v)[2];
            ((T*)this)[3] = ((const pT*)&_v)[3];
        }
    }
    template<template<class> class vT, typename pT>
    vec4(vT<pT>& _v, T _x)
    {
        ((T*)this)[0] = ((const pT*)&_v)[0];
        ((T*)this)[1] = ((const pT*)&_v)[1];
        if (std::is_same<vT<pT>, vec3<pT>>::value ||
            std::is_same<vT<pT>, vec4<pT>>::value)
            ((T*)this)[2] = ((const pT*)&_v)[2];
        else
            ((T*)this)[2] = 0;
        ((T*)this)[3] = _x;
    }
    template<template<class> class vT, typename pT>
    vec4(vT<pT>& _v, T _x, T _y)
    {
        ((T*)this)[0] = ((const pT*)&_v)[0];
        ((T*)this)[1] = ((const pT*)&_v)[1];
        ((T*)this)[2] = _x;
        ((T*)this)[3] = _y;
    }
};

#endif