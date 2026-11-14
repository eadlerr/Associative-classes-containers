#pragma once
#include <string>
#include <iostream>

class Person {
protected:
    std::string name;
    int age;
    std::string country;
    std::string sport;
    int experience;
    int id;
    int rating;

public:
    Person(std::string n, int a, std::string c, std::string s, int exp, int id = 0, int r = 0);
    virtual ~Person() {}

    std::string getName() const { return name; }
    int getAge() const { return age; }
    std::string getCountry() const { return country; }
    std::string getSport() const { return sport; }
    int getExperience() const { return experience; }
    int getId() const { return id; }
    int getRating() const { return rating; }

    void setName(std::string n) { name = n; }
    void setAge(int a) { age = a; }
    void setCountry(std::string c) { country = c; }
    void setSport(std::string s) { sport = s; }
    void setExperience(int exp) { experience = exp; }
    void setId(int v) { id = v; }
    void setRating(int r) { rating = r; }

    virtual int getType() const = 0;
    virtual std::string getMedalsStr() const = 0;

    virtual void printRow(std::ostream& os, int num) const;
    virtual void edit();
};

class Athlete : public Person {
private:
    int medals;
public:
    Athlete(std::string n, int a, std::string c, std::string s, int exp, int m, int id = 0, int r = 0);

    int getType() const override { return 1; }
    std::string getMedalsStr() const override { return std::to_string(medals); }

    int getMedals() const { return medals; }
    void setMedals(int m) { medals = m; }

    void edit() override;
};

class Coach : public Person {
public:
    Coach(std::string n, int a, std::string c, std::string s, int exp, int id = 0, int r = 0);

    int getType() const override { return 2; }
    std::string getMedalsStr() const override { return "-"; }
};
