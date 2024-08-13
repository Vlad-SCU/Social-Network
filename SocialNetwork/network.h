#ifndef NETWORK_H
#define NETWORK_H

#include <vector>
#include <string>
#include "user.h"

class Network {
public:
    // Pre-condition: None
    // Post-condition: Creates a Network object with default values
    Network();

    // Pre-condition: id is a valid user ID
    // Post-condition: Returns a pointer to the User with the specified ID. Returns nullptr if the user is not found
    User* getUser(int id) const;

    // Pre-condition: user passesd to method is a valid User object
    // Post-condition: Adds the given user to the network
    void addUser(User*);

    // Pre-condition: s1 and s2 are valid user names
    // Post-condition: Adds a connection between users with names s1 and s2. Returns 0 on success, -1 if either user is invalid
    int addConnection(const std::string s1, const std::string s2);

    // Pre-condition: s1 and s2 are valid user names
    // Post-condition: Deletes the connection between users with names s1 and s2. Returns 0 on success, -1 if either user is invalid
    int deleteConnection(const std::string s1, const std::string s2);

    // Pre-condition: name is a valid user name
    // Post-condition: Returns the ID of the user with the specified name. Returns -1 if the user is not found
    int getId(const std::string name) const;

    // Pre-condition: None
    // Post-condition: Returns the number of users in the network
    int numUsers() const;

    // Pre-condition: fname is a valid file name
    // Post-condition: Initializes all of the network’s information from a file. Returns 0 on success, -1 on failure
    int readUsers(const char* fname);

    // Pre-condition: fname is a valid file name
    // Post-condition: Writes all of the network’s information to a file. Returns 0 on success, -1 on failure
    int writeUsers(const char* fname);

    // Pre-condition: "from" and "to" are valid user ids
    // Post-condition: Returns a list of ids on the shortest path from "from" to "to"
    std::vector<int> shortestPath(int from, int to);

    // Pre-condition: from is a valid user id and a user exists of distance "distance" away
    // Post-condition: Returns a list of ids on the path from the source to the user of distance "distance" away
    std::vector<int> distanceUser(int from, int& to, int distance);

    // Pre-condition: who is a valid user id
    // Post-condition: Returns a list of the ids of users who have the highest score
    std::vector<int> suggestFriends(int who, int& score);

    // Pre-condition: At least one user exists in the network
    // Post-condition: Returns a list of lists, where each list holds the ids of the users in one connected component
    std::vector<std::vector<int>> groups();

    // Pre-condition: ownerId >= 0, likes >= 0
    // Post-condition: Adds a post to the messages vector of the user whose id is ownerId
    void addPost(int ownerId, std::string message, int likes, bool isIncoming, std::string authorName, bool isPublic);

    // Pre-condition: ownerId >= 0, howMany >= 0
    // Post-condition: Returns a string which is the concatenation of the correct number of Posts' strings for the user whose id is ownerId
    std::string getPostsString(int ownerId, int howMany, bool showOnlyPublic);

    // Pre-condition: fname is a valid file name
    // Post-condition: Initializes all of the network’s posts information from a file. Returns 0 on success, -1 on failure
    int readPosts(const char* fname);

    // Pre-condition: fname is a valid file name
    // Post-condition: Writes all of the network’s posts information to a file. Returns 0 on success, -1 on failure
    int writePosts(const char* fname);

    std::set<int> mutualFriends(User* user1, User* user2);

private:
    std::vector<User*> users;
    int nextMessageId;
};

#endif
