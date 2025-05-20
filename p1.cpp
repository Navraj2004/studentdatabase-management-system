#include <iostream>
#include <string>
using namespace std;

struct Student {
    int roll;
    string name;
    float marks;
    Student* next;
};

// Linked List (Main Database)
Student* head = nullptr;

// Stack (For Undo)
Student* undoStack = nullptr;

// Queue (For Processing)
Student* front = nullptr;
Student* rear = nullptr;

// --- Helper Functions ---

// Push to stack (for undo)
void pushToStack(Student* student) {
    Student* newNode = new Student{student->roll, student->name, student->marks, undoStack};
    undoStack = newNode;
}

// Enqueue (for processing)
void enqueue(Student* student) {
    Student* newNode = new Student{student->roll, student->name, student->marks, nullptr};
    if (!front) {
        front = rear = newNode;
    } else {
        rear->next = newNode;
        rear = newNode;
    }
}

// --- Main Operations ---

// 1. Add Student
void addStudent(int roll, string name, float marks) {
    Student* newStudent = new Student{roll, name, marks, nullptr};

    // Add to linked list
    if (!head) {
        head = newStudent;
    } else {
        Student* temp = head;
        while (temp->next) {
            temp = temp->next;
        }
        temp->next = newStudent;
    }

    // Push to stack (for undo)
    pushToStack(newStudent);

    // Enqueue (for processing)
    enqueue(newStudent);

    cout << "Student added successfully.\n";
}

// 2. Display All Students
void displayStudents() {
    if (!head) {
        cout << "No students in the database.\n";
        return;
    }

    cout << "\n--- Student List ---\n";
    Student* temp = head;
    while (temp) {
        cout << "Roll: " << temp->roll << ", Name: " << temp->name << ", Marks: " << temp->marks << "\n";
        temp = temp->next;
    }
}

// 3. Search Student
void searchStudent(int roll) {
    Student* temp = head;
    while (temp) {
        if (temp->roll == roll) {
            cout << "Found - Name: " << temp->name << ", Marks: " << temp->marks << "\n";
            return;
        }
        temp = temp->next;
    }
    cout << "Student with roll " << roll << " not found.\n";
}

// 4. Undo Last Insert
void undoLastInsert() {
    if (!undoStack) {
        cout << "Nothing to undo.\n";
        return;
    }

    int rollToRemove = undoStack->roll;
    Student* temp = head;
    Student* prev = nullptr;

    // Find and remove the student from the linked list
    while (temp && temp->roll != rollToRemove) {
        prev = temp;
        temp = temp->next;
    }

    if (temp) {
        if (prev) {
            prev->next = temp->next;
        } else {
            head = temp->next;
        }
        cout << "Undo: Removed student - " << temp->name << "\n";
        delete temp; // Free memory
    }

    // Pop from stack
    temp = undoStack;
    undoStack = undoStack->next;
    delete temp;
}

// 5. Process Queue (FIFO)
void processQueue() {
    if (!front) {
        cout << "Queue is empty.\n";
        return;
    }

    cout << "Processing student: " << front->name << "\n";
    Student* temp = front;
    front = front->next;
    delete temp;

    if (!front) {
        rear = nullptr;
    }
}

// --- Menu ---
int main() {
    while (true) {
        cout << "\n--- Student Database Menu ---\n";
        cout << "1. Add Student\n2. Display All\n3. Search\n4. Undo Last Insert\n5. Process Queue\n6. Exit\n";
        cout << "Enter choice: ";
        
        int choice;
        cin >> choice;

        if (choice == 1) {
            int roll;
            string name;
            float marks;
            cout << "Enter roll: ";
            cin >> roll;
            cout << "Enter name: ";
            cin.ignore();
            getline(cin, name);
            cout << "Enter marks: ";
            cin >> marks;
            addStudent(roll, name, marks);
        } 
        else if (choice == 2) {
            displayStudents();
        } 
        else if (choice == 3) {
            int roll;
            cout << "Enter roll to search: ";
            cin >> roll;
            searchStudent(roll);
        } 
        else if (choice == 4) {
            undoLastInsert();
        } 
        else if (choice == 5) {
            processQueue();
        } 
        else if (choice == 6) {
            break;
        } 
        else {
            cout << "Invalid choice. Try again.\n";
        }
    }

    // Free remaining memory (optional, but good practice)
    while (head) {
        Student* temp = head;
        head = head->next;
        delete temp;
    }
    while (undoStack) {
        Student* temp = undoStack;
        undoStack = undoStack->next;
        delete temp;
    }
    while (front) {
        Student* temp = front;
        front = front->next;
        delete temp;
    }

    return 0;
}