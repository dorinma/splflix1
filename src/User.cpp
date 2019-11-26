//
// Created by dorin on 20/11/2019.
//
#include "../include/User.h"
#include "../include/Session.h"
#include "../include/Watchable.h"
#include <cmath>
#include <random>
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>


//Rule of 5!!!!!
//copy constructor
//move constructor
//copy assignment operator
//move assignment operator

//TODO
//rule of five??

//User
User::User(const std::string &name) : name(name) {}

std::vector<Watchable*> User::get_history() const {}

std::string User::getName() const { return this->name; }

//----------------Length recommender----------------
LengthRecommenderUser::LengthRecommenderUser(const std::string &name) :User(name) {}

Watchable* LengthRecommenderUser::getRecommendation(Session &s) {

    //in case last watched content is an episode
    Watchable* lastWatched = history.at(history.size() - 1);
    if(lastWatched->getNextWatchable(s) != nullptr)
        return lastWatched->getNextWatchable(s);

else {
        //get average length
        int count, lenSum = 0;
        for (const auto &elem : history) {
            Watchable *temp = elem;
            count++;
            lenSum += temp->getLength();
        }
        int avgLen = (int) lenSum / count;

        std::vector<Watchable*> recommendations;
        int difLen = INTMAX_MAX; //set the min dif length to the max value

        for (const auto &elem1 : s.getContent()) {
            Watchable *temp = elem1;
            int currentLen = temp->getLength();
            if ((std::abs(avgLen - currentLen)) <= difLen) {
                //check if this content isn't in the user's history
                for (const auto &elem2 : history) {
                    Watchable *secTemp = elem2;
                    if (temp != secTemp) {
                        difLen = std::abs(avgLen - currentLen);
                        recommendations.push_back(temp);
                    }
                }
            }
        }
        Watchable *output = nullptr;
        if (recommendations.size() == 0) return output; //no recommendations
        else if (recommendations.size() == 1) return recommendations.at(0); //one recommendation
        else { //several recommendations
            //get the content with the minimal index
            int i = s.getContent().size(); //set the min index to the max value
            for (const auto &elem : recommendations) {
                Watchable *temp = elem;
                if (temp->getId() < i) {
                    i = temp->getId();
                    output = temp;
                }
            }
        }

        return output;
    }
}

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
        int lastIndex = history.size() - 1;
        int nextIndex = (lastIndex + 1) % n;

        if(nextIndex >= 0 & nextIndex < n)
            output = history.at(nextIndex);

        return output;
    }
}

//----------------Genre recommender----------------
GenreRecommenderUser::GenreRecommenderUser(const std::string &name) :User(name){}

Watchable* GenreRecommenderUser::getRecommendation(Session &s) {

    //in case last watched content is an episode
    Watchable* lastWatched = history.at(history.size() - 1);
    if(lastWatched->getNextWatchable(s) != nullptr)
        return lastWatched->getNextWatchable(s);

    else {
        //1. find all the genres the user watched
        std::vector<std::string> watchedGenres;
        for (const auto &elem : history) {
            Watchable* temp = elem;
            watchedGenres.insert(watchedGenres.end(), temp->getTags().begin(), temp->getTags().end());
            //watchedGenres.push_back(temp->getTags()); //can i push a vector into another vector?
        }

        //2. count how many times he watched every genre
        std::unordered_map<std::string, int> genresCounter;
        for (const auto &elem : watchedGenres) {
            std::string watched = elem;
            bool found = false;
            for (auto currGen : genresCounter) {
                while (!found) {
                    if (watched == currGen.first) {
                        currGen.second++;
                        found = true;
                    }
                    genresCounter.insert({watched, 1});
                }
            }

            //3. find the most popular genre
            int maxTags = 0;
            for (auto currGen : genresCounter) {
                if (currGen.second >= maxTags) {
                    maxTags = currGen.second;
                }
            }

            //4. create a vector with most popular genres (in order to check if there are more than one)
            std::vector<std::string> popGens;
            for (auto currGen : genresCounter) {
                if (currGen.second == maxTags)
                    popGens.push_back(currGen.first);
            }
            sort(popGens.begin(), popGens.end());

            //5. find the content we want to recommend the user, checking he hasn't watched it before
            for (const auto &elem1 : popGens) {
                std::string currGen = elem1;
                for (const auto &elem2 : s.getContent()) {
                    Watchable* currContent = elem2;
                    if (std::find(history.begin(), history.end(), currContent) == history.end()) {
                        for (const auto &elem3 : currContent->getTags()) {
                            std::string currTag = elem3;
                            if (currGen == currTag)
                                return currContent;
                        }
                    }
                }
            }

            return nullptr;
        }
    }
}