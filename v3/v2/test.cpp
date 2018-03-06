#include <iostream>
#include <cstring>
#include <cstdlib>

using namespace std;

int main(int argc, char const *argv[])
{
	char *a = (char*) malloc(5);
	int c;
	for (int i=0; i<6; ++i) {
		cout << (int)a[i] << endl;
	}
	cout << "After\n";
	cout << a << endl;
	return 0;
}