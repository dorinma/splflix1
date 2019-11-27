#include "../include/Watchable.h"
//
// Created by dorin on 20/11/2019.
//

#include "../include/Watchable.h"

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