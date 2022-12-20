#include "dbops.hpp"
using namespace std;

ifstream fin;
ofstream fout;

size_t fileSize(string file) {
  size_t s = 0;
  string line;
  fin.open(file);
  while (getline(fin, line))
    s++;
  fin.close();
  return s;
}

size_t maxID(string file) {
  size_t s = fileSize(file);
  if (!s)
    return 0;
  string temp;
  fin.open(file);
  for (size_t i = 1; i < s; i++)
    getline(fin, temp);
  getline(fin, temp, ',');
  fin.close();
  return stoi(temp);
}

/*----------Fetch Entries----------*/

int fetchUserByEmail(std::string email, Student *studentHead,
                     Instructor *instructorHead) {
  for (Student *current = studentHead; current != nullptr;
       current = current->next)
    if (current->email == email)
      return current->uid;
  for (Instructor *current = instructorHead; current != nullptr;
       current = current->next)
    if (current->email == email)
      return current->uid;
  return -1;
}

/*----------Pull Entries----------*/

UniCourses pullUniCourses() {
  UniCourses courses;
  courses.head = courses.tail = nullptr;
  fin.open("data/courses.csv");
  string line;
  while (getline(fin, line)) {
    stringstream ss(line);
    Course *course = new Course;
    string element;
    getline(ss, element, ',');
    course->uid = stoi(element);
    getline(ss, element, ',');
    course->capacity = stoi(element);
    getline(ss, element, ',');
    course->name = element;
    getline(ss, element, ',');
    course->instructorUid = stoi(element);
    while (getline(ss, element, ',')) {
      Session *session = getSession(element);
      course->timeSchedule.tail = doublyInsertAtTail(
          course->timeSchedule.head, course->timeSchedule.tail, session);
    }
    courses.tail = doublyInsertAtTail(courses.head, courses.tail, course);
  }
  fin.close();
  return courses;
}

void pullAttendance(UniCourses uniCourses, Student *studentHead) {
  fin.open("data/attendance.csv");
  string line;
  while (getline(fin, line)) {
    stringstream ss(line);
    string element;
    getline(ss, element, ',');
    Course *course = fetchEntryByUid(stoi(element), uniCourses.head);
    while (getline(ss, element, ',')) {
      singlyInsertAtTail(course->present, new EnrolledStudent(fetchEntryByUid(
                                              stoi(element), studentHead)));
    }
  }
  fin.close();
}

// Fixed pulling old dropped courses
// Just Added a condition to skip adding a node to the list
// if the node == nullptr
void pullGrades(UniCourses uniCourses) {
  fin.open("data/grades.csv");
  string line;
  while (getline(fin, line)) {
    stringstream ss(line);
    string element;
    getline(ss, element, ',');
    Course *course = fetchEntryByUid(stoi(element), uniCourses.head);
    while (getline(ss, element, ',')) {
      stringstream ss2(element);
      string data;
      ss2 >> data;
      unsigned int uid = stoi(data);
      EnrolledStudent *student =
          course->fetchStudentNode(uid, course->attendees);
      if (student == nullptr)
        continue;
      ss2 >> data;
      double grade = stod(data);
      student->grade = grade;
    }
  }
  fin.close();
}

void pullAll(UniCourses &uniCourses, Student *&studentHead,
             Instructor *&instructorHead) {
  uniCourses = pullUniCourses();
  pullUsers(uniCourses, studentHead, "students");
  pullUsers(uniCourses, instructorHead, "instructors");
  uniCourses.loadAttendees(studentHead);
  pullSchedule(studentHead);
  pullGrades(uniCourses);
  pullAttendance(uniCourses, studentHead);
}

/*----------Push Entries----------*/

void pushUniCourses(UniCourses uniCourses) {
  fout.open("data/courses.csv");
  for (auto *current = uniCourses.head; current != nullptr;
       current = current->next) {
    fout << current->uid << "," << current->capacity << "," << current->name
         << "," << current->instructorUid << ",";
    for (auto *currentSession = current->timeSchedule.head;
         currentSession != nullptr; currentSession = currentSession->next) {
      fout << currentSession->day << ":" << currentSession->start.hour << " "
           << currentSession->start.minute << ":" << currentSession->end.hour
           << " " << currentSession->end.minute << ",";
    }
    fout << "\n";
  }
  fout.close();
}

void pushAttendance(UniCourses uniCourses) {
  fout.open("data/attendance.csv");
  for (auto *current = uniCourses.head; current != nullptr;
       current = current->next) {
    fout << current->uid << ",";
    for (auto *currentPresent = current->present; currentPresent != nullptr;
         currentPresent = currentPresent->next) {
      fout << currentPresent->student->uid << ",";
    }
    fout << "\n";
  }
  fout.close();
}

void pushGrades(UniCourses uniCourses, Student *studentHead) {
  fout.open("data/grades.csv");
  for (auto *current = uniCourses.head; current != nullptr;
       current = current->next) {
    fout << current->uid << ",";
    for (auto *currentStudent = current->attendees; currentStudent != nullptr;
         currentStudent = currentStudent->next) {
      fout << currentStudent->student->uid << " " << currentStudent->grade
           << ",";
    }
    fout << "\n";
  }
  fout.close();
}

void pushAll(UniCourses uniCourses, Student *studentHead,
             Instructor *instructorHead) {
  for (auto *current = instructorHead; current != nullptr;
       current = current->next) {
    deleteList(current->courses.head);
  }
  pushUniCourses(uniCourses);
  pushUsers(studentHead, "students");
  pushUsers(instructorHead, "instructors");
  pushAttendance(uniCourses);
  pushGrades(uniCourses, studentHead);
}

/*----------Sync----------*/

void deleteAll(UniCourses &uniCourses, Student *&studentHead,
               Instructor *&instructorHead) {
  deleteList(uniCourses.head);
  deleteList(studentHead);
  studentHead = nullptr;
  deleteList(instructorHead);
  instructorHead = nullptr;
}

void sync(UniCourses &uniCourses, Student *&studentHead,
          Instructor *&instructorHead) {
  pushAll(uniCourses, studentHead, instructorHead);
  deleteAll(uniCourses, studentHead, instructorHead);
  pullAll(uniCourses, studentHead, instructorHead);
}
