#include <iostream>
#include <string>
#include "network.h"
#include "user.h"
#include "post.h"

using namespace std;

int main(int argc, char* argv[]) {

    Network network;

    if (network.readUsers(argv[1]) == -1) {
        cerr << "Error: Unable to read users from file " << argv[1] << endl;
        return -1;
    }
    
    network.readPosts(argv[2]);

    // Menu
    while (true) {
        cout << "Options:\n";
        cout << "1. Add a user\n";
        cout << "2. Add friend connection\n";
        cout << "3. Delete friend connection\n";
        cout << "4. Print users\n";
        cout << "5. Print friends\n";
        cout << "6. Write to file\n";
        cout << "7. Shortest Path\n";
        cout << "8. User at a given Distance\n";
        cout << "9. Friend Suggestions\n";
        cout << "10. Groups\n";
        cout << "11. Posts\n";

        int option;
        cin >> option;

        switch (option) {
            case 1: {
                // Option 1: Add a user
                string firstName, lastName, name;
                int birthYear, zipCode;
                int id = network.numUsers();
                set<int> friends;
                cin >> firstName >> lastName >> birthYear >> zipCode;
                name = firstName + " " + lastName;
                
                // Create a new user and add to the network
                User* newUser = new User(id, name, birthYear, zipCode, friends);
                network.addUser(newUser);
                break;
            }
            case 2: {
                // Option 2: Add friend connection
                string firstName1, lastName1, firstName2, lastName2, user1, user2;
                cin >> firstName1 >> lastName1 >> firstName2 >> lastName2;
                user1 = firstName1 + " " + lastName1;
                user2 = firstName2 + " " + lastName2;
               
                // Check if both users exist
                int id1 = network.getId(user1);
                int id2 = network.getId(user2);
            
                if (id1 == -1 || id2 == -1) {
                    cout << "Error: One or both users do not exist." << endl;
                } else {
                    // Add friend connection in the network
                    network.addConnection(user1, user2);
                }
                break;
            }
            case 3: {
                // Option 3: Delete friend connection
                string firstName1, lastName1, firstName2, lastName2, user1, user2;
                cin >> firstName1 >> lastName1 >> firstName2 >> lastName2;
                user1 = firstName1 + " " + lastName1;
                user2 = firstName2 + " " + lastName2;
                
                int id1 = network.getId(user1);
                int id2 = network.getId(user2);
            
                if (id1 == -1 || id2 == -1) {
                    cout << "Error: One or both users do not exist." << endl;
                } else {
                    // Check if users are friends
                    if (network.getUser(id1)->getFriends().count(id2) == 0 || 
                        network.getUser(id2)->getFriends().count(id1) == 0) {
                        cout << "Error: Users are not friends." << endl;
                    } else {
                        // Delete friend connection in the network
                        network.deleteConnection(user1, user2);
                    }
                }
                break;
            }
            case 4: {
                // Option 4: Print users
                for (int i = 0; i < network.numUsers(); i++) {
                    User* user = network.getUser(i);
                    if (user) {
                        cout << user->getId() << " " << user->getName() << endl;
                    }
                }
                break;
            }
            case 5: {
                // Option 5: Print friends
                string firstName, lastName, name;
                cin >> firstName >> lastName;
                name = firstName + " " + lastName;

                int id = network.getId(name);

                if (id != -1) {
                    const set<int>& friends = network.getUser(id)->getFriends();
                    set<int>::const_iterator it;
                    for (it = friends.begin(); it != friends.end(); it++) {
                        User* user = network.getUser(*it);
                        if (user) {
                            cout << *it << " " << user->getName() << endl;
                        }
                    }
                } else {
                    cout << "Error: User not found." << endl;
                }
                break;
            }
            case 6: {
                // Option 6: Write to file
                string filename;
                cin >> filename;

                int result = network.writeUsers(filename.c_str());
                
                if (result == 0) {
                    cout << network.numUsers() << " users wrote to file " << filename << endl;
                } else {
                    cerr << "Error: Unable to write users to file " << filename << endl;
                }
                break;
            }
            case 7: {
                // Shortest Path 
                string firstName1, lastName1, firstName2, lastName2, user1, user2;
                cin >> firstName1 >> lastName1 >> firstName2 >> lastName2;
                user1 = firstName1 + " " + lastName1;
                user2 = firstName2 + " " + lastName2;
                
                int id1 = network.getId(user1);
                int id2 = network.getId(user2);
                
                vector<int> path = network.shortestPath(id1, id2);
                if (path[0] == -1){
                    cout << "None" << endl;
                    break;
                }
                
                cout << "Distance: " << (path.size()-1) << endl;
                
                for (auto user : path){
                    string friends = network.getUser(user)->getName();
                    if (user == id1){
                        cout << friends; 
                    } else {
                        cout << " -> " << friends;
                    }
                }
                
                cout << endl;
                break;
            }  
            case 8: {
                // User at a given Distance
                string firstName, lastName, name;
                int distance, id2;
                cin >> firstName >> lastName >> distance;
                name = firstName + " " + lastName;
                
                int id1 = network.getId(name);
                
                vector<int> path = network.distanceUser(id1, id2, distance);
                
                if (path.size() > 0) {
                    cout << network.getUser(id2)->getName() << ": ";
                    
                    for (auto user : path){
                        string friends = network.getUser(user)->getName();
                        if (user == id1){
                            cout << friends; 
                        } else {
                            cout << " -> " << friends;
                        }
                    }
                    cout << endl;
                }
                break;
            }
            case 9: {
                // Friend Suggestions
                string firstName, lastName, name;
                int distance, id2, score = 0;
                cin >> firstName >> lastName;
                name = firstName + " " + lastName;
                
                if (network.getId(name) != -1){
                    int id1 = network.getId(name);
                    vector<int> path = network.suggestFriends(id1, score);
                    
                    if (path.size() > 0){
                        cout << "The suggested friend(s) is/are:" << endl;
                        
                        for (auto user : path){
                            string friends = network.getUser(user)->getName();
                            cout << friends << " Score: " << score << endl;
                        }
                    } else {
                        cout << "None" << endl;
                    }
                }
                break;
            }
            case 10: {
                // Groups
                vector<vector<int>> components = network.groups();
                vector<int> sets;
                string name;

                for (int i = 0; i < components.size(); i++) {
                    cout << "Set " << i + 1 << " => ";
                    sets = components[i];
                    for (int i = 0; i < sets.size(); i++) {
                        name = network.getUser(sets[i])->getName();
                        if (i == sets.size() - 1) {
                             cout << name;
                        } else {
                            cout << name << ", ";
                        }
                    }
                    cout << endl;
                }
                break;
            }
            case 11: {
                // Posts
                string firstName, lastName, name;
                int howMany;
            
                cin >> firstName >> lastName >> howMany;
                
                name = firstName + " " + lastName;
                int id = network.getId(name);
                
                string s = network.getPostsString(id, howMany, false);
                cout << s << endl;
            
                break;
            }
            default:
                cerr << "Invalid option." << endl;
                return 1;
        }
    }
    return 0;
}