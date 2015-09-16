#ifndef SPACEMATH_H
#define SPACEMATH_H

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

template<typename T>
class vec2 : public CommonVectorOperators<T>
{
public:
    union
    {
        T v[2];
        struct { T x, y, r, g, s, t; };

    };
    vec2() { }
    vec2(T _x, T _y) { v[0] = _x; v[1] = _y; }
};

#endif