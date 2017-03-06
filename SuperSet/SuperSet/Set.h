#pragma once
#include <string>
#include <vector>

class InvalidItemException {};

class Set
{
public:
	bool Empty() const;
    int Size() const;
    bool Contains( const std::string& item ) const;
    void Add( const std::string& item );
    Set Union( const Set& set ) const;
    Set Intersection( const Set& set ) const;
    bool Equals( const Set& set ) const;
    bool IsSuperset( const Set& other ) const;
    bool IsSubset( const Set& other ) const;

	Set(void);
	~Set(void);

private:
    std::vector<std::string> items;
};

