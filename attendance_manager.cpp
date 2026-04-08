// Attendance Manager – Shortage Predictor
// Features:
// 1. Add / Delete / Update / Search Student
// 2. Mark attendance (P/A only)
// 3. Attendance summary per student
// 4. Reports: % per student, shortage <75%, class average
// 5. File storage using text files
// 6. OOP concepts used:
//    - Encapsulation
//    - Inheritance
//    - Polymorphism
//    - Exception Handling
//    - Vector + File Handling
//
// Files used:
// students.txt
// attendance.txt
//
// --------------------------------------------

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdexcept>
using namespace std;

// -------------------- BASE CLASS (Inheritance) --------------------
class Person {
protected:
    string regNo;
    string name;

public:
    Person() {}
    Person(string r, string n) : regNo(r), name(n) {}

    virtual void display() const {   // Polymorphism
        cout << "Reg No: " << regNo << ", Name: " << name << endl;
    }

    string getRegNo() const { return regNo; }
    string getName() const { return name; }

    void setName(string n) { name = n; }

    virtual ~Person() {}
};

// -------------------- DERIVED CLASS --------------------
class Student : public Person {
public:
    Student() {}
    Student(string r, string n) : Person(r, n) {}

    void display() const override {   // Runtime Polymorphism
        cout << "Student RegNo: " << regNo << " | Name: " << name << endl;
    }
};

// -------------------- ATTENDANCE RECORD --------------------
class AttendanceRecord {
private:
    string regNo;
    string date;
    char status; // P or A

public:
    AttendanceRecord() {}
    AttendanceRecord(string r, string d, char s) {
        regNo = r;
        date = d;
        status = s;
    }

    string getRegNo() const { return regNo; }
    string getDate() const { return date; }
    char getStatus() const { return status; }

    void display() const {
        cout << "RegNo: " << regNo << " | Date: " << date << " | Status: " << status << endl;
    }
};

// -------------------- ATTENDANCE MANAGER --------------------
class AttendanceManager {
private:
    vector<Student> students;
    vector<AttendanceRecord> records;

    // ---------- FILE LOAD / SAVE ----------
    void loadStudents() {
        students.clear();
        ifstream fin("students.txt");
        if (!fin) return;

        string line, regNo, name;
        while (getline(fin, line)) {
            stringstream ss(line);
            getline(ss, regNo, '|');
            getline(ss, name);
            students.push_back(Student(regNo, name));
        }
        fin.close();
    }

    void saveStudents() {
        ofstream fout("students.txt");
        for (auto &s : students) {
            fout << s.getRegNo() << "|" << s.getName() << endl;
        }
        fout.close();
    }

    void loadAttendance() {
        records.clear();
        ifstream fin("attendance.txt");
        if (!fin) return;

        string line, regNo, date, statusStr;
        while (getline(fin, line)) {
            stringstream ss(line);
            getline(ss, regNo, '|');
            getline(ss, date, '|');
            getline(ss, statusStr);

            if (!statusStr.empty()) {
                records.push_back(AttendanceRecord(regNo, date, statusStr[0]));
            }
        }
        fin.close();
    }

    void saveAttendance() {
        ofstream fout("attendance.txt");
        for (auto &r : records) {
            fout << r.getRegNo() << "|" << r.getDate() << "|" << r.getStatus() << endl;
        }
        fout.close();
    }

    // ---------- HELPER ----------
    int findStudentIndex(string regNo) {
        for (int i = 0; i < students.size(); i++) {
            if (students[i].getRegNo() == regNo)
                return i;
        }
        return -1;
    }

public:
    AttendanceManager() {
        loadStudents();
        loadAttendance();
    }

    // ---------- ADD STUDENT ----------
    void addStudent() {
        try {
            string regNo, name;
            cout << "Enter RegNo: ";
            cin >> regNo;
            cin.ignore();
            cout << "Enter Name: ";
            getline(cin, name);

            if (findStudentIndex(regNo) != -1) {
                throw runtime_error("Duplicate student not allowed!");
            }

            students.push_back(Student(regNo, name));
            saveStudents();
            cout << "Student added successfully.\n";
        }
        catch (exception &e) {
            cout << "Error: " << e.what() << endl;
        }
    }

    // ---------- DELETE STUDENT ----------
    void deleteStudent() {
        try {
            string regNo;
            cout << "Enter RegNo to delete: ";
            cin >> regNo;

            int index = findStudentIndex(regNo);
            if (index == -1) {
                throw runtime_error("Student not found!");
            }

            students.erase(students.begin() + index);

            // Also delete attendance records of that student
            for (int i = 0; i < records.size(); ) {
                if (records[i].getRegNo() == regNo)
                    records.erase(records.begin() + i);
                else
                    i++;
            }

            saveStudents();
            saveAttendance();
            cout << "Student deleted successfully.\n";
        }
        catch (exception &e) {
            cout << "Error: " << e.what() << endl;
        }
    }

    // ---------- UPDATE STUDENT ----------
    void updateStudent() {
        try {
            string regNo, newName;
            cout << "Enter RegNo to update: ";
            cin >> regNo;
            cin.ignore();

            int index = findStudentIndex(regNo);
            if (index == -1) {
                throw runtime_error("Student not found!");
            }

            cout << "Enter new name: ";
            getline(cin, newName);
            students[index].setName(newName);

            saveStudents();
            cout << "Student updated successfully.\n";
        }
        catch (exception &e) {
            cout << "Error: " << e.what() << endl;
        }
    }

    // ---------- SEARCH STUDENT ----------
    void searchStudent() {
        string regNo;
        cout << "Enter RegNo to search: ";
        cin >> regNo;

        int index = findStudentIndex(regNo);
        if (index == -1) {
            cout << "Student not found.\n";
        } else {
            students[index].display();   // Polymorphism
        }
    }

    // ---------- VIEW ALL STUDENTS ----------
    void viewStudents() {
        if (students.empty()) {
            cout << "No students found.\n";
            return;
        }

        cout << "\n--- Student List ---\n";
        for (auto &s : students) {
            s.display();
        }
    }

    // ---------- MARK ATTENDANCE ----------
    void markAttendance() {
        try {
            if (students.empty()) {
                cout << "No students available.\n";
                return;
            }

            string date;
            cout << "Enter date (DD-MM-YYYY): ";
            cin >> date;

            cout << "\nMark attendance for date: " << date << endl;

            for (auto &s : students) {
                char status;
                cout << s.getRegNo() << " - " << s.getName() << " (P/A): ";
                cin >> status;

                if (status != 'P' && status != 'A' && status != 'p' && status != 'a') {
                    throw runtime_error("Invalid input! Only P or A allowed.");
                }

                if (status == 'p') status = 'P';
                if (status == 'a') status = 'A';

                // Prevent duplicate attendance for same student on same date
                bool alreadyMarked = false;
                for (auto &r : records) {
                    if (r.getRegNo() == s.getRegNo() && r.getDate() == date) {
                        alreadyMarked = true;
                        break;
                    }
                }

                if (alreadyMarked) {
                    cout << "Attendance already marked for " << s.getRegNo() << " on " << date << ". Skipped.\n";
                } else {
                    records.push_back(AttendanceRecord(s.getRegNo(), date, status));
                }
            }

            saveAttendance();
            cout << "Attendance marking completed.\n";
        }
        catch (exception &e) {
            cout << "Error: " << e.what() << endl;
        }
    }

    // ---------- STUDENT SUMMARY ----------
    void studentSummary() {
        string regNo;
        cout << "Enter RegNo: ";
        cin >> regNo;

        int index = findStudentIndex(regNo);
        if (index == -1) {
            cout << "Student not found.\n";
            return;
        }

        int total = 0, present = 0;

        cout << "\n--- Attendance Summary ---\n";
        students[index].display();

        for (auto &r : records) {
            if (r.getRegNo() == regNo) {
                r.display();
                total++;
                if (r.getStatus() == 'P') present++;
            }
        }

        double percent = (total == 0) ? 0 : (present * 100.0 / total);

        cout << "Total Days Marked: " << total << endl;
        cout << "Present Days: " << present << endl;
        cout << "Attendance %: " << fixed << setprecision(2) << percent << "%\n";
    }

    // ---------- REPORT: ATTENDANCE % ----------
    void attendancePercentageReport() {
        if (students.empty()) {
            cout << "No students available.\n";
            return;
        }

        cout << "\n--- Attendance % Report ---\n";
        cout << left << setw(15) << "RegNo" << setw(20) << "Name" << setw(15) << "Attendance%" << endl;

        for (auto &s : students) {
            int total = 0, present = 0;

            for (auto &r : records) {
                if (r.getRegNo() == s.getRegNo()) {
                    total++;
                    if (r.getStatus() == 'P') present++;
                }
            }

            double percent = (total == 0) ? 0 : (present * 100.0 / total);

            cout << left << setw(15) << s.getRegNo()
                 << setw(20) << s.getName()
                 << setw(15) << fixed << setprecision(2) << percent << endl;
        }
    }

    // ---------- REPORT: SHORTAGE BELOW 75% ----------
    void shortageReport() {
        if (students.empty()) {
            cout << "No students available.\n";
            return;
        }

        cout << "\n--- Students Below 75% Attendance ---\n";
        bool found = false;

        for (auto &s : students) {
            int total = 0, present = 0;

            for (auto &r : records) {
                if (r.getRegNo() == s.getRegNo()) {
                    total++;
                    if (r.getStatus() == 'P') present++;
                }
            }

            double percent = (total == 0) ? 0 : (present * 100.0 / total);

            if (percent < 75) {
                found = true;
                cout << s.getRegNo() << " | " << s.getName()
                     << " | " << fixed << setprecision(2) << percent << "%\n";
            }
        }

        if (!found) {
            cout << "No shortage students.\n";
        }
    }

    // ---------- REPORT: CLASS AVERAGE ----------
    void classAverageReport() {
        if (students.empty()) {
            cout << "No students available.\n";
            return;
        }

        double totalPercent = 0;

        for (auto &s : students) {
            int total = 0, present = 0;

            for (auto &r : records) {
                if (r.getRegNo() == s.getRegNo()) {
                    total++;
                    if (r.getStatus() == 'P') present++;
                }
            }

            double percent = (total == 0) ? 0 : (present * 100.0 / total);
            totalPercent += percent;
        }

        double classAvg = totalPercent / students.size();

        cout << "\nClass Average Attendance = " << fixed << setprecision(2) << classAvg << "%\n";
    }

    // ---------- MENU ----------
    void menu() {
        int choice;

        do {
            cout << "\n========== Attendance Manager ==========\n";
            cout << "1. Add Student\n";
            cout << "2. Delete Student\n";
            cout << "3. Update Student\n";
            cout << "4. Search Student\n";
            cout << "5. View All Students\n";
            cout << "6. Mark Attendance\n";
            cout << "7. View Student Attendance Summary\n";
            cout << "8. Attendance % Report\n";
            cout << "9. Shortage Report (<75%)\n";
            cout << "10. Class Average Attendance\n";
            cout << "0. Exit\n";
            cout << "Enter choice: ";
            cin >> choice;

            switch (choice) {
                case 1: addStudent(); break;
                case 2: deleteStudent(); break;
                case 3: updateStudent(); break;
                case 4: searchStudent(); break;
                case 5: viewStudents(); break;
                case 6: markAttendance(); break;
                case 7: studentSummary(); break;
                case 8: attendancePercentageReport(); break;
                case 9: shortageReport(); break;
                case 10: classAverageReport(); break;
                case 0: cout << "Exiting program...\n"; break;
                default: cout << "Invalid choice.\n";
            }

        } while (choice != 0);
    }
};

// -------------------- MAIN --------------------
int main() {
    AttendanceManager manager;
    manager.menu();
    return 0;
}