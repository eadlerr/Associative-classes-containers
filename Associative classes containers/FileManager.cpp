#include "FileManager.h"
#include "Utils.h"
#include <fstream>
#include <sstream>

using namespace std;

void printTableHeader(ostream& os) {
    os << "  "
        << padR("№", T::NUM) << string(T::SEP, ' ')
        << padR("ФИО", T::NAME) << string(T::SEP, ' ')
        << padR("Возраст", T::AGE) << string(T::SEP, ' ')
        << padR("Страна", T::COUNTRY) << string(T::SEP, ' ')
        << padR("Вид спорта", T::SPORT) << string(T::SEP, ' ')
        << padR("Опыт (лет)", T::EXP) << string(T::SEP, ' ')
        << padR("Медали", T::MEDALS) << string(T::SEP, ' ')
        << padR("Рейтинг", T::RATING) << "\n";
    os << "  " << string(T::W_MAIN, '-') << "\n";
}

void printEventTable(ostream& os, const Event& e) {
    os << "\n  " << string(T::W_MAIN, '=') << "\n";
    os << "    СОРЕВНОВАНИЕ: " << e.getEventName() << " (" << e.getDate() << ")\n";
    os << "  " << string(T::W_MAIN, '=') << "\n";

    os << "\n  -- Участники (Спортсмены) --\n";
    printTableHeader(os);
    if (e.athleteCount() == 0) os << "  (пусто)\n";
    else for (size_t i = 0; i < e.athleteCount(); ++i) e.getAthlete(i)->printRow(os, static_cast<int>(i + 1));

    os << "\n  -- Тренеры --\n";
    printTableHeader(os);
    if (e.coachCount() == 0) os << "  (пусто)\n";
    else for (size_t i = 0; i < e.coachCount(); ++i) e.getCoach(i)->printRow(os, static_cast<int>(i + 1));
}

void saveAll(const SportsCommittee& reg) {
    ofstream f(DATA_FILE);
    if (!f.is_open()) return;

    for (auto& p : reg.participants) {
        Person* ptr = p.second;
        f << "P|" << p.first << "|" << ptr->getType() << "|" << ptr->getName() << "|"
            << ptr->getAge() << "|" << ptr->getCountry() << "|" << ptr->getSport() << "|"
            << ptr->getExperience() << "|" << ptr->getRating() << "|";
        if (ptr->getType() == 1) {
            Athlete* a = dynamic_cast<Athlete*>(ptr);
            f << a->getMedals() << "\n";
        }
        else {
            f << "0\n";
        }
    }

    for (auto& tm : reg.teamMembers) {
        f << "TM|" << tm.first << "|" << tm.second->getId() << "\n";
    }

    for (auto& ev : reg.eventsMap) {
        f << "EVMAP|" << ev.first << "|" << ev.second << "\n";
    }

    for (auto& ac : reg.achievements) {
        f << "AC|" << ac.first << "|" << ac.second << "\n";
    }

    for (auto& lg : reg.logs) {
        f << "LG|" << lg << "\n";
    }

    for (size_t i = 0; i < reg.getEvents().size(); ++i) {
        const Event& ev = reg.getEvents()[i];
        for (size_t j = 0; j < ev.athleteCount(); ++j) {
            Athlete* a = ev.getAthlete(j);
            f << "EV|" << i << "|A|" << a->getName() << "|" << a->getAge() << "|"
                << a->getCountry() << "|" << a->getSport() << "|" << a->getExperience() << "|" << a->getMedals() << "\n";
        }
        for (size_t j = 0; j < ev.coachCount(); ++j) {
            Coach* c = ev.getCoach(j);
            f << "EV|" << i << "|C|" << c->getName() << "|" << c->getAge() << "|"
                << c->getCountry() << "|" << c->getSport() << "|" << c->getExperience() << "\n";
        }
    }
    f.close();

    ofstream frep(REPORT_FILE);
    if (frep.is_open()) {
        for (size_t i = 0; i < reg.getEvents().size(); ++i) {
            printEventTable(frep, reg.getEvents()[i]);
        }
        frep.close();
    }
}

bool loadAll(SportsCommittee& reg) {
    ifstream f(DATA_FILE);
    if (!f.is_open()) return false;

    reg.clear();
    for (auto& p : reg.participants) delete p.second;
    reg.participants.clear();
    reg.sports.clear(); reg.teamMembers.clear(); reg.leaderboard.clear();
    reg.eventsMap.clear(); reg.teams.clear(); reg.achievements.clear(); reg.logs.clear();

    string line;
    bool hasData = false;
    while (getline(f, line)) {
        if (line.empty()) continue;
        hasData = true;
        stringstream ss(line);
        string tag;
        getline(ss, tag, '|');

        if (tag == "P") {
            string idStr, typeStr, name, ageStr, country, sport, expStr, ratingStr, medalsStr;
            getline(ss, idStr, '|'); getline(ss, typeStr, '|'); getline(ss, name, '|');
            getline(ss, ageStr, '|'); getline(ss, country, '|'); getline(ss, sport, '|');
            getline(ss, expStr, '|'); getline(ss, ratingStr, '|'); getline(ss, medalsStr);

            int id = stoi(idStr);
            int type = stoi(typeStr);
            int age = stoi(ageStr);
            int exp = stoi(expStr);
            int rating = stoi(ratingStr);
            int medals = stoi(medalsStr);

            Person* p = nullptr;
            if (type == 1) p = new Athlete(name, age, country, sport, exp, medals, id, rating);
            else p = new Coach(name, age, country, sport, exp, id, rating);

            // Защита от утечки при дубликатах ID
            auto res = reg.participants.insert({ id, p });
            if (!res.second) {
                delete p;
                p = res.first->second;
            }
            reg.leaderboard.insert(p);
            if (!sport.empty()) reg.sports.insert(sport);
        }
        else if (tag == "TM") {
            string teamName, idStr;
            getline(ss, teamName, '|'); getline(ss, idStr);
            int id = stoi(idStr);
            Person* p = reg.findParticipant(id);
            if (p) reg.teamMembers.insert({ teamName, p });
        }
        else if (tag == "EVMAP") {
            string name, date;
            getline(ss, name, '|'); getline(ss, date);
            reg.eventsMap[name] = date;
        }
        else if (tag == "AC") {
            string idStr, ach;
            getline(ss, idStr, '|'); getline(ss, ach);
            reg.achievements.insert({ stoi(idStr), ach });
        }
        else if (tag == "LG") {
            string log; getline(ss, log);
            reg.logs.insert(log);
        }
        else if (tag == "EV") {
            string idxStr, typeStr;
            getline(ss, idxStr, '|'); getline(ss, typeStr, '|');
            int idx = stoi(idxStr);

            // Защита от выхода за границы массива events
            if (idx >= 0 && idx < 3) {
                if (typeStr == "A") {
                    string name, ageStr, country, sport, expStr, medalsStr;
                    getline(ss, name, '|'); getline(ss, ageStr, '|'); getline(ss, country, '|');
                    getline(ss, sport, '|'); getline(ss, expStr, '|'); getline(ss, medalsStr);
                    reg.getEvent(idx).addAthlete(new Athlete(name, stoi(ageStr), country, sport, stoi(expStr), stoi(medalsStr)));
                }
                else {
                    string name, ageStr, country, sport, expStr;
                    getline(ss, name, '|'); getline(ss, ageStr, '|'); getline(ss, country, '|');
                    getline(ss, sport, '|'); getline(ss, expStr);
                    reg.getEvent(idx).addCoach(new Coach(name, stoi(ageStr), country, sport, stoi(expStr)));
                }
            }
        }
    }
    return hasData;
}