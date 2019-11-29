#include "../include/Watchable.h"
#include <sstream>
#include "../include/Session.h"
#include <utility>

using namespace std;

//const
Watchable :: Watchable(long id, int length, std::vector<std::string>  tags) : id(id), length(length), tags(std::move(tags)) {}

//copy constructor
Watchable :: Watchable(const Watchable& other) : id(other.id) {
    copy(other);
}

//assignment operator
Watchable &Watchable :: operator=(const Watchable& other){

    if (&other != this){
        clean();
        copy(other);
    }
    return *this;
}

//destructor
Watchable :: ~Watchable(){
    clean();
}

//getters
long Watchable :: getId() { return this->id; }
int Watchable :: getLength() { return length; }
std::vector<std::string> Watchable :: getTags() { return tags; }

//class methods
void Watchable :: clean() {
    length = 0;
    tags.clear();
    //id??
}
void Watchable :: copy(const Watchable& other){
    this -> length = other.length;
    for (const auto & tag : other.tags) {
        tags.push_back(tag);
    }
}

Watchable* Watchable::getNextWatchable(Session &) const {}

//__________________________________________________________________________
//MOVIE
//__________________________________________________________________________

//constructor
Movie :: Movie(long id, const std::string& name, int length, const std::vector<std::string> &tags)
        : Watchable(id, length, tags), name(name) {}

//copy constructor
Movie :: Movie(const Movie& other) : Watchable(other), name(name){
    //this->name = other.name;
}

// copy assignment operator
Movie &Movie :: operator=(const Movie& other){
    if (&other != this){
        clean();
        copy(other);
    }
    return *this;
}

//destructor
Movie :: ~Movie(){
    clean();
}

//Class Methods
std::string Movie :: getName() { return name; }

void Movie ::clean() {}

void Movie :: copy(const Movie& other){}

std::string Movie::toString() const { return name; }

Watchable* Movie::getNextWatchable(Session &) const {
    return nullptr;
}



//__________________________________________________________________________
//EPISODE
//__________________________________________________________________________


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