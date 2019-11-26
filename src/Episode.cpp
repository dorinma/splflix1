//
// Created by dorin on 24/11/2019.
//

#include <sstream>
#include "../include/Watchable.h"
#include "../include/Session.h"

//constructor
Episode :: Episode(long id, const std::string& seriesName,int length, int season, int episode ,const std::vector<std::string>& tags)
        : Watchable(id, length, tags), seriesName(seriesName), episode(episode), season(season){
}

//copy constructor
Episode :: Episode(const Episode& other) : Watchable(other) {
    this->seriesName = other.seriesName;
    this->episode = other.episode;
    this->season = other.season;
    //TODO
    //tags?
}

//copy assignment operator
Episode &Episode :: operator=(const Episode& other) {
    if (&other != this) {
        clean();
        copy(other);
    }
    return *this;
}

//destructor
Episode :: ~Episode(){
    clean();
}

void Episode :: clean(){}

void Episode :: copy(const Episode& other){}

std::string Episode :: getSeriesName() { return seriesName; }

int Episode :: getSeason() { return season; }

int Episode:: getEpisode() { return episode; }

long Episode :: getNextEpisodeId() { return nextEpisodeId; }

std::string Episode::toString() const {
    int s = this->season;
    std::stringstream se;
    se << s;
    int e = this->episode;
    std::stringstream ee;
    ee << e;
    std::string name = this->seriesName + " S" + se.str() + "E" + ee.str();
    return name;
}

Watchable* Episode::getNextWatchable(Session &s) const {
    Watchable* nextEp = nullptr;
    if(nextEpisodeId != 0) {
        for (const auto &elem : s.getContent()) {
            Watchable *temp = elem;
            if (nextEpisodeId == temp->getId())
                nextEp = temp;
        }
    }
    return nextEp;
}

void Episode::setNextEpisodeId(long next) {
    nextEpisodeId = next;
}