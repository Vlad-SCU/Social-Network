#include "network.h"
#include "user.h"
#include "post.h"
#include <algorithm>

Post::Post() {
}

Post::Post(int messageId, int ownerId, std::string message, int likes)
    : messageId(messageId), ownerId(ownerId), message(message), likes(likes) {
}

std::string Post::toString() {
    std::string s = message + " Liked by " + std::to_string(likes) + " people.";
    return s;
}

int Post::getMessageId() const {
    return messageId;
}

int Post::getOwnerId() const {
    return ownerId;
}

std::string Post::getMessage() const {
    return message;
}

int Post::getLikes() const {
    return likes;
}

std::string Post::getAuthor() {
    return "";
}

bool Post::getIsPublic() {
    return true;
}

IncomingPost::IncomingPost() {
}

IncomingPost::IncomingPost(int messageId, int ownerId, std::string message, int likes, bool isPublic, std::string author)
    : Post(messageId, ownerId, message, likes), isPublic(isPublic), author(author) {
}

std::string IncomingPost::toString() {
    std::string temp;

    if (isPublic == true) {
        temp = "";
    } else {
        temp = "(private)";
    }

    std::string s = author + " wrote" + temp + ": " + Post::toString();
    return s;
}

std::string IncomingPost::getAuthor() {
    return author;
}

bool IncomingPost::getIsPublic() {
    return isPublic;
}
