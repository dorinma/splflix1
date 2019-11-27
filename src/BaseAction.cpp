

#include <sstream>
#include "../include/Action.h"
#include "../include/Session.h"
#include "../include/User.h"
#include "../include/Watchable.h"


using namespace std;


//-----BaseAction------
BaseAction::BaseAction() {}
ActionStatus BaseAction::getStatus() const { return this->status; };
void BaseAction::complete() { this->status = COMPLETED; }
void BaseAction::error(const std::string &errorMsg) {
    status = ERROR;
    this->errorMsg = errorMsg;
}
std::string BaseAction::getErrorMsg() const { return errorMsg;} //" Error: %n", errorMsg; }


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
    }
    else {
        if (reco == "len") { newUser = new LengthRecommenderUser(name); }
        else if (reco == "rer") { newUser = new RerunRecommenderUser(name); }
        else if (reco == "gen") { newUser = new GenreRecommenderUser(name); }
        else { this->error("wrong recommend was typed"); }

        if (getStatus() != ERROR) {
            complete();
            sess.addUserToMap(newUser);
            sess.addActionToLog(this);
        }
    }
    this->toString();
}
std::string CreateUser::toString() const {
    if (this->getStatus() == ERROR)
        cout<< this->getErrorMsg() << endl;
    else if (this->getStatus() == COMPLETED)
        cout<< "new user was created succesfuly" <<endl;
    else
        cout<< "unkown error" <<endl;
}


//------Change active user-----
ChangeActiveUser::ChangeActiveUser () {}
void ChangeActiveUser::act(Session& sess) {
    std::string userInput = sess.getSessionInput();
    std::string changedUser = userInput.substr(userInput.find(" "));
    User *user;
    user = sess.getUserByString(changedUser);
    if (sess.getUserByString(changedUser) == nullptr) {
         this->error("user to change does not exist");
    }
    else {
        sess.setActiveUser(user);
        complete();
        sess.addActionToLog(this);
    }
    this->toString();
}
std::string ChangeActiveUser::toString() const {
    if (this->getStatus() == ERROR)
        cout<< this->getErrorMsg() << endl;
    else if (this->getStatus() == COMPLETED)
        cout<< "user changed" << endl;
    else
        cout<< "unkown error" <<endl;
}


//------Delete user------
DeleteUser::DeleteUser() {}
void DeleteUser::act(Session& sess) {
    std::string userInput = sess.getSessionInput();
    std::string userToDelete = userInput.substr(userInput.find(" ")+1);
    User *user;
    user = sess.getUserByString(userToDelete);
    if (user == nullptr){
    this->error("user to delete does not exist");
    }
    else{
        sess.setActiveUser(sess.getUserByString("default"));
        sess.deleteUserFromMap(userToDelete);
        complete();
        sess.addActionToLog(this);
    }
    this->toString();
}
std::string DeleteUser::toString() const {
    if (this->getStatus() == ERROR)
        cout<< this->getErrorMsg()<< endl;
    else if (this->getStatus() == COMPLETED)
        cout<< "user deleted" << endl;
    else
        cout<< "unkown error" << endl;
}


//Duplicate user
DuplicateUser::DuplicateUser() {}
void DuplicateUser::act(Session& sess) {
    std::string userInput = sess.getSessionInput();
    std::string oldUserName = userInput.substr(userInput.find(" "), userInput.find(" ")-1);
    std::string newUserName = userInput.substr(userInput.find(" "));
    User *oldUser;
    User *newUser;
    oldUser = sess.getUserByString(oldUserName);
    if (oldUser == nullptr)
    {this->error("user to duplicate from does not exist");}
    newUser = sess.getUserByString(oldUserName);
    if (oldUser != nullptr)
    {this->error("this user name exist already");}
    else {
        newUser = oldUser;
        complete();
        sess.addActionToLog(this);
    }
    this->toString();
}
std::string DuplicateUser::toString() const {}


//Print content list
PrintContentList::PrintContentList() {}
void PrintContentList::act(Session& sess) {

    vector<Watchable *> contentList = sess.getContent();
    for (int i = 0; i < contentList.size(); i++)
    {
        long id = contentList[i]->getId();
        string name = contentList[i]->toString();
        int length = contentList[i]->getLength();
        vector<string> thisTags = contentList[i]->getTags();
        string tags = "";
        for (int k=0; k<thisTags.size(); k++) {
            tags = tags + thisTags[k] + ", ";
        }
        tags = tags.substr(0, tags.size()-2);
        cout << id << ". " << name << " " << length << " minutes [" << tags << "]" << endl;

    }
    complete();
    sess.addActionToLog(this);
}
std::string PrintContentList::toString() const {
    if (this->getStatus() == ERROR)
        cout<< this->getErrorMsg()<< endl;
    else if (this->getStatus() == COMPLETED)
        cout<< "content was printed" << endl;
    else
        cout<< "unkown error" << endl;
}


//Print watch history
PrintWatchHistory::PrintWatchHistory() {}
void PrintWatchHistory::act(Session& sess) {
    string name = sess.getActiveUser()->getName();
    cout << "Watch history for " << name << endl;
    vector<Watchable *> history = sess.getActiveUser()->get_history();
    if (history.size() == 0)
        cout <<"nothing was watched. you need to stop studying" << endl;
    for (int i = 0; i < history.size(); i++)
    {
        long id = history[i]->getId();
        string name = history[i]->toString();
        cout << id << ". " << name << endl;

    }
    complete();
    sess.addActionToLog(this);
}
std::string PrintWatchHistory::toString() const {}


//Print actions log
PrintActionsLog::PrintActionsLog() {}
void PrintActionsLog::act(Session& sess) {
    vector<BaseAction *> action = sess.getActionLog();
    for (int i = 0; i < action.size(); i++)
    {
      cout << action[i]->toString();
    }
    complete();
    sess.addActionToLog(this);
}
std::string PrintActionsLog::toString() const {}


//Watch
Watch::Watch() {}
void Watch::act(Session& sess) {

    std::string userInput = sess.getSessionInput();
    std::string idToWatch = userInput.substr(userInput.find(" ")+1);

        if (sess.getSomethingToWatch(idToWatch) == nullptr) {
            this->error("content with this id doesnt exist");
        } else {
            sess.nowPlaying = idToWatch;
            cout << "Watching " << sess.getSomethingToWatch(idToWatch)->toString() << endl;
            complete();
            sess.addToHIstory(sess.getSomethingToWatch(idToWatch));
            sess.addActionToLog(this);

            bool toContinueWatching = true;
            Watchable* nextWatchable = sess.getActiveUser()->getRecommendation(sess);
            cout << "We recommend watching " << nextWatchable->toString() << " continue watching? [y/n]" <<endl;
            string input;
            getline (cin >> ws, input);
            if(input == "n")
                toContinueWatching = false;
            while (toContinueWatching)
            {
                cout << "Watching " << nextWatchable->toString() << endl;
               int nextId = nextWatchable->getId();

                stringstream ss;
                ss << nextId;
                sess.nowPlaying = ss.str();

                complete();
                sess.addToHIstory(nextWatchable);
                sess.addActionToLog(this);

                nextWatchable = sess.getActiveUser()->getRecommendation(sess);
                cout << "We recommend watching " << nextWatchable->toString() << "continue watching? [y/n]" <<endl;
                string input;
                getline (cin >> ws, input);
                if(input == "n")
                    toContinueWatching = false;
            }
        }
   // this->toString();
}
std::string Watch::toString() const {}


//Exit
Exit::Exit() {}
void Exit::act(Session& sess) {}
std::string Exit::toString() const {}