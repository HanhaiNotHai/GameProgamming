#include <iostream>
#include <windows.h>

using namespace std;

int main()
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	cout << "���潫��ʾ�������ɫ���Ӧ�ı���" << endl;
	int textColor = 0xF0;

	cout << hex << showbase;
	for (int i = 0; i < 16; i++)
	{
		SetConsoleTextAttribute(h, textColor);
		cout << textColor << " ����ɫ" << endl;
		textColor++;
	}

	textColor = 0x0F;
	SetConsoleTextAttribute(h, textColor);
	cout << "���潫��ʾ��������ɫ���Ӧ�ı���" << endl;

	textColor = 0x00;
	for (int i = 0; i < 16; i++)
	{
		SetConsoleTextAttribute(h, textColor);
		cout << textColor << " ����ɫ" << endl;
		textColor += 16;
	}

	system("pause");
	return 0;
}