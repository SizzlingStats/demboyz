
#pragma once

#include <cassert>

class QAngle
{
public:
    float x, y, z;
    void Init( void )
    {
        x = y = z = 0.0f;
    }
    void Init( float _x, float _y, float _z )
    {
        x = _x;
        y = _y;
        z = _z;
    }
};

class Vector
{
public:
    float x, y, z;
    Vector()
    {
    }
    Vector(float X, float Y, float Z)
    {
        x = X; y = Y; z = Z;
    }
    void Init( void )
    {
        x = y = z = 0.0f;
    }
    void Init( float _x, float _y, float _z )
    {
        x = _x;
        y = _y;
        z = _z;
    }
    float& operator[](int i)
    {
        assert( (i >= 0) && (i < 3) );
        return ((float*)this)[i];
    }
    float operator[](int i) const
    {
        assert( (i >= 0) && (i < 3) );
        return ((float*)this)[i];
    }
};
