

#include "../include/Action.h"
#include "../include/Session.h"
#include "../include/User.h"


//-----BaseAction------
BaseAction::BaseAction() {}
ActionStatus BaseAction::getStatus() const { return this->status; };
void BaseAction::complete() { this->status = COMPLETED; }
void BaseAction::error(const std::string &errorMsg) {
    status = ERROR;
    this->errorMsg = errorMsg;
}
std::string BaseAction::getErrorMsg() const { return " Error: %n", errorMsg; }


//-----Create user------
CreateUser::CreateUser () {}
void CreateUser::act(Session& sess) {
    std::string userInput = sess.getSessionInput();
    std::string reco = userInput.substr(userInput.length()-3);
    int end = userInput.length()-5-userInput.find(" ");
    std::string name = userInput.substr(userInput.find(" ")+1, end);

    User *newUser;
    if (sess.getUserByString(name) != nullptr)
    {
        this->error("user with this name exist already");
        return;
    }
    if (reco == "len"){ newUser = new LengthRecommenderUser(name);}
    else if (reco == "rer"){ newUser = new RerunRecommenderUser(name);}
    else if (reco == "gen"){ newUser = new GenreRecommenderUser(name);}
    else {this->error("wrong recommend was typed");}

    if (getStatus()!= ERROR) {
        complete();
        sess.addUserToMap(newUser);
        sess.addActionToLog(this);
    }
    this->toString();
}
std::string CreateUser::toString() const {
    if (this->getStatus() == ERROR)
        std::cout<< this->getErrorMsg();
    else if (this->getStatus() == COMPLETED)
        std::cout<< "new user was created succesfuly";
    else
        std::cout<< "unkown error";
}


//------Change active user-----
ChangeActiveUser::ChangeActiveUser () {}
void ChangeActiveUser::act(Session& sess) {}
std::string ChangeActiveUser::toString() const {}


//------Delete user------
DeleteUser::DeleteUser() {}
void DeleteUser::act(Session& sess) {}
std::string DeleteUser::toString() const {}


//Duplicate user
DuplicateUser::DuplicateUser() {}
void DuplicateUser::act(Session& sess) {}
std::string DuplicateUser::toString() const {}


//Print content list
PrintContentList::PrintContentList() {}
void PrintContentList::act(Session& sess) {}
std::string PrintContentList::toString() const {}


//Print watch history
PrintWatchHistory::PrintWatchHistory() {}
void PrintWatchHistory::act(Session& sess) {}
std::string PrintWatchHistory::toString() const {}


//Print actions log
PrintActionsLog::PrintActionsLog() {}
void PrintActionsLog::act(Session& sess) {}
std::string PrintActionsLog::toString() const {}


//Watch
Watch::Watch() {}
void Watch::act(Session& sess) {}
std::string Watch::toString() const {}


//Exit
Exit::Exit() {}
void Exit::act(Session& sess) {}
std::string Exit::toString() const {}

