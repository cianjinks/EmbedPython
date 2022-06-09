#ifndef VECTOR_H
#define VECTOR_H

struct vec2
{
    float x, y;
    vec2(float x, float y) : x(x), y(y) {};
    void print() { std::cout << x << " " << y << std::endl; }
};

vec2 add_v2(vec2 a, vec2 b)
{
    return vec2(a.x + b.x, a.y + b.y);
}

vec2 mul_v2(vec2 a, vec2 b)
{
    return vec2(a.x * b.x, a.y * b.y); 
}

#endif