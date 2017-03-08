#pragma once

class Ship
{
public:
    Ship( int size ) : m_size( size ), m_life( size )
    {}

    bool IsSunk() const
    {
        return m_life <= 0;
    }

    void Hit()
    {
        --m_life;
    }

    int Size()
    {
        return m_size;
    }

private:
    int m_size;
    int m_life;
};
