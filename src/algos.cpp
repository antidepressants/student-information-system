#include "algos.hpp"
#include <sstream>
#include <string>
using std::cin;
using std::cout;
using std::getline;
using std::stringstream;
using std::vector;

int selectNum(int lBound, int hBound) {
  cout << "[" << lBound << "-" << hBound << "]\n";
  int n;
  do {
    validate(n);
    if (n < lBound || n > hBound)
      cout << "Invalid option!\n";
  } while (n < lBound || n > hBound);
  cin.ignore();
  return n;
}

int selectChoice(string choices[], int size) {
  for (size_t i = 0; i < size; i++) {
    std::cout << "[" << i + 1 << "] " << choices[i] << "\n";
  }
  return selectNum(1, size);
}

bool yN() {
  char c;
  cout << "[y/n]\n>>";
  cin >> c;
  c = tolower(c);
  if (c == 'y')
    return 1;
  if (c == 'n')
    return 0;
  cin.clear();
  cin.ignore(INT_MAX, '\n');
  cout << "Invalid option!\n";
  return yN();
}

void parse(string line, char delimiter, vector<string> &arr) {
  stringstream ss(line);
  string element;
  while (getline(ss, element, delimiter)) {
    arr.push_back(element);
  }
}
