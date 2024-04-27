#pragma once
#include <iostream>
using namespace std;

class Person
{
public:
	virtual void func()
	{
		cout << "I am Person!" << endl;
		hony();
	}
	virtual void hony()
	{
		cout << "I like football!" << endl;
	}
};

class Child : public Person
{
public:
	virtual void func()
	{
		cout << "I am Child!" << endl;
		hony();
	}
	virtual void hony()
	{
		cout << "I like basketball!" << endl;
	}
};


