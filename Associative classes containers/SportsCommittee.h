#pragma once
#include <string>
#include <array>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include "Person.h"
#include "Event.h"

struct RatingCmp {
    bool operator()(const Person* a, const Person* b) const {
        return a->getRating() > b->getRating();
    }
};

class SportsCommittee {
private:
    std::string committeeName;
    std::array<Event, 3> events;
    std::list<std::string> filterResults;

public:
    std::map<int, Person*> participants;
    std::set<std::string> sports;
    std::multimap<std::string, Person*> teamMembers;
    std::multiset<Person*, RatingCmp> leaderboard;
    std::unordered_map<std::string, std::string> eventsMap;
    std::unordered_set<std::string> teams;
    std::unordered_multimap<int, std::string> achievements;
    std::unordered_multiset<std::string> logs;

    SportsCommittee();
    ~SportsCommittee();

    Event& getEvent(int i) { return events[i]; }
    const std::array<Event, 3>& getEvents() const { return events; }
    std::list<std::string>& getFilterResults() { return filterResults; }

    void clear();
    Person* findParticipant(int id) const;
    void fillDemo();
};
