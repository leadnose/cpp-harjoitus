#include <iostream>
#include <vector>
#include <cstdlib>
#include <numeric>
#include <functional>

using namespace std;



int main()
{


  while (true) {
    string operation;
    cin >> operation;
    if (operation == "quit") {
      exit(EXIT_SUCCESS);
    }
    double tmp;
    std::vector<double> operands;
    while (cin >> tmp) {
      operands.push_back(tmp);
    }
    cin.clear();
    cin.ignore(1000, ';');


    if ("+" == operation) {
      cout << accumulate(operands.begin(), operands.end(), 0.0, plus<double>());
    } else if ("-" == operation) {
      cout << accumulate(operands.begin()+1, operands.end(), operands.at(0), minus<double>());
    } else if ("*" == operation) {
      cout << accumulate(operands.begin(), operands.end(), 1.0, multiplies<double>());
    } else if ("/" == operation) {
      cout << accumulate(operands.begin()+1, operands.end(), operands.at(0), divides<double>());
    } else {
      cout << "Unknown operation: " << operation;
    }
    cout << endl;
  }

    
    

}
