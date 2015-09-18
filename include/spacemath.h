#ifndef SPACEMATH_H
#define SPACEMATH_H

#include <iostream>

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

template<class vT, typename T, int A, int B>
class v2Proxy : public ArraySubscriptOperators<T>
{
public:
    static const bool isWritable = (A != B);
    typedef typename std::conditional<isWritable, v2Proxy, struct OperationNotValid>::type assignmentType;

#   define CREATE_ASSIGNMENT_OPERATOR(Op)                                           \
    template<class rhsVT, typename rhsT, int rhsA, int rhsB>                        \
    assignmentType& operator##Op##(const v2Proxy<rhsVT, rhsT, rhsA, rhsB>& _rhs)    \
    {                                                                               \
        ((T*)this)[A] Op ((const rhsT*)&_rhs)[rhsA];                                \
        ((T*)this)[B] Op ((const rhsT*)&_rhs)[rhsB];                                \
        return *this;                                                               \
    }                                                                               \
    assignmentType& operator##Op##(const T _rhs)                                    \
    {                                                                               \
        ((T*)this)[A] Op _rhs;                                                      \
        ((T*)this)[B] Op _rhs;                                                      \
        return *this;                                                               \
    }                                                                               \
    template<template<typename> class rhsVT, typename rhsT>                         \
    assignmentType& operator##Op##(const rhsVT<rhsT>& _rhs)                         \
    {                                                                               \
        ((T*)this)[A] Op _rhs[0];                                                   \
        ((T*)this)[B] Op _rhs[1];                                                   \
        return *this;                                                               \
    }

    CREATE_ASSIGNMENT_OPERATOR(= );

    CREATE_ASSIGNMENT_OPERATOR(+= );
    CREATE_ASSIGNMENT_OPERATOR(-= );
    CREATE_ASSIGNMENT_OPERATOR(*= );
    CREATE_ASSIGNMENT_OPERATOR(/= );

#   undef CREATE_ASSIGNMENT_OPERATOR

#   define CREATE_ARITHMETIC_OPERATOR(Op)                                           \
    template<class rhsVT, typename rhsT, int rhsA, int rhsB>                        \
    vT operator##Op##(const v2Proxy<rhsVT, rhsT, rhsA, rhsB>& _rhs) const           \
    {                                                                               \
        vT result;                                                                  \
        result[0] = ((const T*)this)[A] Op ((const rhsT*)&_rhs)[rhsA];              \
        result[1] = ((const T*)this)[B] Op ((const rhsT*)&_rhs)[rhsB];              \
        return result;                                                              \
    }                                                                               \
    vT operator##Op##(const T _rhs) const                                           \
    {                                                                               \
        vT result;                                                                  \
        result[0] = ((const T*)this)[A] Op _rhs;                                    \
        result[1] = ((const T*)this)[B] Op _rhs;                                    \
        return result;                                                              \
    }                                                                               \
    friend vT operator##Op##(const T _lhs, const v2Proxy& _rhs)                     \
    {                                                                               \
        vT result;                                                                  \
        result[0] = _lhs Op((const T*)& _rhs)[A];                                   \
        result[1] = _lhs Op((const T*)& _rhs)[B];                                   \
        return result;                                                              \
    }                                                                               

    CREATE_ARITHMETIC_OPERATOR(+);
    CREATE_ARITHMETIC_OPERATOR(-);
    CREATE_ARITHMETIC_OPERATOR(*);
    CREATE_ARITHMETIC_OPERATOR(/ );

#   undef CREATE_ARITHMETIC_OPERATOR

#   define CREATE_COMPARISON_OPERATOR(Op)                                           \
    template<class rhsVT, typename rhsT, int rhsA, int rhsB>                        \
    bool operator##Op##(const v2Proxy<rhsVT, rhsT, rhsA, rhsB>& _rhs) const         \
    {                                                                               \
        return (((const T*)this)[A] Op ((const rhsT*)&_rhs)[rhsA]) &&               \
                    (((const T*)this)[B] Op ((const rhsT*)&_rhs)[rhsB]);            \
    }                                                                               \
    bool operator##Op##(const T _rhs) const                                         \
    {                                                                               \
        return (((const T*)this)[A] Op _rhs) &&                                     \
                    (((const T*)this)[B] Op _rhs);                                  \
    }                                                                               \
    friend bool operator##Op##(const T _lhs, const v2Proxy& _rhs)                   \
    {                                                                               \
        return (_lhs Op ((const T*)&_rhs)[A]) &&                                    \
                    (_lhs Op ((const T*)&_rhs)[B]);                                 \
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
class vec2 : public ArraySubscriptOperators<T>
{
public:
    union
    {
        struct { T x, y; };
        struct { T r, g; };
        struct { T s, t; };

        v2Proxy<vec2, T, 0, 0> xx, rr, ss;
        v2Proxy<vec2, T, 0, 1> xy, rg, st;
        v2Proxy<vec2, T, 1, 0> yx, gr, ts;
        v2Proxy<vec2, T, 1, 1> yy, gg, tt;
    };

    template<typename iT>
    vec2(vec2<iT>& _v) { ((T*)this)[0] = ((const iT*)&_v)[0]; ((T*)this)[1] = ((const iT*)&_v)[1]; }
    vec2(T _x, T _y) { ((T*)this)[0] = _x; ((T*)this)[1] = _y; }
    vec2() { }

#   define CREATE_ASSIGNMENT_OPERATOR(Op)                                           \
    template<typename rhsT>                                                         \
    vec2& operator##Op##(const vec2<rhsT>& _rhs)                                    \
    {                                                                               \
        ((T*)this)[0] Op ((const rhsT*)&_rhs)[0];                                   \
        ((T*)this)[1] Op ((const rhsT*)&_rhs)[1];                                   \
        return *this;                                                               \
    }                                                                               \
    vec2& operator##Op##(const T _rhs)                                              \
    {                                                                               \
        ((T*)this)[0] Op _rhs;                                                      \
        ((T*)this)[1] Op _rhs;                                                      \
        return *this;                                                               \
    }                                                                               \
    template<class rhsVT, typename rhsT, int rhsA, int rhsB>                        \
    vec2& operator##Op##(const v2Proxy<rhsVT, rhsT, rhsA, rhsB>& _rhs)              \
    {                                                                               \
        ((T*)this)[0] Op ((const rhsT*)&_rhs)[rhsA];                                \
        ((T*)this)[1] Op ((const rhsT*)&_rhs)[rhsB];                                \
        return *this;                                                               \
    }

    CREATE_ASSIGNMENT_OPERATOR(= );

    CREATE_ASSIGNMENT_OPERATOR(+= );
    CREATE_ASSIGNMENT_OPERATOR(-= );
    CREATE_ASSIGNMENT_OPERATOR(*= );
    CREATE_ASSIGNMENT_OPERATOR(/= );

#   undef CREATE_ASSIGNMENT_OPERATOR

#   define CREATE_ARITHMETIC_OPERATOR(Op)                                           \
    template<class rhsVT, typename rhsT, int rhsA, int rhsB>                        \
    vec2 operator##Op##(const v2Proxy<rhsVT, rhsT, rhsA, rhsB>& _rhs) const         \
    {                                                                               \
        vec2 result;                                                                \
        result[0] = ((const T*)this)[0] Op ((const rhsT*)&_rhs)[rhsA];              \
        result[1] = ((const T*)this)[1] Op ((const rhsT*)&_rhs)[rhsB];              \
        return result;                                                              \
    }                                                                               \
    template<class lhsVT, typename lhsT, int lhsA, int lhsB>                        \
    friend vec2 operator##Op##(const v2Proxy<lhsVT, lhsT, lhsA, lhsB>& _lhs, const vec2& _rhs) \
    {                                                                               \
        vec2 result;                                                                \
        result[0] = ((const lhsT*)&_lhs)[lhsA] Op((const T*)&_rhs)[0];              \
        result[1] = ((const lhsT*)&_lhs)[lhsB] Op((const T*)&_rhs)[1];              \
        return result;                                                              \
    }                                                                               \
    template<typename rhsT>                                                         \
    vec2 operator##Op##(const vec2<rhsT>& _rhs) const                               \
    {                                                                               \
        vec2 result;                                                                \
        result[0] = ((const T*)this)[0] Op ((const rhsT*)&_rhs)[0];                 \
        result[1] = ((const T*)this)[1] Op ((const rhsT*)&_rhs)[1];                 \
        return result;                                                              \
    }                                                                               \
    vec2 operator##Op##(const T _rhs) const                                         \
    {                                                                               \
        vec2 result;                                                                \
        result[0] = ((const T*)this)[0] Op _rhs;                                    \
        result[1] = ((const T*)this)[1] Op _rhs;                                    \
        return result;                                                              \
    }                                                                               \
    friend vec2 operator##Op##(const T _lhs, const vec2& _rhs)                      \
    {                                                                               \
        vec2 result;                                                                \
        result[0] = _lhs Op ((const T*)&_rhs)[0];                                   \
        result[1] = _lhs Op ((const T*)&_rhs)[1];                                   \
        return result;                                                              \
    }

    CREATE_ARITHMETIC_OPERATOR(+);
    CREATE_ARITHMETIC_OPERATOR(-);
    CREATE_ARITHMETIC_OPERATOR(*);
    CREATE_ARITHMETIC_OPERATOR(/ );

#   undef CREATE_ARITHMETIC_OPERATOR

#   define CREATE_COMPARISON_OPERATOR(Op)                                           \
    template<class rhsVT, typename rhsT, int rhsA, int rhsB>                        \
    bool operator##Op##(const v2Proxy<rhsVT, rhsT, rhsA, rhsB>& _rhs) const         \
    {                                                                               \
        return (((const T*)this)[0] Op ((const rhsT*)&_rhs)[rhsA]) &&               \
                    (((const T*)this)[1] Op ((const rhsT*)&_rhs)[rhsB]);            \
    }                                                                               \
    template<class lhsVT, typename lhsT, int lhsA, int lhsB>                        \
    friend bool operator##Op##(const v2Proxy<lhsVT, lhsT, lhsA, lhsB>& _lhs,        \
                               const vec2& _rhs)                                    \
    {                                                                               \
        return (((const lhsT*)&_lhs)[lhsA] Op ((const T*)&_rhs)[0]) &&              \
                    (((const lhsT*)&_lhs)[lhsB] Op ((const T*)&_rhs)[1]);           \
    }                                                                               \
    template<typename rhsT>                                                         \
    bool operator##Op##(const vec2<rhsT>& _rhs) const                               \
    {                                                                               \
        return (((const T*)this)[0] Op ((const rhsT*)&_rhs)[0]) &&                  \
                    (((const T*)this)[1] Op ((const rhsT*)&_rhs)[1]);               \
    }                                                                               \
    bool operator##Op##(const T _rhs) const                                         \
    {                                                                               \
        return (((const T*)this)[0] Op _rhs) && (((const T*)this)[1] Op _rhs);      \
    }                                                                               \
    friend bool operator##Op##(const T _lhs, const vec2& _rhs)                      \
    {                                                                               \
        return (_lhs Op ((const T*)&_rhs)[0]) && (_lhs Op ((const T*)&_rhs)[1]);    \
    }

    CREATE_COMPARISON_OPERATOR(== );
    CREATE_COMPARISON_OPERATOR(!= );
    CREATE_COMPARISON_OPERATOR(<= );
    CREATE_COMPARISON_OPERATOR(>= );
    CREATE_COMPARISON_OPERATOR(< );
    CREATE_COMPARISON_OPERATOR(> );

#   undef CREATE_COMPARISON_OPERATOR
};

#endif