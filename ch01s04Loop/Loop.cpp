#include <iostream>
#include <conio.h>

using namespace std;

int main()
{
	while (!_kbhit())
	{
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < i; j++)
			{
				cout << "*";
			}
			cout << endl;
		}
	}
	cout << "�м�����" << endl;

	system("pause");
	return 0;
}