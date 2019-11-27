

#include <sstream>
#include "../include/Action.h"
#include "../include/Session.h"
#include "../include/User.h"
#include "../include/Watchable.h"


using namespace std;


//-----BaseAction------
BaseAction::BaseAction() = default;
ActionStatus BaseAction::getStatus() const { return this->status; };
void BaseAction::complete() { this->status = COMPLETED; }
void BaseAction::error(const std::string &errorMsg) {
    status = ERROR;
    this->errorMsg = errorMsg;
}
std::string BaseAction::getErrorMsg() const { return errorMsg;} //" Error: %n", errorMsg; }


//-----Create user------ //DONEEEE
CreateUser::CreateUser () = default;
void CreateUser::act(Session& sess) {
    std::string userInput = sess.getSessionInput();
    std::string reco = userInput.substr(userInput.length()-3);
    int end;
    end = (int)userInput.length() - 5 - (int)userInput.find(' ');
    std::string name;
    name = userInput.substr(userInput.find(' ') + 1, end);

    User *newUser = nullptr;
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


//------Change active user----- ////DONEEEEEE
ChangeActiveUser::ChangeActiveUser () = default;
void ChangeActiveUser::act(Session& sess) {
    std::string userInput = sess.getSessionInput();
    std::string changedUser = userInput.substr(userInput.find(' ')+1);
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


//------Delete user------ //DONEEEEE
DeleteUser::DeleteUser() = default;
void DeleteUser::act(Session& sess) {
    std::string userInput = sess.getSessionInput();
    std::string userToDelete = userInput.substr(userInput.find(' ')+1);
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
DuplicateUser::DuplicateUser() = default;
void DuplicateUser::act(Session& sess) {
    std::string userInput = sess.getSessionInput();
    std::string oldUserName = userInput.substr(userInput.find(' ')+1, userInput.find(' ')-2);
    int tmp = (int)oldUserName.size() + 2;
    std::string newUserName = userInput.substr(userInput.find(' ') + tmp);
    User *newUser;
    if (sess.getUserByString(oldUserName) == nullptr){
    this->error("user to duplicate from does not exist");}
    else if(sess.getUserByString(newUserName) != nullptr){
        this->error(("this user new exist already, choose new one"));}
    else
    {
        newUser = sess.getUserByString(oldUserName);
        newUser->setUserName(newUserName);
        sess.addUserToMap(newUser);
        complete();
        sess.addActionToLog(this);
    }
    this->toString();
}
std::string DuplicateUser::toString() const {
    if (this->getStatus() == ERROR)
        cout<< this->getErrorMsg()<< endl;
    else if (this->getStatus() == COMPLETED)
        cout<< "user coppied" << endl;
    else
        cout<< "unkown error" << endl;
}


//Print content list //DONEEE
PrintContentList::PrintContentList() = default;
void PrintContentList::act(Session& sess) {

    vector<Watchable *> contentList = sess.getContent();
    for (auto & i : contentList)
    {
        long id = i->getId();
        string name = i->toString();
        int length = i->getLength();
        vector<string> thisTags = i->getTags();
        string tags;
        for (const auto & thisTag : thisTags) {
            tags += thisTag + ", ";
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


//Print watch history //DONEEEEE
PrintWatchHistory::PrintWatchHistory() = default;
void PrintWatchHistory::act(Session& sess) {
    string name = sess.getActiveUser()->getName();
    cout << "Watch history for " << name << endl;
    vector<Watchable *> history = sess.getActiveUser()->get_history();
    if (history.empty())
        cout <<"nothing was watched. you need to stop studying" << endl;
    for (auto & i : history)
    {
        int id = (int)i->getId();
        string name = i->toString();
        cout << id << ". " << name << endl;

    }
    complete();
    sess.addActionToLog(this);
}
std::string PrintWatchHistory::toString() const {}


//Print actions log
PrintActionsLog::PrintActionsLog() = default;
void PrintActionsLog::act(Session& sess) {
    vector<BaseAction *> action = sess.getActionLog();
    for (auto & i : action)
    {
      cout << i->toString();
    }
    complete();
    sess.addActionToLog(this);
}
std::string PrintActionsLog::toString() const {}


//Watch
Watch::Watch() = default;
void Watch::act(Session& sess) {

    std::string userInput = sess.getSessionInput();
    std::string idToWatch = userInput.substr(userInput.find(' ')+1);

        if (sess.getSomethingToWatch(idToWatch) == nullptr) {
            this->error("content with this id doesnt exist");
        } else {
            sess.nowPlaying = idToWatch;
            cout << "Watching " << sess.getSomethingToWatch(idToWatch)->toString() << endl;
            complete();
            sess.addToHistory(sess.getSomethingToWatch(idToWatch));
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
               int nextId = (int)nextWatchable->getId();

                stringstream ss;
                ss << nextId;
                sess.nowPlaying = ss.str();

                complete();
                sess.addToHistory(nextWatchable);
                sess.addActionToLog(this);

                nextWatchable = sess.getActiveUser()->getRecommendation(sess);
                cout << "We recommend watching " << nextWatchable->toString() << "continue watching? [y/n]" <<endl;
                string input;
                getline (cin >> ws, input);
                if(input != "y") {
                    toContinueWatching = false;
                    cout << "bing stopped. go work on your splflix" << endl;
                }
            }
        }
   // this->toString();
}
std::string Watch::toString() const {}


//Exit //DONEEEE
Exit::Exit() = default;
void Exit::act(Session& sess) {
    sess.setTerminate("false");
}
std::string Exit::toString() const {}