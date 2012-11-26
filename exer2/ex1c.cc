#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>

using namespace std;

int main()
{
	vector<int> inputs;
	int tmp;

	cout << "Give me some ints (EOF quits input)" << endl;

	while (cin >> tmp)
		inputs.push_back(tmp);

	
	if (inputs.size() == 0) 
		return EXIT_FAILURE;

	cout << "You gave me:" << endl;

	for (int i = 0; i < inputs.size(); i++) 
		cout << inputs.at(i) << endl;
	
	return EXIT_SUCCESS;
}
