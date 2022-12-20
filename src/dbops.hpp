#pragma once
#include "algos.hpp"
#include "classes.h"
#include <fstream>
#include <sstream>
using std::stringstream;

extern std::ifstream fin;
extern std::ofstream fout;

size_t fileSize(std::string);
size_t maxID(std::string);

int fetchUserByEmail(std::string, Student *, Instructor *);

template <class T> T *fetchEntryByUid(unsigned int uid, T *head) {
  T *current = head;
  while (current != nullptr) {
    if (current->uid == uid)
      return current;
    current = current->next;
  }
  return nullptr;
}

template <class T>
void createUser(T *&head, Student *studentHead, Instructor *instructorHead) {
  T *user = new T;
  user->uid = (maxID("data/students.csv") > maxID("data/instructors.csv"))
                  ? maxID("data/students.csv")
                  : maxID("data/instructors.csv");
  user->uid++;
  std::cout << "[User Creation]\n";
  std::cout << "First Name\n";
  user->fName = validateName();
  std::cout << "Last Name\n";
  user->lName = validateName();
  std::cout << "Email\n";
  do {
    user->email = validateEmail();
  } while (fetchUserByEmail(user->email, studentHead, instructorHead) != -1);
  std::cout << "Password\n";
  user->password = getPass();
  singlyInsertAtTail(head, user);
}

template <class T>
void pullUsers(UniCourses uniCourses, T *&head, string file) {
  fin.open("data/" + file + ".csv");
  string line;
  while (getline(fin, line)) {
    stringstream ss(line);
    string element;
    T *user = new T;
    getline(ss, element, ',');
    user->uid = stoi(element);
    getline(ss, element, ',');
    user->fName = element;
    getline(ss, element, ',');
    user->lName = element;
    getline(ss, element, ',');
    user->email = element;
    getline(ss, element, ',');
    user->password = element;
    while (getline(ss, element, ',')) {
      EnrolledCourse *course =
          new EnrolledCourse(fetchEntryByUid(stoi(element), uniCourses.head));
      user->courses.tail =
          doublyInsertAtTail(user->courses.head, user->courses.tail, course);
    }
    singlyInsertAtTail(head, user);
  }
  fin.close();
}

template <class T> void pullSchedule(T *head) {
  if (head == nullptr)
    return;
  head->courses.loadSchedule();
  pullSchedule(head->next);
}

template <class T> void pushUsers(T *head, string file) {
  fout.open("data/" + file + ".csv");
  for (auto *current = head; current != nullptr; current = current->next) {
    fout << current->uid << "," << current->fName << "," << current->lName
         << "," << current->email << "," << current->password << ",";
    for (auto *currentCourse = current->courses.head; currentCourse != nullptr;
         currentCourse = currentCourse->next) {
      fout << currentCourse->course->uid << ",";
    }
    fout << "\n";
  }
  fout.close();
}

void addUniCourse(UniCourses uniCourse);

UniCourses pullUniCourses();
void pullAll(UniCourses &, Student *&, Instructor *&);
void pushUniCourses(UniCourses);
void pullAttendance(UniCourses, Student *);
void pushAttendance(UniCourses);
void pullGrades(UniCourses);
void pushGrades(UniCourses, Student *);
void pushAll(UniCourses, Student *, Instructor *);
void deleteAll(UniCourses &, Student *&, Instructor *&);
void sync(UniCourses &, Student *&, Instructor *&);
