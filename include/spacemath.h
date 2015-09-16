#ifndef SPACEMATH_H
#define SPACEMATH_H

#include <iostream>

template<typename T>
class CommonVectorOperators
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

template<typename vecT, typename T, int A, int B>
class v2Proxy : public CommonVectorOperators<T>
{
public:
    static const bool isWritable = (A != B);
    typedef typename std::conditional<isWritable, v2Proxy, struct OperationNotValid>::type assignmentType;

#   define CREATE_ASSIGNMENT_OPERATOR(Op)                                           \
    template<typename rhsVT, typename rhsT, int rhsA, int rhsB>                     \
    assignmentType& operator##Op##(const v2Proxy<rhsVT, rhsT, rhsA, rhsB>& _rhs)    \
    {                                                                               \
        ((T*)this)[A] Op ((const rhsT*)&_rhs)[rhsA];                                \
        ((T*)this)[B] Op ((const rhsT*)&_rhs)[rhsB];                                \
        return *this;                                                               \
    }                                                                               \
    template<typename rhsT>                                                         \
    assignmentType& operator##Op##(const rhsT _rhs)                                 \
    {                                                                               \
        ((T*)this)[A] Op _rhs;                                                      \
        ((T*)this)[B] Op _rhs;                                                      \
        return *this;                                                               \
    }

    CREATE_ASSIGNMENT_OPERATOR(= );

    CREATE_ASSIGNMENT_OPERATOR(+= );
    CREATE_ASSIGNMENT_OPERATOR(-= );
    CREATE_ASSIGNMENT_OPERATOR(*= );
    CREATE_ASSIGNMENT_OPERATOR(/= );

#   undef CREATE_ASSIGNMENT_OPERATOR
};

template<typename T>
class vec2 : public CommonVectorOperators<T>
{
public:
    union
    {
        T v[2];
        struct { T x, y, r, g, s, t; };

        v2Proxy<vec2, T, 0, 0> xx, rr, ss;
        v2Proxy<vec2, T, 0, 1> xy, rg, st;
        v2Proxy<vec2, T, 1, 0> yx, gr, ts;
        v2Proxy<vec2, T, 1, 1> yy, gg, tt;
    };

    vec2() { }
    vec2(T _x, T _y) { v[0] = _x; v[1] = _y; }

#   define CREATE_ASSIGNMENT_OPERATOR(Op)                                   \
    template<typename rhsT>                                                 \
    vec2& operator##Op##(const vec2<rhsT>& _rhs)                            \
    {                                                                       \
        ((T*)this)[0] Op ((const rhsT*)&_rhs)[0];                           \
        ((T*)this)[1] Op ((const rhsT*)&_rhs)[0];                           \
        return *this;                                                       \
    }                                                                       \
    template<typename rhsT>                                                 \
    vec2& operator##Op##(const rhsT _rhs)                                   \
    {                                                                       \
        ((T*)this)[0] Op _rhs;                                              \
        ((T*)this)[1] Op _rhs;                                              \
        return *this;                                                       \
    }                                                                       \
    template<typename rhsVT, typename rhsT, int rhsA, int rhsB>             \
    vec2& operator##Op##(const v2Proxy<rhsVT, rhsT, rhsA, rhsB>& _rhs)      \
    {                                                                       \
        ((T*)this)[0] Op ((const rhsT*)&_rhs)[rhsA];                        \
        ((T*)this)[1] Op ((const rhsT*)&_rhs)[rhsB];                        \
        return *this;                                                       \
    }

    CREATE_ASSIGNMENT_OPERATOR(+= );

#   undef CREATE_ASSIGNMENT_OPERATOR
};

#endif