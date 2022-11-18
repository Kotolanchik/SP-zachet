#include <iostream>
#include <string>
#include <clocale>

#include <Windows.h>
using namespace std;

int countDir = 0;

bool IsNotDotName(std::wstring const& Filename)
{
	return ((Filename != L".") && (Filename != L".."));
}

bool IsDirectory(DWORD FileAttrs)
{
	return (
		(0 != (FileAttrs & FILE_ATTRIBUTE_DIRECTORY)) &&
		(0 == (FileAttrs & FILE_ATTRIBUTE_REPARSE_POINT))
		);
}

void EnumerateFileObjects(std::wstring const& Path)
{

	WIN32_FIND_DATAW FindData;
	wstring const Mask = Path + L"\\*";
	HANDLE  const hFind = FindFirstFileW(Mask.c_str(), &FindData);
	if (INVALID_HANDLE_VALUE != hFind)
	{
		do
		{
			if (IsNotDotName(&FindData.cFileName[0]))
			{
				wstring const FullPath = Path + L"\\" + &FindData.cFileName[0];
				wcout << FullPath << endl;
				wcout << &FindData.cFileName[0] << endl;
				if (IsDirectory(FindData.dwFileAttributes))
				{
					++countDir;
					EnumerateFileObjects(FullPath);
				}
			}
		} while (NULL != FindNextFileW(hFind, &FindData));
		FindClose(hFind);
	}
}

int main()
{
	setlocale(LC_ALL, "russian");
	EnumerateFileObjects(L"C:\\Program Files");
	cout << "В заданной директории " << countDir << " папок";
	return 0;
}