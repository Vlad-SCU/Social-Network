#ifndef USER_H
#define USER_H

#include <string>
#include <set>
#include <vector>
#include <iostream>

class Post;

class User {
public:
    // Pre-Condition: None
    // Post-Condition: Creates a User object with default values.
    User();

    // Pre-Condition: u_id >= 0, u_name is not empty, u_year >= 0, u_zip >= 0
    // Post-Condition: Creates a User object with the specified attributes.
    User(int u_id, const std::string& u_name, int u_year, int u_zip, const std::set<int>& u_friends);

    // Pre-Condition: i >= 0
    // Post-Condition: Adds a friend with the specified id to the user's friends set.
    void addFriend(int i);

    // Pre-Condition: i >= 0
    // Post-Condition: Removes a friend with the specified id from the user's friends set.
    void deleteFriend(int i);

    // Getter functions
    // Pre-Condition: None, Post-Condition: Returns the user's id.
    int getId() const;
    // Pre-Condition: None, Post-Condition: Returns the user's name.
    std::string getName() const;
    // Pre-Condition: None, Post-Condition: Returns the user's birth year.
    int getYear() const;
    // Pre-Condition: None, Post-Condition: Returns the user's zip code.
    int getZip() const;
    // Pre-Condition: None, Post-Condition: Returns a reference to the set of friends of the user.
    std::set<int>& getFriends();

    // Pre-Condition: Passed a valid Post
    // Post-Condition: Adds a post to the messages vector for a given user
    void addPost(Post*);

    // Pre-Condition: None
    // Post-Condition: Returns the messages vector for a given user
    std::vector<Post*> getPosts();

    // Pre-Condition: howMany > 0
    // Post-Condition: Returns a string which is the concatenation of the correct number of Posts' strings
    std::string getPostsString(int howMany, bool showOnlyPublic);

private:
    int id;
    std::string name;
    int year;
    int zip;
    std::set<int> friends;
    std::vector<Post*> messages;
};

#endif
