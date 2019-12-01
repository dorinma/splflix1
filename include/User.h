#ifndef USER_H_
#define USER_H_

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>

class Watchable;
class Session;

class User{
public:
    User(std::string  name);
    User(const User &other);
    virtual User& operator=(const User& other);
    virtual ~User();
    void clean();
    void copy(const User& other);
    virtual Watchable* getRecommendation(Session& s) = 0;
    std::string getName() const;
    std::vector<Watchable*> get_history() const;
    void setToHistory (Watchable *watched);
    void setUserName(std::string newName);
    virtual User* toDuplicate(const std::string &newName, const User &oldUser) =0;
protected:
    std::vector<Watchable*> history;
private:
    std::string name;

};


class LengthRecommenderUser : public User {
public:
    LengthRecommenderUser(const std::string& name);
    LengthRecommenderUser(const LengthRecommenderUser& other);
    virtual Watchable* getRecommendation(Session& s);
    User* toDuplicate(const std::string &newName, const User &oldUser);
    //virtual LengthRecommenderUser& operator=(const LengthRecommenderUser& other);
private:
};

class RerunRecommenderUser : public User {
public:
    RerunRecommenderUser(const std::string& name);
    RerunRecommenderUser(const RerunRecommenderUser& other);
    virtual Watchable* getRecommendation(Session& s);
    User* toDuplicate(const std::string &newName, const User &oldUser);
   // virtual RerunRecommenderUser& operator=(const RerunRecommenderUser& other);
private:
};

class GenreRecommenderUser : public User {
public:
    GenreRecommenderUser(const std::string& name);
    GenreRecommenderUser(const GenreRecommenderUser& other);
    virtual Watchable* getRecommendation(Session& s);
    User* toDuplicate(const std::string &newName, const User &oldUser);
    //virtual GenreRecommenderUser& operator=(const GenreRecommenderUser& other);
private:
};

#endif
