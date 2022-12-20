#include "password.h"
using namespace std;

string validatePassword(size_t t) {
  cout << ">>";
  string pass;
  cin >> pass;
  if (pass.length() < 8 || pass.length() > t) {
    cout << "Invalid password length!\n";
    return validatePassword(t);
  }
  bool a = 0, d = 0, s = 0;
  for (size_t i = 0; i < pass.length(); i++) {
    if (isalpha(pass[i]))
      a = 1;
    else if (isdigit(pass[i]))
      d = 1;
    else
      s = 1;
  }
  if (!a || !d || !s) {
    cout << "Password must include letters, numbers, and special characters!\n";
    return validatePassword(t);
  }
  return pass;
}

unsigned int strSeed(string s) {
  unsigned int seed = 0;
  for (size_t i = 0; i < s.length(); i++) {
    seed += (int)s[i];
  }
  return seed;
}

string hashStr(string s, size_t t, int init) {
  string str = "";
  unsigned int strseed = init * strSeed(s);
  for (size_t i = 0; i < s.length(); i++) {
    srand(strseed);
    unsigned int seed = strseed - (int)s[i];
    srand(seed);
    char c;
    switch (rand() % 3) {
    case 0:
      c = rand() % 10 + 48;
      break;
    case 1:
      c = rand() % 26 + 65;
      break;
    case 2:
      c = rand() % 26 + 97;
    }
    str += c;
  }
  return str;
}

void appendFillers(string &s, size_t t) {
  size_t diff = t - s.length();
  if (!diff)
    return;
  size_t a = t / s.length();
  size_t b = 0;
  while (t % (s.length() + b) != 0)
    b++;
  size_t pos = s.length() - b;
  string sub = s.substr(pos, b);
  sub.append(hashStr(sub, t, 1));
  s.replace(pos, sub.length(), sub);
}

string encryptPassword(string s, size_t t) {
  appendFillers(s, t);
  size_t blocks = t / s.length();
  string str = "";
  for (size_t i = 1; i <= blocks; i++) {
    str.append(hashStr(s, t, i));
  }
  return str;
}
