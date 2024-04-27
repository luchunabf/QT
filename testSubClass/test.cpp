#include "test.h"
#include <vector>

int main()
{
	//Child* child = new Child();
	//Person* person = (Person*)child;
	//person->func();

	vector<int> v;
	v.push_back(100);
	v.push_back(300);
	v.push_back(300);
	v.push_back(500);

	vector<int>::iterator itor;
	for (itor = v.begin(); itor != v.end(); itor++)
	{
		if (*itor == 300)
		{
			itor = v.erase(itor);
		}
	}
	for (itor = v.begin(); itor != v.end(); itor++)
	{
		cout << *itor << " ";
	}




	return 0;
}