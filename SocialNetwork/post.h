#ifndef POST_H
#define POST_H

#include <vector>
#include <string>

class Post {
public:
    // Pre-condition: None
    // Post-condition: Creates a Post object with default values
    Post();

    // Pre-Condition: messageId >= 0,ownerid >= 0, likes >= 0
    // Post-Condition: Creates a Post object with the specified attributes.
    Post(int messageId, int ownerId, std::string message, int likes);

    // Pre-condition: None
    // Post-condition: Returns a string of Post in proper format
    std::string virtual toString();

    // Pre-condition: None
    // Post-condition: Returns messageId of Post
    int getMessageId() const;

    // Pre-condition: None
    // Post-condition: Returns ownerId of Post
    int getOwnerId() const;

    // Pre-condition: None
    // Post-condition: Returns message of Post
    std::string getMessage() const;

    // Pre-condition: None
    // Post-condition: Returns likes of Post
    int getLikes() const;

    // Pre-condition: None
    // Post-condition: Returns an empty string
    std::string virtual getAuthor();

    // Pre-condition: None
    // Post-condition: Returns true
    bool virtual getIsPublic();

private:
    int messageId;
    int ownerId;
    std::string message;
    int likes;
};

class IncomingPost : public Post {
public:
    // Pre-condition: None
    // Post-condition: Creates an IncomingPost object with default values
    IncomingPost();

    // Pre-Condition: messageId >= 0, ownerid >= 0, likes >= 0
    // Post-Condition: Creates an IncomingPost object with the specified attributes.
    IncomingPost(int messageId, int ownerId, std::string message, int likes, bool isPublic, std::string author);

    // Pre-condition: None
    // Post-condition: Returns a string of IncomingPost in proper format
    std::string toString();

    // Pre-condition: None
    // Post-condition: Returns author of IncomingPost
    std::string getAuthor();

    // Pre-condition: None
    // Post-condition: Returns isPublic value of IncomingPost
    bool getIsPublic();

private:
    std::string author;
    bool isPublic;
};

#endif
