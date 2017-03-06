#include "Set.h"
#include <algorithm>

bool Set::Empty() const
{
    return items.empty();
}

int Set::Size() const
{
    return items.size();
}

bool Set::Contains( const std::string& item ) const
{
    return std::find( items.cbegin(), items.cend(), item ) != items.cend();
}

void Set::Add( const std::string& item )
{
    if ( item.empty() )
    {
        throw InvalidItemException();
    }

    if ( !Contains( item ) )
    {
        items.push_back( item );
    }
}

Set Set::Union( const Set& set ) const
{
    Set union_set;

    for( std::string item : items )
    {
        union_set.Add( item );
    }

    for( std::string item : set.items )
    {
        union_set.Add( item );
    }

    return union_set;
}

Set Set::Intersection( const Set& other ) const
{
    Set result;

    for ( std::string item : items )
    {
        if ( other.Contains( item ) )
        {
            result.Add( item );
        }
    }

    return result;
}

bool Set::Equals( const Set& other ) const
{
    if ( Size() != other.Size() )
    {
        return false;
    }
    return Intersection( other ).Size() == Size();
}

bool Set::IsSuperset( const Set& other ) const
{
    return other.Equals( Union( other ) );
}

bool Set::IsSubset( const Set& other ) const
{
    return Equals( Union( other ) );
}

Set::Set(void)
{
}

Set::~Set(void)
{
}
