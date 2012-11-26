#include <iostream>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <algorithm>

static int parseint(char *s)
{
	int i;

	if (1 == sscanf(s, "%d", &i)) {
		return i;
	} else {
		std::cerr << "parsing of int failed" << std::endl;
		exit(EXIT_FAILURE);
	}
		
}

static void mark(int &i) {
  i = 0;
}

static bool marked(const int &i) {
  return i == 0;
}

int main(int argc, char **argv)
{
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " MAX" << std::endl;
		exit(EXIT_FAILURE);
	}

	const int MAX = parseint(argv[1]);	

	std::vector<int> ints(MAX-1);

	
 step1: // Create a list of consecutive integers from 2 to n: (2, 3, 4, ..., n).
	for (int i = 2; i <= MAX; i++) {
		ints[i-2] = i;
	}	 	
	
	/*
	for (auto it = ints.begin(); it != ints.end(); ++it) {
		std::cout << *it << ' ';
	}
	*/

 step2: // Initially, let p equal 2, the first prime number.
	int p = 2; 
	
 step3: // Starting from p, count up in increments of p and mark each
	// of these numbers greater than p itself in the list. These numbers
	// will be 2p, 3p, 4p, etc.; note that some of them may have already
	// been marked. In other words, remove all multiples of p from p to n.
	
	for (size_t idx = p - 2 + p; idx < ints.size(); idx += p) {
	  mark(ints.at(idx));
	}
	
	

 step4: // Find the first number greater than p in the list that
	// is not marked. If there was no such number, stop. Otherwise, let p
	// now equal this number (which is the next prime), and repeat from step 3.
	for (size_t idx = p - 2 + 1; idx < ints.size(); idx++) {
	  if (!marked(ints.at(idx))) {
	    p = ints.at(idx);
	    goto step3;
	  }
	}

 stop:
	for (auto it = ints.begin(); it != ints.end(); ++it) {
	  if (!marked(*it))
	    std::cout << *it << ' ';
	}
	
	

	
}
