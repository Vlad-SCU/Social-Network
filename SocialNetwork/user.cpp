#include "user.h"
#include "post.h"

User::User() : id(0), year(0), zip(0) {
}

User::User(int u_id, const std::string& u_name, int u_year, int u_zip, const std::set<int>& u_friends)
    : id(u_id), name(u_name), year(u_year), zip(u_zip), friends(u_friends) {
}

void User::addFriend(int i) {
    friends.insert(i);
}

void User::deleteFriend(int i) {
    friends.erase(i);
}

int User::getId() const {
    return id;
}

std::string User::getName() const {
    return name;
}

int User::getYear() const {
    return year;
}

int User::getZip() const {
    return zip;
}

std::set<int>& User::getFriends() {
    return friends;
}

void User::addPost(Post* post) {
    std::string author = post->getAuthor();

    if (author == "") {
        messages.push_back(post);
    } else {
        messages.push_back(post);
    }
}

std::vector<Post*> User::getPosts() {
    return messages;
}

std::string User::getPostsString(int howMany, bool showOnlyPublic) {
    std::string s;
    std::string temp;

    for (int i = messages.size()-1; i >= 0; i--) {
        if (howMany > 0 && messages.size() - i > howMany) {
            break;
        }

        if (showOnlyPublic == true) {
            if (messages[i]->getIsPublic()) {
                temp = messages[i]->toString();
                s += temp + '\n' + '\n';
            }
        } else {
            temp = messages[i]->toString();
            s += temp + '\n' + '\n';
        }
    }

    if (!s.empty()) {
        s.pop_back();
        s.pop_back();
    }

    return s;
}
