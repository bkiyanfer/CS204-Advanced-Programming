//Beril Kiyanfer 30565

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

struct NodeLike {
    string subject;
    string object;
    NodeLike *next;
    NodeLike(string subj, string obj, NodeLike *nxt = nullptr) : subject(subj), object(obj), next(nxt) {}
};

struct NodeTaken {
    string name;
    NodeTaken *next;
    NodeTaken(string nm, NodeTaken *nxt = nullptr) : name(nm), next(nxt) {}
};

bool searchTakenList(NodeTaken* head, const string& name) {
    NodeTaken* current = head;
    while (current != nullptr) {
        if (current->name == name) return true;
        current = current->next;
    }
    return false;
}

bool searchLikesList(NodeLike* head, const string& subject, const string& object) {
    NodeLike* current = head;
    while (current != nullptr) {
        if (current->subject == subject && current->object == object) return true;
        current = current->next;
    }
    return false;
}

void insertIntoTakenList(NodeTaken*& head, const string& name) {
    NodeTaken* newNode = new NodeTaken(name);
    if (!head || head->name > name) {
        newNode->next = head;
        head = newNode;
    } else {
        NodeTaken* current = head;
        while (current->next && current->next->name < name) current = current->next;
        newNode->next = current->next;
        current->next = newNode;
    }
}

void removeFromLikesList(NodeLike*& head, const string& name) {
    NodeLike *current = head, *prev = nullptr;
    while (current != nullptr) {
        if (current->subject == name || current->object == name) {
            if (prev) prev->next = current->next;
            else head = current->next;

            NodeLike* temp = current;
            current = current->next;
            delete temp;
        } else {
            prev = current;
            current = current->next;
        }
    }
}

void insertIntoLikesList(NodeLike*& headLikes, const string& subject, const string& object, NodeTaken* headTaken) {
    if (searchTakenList(headTaken, subject) || searchTakenList(headTaken, object) || searchLikesList(headLikes, subject, object)) return;

    NodeLike* newNode = new NodeLike(subject, object);
    if (!headLikes) {
        // If the list is empty, this becomes the first node.
        headLikes = newNode;
    } else {
        NodeLike* temp = headLikes;
        NodeLike* lastSubjectNode = nullptr;

        // Find the last node where subject matches or reach the end of the list.
        while (temp != nullptr) {
            if (temp->subject == subject) {
                lastSubjectNode = temp; // Last node with the same subject found.
            }
            if (temp->next == nullptr) break; // End of the list reached.
            temp = temp->next;
        }

        // Insertion logic
        if (lastSubjectNode != nullptr) {
            // If a node with the same subject was found, insert after the last such node.
            if (lastSubjectNode->next != nullptr) {
                // If the last subject node is not the last node in the list.
                newNode->next = lastSubjectNode->next;
                lastSubjectNode->next = newNode;
            } else {
                // If the last subject node is the last node in the list.
                lastSubjectNode->next = newNode;
            }
        } else {
            // No node with the same subject was found, insert at the end.
            temp->next = newNode;
        }
    }
}


void printLists(NodeLike* headLikes, NodeTaken* headTaken) {
    cout << "Likes list: ";
    while (headLikes) {
        cout << "(" << headLikes->subject << ", " << headLikes->object << ") ";
        headLikes = headLikes->next;
    }
    cout << "\nTaken list: ";
    while (headTaken) {
        cout << headTaken->name << " ";
        headTaken = headTaken->next;
    }
    cout << "\n\n";
}

void deleteLists(NodeLike*& headLikes, NodeTaken*& headTaken) {
    while (headLikes) {
        NodeLike* temp = headLikes;
        headLikes = headLikes->next;
        delete temp;
    }
    while (headTaken) {
        NodeTaken* temp = headTaken;
        headTaken = headTaken->next;
        delete temp;
    }
}

void processLine(NodeLike*& headLikes, NodeTaken*& headTaken, const string& line, int lineCount) {
    istringstream iss(line);
    string subject, verb, object;
    iss >> subject >> verb >> object;

    // Print the line being processed
    cout <<"Read line number (" << lineCount << "): " << subject << " likes " << object << endl;

    if (searchTakenList(headTaken, subject) || searchTakenList(headTaken, object)) {
        // Case when either or both names are in the taken list
        cout << "Either " << subject << " or " << object << " or both is/are already taken, so the like relation is not added." << endl;
    } else if (searchLikesList(headLikes, object, subject)) {
        // Mutual like found
        cout << "Match found: " << subject << " likes " << object << " and " << object << " likes " << subject << "."<<endl;
        cout << "Added to taken list: " << subject << endl;
        cout << "Added to taken list: " << object << endl;
        cout << "Any node that has " << subject << " or " << object << " or both in it is removed from the likes list." << endl;

        // The actual insertion and removal logic
        if (!searchTakenList(headTaken, subject)) insertIntoTakenList(headTaken, subject);
        if (!searchTakenList(headTaken, object)) insertIntoTakenList(headTaken, object);
        removeFromLikesList(headLikes, subject);
        removeFromLikesList(headLikes, object);
    } else if (!searchLikesList(headLikes, subject, object)) {
        // New like relation not existing in the list
        insertIntoLikesList(headLikes, subject, object, headTaken);
        cout << subject << " likes " << object << " relation has been added to the likes list." << endl;
    } else {
        // Relation already exists
        cout << subject << " likes " << object << " relation already exists in the likes list, so it is not added to it." << endl;
    }

    // Print the current state of the lists
    printLists(headLikes, headTaken);
}

void processFile(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cout << "Could not open the file." << endl;
        return;
    }

    NodeLike* headLikes = nullptr;
    NodeTaken* headTaken = nullptr;
    string line;
    int lineCount = 0;

    while (getline(file, line)) {
        // Call the processLine function for each line with updated arguments
        processLine(headLikes, headTaken, line, ++lineCount);
    }

    // Delete lists and clean up
    deleteLists(headLikes, headTaken);
    file.close();
    cout << "Lists are deleted and the program ends successfully." << endl;
}


int main() {
    string filename;
    cout << "Please enter the file name: "<<endl;
    while (getline(cin, filename)) {
        ifstream file(filename);
        if (file) {
            file.close();
            break;
        }

        cout << "Could not open the file. Please enter a valid file name: "<<endl;
    }

    processFile(filename);
    return 0;
}
