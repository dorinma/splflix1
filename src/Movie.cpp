//
// Created by dorin on 24/11/2019.
//

#include "../include/Watchable.h"

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

std::string Movie::toString() const {}

Watchable* Movie::getNextWatchable(Session &) const {}