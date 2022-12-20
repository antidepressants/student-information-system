#include "classes.h"
#include "algos.hpp"
#include "dbops.hpp"
using std::cin;
using std::cout;
using std::getline;
using std::to_string;

void Time::display() {
  string h = (hour > 9) ? to_string(hour) : "0" + to_string(hour);
  string m = (minute > 9) ? to_string(minute) : "0" + to_string(minute);
  cout << h << ":" << m;
}

float Session::value() {
  float minute = start.minute;
  return start.hour + (minute / 10);
}

void Session::display() {
  cout << "[";
  start.display();
  cout << " => ";
  end.display();
  cout << "]";
}

Session *getSession(string element) {
  string field;
  stringstream ss2(element);
  string day;
  unsigned int startHour, startMinute, endHour, endMinute;
  getline(ss2, field, ':');
  day = field;
  getline(ss2, field, ':');
  stringstream ss3(field);
  string time;
  ss3 >> time;
  startHour = stoi(time);
  ss3 >> time;
  startMinute = stoi(time);
  getline(ss2, field);
  stringstream ss4(field);
  ss4 >> time;
  endHour = stoi(time);
  ss4 >> time;
  endMinute = stoi(time);
  return new Session(day, {startHour, startMinute}, {endHour, endMinute});
}

void Schedule::display() {
  Session *current = head;
  while (current != nullptr) {
    cout << current->day << "\t";
    current->display();
    cout << "\n";
    current = current->next;
  }
}

void Schedule::sortByTime() {
  for (Session *base = head; base != nullptr; base = base->next) {
    float val = base->value();
    for (Session *top = base; top != head and top->prev->value() < val;
         top = top->prev) {
      string tmp = top->day;
      top->day = top->prev->day;
      top->prev->day = tmp;
      Time start1 = top->start;
      Time end1 = top->end;
      top->start = top->prev->start;
      top->end = top->prev->end;
      top->prev->start = start1;
      top->prev->end = end1;
    }
  }
}

void Schedule::sortByDay() {
  for (Session *base = head; base != nullptr; base = base->next) {
    int val = dayValue(base->day);
    for (Session *top = base; top != head and dayValue(top->prev->day) > val;
         top = top->prev) {
      string tmp = top->day;
      top->day = top->prev->day;
      top->prev->day = tmp;
      Time start1 = top->start;
      Time end1 = top->end;
      top->start = top->prev->start;
      top->end = top->prev->end;
      top->prev->start = start1;
      top->prev->end = end1;
    }
  }
}

double Course::grade(Student *student) {
  EnrolledStudent *current = attendees;
  return fetchStudentNode(student->uid, attendees)->grade;
}

void Course::loadAttendees(Student *head) {
  for (Student *current = head; current != nullptr; current = current->next) {
    if (current->courses.fetchCourseNode(uid) != nullptr)
      singlyInsertAtTail(attendees, new EnrolledStudent(current));
  }
}

void Course::viewAttendees() {
  EnrolledStudent *current = attendees;
  if (current == nullptr) {
    cout << "No one is taking this class!\n";
    return;
  }
  cout << "Attendees:\n";
  while (current != nullptr) {
    Student *student = current->student;
    student->display();
    cout << "\n";
    current = current->next;
  }
}

void Course::viewAttendance(Student *head) {
  for (auto *current = attendees; current != nullptr; current = current->next) {
    Student *student = current->student;
    char presentSymbol = ' ';
    if (fetchStudentNode(student->uid, present) != nullptr)
      presentSymbol = '*';
    cout << "<" << presentSymbol << "> ";
    student->display();
    cout << "\n";
  }
}

EnrolledStudent *Course::fetchStudentNode(unsigned int uid,
                                          EnrolledStudent *head) {
  if (head == nullptr)
    return nullptr;
  if (uid == head->student->uid)
    return head;
  return fetchStudentNode(uid, head->next);
}

void Course::display() {
  cout << "Course ID: " << uid << "\n";
  cout << "Course Name: " << name << "\n";
  cout << "Free seats: " << capacity << "\n";
  cout << "Schedule:\n";
  timeSchedule.display();
}

void UniCourses::display() {
  for (Course *current = head; current != nullptr; current = current->next) {
    current->display();
  }
}

void UniCourses::sort() {
  for (Course *current = head; current != nullptr; current = current->next) {
    current->timeSchedule.sortByTime();
    current->timeSchedule.sortByDay();
  }
}

void UniCourses::loadAttendees(Student *studentHead) {
  for (auto *current = head; current != nullptr; current = current->next) {
    current->loadAttendees(studentHead);
  }
}

Course *UserCourses::fetchCourse(unsigned int uid) {
  for (EnrolledCourse *current = head; current != nullptr;
       current = current->next) {
    if (uid == current->course->uid)
      return current->course;
  }
  return nullptr;
}

EnrolledCourse *UserCourses::fetchCourseNode(unsigned int uid) {
  for (EnrolledCourse *current = head; current != nullptr;
       current = current->next) {
    if (uid == current->course->uid)
      return current;
  }
  return nullptr;
}

void UserCourses::list() {
  if (head == nullptr) {
    cout << "You don't have any courses!\n";
    return;
  }
  for (EnrolledCourse *current = head; current != nullptr;
       current = current->next) {
    cout << "{" << current->course->uid << "} " << current->course->name
         << "\n";
  }
}

void User::display() { cout << "{" << uid << "} " << fName << " " << lName; }

void Student::enrollCourse(UniCourses uniCourses) {
  uniCourses.display();
  unsigned int uid;
  validate(uid);
  Course *course = fetchEntryByUid(uid, uniCourses.head);
  if (course == nullptr) {
    cout << "Course unavailable!\n";
    return;
  }
  for (EnrolledCourse *current = courses.head; current != nullptr;
       current = current->next) {
    if (current->course->name == course->name) {
      cout << "Already registered in course!\n";
      return;
    }
  }
  courses.tail = doublyInsertAtTail(courses.head, courses.tail,
                                    new EnrolledCourse(course));
  course->capacity--;
}

void Student::dropCourse() {
  courses.list();
  unsigned int uid;
  validate(uid);
  EnrolledCourse *courseNode = courses.fetchCourseNode(uid);
  cout << courseNode->course->uid << "\n";
  if (courseNode == nullptr) {
    cout << "Course unavailable!\n";
    return;
  }
  courseNode->course->capacity++;
  EnrolledStudent *studentNode =
      courseNode->course->fetchStudentNode(uid, courseNode->course->attendees);
  doublyRemove(courses.head, courses.tail, courseNode);
}

void Student::swapCourses(UniCourses uniCourses) {
  cout << "Select course to swap in\n";
  for (Course *current = uniCourses.head; current != nullptr;
       current = current->next) {
    if (courses.fetchCourseNode(current->uid) != nullptr)
      continue;
    current->display();
    cout << "\n";
  }
  unsigned int newUid;
  validate(newUid);
  Course *newCourse = fetchEntryByUid(newUid, uniCourses.head);
  if (newCourse == nullptr) {
    cout << "Course unavailable!\n";
    return swapCourses(uniCourses);
  }
  if (courses.fetchCourse(newUid) != nullptr) {
    cout << "Course already enrolled!\n";
    return swapCourses(uniCourses);
  }
  if (newCourse->capacity == 0) {
    cout << "Course full!\n";
    return swapCourses(uniCourses);
  }
  cout << "Select a course to swap out\n";
  courses.list();
  unsigned int oldUid;
  validate(oldUid);
  EnrolledCourse *oldCourseNode = courses.fetchCourseNode(oldUid);
  if (oldCourseNode == nullptr) {
    cout << "Course unavailable!\n";
    return;
  }
  oldCourseNode->course->capacity++;
  newCourse->capacity--;
  oldCourseNode->course = newCourse;
}

void Student::viewGrades() {
  double gpa = 0;
  size_t count = 0;
  for (auto *current = courses.head; current != nullptr;
       current = current->next, count++) {
    double grade =
        current->course->fetchStudentNode(uid, current->course->attendees)
            ->grade;
    gpa += grade;
    cout << "{" << current->course->uid << "} " << current->course->name << "\t"
         << grade << "\n";
  }
  cout << "\tGPA\t" << gpa / count << "\n";
}

void Instructor::loadCourses(UniCourses uniCourses) {
  for (Course *current = uniCourses.head; current != nullptr;
       current = current->next) {
    if (current->instructorUid == uid) {
      courses.tail = doublyInsertAtTail(courses.head, courses.tail,
                                        new EnrolledCourse(current));
    }
  }
}

void Instructor::takeAttendance(Course *course, Student *studentHead) {
  course->viewAttendees();
  string uids;
  cout << "Enter uids of present students\n";
  cin.clear();
  cin.ignore(INT_MAX, '\n');
  getline(cin, uids);
  std::vector<string> uidVector;
  parse(uids, ' ', uidVector);
  deleteList(course->present);
  course->present = nullptr;
  for (auto a : uidVector) {
    Student *present = fetchEntryByUid(stoi(a), studentHead);
    singlyInsertAtTail(course->present, new EnrolledStudent(present));
  }
}

void Instructor::gradeStudents(Course *course, Student *studentHead) {
  course->viewAttendees();
  string studentGrades;
  cout << "Enter grades according to the following format: uid1 grade1;uid2 "
          "grade2;\n";
  cin.clear();
  cin.ignore(INT_MAX, '\n');
  getline(cin, studentGrades);
  std::vector<string> gradesVector;
  parse(studentGrades, ';', gradesVector);
  for (auto g : gradesVector) {
    stringstream ss(g);
    string data;
    getline(ss, data, ' ');
    EnrolledStudent *graded =
        course->fetchStudentNode(stoi(data), course->attendees);
    getline(ss, data);
    graded->grade = stod(data);
  }
}

void Instructor::addCourse(UniCourses &uniCourses) {
  Course *course = new Course;
  course->uid = maxID("data/courses.csv") + 1;
  cout << "Name: ";
  cin.clear();
  getline(cin, course->name);
  cout << "Capacity:\n";
  unsigned int capacity;
  validate(capacity);
  course->capacity = capacity;
  course->instructorUid = uid;
  cin.clear();
  cin.ignore(INT_MAX, '\n');
  addSchedule(course);
  uniCourses.tail =
      doublyInsertAtTail(uniCourses.head, uniCourses.tail, course);
}

void Instructor::addSchedule(Course *course) {
  string schedule;
  cout << "Schedule:\nFormat:\nday:startHour startMinute:endHour "
          "endMinute,day2:startHour2 ...\n";
  getline(cin, schedule);
  stringstream ss(schedule);
  string element;
  while (getline(ss, element, ',')) {
    Session *session = getSession(element);
    course->timeSchedule.tail = doublyInsertAtTail(
        course->timeSchedule.head, course->timeSchedule.tail, session);
  }
}

float UserSession::value() {
  float minute = start.minute;
  return start.hour + (minute / 100);
}

void UserSession::display() {
  cout << "[";
  start.display();
  cout << " => ";
  end.display();
  cout << "]";
}

void UserSchedule::sortByTime() {
  for (auto *base = head; base != nullptr; base = base->next) {
    float val = base->value();
    for (auto *top = base; top != head and top->prev->value() > val;
         top = top->prev) {
      string tmp = top->day;
      top->day = top->prev->day;
      top->prev->day = tmp;
      Time start1 = top->start;
      Time end1 = top->end;
      top->start = top->prev->start;
      top->end = top->prev->end;
      top->prev->start = start1;
      top->prev->end = end1;
      Course *course1 = top->course;
      top->course = top->prev->course;
      top->prev->course = course1;
    }
  }
}

void UserSchedule::sortByDay() {
  for (auto *base = head; base != nullptr; base = base->next) {
    int val = dayValue(base->day);
    for (auto *top = base; top != head and dayValue(top->prev->day) > val;
         top = top->prev) {
      string tmp = top->day;
      top->day = top->prev->day;
      top->prev->day = tmp;
      Time start1 = top->start;
      Time end1 = top->end;
      top->start = top->prev->start;
      top->end = top->prev->end;
      top->prev->start = start1;
      top->prev->end = end1;
      Course *course1 = top->course;
      top->course = top->prev->course;
      top->prev->course = course1;
    }
  }
}

void UserCourses::loadSchedule() {
  for (auto *current = head; current != nullptr; current = current->next) {
    for (auto *session = current->course->timeSchedule.head; session != nullptr;
         session = session->next) {
      UserSession *userSession = new UserSession(current->course, session->day,
                                                 session->start, session->end);
      timeSchedule.tail =
          doublyInsertAtTail(timeSchedule.head, timeSchedule.tail, userSession);
    }
  }
  sort();
}

void UserCourses::sort() {
  timeSchedule.sortByTime();
  timeSchedule.sortByDay();
}

void UserCourses::displayCalendar() {
  string days[] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
  for (string day : days) {
    cout << day << ":\n";
    for (auto *session = timeSchedule.head; session != nullptr;
         session = session->next) {
      if (session->day != day)
        continue;
      auto *course = session->course;
      session->display();
      cout << " " << course->name << "\t";
    }
    cout << "\n";
  }
}
