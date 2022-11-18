#include <iostream>
#include <windows.h> 
using namespace std;

string checkBoolean(int state) {
	return state == 1 ? "yes\n" : "no\n";
}

int main()
{
	cout << "Type keyboard: ";
	switch (GetKeyboardType(0))
	{
	case 4:
		cout << "Enhanced 101- or 102-key keyboards (and compatibles)" << endl;
		break;
	case 7:
		cout << "Japanese Keyboard" << endl;
		break;
	case 8:
		cout << "Korean Keyboard" << endl;
		break;
	default:
		cout << "Unknown type or HID keyboard" << endl;
		break;
	}

	cout << "State numlock: " + checkBoolean(GetKeyState(VK_NUMLOCK));
	cout << "State numlock: " + checkBoolean(GetKeyState(VK_SCROLL));
	cout << "State numlock: " + checkBoolean(GetKeyState(VK_CAPITAL));
}