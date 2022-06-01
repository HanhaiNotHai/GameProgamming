#include <iostream>
#include <windows.h>

using namespace std;

int main()
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	cout << "下面将显示字体的颜色与对应的编码" << endl;
	int textColor = 0xF0;

	cout << hex << showbase;
	for (int i = 0; i < 16; i++)
	{
		SetConsoleTextAttribute(h, textColor);
		cout << textColor << " 的颜色" << endl;
		textColor++;
	}

	textColor = 0x0F;
	SetConsoleTextAttribute(h, textColor);
	cout << "下面将显示背景的颜色与对应的编码" << endl;

	textColor = 0x00;
	for (int i = 0; i < 16; i++)
	{
		SetConsoleTextAttribute(h, textColor);
		cout << textColor << " 的颜色" << endl;
		textColor += 16;
	}

	system("pause");
	return 0;
}