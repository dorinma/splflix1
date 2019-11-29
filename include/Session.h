#ifndef SESSION_H_
#define SESSION_H_

#include <vector>
#include <unordered_map>
#include <string>
#include "Action.h"

class User;
class Watchable;

class Session{
public:
    Session(const std::string &configFilePath);
    std::string nowPlaying;
    ~Session();
    Session(const Session &other);
    Session& operator=(const Session& other);
    void start();
    void clean();
    std::string getSessionInput();
    std::unordered_map<std::string, User*> getUserMap();
    User* getUserByString(const std::string &name);
    User* getActiveUser();
    void addUserToMap(User *u);
    Watchable* getSomethingToWatch(const std::string &id);
    void addToHistory(Watchable *watched);
    void addActionToLog(BaseAction* ba);
    void setActiveUser(User* user);
    void deleteUserFromMap(const std::string &name);
    std::vector<Watchable*> getContent();
    std::vector<BaseAction*> getActionLog();
    void setTerminate(bool toContinue);

private:
    std::vector<Watchable*> content;
    std::vector<BaseAction*> actionsLog;
    std::unordered_map<std::string,User*> userMap;
    User* activeUser;
    std::string sessionInput;
    bool terminate = false;
};
#endif
