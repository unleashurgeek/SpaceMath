#include<iostream>

#include <spacemath.h>

int main()
{
    vec2<float> t_float(1.6f, 3.5f);
    vec2<int> t_int(5, 4);

    //st_float.xy += st_float;

    //t_float += t_int;
    //t_float += 2;
    //t_float += t_int.yx;
    //t_float.xy += t_int.xy;
    //t_float.xy += t_int.x;
    t_float = t_float.xy + t_float.yx;
    //t_float.xy += t_int;

    std::cout << t_float.r << ", " << t_float.g << std::endl;
    //std::cout << t_float << std::endl;
    std::cin.get();
    return 0;
}