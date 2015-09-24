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

        vProxy<vec2, T, 0, 0, -1, -1> xx, rr, ss;
        vProxy<vec2, T, 0, 1, -1, -1> xy, rg, st;
        vProxy<vec2, T, 1, 0, -1, -1> yx, gr, ts;
        vProxy<vec2, T, 1, 1, -1, -1> yy, gg, tt;
    };

    vec2() { }
    vec2(T _x, T _y) { ((T*)this)[0] = _x; ((T*)this)[1] = _y; }
    template<typename iT>
    vec2(vec2<iT>& _v)
    {
        ((T*)this)[0] = ((const iT*)&_v)[0];
        ((T*)this)[1] = ((const iT*)&_v)[1];
    }
    template<typename pT, int A, int B, int C, int D>
    vec2(vProxy<vec2, pT, A, B, C, D>& _v)
    {
        ((T*)this)[0] = ((const pT*)&_v)[A];
        ((T*)this)[1] = ((const pT*)&_v)[B];
    }
};

template<typename T>
class vec3 : public ArraySubscriptOperators<T>
{
    union
    {
        
    };
    vec3() { }
};

template<typename T>
class vec4 : public ArraySubscriptOperators<T>
{
    union
    {

    };
    vec4() { }
};

#endif