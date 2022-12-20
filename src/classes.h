#pragma once
#include "specalgos.h"
#include <string>
using std::string;

struct Time;
struct Session;
struct Schedule;
struct User;
struct Student;
struct Instructor;
struct Course;
struct UniCourses;
struct EnrolledStudent;
struct EnrolledCourse;
struct UserCourses;
struct UserIdentifier;
struct UserSession;

void sortSchedule(Schedule &);
Session *getSession(string);

struct Time {
  unsigned int hour, minute;
  void display();
};

struct Session {
  string day;
  Time start, end;
  Session(string dayName, Time startTime, Time endTime) {
    day = dayName;
    start = startTime;
    end = endTime;
  }
  float value();
  void display();
  Session *next = nullptr, *prev = nullptr;
};

struct Schedule {
  Session *head = nullptr, *tail = nullptr;
  void sortByTime();
  void sortByDay();
  void display();
};

struct EnrolledStudent {
  Student *student;
  double grade = 0;
  EnrolledStudent(Student *studentPointer) { student = studentPointer; }
  EnrolledStudent *next = nullptr;
};

struct Course {
  unsigned int uid, capacity, instructorUid;
  string name;
  Schedule timeSchedule;
  EnrolledStudent *attendees = nullptr;
  EnrolledStudent *present = nullptr;
  void loadAttendees(Student *);
  double grade(Student *);
  void viewAttendees();
  void viewAttendance(Student *);
  EnrolledStudent *fetchStudentNode(unsigned int, EnrolledStudent *);
  void display();
  Course *next = nullptr, *prev = nullptr;
};

struct EnrolledCourse {
  Course *course = nullptr;
  EnrolledCourse(Course *coursePointer) { course = coursePointer; }
  EnrolledCourse *next = nullptr, *prev = nullptr;
  void sortByTime();
  void sortByDay();
};

struct UniCourses {
  Course *head = nullptr, *tail = nullptr;
  void sort();
  void display();
  void loadAttendees(Student *);
};

struct UserSession {
  Course *course;
  string day;
  Time start, end;
  UserSession(Course *coursePtr, string dayName, Time startTime, Time endTime) {
    course = coursePtr;
    day = dayName;
    start = startTime;
    end = endTime;
  }
  float value();
  void display();
  UserSession *next = nullptr, *prev = nullptr;
};

struct UserSchedule {
  UserSession *head = nullptr, *tail = nullptr;
  void sortByTime();
  void sortByDay();
};

struct UserCourses {
  EnrolledCourse *head = nullptr, *tail = nullptr;
  UserSchedule timeSchedule;
  void loadSchedule();
  void displayCalendar();
  void list();
  void sort();
  Course *fetchCourse(unsigned int);
  EnrolledCourse *fetchCourseNode(unsigned int);
};

struct User {
  UserCourses courses;
  unsigned int uid;
  string fName, lName, email, password;
  void display();
};

struct Student : User {
  void enrollCourse(UniCourses);
  void dropCourse();
  void swapCourses(UniCourses);
  void viewGrades();
  Student *next = nullptr;
};

struct Instructor : User {
  void loadCourses(UniCourses);
  void takeAttendance(Course *, Student *);
  void gradeStudents(Course *, Student *);
  void addCourse(UniCourses &);
  void addSchedule(Course *);
  Instructor *next = nullptr;
};

struct UserIdentifier {
  string type;
  int uid = -1;
};
