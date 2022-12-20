#include "specalgos.h"
using std::cin;
using std::cout;

string validateName() {
  string name;
  cout << ">>";
  getline(cin, name);
  for (auto c : name) {
    if (!isalpha(c) && c != ' ') {
      cout << "Invalid Input!\n";
      return validateName();
    }
  }
  return name;
}

string validateEmail() {
  string email;
  cout << ">>";
  cin >> email;
  bool valid = 0;
  int atpos = email.find("@");
  if (atpos != string::npos && atpos != 0) {
    string domain = email.substr(atpos + 1, email.length() - atpos - 1);
    int dotpos = domain.find(".");
    if (dotpos != string::npos && dotpos != 0 && dotpos != domain.length() - 1)
      valid = 1;
  }
  char illegal[] = "',<>/{}()*&^%$#!-+=`~|\'\"\\";
  for (auto c : illegal)
    if (email.find(c) != string::npos) {
      valid = 0;
      break;
    }
  if (valid == 0) {
    cout << "Invalid Email!\n";
    return validateEmail();
  }
  return email;
}

string getPass() {
  string pass = validatePassword(32);
  return encryptPassword(pass, 32);
}

int dayValue(string day) {
  string days[] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
  size_t size = sizeof(days) / sizeof(string);
  for (size_t i = 0; i <= size; i++) {
    if (days[i] == day)
      return i + 1;
  }
  return 0;
}
