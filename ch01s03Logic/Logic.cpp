#include <iostream>

using namespace std;

int main()
{
	int a, b;

	cout << "输入第一个数: ";
	cin >> a;
	cout << "输入第二个数: ";
	cin >> b;

	if (a > b)
	{
		cout << "数值 a 比数值 b 大." << endl;
	}
	else
	{
		cout << "数值 b 比数值 a 大." << endl;
	}
	cout << "数值 a 是 " << a << endl
		<< "数值 b 是 " << b << endl;

	system("pause");
	return 0;
}