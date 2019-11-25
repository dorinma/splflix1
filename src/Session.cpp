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

//dorin ha malka


void Session :: start() {

    cout << "SPLFLIX is now on!" << endl;
    bool terminate = false;

    while (terminate != true){
        cout << "Please insert input." << endl;
        std::string input;
        getline (cin >> ws, input);
        sessionInput = input;
        std::istringstream iss(input);
        std::vector<std::string> result(std::istream_iterator<std::string>{iss},
                                        std::istream_iterator<std::string>());

        BaseAction *baseAction;

            if (result[0].compare("createuser") == 0) {
                baseAction = new CreateUser();
            }

            if (result[0].compare("changeuser") == 0) {
                baseAction = new ChangeActiveUser();
            }

            if (result[0].compare("deleteuser") == 0) {
                baseAction = new DeleteUser();
            }

            if (result[0].compare("dupuser") == 0) {
                baseAction = new DuplicateUser();
            }

            if (result[0].compare("content") == 0) {
                baseAction = new PrintContentList();
            }

            if (result[0].compare("watchhist") == 0) {
                baseAction = new PrintContentList();
            }

            if (result[0].compare("watch") == 0) {
                baseAction = new Watch();
            }

            if (result[0].compare("log") == 0) {
                baseAction = new PrintActionsLog();
            }

            if (result[0].compare("exit") == 0) {
                baseAction = new Exit();
                //terminate = true;
            }

            baseAction->act(*this);
        }
    }

