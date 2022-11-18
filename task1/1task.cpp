#include <iostream>
#include <windows.h>
#include <string>
#include <clocale>
#include <conio.h>

using namespace std;

wstring ConvertStrToWstr(string str);

int main()
{
	setlocale(LC_ALL, "russian");
	const int ALPHABET_SIZE = 26, ARRAY_SIZE = 30;
	int drives = GetLogicalDrives();

	for (int i = 0, pointer = 1; i < ALPHABET_SIZE; ++i, pointer = pointer << 1)
	{
		// Если pointer дойдет до существующего диска
		if ((drives & pointer) != 0)
		{
			wstring NameDisk = ConvertStrToWstr((char)('A' + i) + (string)":\\");
			cout << "Диск: " << (char)('A' + i) << '\t'<< '\t';

			DWORD VSNumber;
			if (GetVolumeInformation(NameDisk.c_str(), NULL, NULL,
				&VSNumber, NULL, NULL, NULL, NULL))
			{
				cout << "Серийный номер: " << VSNumber << endl;
			}
		}
	}
	return 0;
}

wstring ConvertStrToWstr(string str)
{
	return wstring(str.begin(), str.end());
}