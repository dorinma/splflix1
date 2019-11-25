//
// Created by dorin on 20/11/2019.
//
#include "../include/User.h"

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

//Length recommender
LengthRecommenderUser::LengthRecommenderUser(const std::string &name) :User(name) {}

Watchable* LengthRecommenderUser::getRecommendation(Session &s) {}

//Rerun recommender
RerunRecommenderUser::RerunRecommenderUser(const std::string &name) :User(name){}

Watchable* RerunRecommenderUser::getRecommendation(Session &s) {}

//Genre recommender
GenreRecommenderUser::GenreRecommenderUser(const std::string &name) :User(name){}

Watchable* GenreRecommenderUser::getRecommendation(Session &s) {}