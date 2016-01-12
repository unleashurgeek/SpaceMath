# SpaceMath
SpaceMath is a header-only, dependency free library focused on brining "swizzle" syntax (found in shader languages GLSL & HLSL) and spacial vector mathematics (hense the name) / linear mathematics into C++. For more information on Swizzle syntax read here: https://www.opengl.org/wiki/Data_Type_(GLSL)#Swizzling 

This libary allows you to simply create and manipulate vectors like the following:
```C++
vec4<int> foo(0);        // 0,  0, 0, 0
foo.yx = vec2<int>(4, 1);     // 1,  4, 0, 0
foo.zw = foo.xx * 2;     // 1,  4, 2, 2
vec3<int> bar(foo);      // 1,  4, 2
bar += foo.xxx           // 2,  5, 3
bar *= foo;              // 2, 20, 6
// etc.
```

The main advantage to using this library is for easy to use syntax allowing you to create a vector of any type and manipulate it with C++'s built in operators (including bitwise operators if data type is an int). Furthermore its similarity to GLSL/HLSL swizziling makes it much easier to transfer over code. 


#### TODO
* Add matrix definitons and basic operator comparison
* Rewrite vProxy and VectorOperators so left-hand does not determine the vector size being calculated.
