#include "../include/User.h"
#include "../include/Watchable.h"
#include "../include/Session.h"
#include "../include/json.hpp"
#include <fstream>
#include <sstream>

using namespace std;
using json = nlohmann :: json;

//copy constructor
//move constructor
//copy assignment operator
//move assignment operator

Session :: Session(const std::string &configFilePath){
    //read the json file

    nlohmann::json j;
    std::ifstream jlnFile(configFilePath, std::ifstream::binary);
    jlnFile >> j;

    //make the json data watchable object
    int i = 0;

    while(j["movies"][i] != nullptr){
        vector<string> tags;
        int tagsCount = 0;
        while(j["movies"][i]["tags"][tagsCount] != nullptr){
           tags.push_back(j["movies"][i]["tags"][tagsCount]);
           tagsCount++;

        }
        content.push_back(new Movie(i + 1,j["movies"][i]["name"],j["movies"][i]["length"], tags));
        i++;
    }

    int k = 0;
    while(j["tv_series"][k] != nullptr){
        //count seasons
        int seasons = 0;
        while(j["tv_series"][k]["seasons"][seasons] != nullptr) {
            seasons++;
        }
        //count tags
        vector<string> tags;
        int tagsCount = 0;
        while(j["tv_series"][k]["tags"][tagsCount] != nullptr){
            tags.push_back(j["tv_series"][k]["tags"][tagsCount]);
            tagsCount++;
        }
        for (int s = 1; s <= seasons; ++s) {
            for (int e = 1; e <= j["tv_series"][k]["seasons"][s]; ++e) {
                content.push_back(new Episode(i + 1, j["tv_series"][k]["name"],j["tv_series"][k]["episode_length"],s, e, tags));
            }
        }
        k++;
        i++;

        //create default user
        LengthRecommenderUser *defUser = new LengthRecommenderUser("default");
        userMap.insert({"default", defUser});
        this->activeUser = defUser;
    }



}

//destructor
Session :: ~Session(){}

void Session :: clean() {}

std::string Session::getSessionInput() {return this->sessionInput;}

std::unordered_map<std::string, User*> Session::getUserMap() { return this->userMap;}

User* Session::getUserByString(std::string name) {
    User* user = nullptr;
    for(const auto& elem : userMap)
    {
        if (elem.first == name)
            user = elem.second;
    }
    return user;
}

void Session::addUserToMap(User *u) {userMap.insert({u->getName(), u});}

void Session::addActionToLog(BaseAction *ba) {actionsLog.push_back(ba);}

std::vector<Watchable*> Session::getContent() { return content; }

void Session :: start() {

    cout << "SPLFLIX is now on!" << endl;
    bool terminate = false;

    //cout << "Please insert input." << endl;

    while (terminate != true){

        std::string input;
        getline (cin >> ws, input);
        sessionInput = input;
        std::istringstream iss(input);
        std::vector<std::string> result(std::istream_iterator<std::string>{iss},
                                        std::istream_iterator<std::string>());

        BaseAction *baseAction;

            if (result[0] == "createuser") {
                baseAction = new CreateUser();
            }

            if (result[0] =="changeuser") {
                baseAction = new ChangeActiveUser();
            }

            if (result[0] =="deleteuser") {
                baseAction = new DeleteUser();
            }

            if (result[0] == "dupuser") {
                baseAction = new DuplicateUser();
            }

            if (result[0] == "content") {
                baseAction = new PrintContentList();
            }

            if (result[0] == "watchhist") {
                baseAction = new PrintContentList();
            }

            if (result[0] == "watch") {
                baseAction = new Watch();
            }

            if (result[0] == "log") {
                baseAction = new PrintActionsLog();
            }

            if (result[0] == "exit") {
                baseAction = new Exit();
                terminate = true;
            }

            baseAction->act(*this);
        }
    }
