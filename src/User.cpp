//
// Created by dorin on 20/11/2019.
//
#include "../include/User.h"
#include "../include/Session.h"
#include "../include/Watchable.h"
#include <cmath>
#include <random>
#include <utility>
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>


//User
User::User(std::string name) : history(), name(std::move(name)) {}

User::User(const User &other) : history(), name() {
    copy(other);
}

User::~User() {
    clean();
}

void User::copy(const User& other) {
    this->name = other.name;
    for (size_t i=0; i<other.history.size(); i++)
    {
        this->history.push_back(other.history[i]);
    }
}

class User & User::operator=(const class User & other) {
    if (&other != this)
    {
        this->clean();
        this->copy(other);
    }
    return *this;
}

void User::clean() {
    this->history.clear();
}

std::vector<Watchable*> User::get_history() const {return this->history;}

std::string User::getName() const { return this->name; }

void User::setUserName(std::string newName) { this->name = std::move(newName); }

void User::setToHistory(Watchable *watched) { /////////NEW
    this->history.push_back(watched);

}

void User::setHistory(std::vector<Watchable *> w) {
    this->history = w;
}

//----------------Length recommender----------------
LengthRecommenderUser::LengthRecommenderUser(const std::string &name) :User(name) {}

Watchable* LengthRecommenderUser::getRecommendation(Session &s) {

    //in case last watched content is an episode
    Watchable* lastWatched = history.at(history.size() - 1);
    if(lastWatched->getNextWatchable(s) != nullptr)
        return lastWatched->getNextWatchable(s);
    else {
        //get average length
        int count = 0;
        int lenSum = 0;
        for (const auto &elem : history) {
            Watchable *temp = elem;
            count++;
            lenSum += temp->getLength();
        }
        int avgLen = (int) lenSum / count;

        std::vector<Watchable *> recommendations;

        //find the minimal difference between average length to every content's length
        int difLen = 1000000; //set the min dif length to a max value
        for (const auto &elem1 : s.getContent()) {
            Watchable *temp = elem1;
            int currentLen = temp->getLength();
            if ((std::abs(avgLen - currentLen)) <= difLen) {
                //check if this content isn't in the user's history
                bool exists = false;
                for (const auto &elem2 : history) {
                    Watchable *secTemp = elem2;
                    if (temp == secTemp)
                        exists = true;
                }
                if (!exists)
                    difLen = std::abs(avgLen - currentLen);
            }
        }

        //get every content with wanted length
        for (const auto &elem1 : s.getContent()) {
            Watchable *temp = elem1;
            int currentLen = temp->getLength();
            if ((std::abs(avgLen - currentLen)) == difLen) {
                //check if this content isn't in the user's history
                bool exists = false;
                for (const auto &elem2 : history) {
                    Watchable *secTemp = elem2;
                    if (temp == secTemp)
                        exists = true;
                }
                if (!exists)
                    recommendations.push_back(temp);
            }
        }

        Watchable *output = nullptr;
        if (recommendations.empty()) return output; //no recommendations
        else if (recommendations.size() == 1) return recommendations.at(0); //one recommendation
        else { //several recommendations
            //get the content with the minimal index
            int i = s.getContent().size(); //set the min index to the max value
            for (const auto &elem : recommendations) {
                Watchable *temp = elem;
                if (temp->getId() < i) {
                    i = (int)temp->getId();
                    output = temp;
                }
            }
        }

        return output;
    }
}
User* LengthRecommenderUser::toDuplicate(const std::string &newName, const User &oldUser) {
    User *newUser = new LengthRecommenderUser(newName);
    std::vector<Watchable*> historyToCopy = oldUser.get_history();
    for (size_t i=0; i<historyToCopy.size(); i++)
    {
        newUser->setToHistory(historyToCopy.at(i));
    }
    return newUser;
}



LengthRecommenderUser::LengthRecommenderUser(const LengthRecommenderUser &other) : User(other){}


//----------------Rerun recommender----------------
RerunRecommenderUser::RerunRecommenderUser(const std::string &name) :User(name){}

Watchable* RerunRecommenderUser::getRecommendation(Session &s) {

    //in case last watched content is an episode
    Watchable* lastWatched = history.at(history.size() - 1);
    if(lastWatched->getNextWatchable(s) != nullptr)
        return lastWatched->getNextWatchable(s);

    else {
        Watchable* output = nullptr;
        int n = history.size();
        int lastIndex = (int)history.size() - 1;
        int nextIndex = (lastIndex + 1) % n;

        if((nextIndex >= 0) & (nextIndex < n))
            output = history.at(nextIndex);

        return output;
    }
}
User* RerunRecommenderUser::toDuplicate(const std::string &newName, const User &oldUser) {
    User *newUser = new RerunRecommenderUser(newName);
    std::vector<Watchable *> historyToCopy = oldUser.get_history();
    for (size_t i = 0; i < historyToCopy.size(); i++) {
        newUser->setToHistory(historyToCopy.at(i));
    }
    return newUser;
}
RerunRecommenderUser::RerunRecommenderUser(const RerunRecommenderUser &other) : User(other){}


//----------------Genre recommender----------------
GenreRecommenderUser::GenreRecommenderUser(const std::string &name) :User(name){}

Watchable* GenreRecommenderUser::getRecommendation(Session &s) {

    //in case last watched content is an episode
    Watchable *lastWatched = history.at(history.size() - 1);
    if (lastWatched->getNextWatchable(s) != nullptr)
        return lastWatched->getNextWatchable(s);

    else {
        //1. find all the genres the user watched
        std::vector<std::string> watchedGenres;
        for (const auto &elem : history) {
            Watchable *temp = elem;
            for (const auto &tag : temp->getTags()) {
                watchedGenres.push_back(tag);
            }
            //watchedGenres.insert(watchedGenres.end(), temp->getTags().begin(), temp->getTags().end());
        }

        //2. count how many times he watched every genre
        std::unordered_map<std::string, int> genresCounter;
        for (const auto &elem : watchedGenres) {
            std::string watched = elem;
            bool found = false;
            if (genresCounter.empty())
                genresCounter.insert({watched, 1});
            else {
                for (auto currGen : genresCounter) {
                    while (!found) {
                        if (watched == currGen.first) {
                            currGen.second++;
                            found = true;
                        } else {
                            genresCounter.insert({watched, 1});
                            found = true;
                        }
                    }
                }
            }
        }

        //3. find the most popular genre
        int maxTags = 0;
        for (const auto& currGen : genresCounter) {
            if (currGen.second >= maxTags) {
                maxTags = currGen.second;
            }
        }

        //4. create a vector with most popular genres (in order to check if there are more than one)
        std::vector<std::string> popGens;
        for (const auto& currGen : genresCounter) {
            if (currGen.second == maxTags)
                popGens.push_back(currGen.first);
        }
        sort(popGens.begin(), popGens.end());

        //5. find the content we want to recommend the user, checking he hasn't watched it before
        for (const auto &currGen : popGens) {
            for (const auto &elem2 : s.getContent()) {
                Watchable *currContent = elem2;
                if (std::find(history.begin(), history.end(), currContent) == history.end()) {
                    for (const auto &currTag : currContent->getTags()) {
                        //std::string currTag = elem3;
                        if (currGen == currTag)
                            return currContent;
                    }
                }
            }
        }

        return nullptr;
    }
}
User* GenreRecommenderUser::toDuplicate(const std::string &newName, const User &oldUser) {
    User *newUser = new GenreRecommenderUser(newName);
    std::vector<Watchable*> historyToCopy = oldUser.get_history();
    for (size_t i=0; i<historyToCopy.size(); i++)
    {
        newUser->setToHistory(historyToCopy.at(i));
    }
    return newUser;
}
GenreRecommenderUser::GenreRecommenderUser(const GenreRecommenderUser &other) : User(other){}
