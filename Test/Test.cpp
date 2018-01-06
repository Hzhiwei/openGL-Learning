// Test.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <iostream>
#include <vector>

using namespace std;

int main()
{
	vector<int> temp;

	temp.push_back(1);
	temp.push_back(2);

	cout << &temp << "," << temp.data() << endl;
	cout << sizeof(temp) << "," << sizeof(int) * 2 << endl;

	system("pause");

    return 0;
}

