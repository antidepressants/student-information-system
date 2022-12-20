#pragma once
#include "algos.hpp"

std::string validatePassword(size_t t);
unsigned int strSeed(std::string s);
std::string hashStr(std::string s, size_t t, int init);
void appendFillers(std::string &s, size_t t);
std::string encryptPassword(std::string s, size_t t);
