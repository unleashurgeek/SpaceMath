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
        vProxy<vec2<T>, T, 0, 3, -1, -1> xa, ra, sq;
        vProxy<vec2<T>, T, 1, 0, -1, -1> yx, gr, ts;
        vProxy<vec2<T>, T, 1, 1, -1, -1> yy, gg, tt;
        vProxy<vec2<T>, T, 1, 2, -1, -1> yz, gb, tp;
        vProxy<vec2<T>, T, 1, 3, -1, -1> ya, ga, tq;
        vProxy<vec2<T>, T, 2, 0, -1, -1> zx, br, ps;
        vProxy<vec2<T>, T, 2, 1, -1, -1> zy, bg, pt;
        vProxy<vec2<T>, T, 2, 2, -1, -1> zz, bb, pp;
        vProxy<vec2<T>, T, 2, 3, -1, -1> za, ba, pq;
        vProxy<vec2<T>, T, 3, 0, -1, -1> ax, ar, qs;
        vProxy<vec2<T>, T, 3, 1, -1, -1> ay, ag, qt;
        vProxy<vec2<T>, T, 3, 2, -1, -1> az, ab, qp;
        vProxy<vec2<T>, T, 3, 3, -1, -1> aa, qq;
        vProxy<vec3<T>, T, 0, 0, 0, -1> xxx, rrr, sss;
        vProxy<vec3<T>, T, 0, 0, 1, -1> xxy, rrg, sst;
        vProxy<vec3<T>, T, 0, 0, 2, -1> xxz, rrb, ssp;
        vProxy<vec3<T>, T, 0, 0, 3, -1> xxa, rra, ssq;
        vProxy<vec3<T>, T, 0, 1, 0, -1> xyx, rgr, sts;
        vProxy<vec3<T>, T, 0, 1, 1, -1> xyy, rgg, stt;
        vProxy<vec3<T>, T, 0, 1, 2, -1> xyz, rgb, stp;
        vProxy<vec3<T>, T, 0, 1, 3, -1> xya, rga, stq;
        vProxy<vec3<T>, T, 0, 2, 0, -1> xzx, rbr, sps;
        vProxy<vec3<T>, T, 0, 2, 1, -1> xzy, rbg, spt;
        vProxy<vec3<T>, T, 0, 2, 2, -1> xzz, rbb, spp;
        vProxy<vec3<T>, T, 0, 2, 3, -1> xza, rba, spq;
        vProxy<vec3<T>, T, 0, 3, 0, -1> xax, rar, sqs;
        vProxy<vec3<T>, T, 0, 3, 1, -1> xay, rag, sqt;
        vProxy<vec3<T>, T, 0, 3, 2, -1> xaz, rab, sqp;
        vProxy<vec3<T>, T, 0, 3, 3, -1> xaa, raa, sqq;
        vProxy<vec3<T>, T, 1, 0, 0, -1> yxx, grr, tss;
        vProxy<vec3<T>, T, 1, 0, 1, -1> yxy, grg, tst;
        vProxy<vec3<T>, T, 1, 0, 2, -1> yxz, grb, tsp;
        vProxy<vec3<T>, T, 1, 0, 3, -1> yxa, gra, tsq;
        vProxy<vec3<T>, T, 1, 1, 0, -1> yyx, ggr, tts;
        vProxy<vec3<T>, T, 1, 1, 1, -1> yyy, ggg, ttt;
        vProxy<vec3<T>, T, 1, 1, 2, -1> yyz, ggb, ttp;
        vProxy<vec3<T>, T, 1, 1, 3, -1> yya, gga, ttq;
        vProxy<vec3<T>, T, 1, 2, 0, -1> yzx, gbr, tps;
        vProxy<vec3<T>, T, 1, 2, 1, -1> yzy, gbg, tpt;
        vProxy<vec3<T>, T, 1, 2, 2, -1> yzz, gbb, tpp;
        vProxy<vec3<T>, T, 1, 2, 3, -1> yza, gba, tpq;
        vProxy<vec3<T>, T, 1, 3, 0, -1> yax, gar, tqs;
        vProxy<vec3<T>, T, 1, 3, 1, -1> yay, gag, tqt;
        vProxy<vec3<T>, T, 1, 3, 2, -1> yaz, gab, tqp;
        vProxy<vec3<T>, T, 1, 3, 3, -1> yaa, gaa, tqq;
        vProxy<vec3<T>, T, 2, 0, 0, -1> zxx, brr, pss;
        vProxy<vec3<T>, T, 2, 0, 1, -1> zxy, brg, pst;
        vProxy<vec3<T>, T, 2, 0, 2, -1> zxz, brb, psp;
        vProxy<vec3<T>, T, 2, 0, 3, -1> zxa, bra, psq;
        vProxy<vec3<T>, T, 2, 1, 0, -1> zyx, bgr, pts;
        vProxy<vec3<T>, T, 2, 1, 1, -1> zyy, bgg, ptt;
        vProxy<vec3<T>, T, 2, 1, 2, -1> zyz, bgb, ptp;
        vProxy<vec3<T>, T, 2, 1, 3, -1> zya, bga, ptq;
        vProxy<vec3<T>, T, 2, 2, 0, -1> zzx, bbr, pps;
        vProxy<vec3<T>, T, 2, 2, 1, -1> zzy, bbg, ppt;
        vProxy<vec3<T>, T, 2, 2, 2, -1> zzz, bbb, ppp;
        vProxy<vec3<T>, T, 2, 2, 3, -1> zza, bba, ppq;
        vProxy<vec3<T>, T, 2, 3, 0, -1> zax, bar, pqs;
        vProxy<vec3<T>, T, 2, 3, 1, -1> zay, bag, pqt;
        vProxy<vec3<T>, T, 2, 3, 2, -1> zaz, bab, pqp;
        vProxy<vec3<T>, T, 2, 3, 3, -1> zaa, baa, pqq;
        vProxy<vec3<T>, T, 3, 0, 0, -1> axx, arr, qss;
        vProxy<vec3<T>, T, 3, 0, 1, -1> axy, arg, qst;
        vProxy<vec3<T>, T, 3, 0, 2, -1> axz, arb, qsp;
        vProxy<vec3<T>, T, 3, 0, 3, -1> axa, ara, qsq;
        vProxy<vec3<T>, T, 3, 1, 0, -1> ayx, agr, qts;
        vProxy<vec3<T>, T, 3, 1, 1, -1> ayy, agg, qtt;
        vProxy<vec3<T>, T, 3, 1, 2, -1> ayz, agb, qtp;
        vProxy<vec3<T>, T, 3, 1, 3, -1> aya, aga, qtq;
        vProxy<vec3<T>, T, 3, 2, 0, -1> azx, abr, qps;
        vProxy<vec3<T>, T, 3, 2, 1, -1> azy, abg, qpt;
        vProxy<vec3<T>, T, 3, 2, 2, -1> azz, abb, qpp;
        vProxy<vec3<T>, T, 3, 2, 3, -1> aza, aba, qpq;
        vProxy<vec3<T>, T, 3, 3, 0, -1> aax, aar, qqs;
        vProxy<vec3<T>, T, 3, 3, 1, -1> aay, aag, qqt;
        vProxy<vec3<T>, T, 3, 3, 2, -1> aaz, aab, qqp;
        vProxy<vec3<T>, T, 3, 3, 3, -1> aaa, qqq;
        vProxy<vec4<T>, T, 0, 0, 0, 0> xxxx, rrrr, ssss;
        vProxy<vec4<T>, T, 0, 0, 0, 1> xxxy, rrrg, ssst;
        vProxy<vec4<T>, T, 0, 0, 0, 2> xxxz, rrrb, sssp;
        vProxy<vec4<T>, T, 0, 0, 0, 3> xxxa, rrra, sssq;
        vProxy<vec4<T>, T, 0, 0, 1, 0> xxyx, rrgr, ssts;
        vProxy<vec4<T>, T, 0, 0, 1, 1> xxyy, rrgg, sstt;
        vProxy<vec4<T>, T, 0, 0, 1, 2> xxyz, rrgb, sstp;
        vProxy<vec4<T>, T, 0, 0, 1, 3> xxya, rrga, sstq;
        vProxy<vec4<T>, T, 0, 0, 2, 0> xxzx, rrbr, ssps;
        vProxy<vec4<T>, T, 0, 0, 2, 1> xxzy, rrbg, sspt;
        vProxy<vec4<T>, T, 0, 0, 2, 2> xxzz, rrbb, sspp;
        vProxy<vec4<T>, T, 0, 0, 2, 3> xxza, rrba, sspq;
        vProxy<vec4<T>, T, 0, 0, 3, 0> xxax, rrar, ssqs;
        vProxy<vec4<T>, T, 0, 0, 3, 1> xxay, rrag, ssqt;
        vProxy<vec4<T>, T, 0, 0, 3, 2> xxaz, rrab, ssqp;
        vProxy<vec4<T>, T, 0, 0, 3, 3> xxaa, rraa, ssqq;
        vProxy<vec4<T>, T, 0, 1, 0, 0> xyxx, rgrr, stss;
        vProxy<vec4<T>, T, 0, 1, 0, 1> xyxy, rgrg, stst;
        vProxy<vec4<T>, T, 0, 1, 0, 2> xyxz, rgrb, stsp;
        vProxy<vec4<T>, T, 0, 1, 0, 3> xyxa, rgra, stsq;
        vProxy<vec4<T>, T, 0, 1, 1, 0> xyyx, rggr, stts;
        vProxy<vec4<T>, T, 0, 1, 1, 1> xyyy, rggg, sttt;
        vProxy<vec4<T>, T, 0, 1, 1, 2> xyyz, rggb, sttp;
        vProxy<vec4<T>, T, 0, 1, 1, 3> xyya, rgga, sttq;
        vProxy<vec4<T>, T, 0, 1, 2, 0> xyzx, rgbr, stps;
        vProxy<vec4<T>, T, 0, 1, 2, 1> xyzy, rgbg, stpt;
        vProxy<vec4<T>, T, 0, 1, 2, 2> xyzz, rgbb, stpp;
        vProxy<vec4<T>, T, 0, 1, 2, 3> xyza, rgba, stpq;
        vProxy<vec4<T>, T, 0, 1, 3, 0> xyax, rgar, stqs;
        vProxy<vec4<T>, T, 0, 1, 3, 1> xyay, rgag, stqt;
        vProxy<vec4<T>, T, 0, 1, 3, 2> xyaz, rgab, stqp;
        vProxy<vec4<T>, T, 0, 1, 3, 3> xyaa, rgaa, stqq;
        vProxy<vec4<T>, T, 0, 2, 0, 0> xzxx, rbrr, spss;
        vProxy<vec4<T>, T, 0, 2, 0, 1> xzxy, rbrg, spst;
        vProxy<vec4<T>, T, 0, 2, 0, 2> xzxz, rbrb, spsp;
        vProxy<vec4<T>, T, 0, 2, 0, 3> xzxa, rbra, spsq;
        vProxy<vec4<T>, T, 0, 2, 1, 0> xzyx, rbgr, spts;
        vProxy<vec4<T>, T, 0, 2, 1, 1> xzyy, rbgg, sptt;
        vProxy<vec4<T>, T, 0, 2, 1, 2> xzyz, rbgb, sptp;
        vProxy<vec4<T>, T, 0, 2, 1, 3> xzya, rbga, sptq;
        vProxy<vec4<T>, T, 0, 2, 2, 0> xzzx, rbbr, spps;
        vProxy<vec4<T>, T, 0, 2, 2, 1> xzzy, rbbg, sppt;
        vProxy<vec4<T>, T, 0, 2, 2, 2> xzzz, rbbb, sppp;
        vProxy<vec4<T>, T, 0, 2, 2, 3> xzza, rbba, sppq;
        vProxy<vec4<T>, T, 0, 2, 3, 0> xzax, rbar, spqs;
        vProxy<vec4<T>, T, 0, 2, 3, 1> xzay, rbag, spqt;
        vProxy<vec4<T>, T, 0, 2, 3, 2> xzaz, rbab, spqp;
        vProxy<vec4<T>, T, 0, 2, 3, 3> xzaa, rbaa, spqq;
        vProxy<vec4<T>, T, 0, 3, 0, 0> xaxx, rarr, sqss;
        vProxy<vec4<T>, T, 0, 3, 0, 1> xaxy, rarg, sqst;
        vProxy<vec4<T>, T, 0, 3, 0, 2> xaxz, rarb, sqsp;
        vProxy<vec4<T>, T, 0, 3, 0, 3> xaxa, rara, sqsq;
        vProxy<vec4<T>, T, 0, 3, 1, 0> xayx, ragr, sqts;
        vProxy<vec4<T>, T, 0, 3, 1, 1> xayy, ragg, sqtt;
        vProxy<vec4<T>, T, 0, 3, 1, 2> xayz, ragb, sqtp;
        vProxy<vec4<T>, T, 0, 3, 1, 3> xaya, raga, sqtq;
        vProxy<vec4<T>, T, 0, 3, 2, 0> xazx, rabr, sqps;
        vProxy<vec4<T>, T, 0, 3, 2, 1> xazy, rabg, sqpt;
        vProxy<vec4<T>, T, 0, 3, 2, 2> xazz, rabb, sqpp;
        vProxy<vec4<T>, T, 0, 3, 2, 3> xaza, raba, sqpq;
        vProxy<vec4<T>, T, 0, 3, 3, 0> xaax, raar, sqqs;
        vProxy<vec4<T>, T, 0, 3, 3, 1> xaay, raag, sqqt;
        vProxy<vec4<T>, T, 0, 3, 3, 2> xaaz, raab, sqqp;
        vProxy<vec4<T>, T, 0, 3, 3, 3> xaaa, raaa, sqqq;
        vProxy<vec4<T>, T, 1, 0, 0, 0> yxxx, grrr, tsss;
        vProxy<vec4<T>, T, 1, 0, 0, 1> yxxy, grrg, tsst;
        vProxy<vec4<T>, T, 1, 0, 0, 2> yxxz, grrb, tssp;
        vProxy<vec4<T>, T, 1, 0, 0, 3> yxxa, grra, tssq;
        vProxy<vec4<T>, T, 1, 0, 1, 0> yxyx, grgr, tsts;
        vProxy<vec4<T>, T, 1, 0, 1, 1> yxyy, grgg, tstt;
        vProxy<vec4<T>, T, 1, 0, 1, 2> yxyz, grgb, tstp;
        vProxy<vec4<T>, T, 1, 0, 1, 3> yxya, grga, tstq;
        vProxy<vec4<T>, T, 1, 0, 2, 0> yxzx, grbr, tsps;
        vProxy<vec4<T>, T, 1, 0, 2, 1> yxzy, grbg, tspt;
        vProxy<vec4<T>, T, 1, 0, 2, 2> yxzz, grbb, tspp;
        vProxy<vec4<T>, T, 1, 0, 2, 3> yxza, grba, tspq;
        vProxy<vec4<T>, T, 1, 0, 3, 0> yxax, grar, tsqs;
        vProxy<vec4<T>, T, 1, 0, 3, 1> yxay, grag, tsqt;
        vProxy<vec4<T>, T, 1, 0, 3, 2> yxaz, grab, tsqp;
        vProxy<vec4<T>, T, 1, 0, 3, 3> yxaa, graa, tsqq;
        vProxy<vec4<T>, T, 1, 1, 0, 0> yyxx, ggrr, ttss;
        vProxy<vec4<T>, T, 1, 1, 0, 1> yyxy, ggrg, ttst;
        vProxy<vec4<T>, T, 1, 1, 0, 2> yyxz, ggrb, ttsp;
        vProxy<vec4<T>, T, 1, 1, 0, 3> yyxa, ggra, ttsq;
        vProxy<vec4<T>, T, 1, 1, 1, 0> yyyx, gggr, ttts;
        vProxy<vec4<T>, T, 1, 1, 1, 1> yyyy, gggg, tttt;
        vProxy<vec4<T>, T, 1, 1, 1, 2> yyyz, gggb, tttp;
        vProxy<vec4<T>, T, 1, 1, 1, 3> yyya, ggga, tttq;
        vProxy<vec4<T>, T, 1, 1, 2, 0> yyzx, ggbr, ttps;
        vProxy<vec4<T>, T, 1, 1, 2, 1> yyzy, ggbg, ttpt;
        vProxy<vec4<T>, T, 1, 1, 2, 2> yyzz, ggbb, ttpp;
        vProxy<vec4<T>, T, 1, 1, 2, 3> yyza, ggba, ttpq;
        vProxy<vec4<T>, T, 1, 1, 3, 0> yyax, ggar, ttqs;
        vProxy<vec4<T>, T, 1, 1, 3, 1> yyay, ggag, ttqt;
        vProxy<vec4<T>, T, 1, 1, 3, 2> yyaz, ggab, ttqp;
        vProxy<vec4<T>, T, 1, 1, 3, 3> yyaa, ggaa, ttqq;
        vProxy<vec4<T>, T, 1, 2, 0, 0> yzxx, gbrr, tpss;
        vProxy<vec4<T>, T, 1, 2, 0, 1> yzxy, gbrg, tpst;
        vProxy<vec4<T>, T, 1, 2, 0, 2> yzxz, gbrb, tpsp;
        vProxy<vec4<T>, T, 1, 2, 0, 3> yzxa, gbra, tpsq;
        vProxy<vec4<T>, T, 1, 2, 1, 0> yzyx, gbgr, tpts;
        vProxy<vec4<T>, T, 1, 2, 1, 1> yzyy, gbgg, tptt;
        vProxy<vec4<T>, T, 1, 2, 1, 2> yzyz, gbgb, tptp;
        vProxy<vec4<T>, T, 1, 2, 1, 3> yzya, gbga, tptq;
        vProxy<vec4<T>, T, 1, 2, 2, 0> yzzx, gbbr, tpps;
        vProxy<vec4<T>, T, 1, 2, 2, 1> yzzy, gbbg, tppt;
        vProxy<vec4<T>, T, 1, 2, 2, 2> yzzz, gbbb, tppp;
        vProxy<vec4<T>, T, 1, 2, 2, 3> yzza, gbba, tppq;
        vProxy<vec4<T>, T, 1, 2, 3, 0> yzax, gbar, tpqs;
        vProxy<vec4<T>, T, 1, 2, 3, 1> yzay, gbag, tpqt;
        vProxy<vec4<T>, T, 1, 2, 3, 2> yzaz, gbab, tpqp;
        vProxy<vec4<T>, T, 1, 2, 3, 3> yzaa, gbaa, tpqq;
        vProxy<vec4<T>, T, 1, 3, 0, 0> yaxx, garr, tqss;
        vProxy<vec4<T>, T, 1, 3, 0, 1> yaxy, garg, tqst;
        vProxy<vec4<T>, T, 1, 3, 0, 2> yaxz, garb, tqsp;
        vProxy<vec4<T>, T, 1, 3, 0, 3> yaxa, gara, tqsq;
        vProxy<vec4<T>, T, 1, 3, 1, 0> yayx, gagr, tqts;
        vProxy<vec4<T>, T, 1, 3, 1, 1> yayy, gagg, tqtt;
        vProxy<vec4<T>, T, 1, 3, 1, 2> yayz, gagb, tqtp;
        vProxy<vec4<T>, T, 1, 3, 1, 3> yaya, gaga, tqtq;
        vProxy<vec4<T>, T, 1, 3, 2, 0> yazx, gabr, tqps;
        vProxy<vec4<T>, T, 1, 3, 2, 1> yazy, gabg, tqpt;
        vProxy<vec4<T>, T, 1, 3, 2, 2> yazz, gabb, tqpp;
        vProxy<vec4<T>, T, 1, 3, 2, 3> yaza, gaba, tqpq;
        vProxy<vec4<T>, T, 1, 3, 3, 0> yaax, gaar, tqqs;
        vProxy<vec4<T>, T, 1, 3, 3, 1> yaay, gaag, tqqt;
        vProxy<vec4<T>, T, 1, 3, 3, 2> yaaz, gaab, tqqp;
        vProxy<vec4<T>, T, 1, 3, 3, 3> yaaa, gaaa, tqqq;
        vProxy<vec4<T>, T, 2, 0, 0, 0> zxxx, brrr, psss;
        vProxy<vec4<T>, T, 2, 0, 0, 1> zxxy, brrg, psst;
        vProxy<vec4<T>, T, 2, 0, 0, 2> zxxz, brrb, pssp;
        vProxy<vec4<T>, T, 2, 0, 0, 3> zxxa, brra, pssq;
        vProxy<vec4<T>, T, 2, 0, 1, 0> zxyx, brgr, psts;
        vProxy<vec4<T>, T, 2, 0, 1, 1> zxyy, brgg, pstt;
        vProxy<vec4<T>, T, 2, 0, 1, 2> zxyz, brgb, pstp;
        vProxy<vec4<T>, T, 2, 0, 1, 3> zxya, brga, pstq;
        vProxy<vec4<T>, T, 2, 0, 2, 0> zxzx, brbr, psps;
        vProxy<vec4<T>, T, 2, 0, 2, 1> zxzy, brbg, pspt;
        vProxy<vec4<T>, T, 2, 0, 2, 2> zxzz, brbb, pspp;
        vProxy<vec4<T>, T, 2, 0, 2, 3> zxza, brba, pspq;
        vProxy<vec4<T>, T, 2, 0, 3, 0> zxax, brar, psqs;
        vProxy<vec4<T>, T, 2, 0, 3, 1> zxay, brag, psqt;
        vProxy<vec4<T>, T, 2, 0, 3, 2> zxaz, brab, psqp;
        vProxy<vec4<T>, T, 2, 0, 3, 3> zxaa, braa, psqq;
        vProxy<vec4<T>, T, 2, 1, 0, 0> zyxx, bgrr, ptss;
        vProxy<vec4<T>, T, 2, 1, 0, 1> zyxy, bgrg, ptst;
        vProxy<vec4<T>, T, 2, 1, 0, 2> zyxz, bgrb, ptsp;
        vProxy<vec4<T>, T, 2, 1, 0, 3> zyxa, bgra, ptsq;
        vProxy<vec4<T>, T, 2, 1, 1, 0> zyyx, bggr, ptts;
        vProxy<vec4<T>, T, 2, 1, 1, 1> zyyy, bggg, pttt;
        vProxy<vec4<T>, T, 2, 1, 1, 2> zyyz, bggb, pttp;
        vProxy<vec4<T>, T, 2, 1, 1, 3> zyya, bgga, pttq;
        vProxy<vec4<T>, T, 2, 1, 2, 0> zyzx, bgbr, ptps;
        vProxy<vec4<T>, T, 2, 1, 2, 1> zyzy, bgbg, ptpt;
        vProxy<vec4<T>, T, 2, 1, 2, 2> zyzz, bgbb, ptpp;
        vProxy<vec4<T>, T, 2, 1, 2, 3> zyza, bgba, ptpq;
        vProxy<vec4<T>, T, 2, 1, 3, 0> zyax, bgar, ptqs;
        vProxy<vec4<T>, T, 2, 1, 3, 1> zyay, bgag, ptqt;
        vProxy<vec4<T>, T, 2, 1, 3, 2> zyaz, bgab, ptqp;
        vProxy<vec4<T>, T, 2, 1, 3, 3> zyaa, bgaa, ptqq;
        vProxy<vec4<T>, T, 2, 2, 0, 0> zzxx, bbrr, ppss;
        vProxy<vec4<T>, T, 2, 2, 0, 1> zzxy, bbrg, ppst;
        vProxy<vec4<T>, T, 2, 2, 0, 2> zzxz, bbrb, ppsp;
        vProxy<vec4<T>, T, 2, 2, 0, 3> zzxa, bbra, ppsq;
        vProxy<vec4<T>, T, 2, 2, 1, 0> zzyx, bbgr, ppts;
        vProxy<vec4<T>, T, 2, 2, 1, 1> zzyy, bbgg, pptt;
        vProxy<vec4<T>, T, 2, 2, 1, 2> zzyz, bbgb, pptp;
        vProxy<vec4<T>, T, 2, 2, 1, 3> zzya, bbga, pptq;
        vProxy<vec4<T>, T, 2, 2, 2, 0> zzzx, bbbr, ppps;
        vProxy<vec4<T>, T, 2, 2, 2, 1> zzzy, bbbg, pppt;
        vProxy<vec4<T>, T, 2, 2, 2, 2> zzzz, bbbb, pppp;
        vProxy<vec4<T>, T, 2, 2, 2, 3> zzza, bbba, pppq;
        vProxy<vec4<T>, T, 2, 2, 3, 0> zzax, bbar, ppqs;
        vProxy<vec4<T>, T, 2, 2, 3, 1> zzay, bbag, ppqt;
        vProxy<vec4<T>, T, 2, 2, 3, 2> zzaz, bbab, ppqp;
        vProxy<vec4<T>, T, 2, 2, 3, 3> zzaa, bbaa, ppqq;
        vProxy<vec4<T>, T, 2, 3, 0, 0> zaxx, barr, pqss;
        vProxy<vec4<T>, T, 2, 3, 0, 1> zaxy, barg, pqst;
        vProxy<vec4<T>, T, 2, 3, 0, 2> zaxz, barb, pqsp;
        vProxy<vec4<T>, T, 2, 3, 0, 3> zaxa, bara, pqsq;
        vProxy<vec4<T>, T, 2, 3, 1, 0> zayx, bagr, pqts;
        vProxy<vec4<T>, T, 2, 3, 1, 1> zayy, bagg, pqtt;
        vProxy<vec4<T>, T, 2, 3, 1, 2> zayz, bagb, pqtp;
        vProxy<vec4<T>, T, 2, 3, 1, 3> zaya, baga, pqtq;
        vProxy<vec4<T>, T, 2, 3, 2, 0> zazx, babr, pqps;
        vProxy<vec4<T>, T, 2, 3, 2, 1> zazy, babg, pqpt;
        vProxy<vec4<T>, T, 2, 3, 2, 2> zazz, babb, pqpp;
        vProxy<vec4<T>, T, 2, 3, 2, 3> zaza, baba, pqpq;
        vProxy<vec4<T>, T, 2, 3, 3, 0> zaax, baar, pqqs;
        vProxy<vec4<T>, T, 2, 3, 3, 1> zaay, baag, pqqt;
        vProxy<vec4<T>, T, 2, 3, 3, 2> zaaz, baab, pqqp;
        vProxy<vec4<T>, T, 2, 3, 3, 3> zaaa, baaa, pqqq;
        vProxy<vec4<T>, T, 3, 0, 0, 0> axxx, arrr, qsss;
        vProxy<vec4<T>, T, 3, 0, 0, 1> axxy, arrg, qsst;
        vProxy<vec4<T>, T, 3, 0, 0, 2> axxz, arrb, qssp;
        vProxy<vec4<T>, T, 3, 0, 0, 3> axxa, arra, qssq;
        vProxy<vec4<T>, T, 3, 0, 1, 0> axyx, argr, qsts;
        vProxy<vec4<T>, T, 3, 0, 1, 1> axyy, argg, qstt;
        vProxy<vec4<T>, T, 3, 0, 1, 2> axyz, argb, qstp;
        vProxy<vec4<T>, T, 3, 0, 1, 3> axya, arga, qstq;
        vProxy<vec4<T>, T, 3, 0, 2, 0> axzx, arbr, qsps;
        vProxy<vec4<T>, T, 3, 0, 2, 1> axzy, arbg, qspt;
        vProxy<vec4<T>, T, 3, 0, 2, 2> axzz, arbb, qspp;
        vProxy<vec4<T>, T, 3, 0, 2, 3> axza, arba, qspq;
        vProxy<vec4<T>, T, 3, 0, 3, 0> axax, arar, qsqs;
        vProxy<vec4<T>, T, 3, 0, 3, 1> axay, arag, qsqt;
        vProxy<vec4<T>, T, 3, 0, 3, 2> axaz, arab, qsqp;
        vProxy<vec4<T>, T, 3, 0, 3, 3> axaa, araa, qsqq;
        vProxy<vec4<T>, T, 3, 1, 0, 0> ayxx, agrr, qtss;
        vProxy<vec4<T>, T, 3, 1, 0, 1> ayxy, agrg, qtst;
        vProxy<vec4<T>, T, 3, 1, 0, 2> ayxz, agrb, qtsp;
        vProxy<vec4<T>, T, 3, 1, 0, 3> ayxa, agra, qtsq;
        vProxy<vec4<T>, T, 3, 1, 1, 0> ayyx, aggr, qtts;
        vProxy<vec4<T>, T, 3, 1, 1, 1> ayyy, aggg, qttt;
        vProxy<vec4<T>, T, 3, 1, 1, 2> ayyz, aggb, qttp;
        vProxy<vec4<T>, T, 3, 1, 1, 3> ayya, agga, qttq;
        vProxy<vec4<T>, T, 3, 1, 2, 0> ayzx, agbr, qtps;
        vProxy<vec4<T>, T, 3, 1, 2, 1> ayzy, agbg, qtpt;
        vProxy<vec4<T>, T, 3, 1, 2, 2> ayzz, agbb, qtpp;
        vProxy<vec4<T>, T, 3, 1, 2, 3> ayza, agba, qtpq;
        vProxy<vec4<T>, T, 3, 1, 3, 0> ayax, agar, qtqs;
        vProxy<vec4<T>, T, 3, 1, 3, 1> ayay, agag, qtqt;
        vProxy<vec4<T>, T, 3, 1, 3, 2> ayaz, agab, qtqp;
        vProxy<vec4<T>, T, 3, 1, 3, 3> ayaa, agaa, qtqq;
        vProxy<vec4<T>, T, 3, 2, 0, 0> azxx, abrr, qpss;
        vProxy<vec4<T>, T, 3, 2, 0, 1> azxy, abrg, qpst;
        vProxy<vec4<T>, T, 3, 2, 0, 2> azxz, abrb, qpsp;
        vProxy<vec4<T>, T, 3, 2, 0, 3> azxa, abra, qpsq;
        vProxy<vec4<T>, T, 3, 2, 1, 0> azyx, abgr, qpts;
        vProxy<vec4<T>, T, 3, 2, 1, 1> azyy, abgg, qptt;
        vProxy<vec4<T>, T, 3, 2, 1, 2> azyz, abgb, qptp;
        vProxy<vec4<T>, T, 3, 2, 1, 3> azya, abga, qptq;
        vProxy<vec4<T>, T, 3, 2, 2, 0> azzx, abbr, qpps;
        vProxy<vec4<T>, T, 3, 2, 2, 1> azzy, abbg, qppt;
        vProxy<vec4<T>, T, 3, 2, 2, 2> azzz, abbb, qppp;
        vProxy<vec4<T>, T, 3, 2, 2, 3> azza, abba, qppq;
        vProxy<vec4<T>, T, 3, 2, 3, 0> azax, abar, qpqs;
        vProxy<vec4<T>, T, 3, 2, 3, 1> azay, abag, qpqt;
        vProxy<vec4<T>, T, 3, 2, 3, 2> azaz, abab, qpqp;
        vProxy<vec4<T>, T, 3, 2, 3, 3> azaa, abaa, qpqq;
        vProxy<vec4<T>, T, 3, 3, 0, 0> aaxx, aarr, qqss;
        vProxy<vec4<T>, T, 3, 3, 0, 1> aaxy, aarg, qqst;
        vProxy<vec4<T>, T, 3, 3, 0, 2> aaxz, aarb, qqsp;
        vProxy<vec4<T>, T, 3, 3, 0, 3> aaxa, aara, qqsq;
        vProxy<vec4<T>, T, 3, 3, 1, 0> aayx, aagr, qqts;
        vProxy<vec4<T>, T, 3, 3, 1, 1> aayy, aagg, qqtt;
        vProxy<vec4<T>, T, 3, 3, 1, 2> aayz, aagb, qqtp;
        vProxy<vec4<T>, T, 3, 3, 1, 3> aaya, aaga, qqtq;
        vProxy<vec4<T>, T, 3, 3, 2, 0> aazx, aabr, qqps;
        vProxy<vec4<T>, T, 3, 3, 2, 1> aazy, aabg, qqpt;
        vProxy<vec4<T>, T, 3, 3, 2, 2> aazz, aabb, qqpp;
        vProxy<vec4<T>, T, 3, 3, 2, 3> aaza, aaba, qqpq;
        vProxy<vec4<T>, T, 3, 3, 3, 0> aaax, aaar, qqqs;
        vProxy<vec4<T>, T, 3, 3, 3, 1> aaay, aaag, qqqt;
        vProxy<vec4<T>, T, 3, 3, 3, 2> aaaz, aaab, qqqp;
        vProxy<vec4<T>, T, 3, 3, 3, 3> aaaa, qqqq;

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
            ((T*)this)[3] = 0;
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