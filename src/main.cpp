#include "cli.hpp"
#include "dbops.hpp"
#include <cstdlib>
using std::cerr;
using std::cout;

int main() {
  UniCourses uniCourses;
  Student *studentHead = nullptr;
  Instructor *instructorHead = nullptr;
  start(uniCourses, studentHead, instructorHead);
  string loginChoices[] = {"Login", "Register", "Quit"};
  while (1) {
    UserIdentifier userPtr;
    sysClear();
    cout << "-----Main Menu-----\n";
    switch (selectChoice(loginChoices, 3)) {
    case 1: {
      sysClear();
      userPtr = login(studentHead, instructorHead);
      break;
    }
    case 2: {
      sysClear();
      cout << "Select user type\n";
      string userTypes[] = {"Student", "Instructor"};
      switch (selectChoice(userTypes, 3)) {
      case 1:
        createUser(studentHead, studentHead, instructorHead);
        break;
      case 2:
        createUser(instructorHead, studentHead, instructorHead);
        break;
      default:
        cout << "Invalid option!\n";
      }
      break;
    }
    case 3: {
      sysClear();
      return 0;
    }
    case 4: {
      cout << "Invalid option!\n";
    }
    }
    sysPause();
    sync(uniCourses, studentHead, instructorHead);
    if (!fout) {
      cerr << "Error writing to file!\n";
      return EXIT_FAILURE;
    }
    while (userPtr.uid != -1) {
      sysClear();
      if (userPtr.type == "admin") {
      } else if (userPtr.type == "student") {
        Student *student = fetchEntryByUid(userPtr.uid, studentHead);
        userPtr.uid =
            studentMenu(student, uniCourses, studentHead, instructorHead);
      } else {
        Instructor *instructor = fetchEntryByUid(userPtr.uid, instructorHead);
        userPtr.uid =
            instructorMenu(instructor, uniCourses, studentHead, instructorHead);
      }
    }
  }
}
