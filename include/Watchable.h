#ifndef WATCHABLE_H_
#define WATCHABLE_H_

#include <string>
#include <vector>


class Session;

class Watchable{
public:
    Watchable(long id, int length, std::vector<std::string>  tags); //regular const
    Watchable(const Watchable& other);  //copy constructor
    Watchable& operator=(const Watchable& rhs); // copy assignment operator
    virtual ~Watchable(); //destructor
    virtual void clean();
    virtual void copy(const Watchable& other);
    virtual std::string toString() const = 0;
    virtual Watchable* getNextWatchable(Session&) const = 0;

    long getId();
    int getLength();
    std::vector<std::string> getTags();
private:
    const long id;
    int length;
    std::vector<std::st{}ring> tags;
};

class Movie : public Watchable{
public:
    Movie(long id, const std::string& name, int length, const std::vector<std::string>& tags); //regular const
    Movie(const Movie& other);  //copy constructor
    Movie& operator=(const Movie& rhs); // copy assignment operator
    virtual ~Movie(); //destructor
    virtual void clean();
    virtual void copy(const Movie& other);
    virtual std::string toString() const;
    virtual Watchable* getNextWatchable(Session&) const;
    std::string getName();
private:
    std::string name;
};


class Episode: public Watchable{
public:
    Episode(long id, const std::string& seriesName,int length, int season, int episode ,const std::vector<std::string>& tags);
    Episode(const Episode& other);  //copy constructor
    Episode& operator=(const Episode& rhs); // copy assignment operator
    virtual ~Episode(); //destructor
    virtual void clean();
    virtual void copy(const Episode& other);
    virtual std::string toString() const ;
    virtual Watchable* getNextWatchable(Session&) const ;
    std::string getSeriesName();
    int getSeason();
    int getEpisode();
    long getNextEpisodeId();
    void setNextEpisodeId(long currEpisodeId);
private:
    std::string seriesName;
    int season;
    int episode;
    long nextEpisodeId;
};

#endif