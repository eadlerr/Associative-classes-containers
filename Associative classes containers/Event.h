#pragma once
#include <string>
#include <vector>
#include "Person.h"

class Event {
private:
    std::string eventName;
    std::string date;
    std::vector<Athlete*> athletes;
    std::vector<Coach*> coaches;

public:
    Event() : eventName(""), date("") {}
    Event(std::string name, std::string d) : eventName(name), date(d) {}
    ~Event() { clear(); }

    Event(const Event&) = delete;
    Event& operator=(const Event&) = delete;

    void clear();

    std::string getEventName() const { return eventName; }
    std::string getDate() const { return date; }
    void setEventName(std::string n) { eventName = n; }
    void setDate(std::string d) { date = d; }

    size_t athleteCount() const { return athletes.size(); }
    size_t coachCount() const { return coaches.size(); }
    Athlete* getAthlete(size_t i) const { return athletes[i]; }
    Coach* getCoach(size_t i) const { return coaches[i]; }

    void addAthlete(Athlete* a) { athletes.push_back(a); }
    void addCoach(Coach* c) { coaches.push_back(c); }
};
