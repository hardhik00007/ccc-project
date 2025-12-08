#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <algorithm>
using namespace std;

const string STUDENT_FILE = "students.txt";
const string ATTEND_FILE = "attendance.txt";
const char DELIM = '|';

// Utility: get today's date YYYY-MM-DD
string todayDate() {
    time_t t = time(nullptr);
    tm *lt = localtime(&t);
    char buf[11];
    snprintf(buf, sizeof(buf), "%04d-%02d-%02d",
             lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday);
    return string(buf);
}

struct StudentProfile {
    int id;
    string name;
    string dob;
    string address;
    string year;
    double cgpa;
};

struct AttendanceRecord {
    int id;
    string subject;
    string date;   // YYYY-MM-DD
    string status; // "Present" or "Absent"
};

// Helpers to trim whitespace (small utility)
static inline void rtrim(string &s) {
    while (!s.empty() && isspace((unsigned char)s.back())) s.pop_back();
}
static inline void ltrim(string &s) {
    while (!s.empty() && isspace((unsigned char)s.front())) s.erase(s.begin());
}
static inline void trim(string &s) { ltrim(s); rtrim(s); }

// ====== Profiles: load / save / append ======
vector<StudentProfile> loadProfiles() {
    vector<StudentProfile> v;
    ifstream ifs(STUDENT_FILE);
    if (!ifs.is_open()) return v;
    string line;
    while (getline(ifs, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string field;
        StudentProfile p;
        // id
        if (!getline(ss, field, DELIM)) continue;
        trim(field);
        try { p.id = stoi(field); } catch(...) { continue; }
        // name
        if (!getline(ss, p.name, DELIM)) continue;
        trim(p.name);
        // dob
        if (!getline(ss, p.dob, DELIM)) continue;
        trim(p.dob);
        // address
        if (!getline(ss, p.address, DELIM)) continue;
        trim(p.address);
        // year
        if (!getline(ss, p.year, DELIM)) continue;
        trim(p.year);
        // cgpa
        if (!getline(ss, field, DELIM)) continue;
        trim(field);
        try { p.cgpa = stod(field); } catch(...) { p.cgpa = 0.0; }
        v.push_back(p);
    }
    return v;
}

bool saveAllProfiles(const vector<StudentProfile> &v) {
    ofstream ofs(STUDENT_FILE, ios::trunc);
    if (!ofs.is_open()) return false;
    for (const auto &p : v) {
        ofs << p.id << DELIM
            << p.name << DELIM
            << p.dob << DELIM
            << p.address << DELIM
            << p.year << DELIM
            << fixed << setprecision(2) << p.cgpa << DELIM
            << "\n";
    }
    return true;
}

bool appendProfile(const StudentProfile &p) {
    ofstream ofs(STUDENT_FILE, ios::app);
    if (!ofs.is_open()) return false;
    ofs << p.id << DELIM
        << p.name << DELIM
        << p.dob << DELIM
        << p.address << DELIM
        << p.year << DELIM
        << fixed << setprecision(2) << p.cgpa << DELIM
        << "\n";
    return true;
}

// ====== Attendance: load / append / rewrite ======
vector<AttendanceRecord> loadAttendance() {
    vector<AttendanceRecord> v;
    ifstream ifs(ATTEND_FILE);
    if (!ifs.is_open()) return v;
    string line;
    while (getline(ifs, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string field;
        AttendanceRecord r;
        // id
        if (!getline(ss, field, DELIM)) continue;
        trim(field);
        try { r.id = stoi(field); } catch(...) { continue; }
        // subject
        if (!getline(ss, r.subject, DELIM)) continue;
        trim(r.subject);
        // date
        if (!getline(ss, r.date, DELIM)) continue;
        trim(r.date);
        // status
        if (!getline(ss, r.status, DELIM)) continue;
        trim(r.status);
        v.push_back(r);
    }
    return v;
}

bool appendAttendance(const AttendanceRecord &r) {
    ofstream ofs(ATTEND_FILE, ios::app);
    if (!ofs.is_open()) return false;
    ofs << r.id << DELIM
        << r.subject << DELIM
        << r.date << DELIM
        << r.status << DELIM
        << "\n";
    return true;
}

bool rewriteAttendance(const vector<AttendanceRecord> &v) {
    ofstream ofs(ATTEND_FILE, ios::trunc);
    if (!ofs.is_open()) return false;
    for (const auto &r : v) {
        ofs << r.id << DELIM
            << r.subject << DELIM
            << r.date << DELIM
            << r.status << DELIM
            << "\n";
    }
    return true;
}

// ====== Utility: find profile by id ======
int findProfileIndex(const vector<StudentProfile> &v, int id) {
    for (size_t i = 0; i < v.size(); ++i)
        if (v[i].id == id) return (int)i;
    return -1;
}

// ====== Feature: Add student profile ======
void addStudentProfile() {
    StudentProfile p;
    cout << "Enter Admission ID (integer): ";
    while (!(cin >> p.id)) {
        cout << "Invalid input. Enter numeric Admission ID: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Enter Full Name: ";
    getline(cin, p.name);
    cout << "Enter Date of Birth (YYYY-MM-DD): ";
    getline(cin, p.dob);
    cout << "Enter Address: ";
    getline(cin, p.address);
    cout << "Enter Year of Study (e.g., 2nd Year): ";
    getline(cin, p.year);
    cout << "Enter CGPA (e.g., 8.25): ";
    string s;
    getline(cin, s);
    trim(s);
    try { p.cgpa = stod(s); } catch(...) { p.cgpa = 0.0; }

    // check duplicate id
    vector<StudentProfile> profiles = loadProfiles();
    if (findProfileIndex(profiles, p.id) != -1) {
        cout << "A student with this ID already exists. Aborting.\n";
        return;
    }
    if (appendProfile(p)) cout << "Student profile added successfully.\n";
    else cout << "Failed to write student file. Check permissions.\n";
}

// ====== Feature: View student details by admission number ======
void viewStudentDetails() {
    cout << "Enter Admission ID to search: ";
    int id;
    while (!(cin >> id)) {
        cout << "Invalid input. Enter numeric Admission ID: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    vector<StudentProfile> profiles = loadProfiles();
    int idx = findProfileIndex(profiles, id);
    if (idx == -1) {
        cout << "No student found with ID " << id << ".\n";
        return;
    }
    StudentProfile &p = profiles[idx];
    cout << "\n--- Student Profile ---\n";
    cout << "Admission ID : " << p.id << "\n";
    cout << "Name         : " << p.name << "\n";
    cout << "DOB          : " << p.dob << "\n";
    cout << "Address      : " << p.address << "\n";
    cout << "Year         : " << p.year << "\n";
    cout << "CGPA         : " << fixed << setprecision(2) << p.cgpa << "\n";

    // compute attendance summary from attendance file
    vector<AttendanceRecord> records = loadAttendance();
    int total = 0, present = 0;
    map<string, pair<int,int>> subjCount; // subject -> (total, present)
    for (const auto &r : records) {
        if (r.id != id) continue;
        total++;
        string st = r.status;
        // normalize
        string stl = st; transform(stl.begin(), stl.end(), stl.begin(), ::tolower);
        bool isPresent = (stl.find("present") != string::npos) || (stl == "y") || (stl == "yes");
        if (isPresent) present++;
        auto &pairRef = subjCount[r.subject];
        pairRef.first += 1;
        if (isPresent) pairRef.second += 1;
    }

    cout << "\n--- Attendance Summary ---\n";
    if (total == 0) {
        cout << "No attendance records found for this student.\n";
    } else {
        double overall = (double)present / total * 100.0;
        cout << "Total classes recorded : " << total << "\n";
        cout << "Total present         : " << present << "\n";
        cout << "Overall attendance    : " << fixed << setprecision(2) << overall << "%\n";

        cout << "\nSubject-wise breakdown:\n";
        cout << left << setw(20) << "Subject" << setw(10) << "Total" << setw(10) << "Present" << setw(12) << "Percent" << "\n";
        cout << string(54, '-') << "\n";
        for (const auto &item : subjCount) {
            const string &subj = item.first;
            int t = item.second.first;
            int pr = item.second.second;
            double perc = (t == 0) ? 0.0 : (double)pr / t * 100.0;
            cout << left << setw(20) << subj << setw(10) << t << setw(10) << pr << setw(12) << fixed << setprecision(2) << perc << "\n";
        }
    }
    cout << "\n";
}

// ====== Feature: Take attendance for all students for a subject (today) ======
void takeAttendanceToday() {
    vector<StudentProfile> profiles = loadProfiles();
    if (profiles.empty()) {
        cout << "No students registered. Add profiles first.\n";
        return;
    }
    string subject;
    cout << "Enter Subject Name: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, subject);
    string date = todayDate();
    cout << "Taking attendance for " << subject << " on " << date << "\n";

    for (const auto &p : profiles) {
        char ch;
        cout << "Is " << p.name << " (ID " << p.id << ") present? (y/n): ";
        while (!(cin >> ch)) { cin.clear(); cin.ignore(1); }
        string status = (ch == 'y' || ch == 'Y') ? "Present" : "Absent";
        AttendanceRecord rec{p.id, subject, date, status};
        if (!appendAttendance(rec)) {
            cout << "Failed to append attendance for ID " << p.id << "\n";
        }
    }
    cout << "Attendance recorded for all students.\n";
}

// ====== Feature: Add past attendance for a subject & date (for all students) ======
void addPastAttendanceForAll() {
    vector<StudentProfile> profiles = loadProfiles();
    if (profiles.empty()) {
        cout << "No students registered. Add profiles first.\n";
        return;
    }
    string subject, date;
    cout << "Enter Subject Name: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, subject);
    cout << "Enter Date (YYYY-MM-DD): ";
    getline(cin, date);

    cout << "Adding past attendance for " << subject << " on " << date << "\n";
    for (const auto &p : profiles) {
        char ch;
        cout << "Was " << p.name << " (ID " << p.id << ") present on " << date << "? (y/n): ";
        while (!(cin >> ch)) { cin.clear(); cin.ignore(1); }
        string status = (ch == 'y' || ch == 'Y') ? "Present" : "Absent";
        AttendanceRecord rec{p.id, subject, date, status};
        if (!appendAttendance(rec)) {
            cout << "Failed to append attendance for ID " << p.id << "\n";
        }
    }
    cout << "Past attendance entries added successfully.\n";
}

// ====== Feature: View all students (basic) ======
void listAllStudents() {
    auto profiles = loadProfiles();
    if (profiles.empty()) {
        cout << "No students registered yet.\n";
        return;
    }
    cout << left << setw(8) << "ID" << setw(25) << "Name" << setw(12) << "Year" << setw(8) << "CGPA" << "\n";
    cout << string(55, '-') << "\n";
    for (const auto &p : profiles) {
        cout << left << setw(8) << p.id << setw(25) << p.name << setw(12) << p.year << setw(8) << fixed << setprecision(2) << p.cgpa << "\n";
    }
}

// ====== Feature: Delete student (profile + attendance) ======
void deleteStudentByID() {
    cout << "Enter Admission ID to delete: ";
    int id;
    while (!(cin >> id)) {
        cout << "Invalid input. Enter numeric Admission ID: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    vector<StudentProfile> profiles = loadProfiles();
    int idx = findProfileIndex(profiles, id);
    if (idx == -1) {
        cout << "No student found with ID " << id << ".\n";
        return;
    }
    // remove profile
    profiles.erase(profiles.begin() + idx);
    if (!saveAllProfiles(profiles)) {
        cout << "Failed to update profiles file. Aborting delete.\n";
        return;
    }
    // remove attendance records
    vector<AttendanceRecord> recs = loadAttendance();
    vector<AttendanceRecord> kept;
    kept.reserve(recs.size());
    for (const auto &r : recs) if (r.id != id) kept.push_back(r);
    if (!rewriteAttendance(kept)) {
        cout << "Failed to update attendance file. Manual cleanup required.\n";
        return;
    }
    cout << "Student profile and attendance removed successfully.\n";
}

// ====== Feature: View attendance report (all) ======
void viewAttendanceReportAll() {
    vector<StudentProfile> profiles = loadProfiles();
    vector<AttendanceRecord> recs = loadAttendance();
    if (profiles.empty()) {
        cout << "No students registered.\n";
        return;
    }
    // build map id -> (total, present)
    map<int, pair<int,int>> totals;
    for (const auto &r : recs) {
        totals[r.id].first += 1;
        string s = r.status; transform(s.begin(), s.end(), s.begin(), ::tolower);
        if (s.find("present") != string::npos || s == "y" || s == "yes") totals[r.id].second += 1;
    }

    cout << left << setw(8) << "ID" << setw(25) << "Name" << setw(12) << "Total" << setw(12) << "Present" << setw(12) << "Percent" << "\n";
    cout << string(70, '-') << "\n";
    for (const auto &p : profiles) {
        int t = totals[p.id].first;
        int pr = totals[p.id].second;
        double perc = (t == 0) ? 0.0 : (double)pr / t * 100.0;
        cout << left << setw(8) << p.id << setw(25) << p.name << setw(12) << t << setw(12) << pr << setw(12) << fixed << setprecision(2) << perc << "\n";
    }
}

// ====== MENU ======
void showMenu() {
    cout << "\n===== STUDENT INFORMATION & ATTENDANCE SYSTEM =====\n";
    cout << "1. Add New Student Profile\n";
    cout << "2. Add Attendance for Today (subject-wise)\n";
    cout << "3. Add Past Attendance (subject + date)\n";
    cout << "4. View Student Details by Admission ID\n";
    cout << "5. List All Students\n";
    cout << "6. View Attendance Report (All Students)\n";
    cout << "7. Delete Student (Profile + Attendance)\n";
    cout << "8. Exit\n";
    cout << "Enter your choice: ";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Ensure files exist (create if not)
    {
        ofstream ofs;
        ofs.open(STUDENT_FILE, ios::app); ofs.close();
        ofs.open(ATTEND_FILE, ios::app); ofs.close();
    }

    int ch = 0;
    do {
        showMenu();
        if (!(cin >> ch)) {
            cout << "Invalid input. Enter a number between 1-8.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        switch (ch) {
            case 1: addStudentProfile(); break;
            case 2: takeAttendanceToday(); break;
            case 3: addPastAttendanceForAll(); break;
            case 4: viewStudentDetails(); break;
            case 5: listAllStudents(); break;
            case 6: viewAttendanceReportAll(); break;
            case 7: deleteStudentByID(); break;
            case 8: cout << "Exiting. Data files: '" << STUDENT_FILE << "', '" << ATTEND_FILE << "'.\n"; break;
            default: cout << "Invalid choice. Try again.\n";
        }
    } while (ch != 8);

    return 0;
}
