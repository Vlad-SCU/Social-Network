#include "network.h"
#include "user.h"
#include "post.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <queue>
#include <stack>
#include <algorithm>

Network::Network() {
}

User* Network::getUser(int id) const{
    for (auto user : users) {
        if (user->getId() == id) {
            return user;
        }
    }
    return nullptr;
}

void Network::addUser(User* user) {
    int i = users.size();
    std::string name1 = user->getName();
    int year1 = user->getYear();
    int zip1 = user->getZip();
    const std::set<int>& friends1 = user->getFriends();

    // Creating a new user with a unique ID and adding it to the network
    User* newUser = new User(i, name1, year1, zip1, friends1);
    users.push_back(newUser);
}

int Network::addConnection(const std::string s1, const std::string s2) {
    int s1Id, s2Id;
    bool found1 = false;
    bool found2 = false;

    // Finding the IDs of the users with the given names, making sure users are different
    if (s1 != s2) {
        for (auto user : users) {
            if (user->getName() == s1) {
                s1Id = user->getId();
                found1 = true;
            }
            if (user->getName() == s2) {
                s2Id = user->getId();
                found2 = true;
            }
        }
    }

    // Adding a friend connection between the two users (assuming both are valid)
    if (found1 && found2) {
        for (auto user : users) {
            if (user->getName() == s1) {
                user->addFriend(s2Id);
            }
            if (user->getName() == s2) {
                user->addFriend(s1Id);
            }
        }
        return 0;
    }

    return -1; // Failure
}

int Network::deleteConnection(const std::string s1, const std::string s2) {
    int s1Id, s2Id;
    bool found1 = false;
    bool found2 = false;

    // Finding the IDs of the users with the given names, making sure users are different
    if (s1 != s2) {
        for (auto user : users) {
            if (user->getName() == s1) {
                s1Id = user->getId();
                found1 = true;
            }
            if (user->getName() == s2) {
                s2Id = user->getId();
                found2 = true;
            }
        }
    }

    // Deleting a friend connection between the two users
    if (found1 && found2) {
        for (auto user : users) {
            if (user->getName() == s1) {
                user->deleteFriend(s2Id);
            }
            if (user->getName() == s2) {
                user->deleteFriend(s1Id);
            }
        }
        return 0;
    }

    return -1; // Failure
}

int Network::getId(const std::string name) const {
    for (auto user : users) {
        if (user->getName() == name) {
            return user->getId();
        }
    }

    return -1; // User not found
}

int Network::numUsers() const {
    return users.size();
}

int Network::readUsers(const char* fname) {
    std::ifstream infile(fname);

    if (!infile.is_open()) {
        std::cerr << "Error: Unable to open file " << fname << std::endl;
        return -1; // Failure
    }

    std::string line, firstName, lastName, name;
    int fileNum, id, year, zip;
    std::set<int> friends;

    getline(infile, line);
    std::istringstream iss(line);
    iss >> fileNum;

    for (int i = 0; i < fileNum; i++) {
        std::getline(infile, line);
        std::istringstream iss(line);
        iss >> id;

        std::getline(infile, line);
        std::istringstream iss2(line);
        iss2 >> firstName >> lastName;
        name = firstName + " " + lastName;

        std::getline(infile, line);
        std::istringstream iss3(line);
        iss3 >> year;

        std::getline(infile, line);
        std::istringstream iss4(line);
        iss4 >> zip;

        std::getline(infile, line);
        std::istringstream iss5(line);
        int friendId;
        while (iss5 >> friendId) {
            friends.insert(friendId);
        }

        // Create a User with the friends set
        User* newUser = new User(id, name, year, zip, friends);
        addUser(newUser);

        // Ensures friends doesn't carry over from previous user
        friends.clear();
    }

    infile.close();
    return 0;
}

int Network::writeUsers(const char* fname) {
    std::ofstream outfile(fname);

    if (!outfile.is_open()) {
        std::cerr << "Error: Unable to open file " << fname << std::endl;
        return -1; // Failure
    }

    outfile << users.size() << '\n';

    for (auto user : users) {
        outfile << user->getId() << '\n' << '\t' << user->getName()
                << '\n' << '\t' << user->getYear() << '\n' << '\t'
                << user->getZip() << '\n' << '\t';

        const std::set<int>& friends = user->getFriends();
        for (auto friendId : friends) {
            outfile << friendId << " ";
        }
        outfile << '\n';
    }

    outfile.close();
    return 0;
}

std::vector<int> Network::shortestPath(int from, int to) {
    int n = users.size();
    std::queue<int> q;
    std::vector<bool> visited (n, 0);
    std::vector<int> prev (n, -1);
    std::vector<int> output;

    if (from == to) {
        output.push_back(from);
        return output;
    }

    visited[from] = true;
    q.push(from);

    while (q.size() > 0) {
        int cur = q.front();
        q.pop();

        std::set<int>& neighbors = users[cur]->getFriends();

        for (auto neighbor : neighbors) {
            if (!visited[neighbor]) {
                prev[neighbor] = cur;
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }

    for (int i = 0; i < n; i++) {
        if (visited[i] == false) {
            std::vector<int> nopath = {-1};
            return nopath;
        }
    }

    int cur = to;

    while (cur != from) {
        output.push_back(cur);
        cur = prev[cur];
    }

    output.push_back(from);
    reverse(output.begin(), output.end());

    return output;
}

std::vector<int> Network::distanceUser(int from, int& to, int distance) {
    to = -1;
    int n = users.size();
    std::queue<int> q;
    std::vector<bool> visited (n, 0);
    std::vector<int> prev (n, -1);
    std::vector<int> dist (n, -1);

    dist[from] = 0;
    visited[from] = true;
    q.push(from);

    while (q.size() > 0) {
        int cur = q.front();
        q.pop();

        std::set<int>& neighbors = users[cur]->getFriends();

        for (auto neighbor : neighbors) {
            if (!visited[neighbor]) {
                dist[neighbor] = dist[cur] + 1;
                prev[neighbor] = cur;
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }

    for (int i = 0; i < n; i++) {
        if (dist[i] == distance) {
            to = i;
            break;
        }
    }

    std::vector<int> output;
    int cur = to;

    if (cur != -1) {
        while (cur != from) {
            output.push_back(cur);
            cur = prev[cur];
        }
        output.push_back(from);
        reverse(output.begin(), output.end());
    }

    return output;
}

std::vector<int> Network::suggestFriends(int who, int& score) {
    int n = users.size();
    int temp = 0;
    std::queue<int> q;
    std::vector<bool> visited (n, 0);
    std::vector<int> dist (n, -1);
    std::vector<int> common (n, 0);

    dist[who] = 0;
    visited[who] = true;
    q.push(who);

    std::set<int>& source = users[who]->getFriends();

    while (q.size() > 0) {
        int cur = q.front();
        q.pop();

        std::set<int>& neighbors = users[cur]->getFriends();

        for (auto neighbor : neighbors) {
            if (!visited[neighbor] && dist[cur] < 2) {
                dist[neighbor] = dist[cur] + 1;
                visited[neighbor] = true;
                q.push(neighbor);
            } else if (dist[cur] == 2) {
                for (auto vertex : source) {
                    if (neighbor == vertex) {
                        common[cur] += 1;
                    }
                }
            }
        }
    }

    for (auto com : common) {
        if (com >= temp) {
            temp = com;
        }
    }

    score = temp;
    std::vector<int> output;

    // Checks if a friend at distance 2 still exists but has a score of 0
    for (int i = 0; i < n ; i++) {
        if (score == 0) {
            if (dist[i] == 2) {
                output.push_back(i);
            }
        } else if (common[i] == score) {
            output.push_back(i);
        }
    }

    return output;
}

std::vector<std::vector<int>> Network::groups() {
    int n = users.size();
    std::stack<int> s;
    std::vector<std::vector<int>> output;
    std::vector<int> component;
    std::vector<bool> visited (n, 0);
    std::vector<bool> connected (n, 0);

    visited[0] = true;
    s.push(0);

    while (s.size() > 0) {
        int cur = s.top();
        s.pop();

        std::set<int>& neighbors = users[cur]->getFriends();

        for (auto neighbor : neighbors) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                s.push(neighbor);
            }
        }

        if (s.size() == 0) {
            for (int i = 0; i < n; i++) {
                if (visited[i] && !connected[i]) {
                    component.push_back(i);
                    connected[i] = true;
                }
            }

            output.push_back(component);

            for (int i = 0; i < n; i++) {
                if (!visited[i]) {
                    visited[i] = true;
                    s.push(i);
                    break;
                }
            }
            component.clear();
        }
    }

    return output;
}

void Network::addPost(int ownerId, std::string message, int likes, bool isIncoming, std::string authorName, bool isPublic) {
    User* u = Network::getUser(ownerId);
    int messageId = nextMessageId++;

    if (isIncoming) {
        IncomingPost * ip = new IncomingPost(messageId, ownerId, message, likes, isPublic, authorName);
        u->addPost(ip);
    } else {
        Post* p = new Post(messageId, ownerId, message, likes);
        u->addPost(p);
    }
}

std::string Network::getPostsString(int ownerId, int howMany, bool showOnlyPublic) {
    std::string s = Network::getUser(ownerId)->getPostsString(howMany, showOnlyPublic);

    return s;
}

int Network::readPosts(const char* fname) {
    std::ifstream file;
    file.open(fname);
    nextMessageId = 0;

    if (!file.is_open()) {
        return -1;
    }

    int n;
    file >> n;

    for (int i = 0; i < n; i ++) {
        std::string line, message ="", temp, pub;
        std::string author = "", authorFirst = "", authorLast;
        bool isPublic = false;
        int messageId, ownerId, likes;

        file >> messageId;

        std::getline(file, line);
        std::getline(file, line);
        std::istringstream iss(line);
        while (iss >> temp) {
            message += temp + " ";
        }

        if (!message.empty()) {
            message.pop_back();
        }

        file >> ownerId;
        file >> likes;

        std::getline(file, line);
        std::getline(file, pub);

        std::getline(file, line);
        std::istringstream iss2(line);
        iss2 >> authorFirst >> authorLast;

        author = authorFirst + " " + authorLast;

        if (pub == "\tpublic") {
            isPublic = true;
        }

        if (authorFirst == "") {
            Network::addPost(ownerId, message, likes, false, "", true);
        } else {
            Network::addPost(ownerId, message, likes, true, author, isPublic);
        }

        message = "";
    }

    return 0;
}

bool comp(Post* p1, Post* p2) {
    return p1->getMessageId() < p2->getMessageId();
}

int Network::writePosts(const char* fname) {
    std::ofstream outfile(fname);

    if (!outfile.is_open()) {
        return -1;
    }

    std::vector<Post*> allPosts;

    for (int i = 0; i < users.size(); i++) {
        std::vector<Post*> uPosts = Network::getUser(i)->getPosts();
        for (auto post : uPosts) {
            allPosts.push_back(post);
        }
    }

    outfile << allPosts.size() << '\n';
    std::sort (allPosts.begin(), allPosts.end(), comp);

    std::string isPublic;

    for (auto post : allPosts) {
        if (post->getIsPublic() && post->getAuthor() != "") {
            isPublic = "public";
        } else if (!post->getIsPublic()) {
            isPublic = "private";
        } else {
            isPublic = "";
        }
        outfile << post->getMessageId() << '\n' << '\t' << post->getMessage()
                << '\n' << '\t' << post->getOwnerId() << '\n' << '\t'
                << post->getLikes() << '\n' << '\t' << isPublic
                << '\n' << '\t' << post->getAuthor() << '\n';
    }

    return 0;
}

std::set<int> Network::mutualFriends(User* user1, User* user2) {
    std::set<int>mutuals;

    for (auto friend1 : user1->getFriends()) {
        for (auto friend2 : user2->getFriends()) {
            if (friend2 == friend1) {
                mutuals.insert(friend2);
            }
        }
    }

    return mutuals;
}
