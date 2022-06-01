#include <iostream>

using namespace std;

int main()
{
	int a;
	double b, c;

	a = 0;
	b = 0;

	cout << "Enter a: ";
	cin >> a;
	cout << "Enter b: ";
	cin >> b;

	c = a + b;

	cout << "Value of a is: " << a << endl;
	cout << "Value of b is: " << b << endl;
	cout << "c = a + b = " << c << endl;

	system("pause");
	return 0;
}