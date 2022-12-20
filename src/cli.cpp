#include "cli.hpp"
#include "algos.hpp"
using std::cout;

bool isWindows = 0;
#ifdef _WIN32
windows = 1;
#endif

void sysPause() {
  (isWindows) ? system("pause")
              : system("echo 'Press any button to continue...' && read");
}
void sysClear() { (isWindows) ? system("cls") : system("clear"); }

UserIdentifier login(Student *studentHead, Instructor *instructorHead) {
  cout << "-----Login-----\n";
  cout << "Email Address\n";
  string email = validateEmail();
  int uid;
  uid = fetchUserByEmail(email, studentHead, instructorHead);
  if (uid == -1) {
    cout << "User not found!\n";
    return login(studentHead, instructorHead);
  }
  string type = "";
  Student *student = fetchEntryByUid(uid, studentHead);
  Instructor *instructor = fetchEntryByUid(uid, instructorHead);
  if (student)
    type = "student";
  else if (instructor)
    type = "instructor";
  cout << "Password\n";
  string pass = getPass();
  string userPass;
  if (type == "student") {
    userPass = student->password;
  } else if (type == "instructor") {
    userPass = instructor->password;
  }
  if (userPass == pass) {
    cout << "Login success!\n";
    return {type, uid};
  }
  cout << "Login failed!\n";
  return {type, -1};
}

int studentMenu(Student *student, UniCourses &uniCourses, Student *&studentHead,
                Instructor *&instructorHead) {
  cout << "-----Student Menu-----\n";
  string options[] = {"Add course",
                      "Drop course",
                      "Swap courses",
                      "View schedule",
                      "View course participants",
                      "View course attendance",
                      "View grades",
                      "Logout"};
  size_t size = sizeof(options) / sizeof(string);
  int choice = selectChoice(options, size);
  switch (choice) {
  case 1: {
    cout << "Enroll Course\n";
    student->enrollCourse(uniCourses);
    break;
  }
  case 2: {
    cout << "Drop Course\n";
    student->dropCourse();
    break;
  }
  case 3: {
    cout << "Swap Courses\n";
    student->swapCourses(uniCourses);
    break;
  }
  case 4: {
    cout << "Your Schedule\n";
    student->courses.displayCalendar();
    break;
  }
  case 5: {
    cout << "Course Participants\n";
    Course *course = getCourse(student, studentHead);
    if (course == nullptr) {
      cout << "Course unavailable!\n";
      break;
    }
    course->viewAttendees();
    break;
  }
  case 6: {
    cout << "Course Attendance\n";
    Course *course = getCourse(student, studentHead);
    if (course == nullptr) {
      cout << "Course unavailable!\n";
      break;
    }
    course->viewAttendance(studentHead);
    break;
  }
  case 7: {
    cout << "Course Grades\n";
    student->viewGrades();
    break;
  }
  case 8: {
    return -1;
  }
  default: {
    cout << "Invalid option!\n";
    return studentMenu(student, uniCourses, studentHead, instructorHead);
  }
  }
  sysPause();
  unsigned int uid = student->uid;
  sync(uniCourses, studentHead, instructorHead);
  return uid;
}

int instructorMenu(Instructor *instructor, UniCourses &uniCourses,
                   Student *&studentHead, Instructor *&instructorHead) {
  cout << "-----Instructor Menu-----\n";
  string options[] = {"View Schedule", "Take attendance",        "Add grades",
                      "Add course",    "Add sessions to Course", "Logout"};
  int size = sizeof(options) / sizeof(string);
  int choice = selectChoice(options, size);
  instructor->loadCourses(uniCourses);
  instructor->courses.loadSchedule();
  switch (choice) {
  case 1: {
    cout << "Your Schedule\n";
    instructor->courses.displayCalendar();
    break;
  }
  case 2: {
    cout << "Take Attendance\n";
    Course *course = getCourse(instructor, instructorHead);
    if (course == nullptr) {
      cout << "Course unavailable!\n";
      break;
    }
    instructor->takeAttendance(course, studentHead);
    break;
  }
  case 3: {
    cout << "Grade Students\n";
    Course *course = getCourse(instructor, instructorHead);
    if (course == nullptr) {
      cout << "Course unavailable!\n";
      break;
    }
    instructor->gradeStudents(course, studentHead);
    break;
  }
  case 4: {
    cout << "Add Course\n";
    instructor->addCourse(uniCourses);
    break;
  }
  case 5: {
    cout << "Add Sessions\n";
    Course *course = getCourse(instructor, instructorHead);
    if (course == nullptr) {
      cout << "Course unavailable!\n";
      break;
    }
    instructor->addSchedule(course);
  }
  case 6: {
    deleteList(instructor->courses.head);
    instructor->courses.head = instructor->courses.tail = nullptr;
    return -1;
  }
  }
  sysPause();
  unsigned int uid = instructor->uid;
  deleteList(instructor->courses.head);
  instructor->courses.head = instructor->courses.tail = nullptr;
  sync(uniCourses, studentHead, instructorHead);
  return uid;
}

void firstSetup(UniCourses &uniCourses, Student *&studentHead,
                Instructor *&instructorHead) {
  cout << "No Data Found; Entering first-time setup...\n";
  cout << "Creating files...\n";
  fout.open("data/courses.csv");
  fout.close();
  fout.open("data/students.csv");
  fout.close();
  fout.open("data/instructors.csv");
  fout.close();
  fout.open("data/attendance.csv");
  fout.close();
  fout.open("data/grades.csv");
  fout.close();
  cout << "Created files.\n";
  sync(uniCourses, studentHead, instructorHead);
}

void start(UniCourses &uniCourses, Student *&studentHead,
           Instructor *&instructorHead) {
  sysClear();
  cout << "Loading data...\n";
  size_t s = fileSize("data/courses.csv");
  if (!s)
    firstSetup(uniCourses, studentHead, instructorHead);
  else
    pullAll(uniCourses, studentHead, instructorHead);
  cout << "Loaded data.\n";
  sysPause();
}
