#include <iostream>

using namespace std;

int main()
{
	int a, b;

	cout << "�����һ����: ";
	cin >> a;
	cout << "����ڶ�����: ";
	cin >> b;

	if (a > b)
	{
		cout << "��ֵ a ����ֵ b ��." << endl;
	}
	else
	{
		cout << "��ֵ b ����ֵ a ��." << endl;
	}
	cout << "��ֵ a �� " << a << endl
		<< "��ֵ b �� " << b << endl;

	system("pause");
	return 0;
}