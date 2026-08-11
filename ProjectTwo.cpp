//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Rayia Lagard
// Course      : CS 300
// Description : ABCU Advising Assistance Program
//
// This program reads course data from a comma-separated text file, stores each
// course in a binary search tree, prints all courses in alphanumeric order,
// and displays a selected course with its prerequisite numbers and titles.
//============================================================================

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// Stores the information for one course.
struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;
};

// Removes spaces from the beginning and end of a string.
string trim(const string& value) {
    size_t first = value.find_first_not_of(" \t\r\n");

    if (first == string::npos) {
        return "";
    }

    size_t last = value.find_last_not_of(" \t\r\n");
    return value.substr(first, last - first + 1);
}

// Converts a string to uppercase so course searches are not case-sensitive.
string toUpperCase(string value) {
    for (size_t i = 0; i < value.length(); ++i) {
        value[i] = static_cast<char>(
            toupper(static_cast<unsigned char>(value[i]))
        );
    }

    return value;
}

// Represents one node in the binary search tree.
struct Node {
    Course course;
    Node* left;
    Node* right;

    Node(const Course& courseValue) {
        course = courseValue;
        left = NULL;
        right = NULL;
    }
};

// Binary search tree that stores all course objects.
class BinarySearchTree {
private:
    Node* root;

    // Deletes all nodes recursively to release allocated memory.
    void deleteTree(Node* node) {
        if (node == NULL) {
            return;
        }

        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }

    // Inserts one course according to its course number.
    Node* insertNode(Node* node, const Course& course) {
        if (node == NULL) {
            return new Node(course);
        }

        if (course.courseNumber < node->course.courseNumber) {
            node->left = insertNode(node->left, course);
        } else if (course.courseNumber > node->course.courseNumber) {
            node->right = insertNode(node->right, course);
        } else {
            // Replace duplicate course records with the newest record.
            node->course = course;
        }

        return node;
    }

    // In-order traversal prints the courses in alphanumeric order.
    void printInOrder(Node* node) const {
        if (node == NULL) {
            return;
        }

        printInOrder(node->left);
        cout << node->course.courseNumber << ", "
             << node->course.courseTitle << endl;
        printInOrder(node->right);
    }

    // Copying is disabled to protect the dynamically allocated tree.
    BinarySearchTree(const BinarySearchTree&);
    BinarySearchTree& operator=(const BinarySearchTree&);

public:
    BinarySearchTree() {
        root = NULL;
    }

    ~BinarySearchTree() {
        deleteTree(root);
        root = NULL;
    }

    // Clears old data before loading a different course file.
    void clear() {
        deleteTree(root);
        root = NULL;
    }

    // Inserts a course into the tree.
    void insert(const Course& course) {
        root = insertNode(root, course);
    }

    // Searches for a course using its course number.
    const Course* search(const string& courseNumber) const {
        Node* current = root;

        while (current != NULL) {
            if (courseNumber == current->course.courseNumber) {
                return &(current->course);
            }

            if (courseNumber < current->course.courseNumber) {
                current = current->left;
            } else {
                current = current->right;
            }
        }

        return NULL;
    }

    // Prints every course in sorted order.
    void printCourseList() const {
        printInOrder(root);
    }

    bool empty() const {
        return root == NULL;
    }
};

// Splits a comma-separated line into individual values.
vector<string> splitLine(const string& line) {
    vector<string> tokens;
    string token;
    stringstream lineStream(line);

    while (getline(lineStream, token, ',')) {
        tokens.push_back(trim(token));
    }

    return tokens;
}

// Loads and validates course records before inserting them into the tree.
bool loadCourses(const string& fileName, BinarySearchTree& courseTree) {
    ifstream inputFile(fileName.c_str());

    if (!inputFile.is_open()) {
        cerr << "Error: Unable to open file \"" << fileName << "\"." << endl;
        return false;
    }

    vector<Course> courses;
    string line;
    int lineNumber = 0;

    // Parse each line into one Course object.
    while (getline(inputFile, line)) {
        ++lineNumber;

        if (trim(line).empty()) {
            continue;
        }

        vector<string> tokens = splitLine(line);

        if (tokens.size() < 2 || tokens[0].empty() || tokens[1].empty()) {
            cerr << "Error: Invalid file format on line "
                 << lineNumber
                 << ". Each line needs a course number and title."
                 << endl;
            inputFile.close();
            return false;
        }

        Course course;
        course.courseNumber = toUpperCase(tokens[0]);
        course.courseTitle = tokens[1];

        for (size_t index = 2; index < tokens.size(); ++index) {
            if (!tokens[index].empty()) {
                course.prerequisites.push_back(
                    toUpperCase(tokens[index])
                );
            }
        }

        courses.push_back(course);
    }

    inputFile.close();

    if (courses.empty()) {
        cerr << "Error: The selected file does not contain course data."
             << endl;
        return false;
    }

    // Confirm that every prerequisite appears as a course in the file.
    for (size_t i = 0; i < courses.size(); ++i) {
        for (size_t j = 0;
             j < courses[i].prerequisites.size();
             ++j) {

            string prerequisite = courses[i].prerequisites[j];
            bool prerequisiteExists = false;

            for (size_t k = 0; k < courses.size(); ++k) {
                if (courses[k].courseNumber == prerequisite) {
                    prerequisiteExists = true;
                    break;
                }
            }

            if (!prerequisiteExists) {
                cerr << "Error: " << prerequisite
                     << " is listed as a prerequisite for "
                     << courses[i].courseNumber
                     << " but does not exist in the file."
                     << endl;
                return false;
            }
        }
    }

    // Replace any previously loaded data only after validation succeeds.
    courseTree.clear();

    for (size_t i = 0; i < courses.size(); ++i) {
        courseTree.insert(courses[i]);
    }

    cout << courses.size()
         << " courses loaded successfully."
         << endl;

    return true;
}

// Displays the requested course and its prerequisite numbers and titles.
void displayCourseInformation(
    const BinarySearchTree& courseTree,
    const string& requestedCourseNumber) {

    string normalizedNumber =
        toUpperCase(trim(requestedCourseNumber));

    const Course* course =
        courseTree.search(normalizedNumber);

    if (course == NULL) {
        cout << "Course "
             << normalizedNumber
             << " was not found."
             << endl;
        return;
    }

    cout << course->courseNumber
         << ", "
         << course->courseTitle
         << endl;

    cout << "Prerequisites: ";

    if (course->prerequisites.empty()) {
        cout << "None" << endl;
        return;
    }

    for (size_t i = 0;
         i < course->prerequisites.size();
         ++i) {

        string prerequisiteNumber =
            course->prerequisites[i];

        const Course* prerequisiteCourse =
            courseTree.search(prerequisiteNumber);

        if (i > 0) {
            cout << "; ";
        }

        cout << prerequisiteNumber;

        if (prerequisiteCourse != NULL) {
            cout << ", "
                 << prerequisiteCourse->courseTitle;
        }
    }

    cout << endl;
}

// Safely reads a numeric menu choice.
int readMenuChoice() {
    int choice;

    cout << "What would you like to do? ";

    while (!(cin >> choice)) {
        cin.clear();
        cin.ignore(
            numeric_limits<streamsize>::max(),
            '\n'
        );

        cout << "Invalid input. Enter 1, 2, 3, or 9: ";
    }

    cin.ignore(
        numeric_limits<streamsize>::max(),
        '\n'
    );

    return choice;
}

int main() {
    BinarySearchTree courseTree;
    bool dataLoaded = false;
    int choice = 0;

    cout << "Welcome to the ABCU Course Planner." << endl;

    while (choice != 9) {
        cout << endl;
        cout << "1. Load Data Structure." << endl;
        cout << "2. Print Course List." << endl;
        cout << "3. Print Course." << endl;
        cout << "9. Exit" << endl;

        choice = readMenuChoice();

        switch (choice) {
            case 1: {
                string fileName;

                cout << "Enter the course data file name: ";
                getline(cin, fileName);

                dataLoaded =
                    loadCourses(trim(fileName), courseTree);
                break;
            }

            case 2:
                if (!dataLoaded || courseTree.empty()) {
                    cout << "Please load the course data first."
                         << endl;
                } else {
                    cout << endl;
                    cout << "Here is a sample schedule:"
                         << endl
                         << endl;

                    courseTree.printCourseList();
                }
                break;

            case 3: {
                if (!dataLoaded || courseTree.empty()) {
                    cout << "Please load the course data first."
                         << endl;
                    break;
                }

                string courseNumber;

                cout << "What course do you want to know about? ";
                getline(cin, courseNumber);

                displayCourseInformation(
                    courseTree,
                    courseNumber
                );

                break;
            }

            case 9:
                cout << "Thank you for using the course planner!"
                     << endl;
                break;

            default:
                cout << choice
                     << " is not a valid option."
                     << endl;
                break;
        }
    }

    return 0;
}
