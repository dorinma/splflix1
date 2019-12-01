
#include <sstream>
#include "../include/Action.h"
#include "../include/Session.h"
#include "../include/User.h"
#include "../include/Watchable.h"

using namespace std;

//-----BaseAction------
BaseAction::BaseAction() : errorMsg(""), status(PENDING) {};
ActionStatus BaseAction::getStatus() const { return this->status; };
void BaseAction::complete() { this->status = COMPLETED; }
void BaseAction::error(const std::string &errorMsg)  {
    status = ERROR;
    this->errorMsg = errorMsg;
}
std::string BaseAction::getErrorMsg() const { return errorMsg;}


BaseAction::BaseAction(const BaseAction &ba) : errorMsg(ba.errorMsg), status(ba.status){}
BaseAction & BaseAction::operator=(const class BaseAction & other) {
    if (&other != this)
    {
       this->status = other.status;
       this->errorMsg = other.errorMsg;
    }
    return *this;
}
BaseAction:: ~BaseAction() = default;


//-----Create user------
CreateUser::CreateUser () = default;
CreateUser::CreateUser(const CreateUser &other) : BaseAction(other) {}
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
        sess.addActionToLog(this);
    }
    else {
        if (reco == "len") { newUser = new LengthRecommenderUser(name); }
        else if (reco == "rer") { newUser = new RerunRecommenderUser(name); }
        else if (reco == "gen") { newUser = new GenreRecommenderUser(name); }
        else
            {
            this->error("wrong recommend was typed");
            sess.addActionToLog(this);
            }

        if (getStatus() != ERROR) {
            complete();
            sess.addUserToMap(newUser);
            sess.addActionToLog(this);
        }
    }
    //this->toString();
}
std::string CreateUser::toString() const {
    if (this->getStatus() == ERROR)
        cout<<"ERROR: " << this->getErrorMsg() << endl;
    else if (this->getStatus() == COMPLETED)
        cout<< "createuser COMPLETED" <<endl;
    else
        cout<< "unkown error" <<endl;
    return "";
}
CreateUser* CreateUser::dupAction() {
    CreateUser *newCa = new CreateUser(*this);
    return newCa;
}
CreateUser::~CreateUser() = default;


//------Change active user-----
ChangeActiveUser::ChangeActiveUser () = default;
ChangeActiveUser::ChangeActiveUser(const ChangeActiveUser &other) : BaseAction(other){}
void ChangeActiveUser::act(Session& sess) {
    std::string userInput = sess.getSessionInput();
    std::string changedUser = userInput.substr(userInput.find(' ')+1);
    User *user;
    user = sess.getUserByString(changedUser);
    if (sess.getUserByString(changedUser) == nullptr) {
        this->error("user to change does not exist");
        sess.addActionToLog(this);
    }
    else {
        sess.setActiveUser(user);
        complete();
        sess.addActionToLog(this);
    }
    //this->toString();
}
std::string ChangeActiveUser::toString() const {
    if (this->getStatus() == ERROR)
        cout<< "ERROR: " << this->getErrorMsg() << endl;
    else if (this->getStatus() == COMPLETED)
        cout<< "changeuser COMPLETED" << endl;
    else
        cout<< "unkown error" <<endl;

    return "";
}
ChangeActiveUser* ChangeActiveUser::dupAction() {
    ChangeActiveUser *newChu = new ChangeActiveUser(*this);
    return  newChu;
}
ChangeActiveUser::~ChangeActiveUser() = default;


//------Delete user------
DeleteUser::DeleteUser() = default;
DeleteUser::DeleteUser(const DeleteUser &other) : BaseAction(other){}
void DeleteUser::act(Session& sess) {
    std::string userInput = sess.getSessionInput();
    std::string userToDelete = userInput.substr(userInput.find(' ')+1);
    User *user;
    user = sess.getUserByString(userToDelete);
    if (user == nullptr){
        this->error("user to delete does not exist");
        sess.addActionToLog(this);
    }
    else{
        sess.deleteUserFromMap(userToDelete);
        complete();
        sess.addActionToLog(this);
    }
   // this->toString();
}
std::string DeleteUser::toString() const {
    if (this->getStatus() == ERROR)
        cout<< "ERROR: " << this->getErrorMsg() << endl;
    else if (this->getStatus() == COMPLETED)
        cout<< "deleteuser COMPLETED" << endl;
    else
        cout<< "unkown error" <<endl;
    return "";
}
DeleteUser* DeleteUser::dupAction() {
    DeleteUser *newDu = new DeleteUser(*this);
    return newDu;
}
DeleteUser::~DeleteUser() = default;



//------Duplicate user------
DuplicateUser::DuplicateUser() = default;
DuplicateUser::DuplicateUser(const DuplicateUser &other) : BaseAction(other) {}
void DuplicateUser::act(Session& sess) {
    std::string userInput = sess.getSessionInput();
    int count = 0;
    string word = "";
    std::string newUserName;
    std::string oldUserName;
    for (auto x : userInput)
    {
        if (x == ' ')
        {
            count++;
            if (count == 2)
                oldUserName = word;
            word = "";
        }
        else
        {
            word = word + x;
        }
        newUserName = word;
    }
    User *tempUser;
    if (sess.getUserByString(oldUserName) == nullptr){
        this->error("user to duplicate from does not exist");
        sess.addActionToLog(this);
    }
    else if(sess.getUserByString(newUserName) != nullptr){
        this->error(("this user name exist already, choose new one"));
        sess.addActionToLog(this);
    }
    else
    {
        tempUser = sess.getUserByString(oldUserName);
        User *newUser =  tempUser->toDuplicate(newUserName, *tempUser);
        sess.addUserToMap(newUser);
        complete();
        sess.addActionToLog(this);
    }
    //this->toString();
}
std::string DuplicateUser::toString() const {
    if (this->getStatus() == ERROR)
        cout<< "ERROR: " << this->getErrorMsg() << endl;
    else if (this->getStatus() == COMPLETED)
        cout<< "dupuser COMPLETED" << endl;
    else
        cout<< "unkown error" <<endl;
    return "";
}

DuplicateUser * DuplicateUser::dupAction() {
    DuplicateUser *newDu = new DuplicateUser(*this);
    return newDu;
}
DuplicateUser::~DuplicateUser() = default;



//------Print content list------
PrintContentList::PrintContentList() = default;
PrintContentList::PrintContentList(const PrintContentList &other) : BaseAction(other){}
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
        cout<< "ERROR: " << this->getErrorMsg() << endl;
    else if (this->getStatus() == COMPLETED)
        cout<< "content COMPLETED" << endl;
    else
        cout<< "unkown error" <<endl;
    return "";
}
PrintContentList * PrintContentList::dupAction() {
    PrintContentList *newPcl = new PrintContentList(*this);
    return newPcl;
}
PrintContentList::~PrintContentList() = default;



//-----Print watch history-----
PrintWatchHistory::PrintWatchHistory() = default;
PrintWatchHistory::PrintWatchHistory(const PrintWatchHistory &other) : BaseAction(other){}
void PrintWatchHistory::act(Session& sess) {
    string name = sess.getActiveUser()->getName();
    cout << "Watch history for " << name << endl;
    vector<Watchable *> history = sess.getActiveUser()->get_history();
    if (history.empty()) {
        cout << ("nothing was watched. you need to stop studying") << endl;
        complete();
        sess.addActionToLog(this);
    }
    else {
        for (auto &i : history) {
            int id = (int) i->getId();
            string name = i->toString();
            cout << id << ". " << name << endl;

        }
        complete();
        sess.addActionToLog(this);
    }
}
std::string PrintWatchHistory::toString() const {
    if (this->getStatus() == ERROR)
        cout<< "ERROR: " << this->getErrorMsg() << endl;
    else if (this->getStatus() == COMPLETED)
        cout<< "watchhist COMPLETED" << endl;
    else
        cout<< "unkown error" <<endl;
    return "";
}
PrintWatchHistory * PrintWatchHistory::dupAction() {
    PrintWatchHistory *newPwh = new PrintWatchHistory(*this);
    return newPwh;
}
PrintWatchHistory::~PrintWatchHistory() = default;



//-----Print actions log------
PrintActionsLog::PrintActionsLog() = default;
PrintActionsLog::PrintActionsLog(const PrintActionsLog &other) : BaseAction(other) {}
void PrintActionsLog::act(Session& sess) {
    vector<BaseAction *> action = sess.getActionLog();
    for (const auto & i : action)
    {
        i->toString();
    }
    complete();
    sess.addActionToLog(this);
}
std::string PrintActionsLog::toString() const {
    if (this->getStatus() == ERROR)
        cout<< "ERROR: " << this->getErrorMsg() << endl;
    else if (this->getStatus() == COMPLETED)
        cout<< "log COMPLETED" << endl;
    else
        cout<< "unkown error" <<endl;
    return "";
}
PrintActionsLog * PrintActionsLog::dupAction() {
    PrintActionsLog *newPal = new PrintActionsLog(*this);
    return newPal;
}
PrintActionsLog::~PrintActionsLog() = default;



//-----Watch------
Watch::Watch() = default;
Watch::Watch(const Watch &other) : BaseAction(other){}
void Watch::act(Session& sess) {

    std::string userInput = sess.getSessionInput();
    std::string idToWatch = userInput.substr(userInput.find(' ')+1);

    if (sess.getSomethingToWatch(idToWatch) == nullptr) {
        this->error("content with this id doesnt exist");
        sess.addActionToLog(this);
    }
     else {
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

            complete();
            sess.addToHistory(nextWatchable);
            Watch *watching = new Watch();
            watching->complete();
            sess.addActionToLog(watching);

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
}
std::string Watch::toString() const {
    if (this->getStatus() == ERROR)
        cout<< "ERROR: " << this->getErrorMsg() << endl;
    else if (this->getStatus() == COMPLETED)
        cout<< "watch COMPLETED" << endl;
    else
        cout<< "unkown error" <<endl;
    return "";
}
Watch * Watch::dupAction() {
    Watch *newW = new Watch(*this);
    return newW;
}
Watch::~Watch() = default;


//------Exit------
Exit::Exit() = default;
Exit::Exit(const Exit &other) : BaseAction(other){}
void Exit::act(Session& sess) {
    sess.setTerminate("false");
    complete();
    sess.addActionToLog(this);
}
std::string Exit::toString() const {
    if (this->getStatus() == ERROR)
        cout<< "ERROR: " << this->getErrorMsg() << endl;
    else if (this->getStatus() == COMPLETED)
        cout<< "exit COMPLETED" << endl;
    else
        cout<< "unkown error" <<endl;
    return "";
}
Exit* Exit::dupAction() {
    Exit *newE = new Exit(*this);
    return newE;
}
Exit::~Exit() = default;
