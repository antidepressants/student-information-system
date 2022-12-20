#pragma once
#include "dbops.hpp"

void sysPause();
void sysClear();

UserIdentifier login(Student *, Instructor *);

int adminMenu(User *, UniCourses &, Student *&, Instructor *&);
int studentMenu(Student *, UniCourses &, Student *&, Instructor *&);
int instructorMenu(Instructor *, UniCourses &, Student *&, Instructor *&);

void firstSetup(UniCourses *&, Student *&, Instructor *&);
void start(UniCourses &, Student *&, Instructor *&);

template <class T> Course *getCourse(T *user, T *head) {
  if (user->courses.head == nullptr)
    return nullptr;
  user->courses.list();
  unsigned int uid;
  validate(uid);
  Course *course = user->courses.fetchCourse(uid);
  return course;
}
