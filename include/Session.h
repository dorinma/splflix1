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
    ~Session();
    void start();
    void clean();
    std::string getSessionInput();
    std::unordered_map<std::string, User*> getUserMap();
    std::vector<Watchable*> getContent();
    User* getUserByString(std::string name);
    void addUserToMap(User *u);
    void addActionToLog(BaseAction* ba);
    void setActiveUser(User* user);
    void deleteUserFromMap(std::string name);

private:
    std::vector<Watchable*> content;
    std::vector<BaseAction*> actionsLog;
    std::unordered_map<std::string,User*> userMap;
    User* activeUser;
    std::string sessionInput;
};
#endif