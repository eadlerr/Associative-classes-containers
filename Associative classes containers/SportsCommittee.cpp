#include "SportsCommittee.h"
#include <iostream>

using namespace std;

SportsCommittee::SportsCommittee() : committeeName("Городской спорткомитет") {
    events[0].setEventName("Кубок города");
    events[0].setDate("01.12.2026");
    events[1].setEventName("Чемпионат области");
    events[1].setDate("15.12.2026");
    events[2].setEventName("Первенство республики");
    events[2].setDate("25.12.2026");
}

SportsCommittee::~SportsCommittee() {
    clear();
    for (auto& p : participants) delete p.second;
}

void SportsCommittee::clear() {
    for (size_t i = 0; i < events.size(); ++i) {
        events[i].clear();
    }
}

Person* SportsCommittee::findParticipant(int id) const {
    auto it = participants.find(id);
    return it == participants.end() ? nullptr : it->second;
}

void SportsCommittee::fillDemo() {
    clear();
    for (auto& p : participants) delete p.second;
    participants.clear(); sports.clear(); teamMembers.clear(); leaderboard.clear();
    eventsMap.clear(); teams.clear(); achievements.clear(); logs.clear();

    Athlete* a1 = new Athlete("Иванов И.И.", 22, "Беларусь", "Легкая атлетика", 5, 3, 1, 95);
    Athlete* a2 = new Athlete("Смирнова Е.А.", 21, "Беларусь", "Гимнастика", 4, 5, 2, 88);
    Athlete* a3 = new Athlete("Петров П.П.", 24, "Россия", "Плавание", 6, 1, 3, 75);
    Coach* c1 = new Coach("Сидоров С.С.", 45, "Беларусь", "Легкая атлетика", 20, 4, 92);
    Coach* c2 = new Coach("Козлов К.К.", 50, "Беларусь", "Гимнастика", 25, 5, 85);

    participants.insert({ 1, a1 });
    participants.insert({ 2, a2 });
    participants.insert({ 3, a3 });
    participants.insert({ 4, c1 });
    participants.insert({ 5, c2 });

    sports.insert("Легкая атлетика"); sports.insert("Гимнастика"); sports.insert("Плавание");

    teamMembers.insert({ "Сборная БГУИР", a1 }); teamMembers.insert({ "Сборная БГУИР", c1 });
    teamMembers.insert({ "Сборная БГУ", a2 }); teamMembers.insert({ "Сборная БГУ", c2 });
    teamMembers.insert({ "Динамо", a3 });

    leaderboard.insert(a1); leaderboard.insert(a2); leaderboard.insert(a3);
    leaderboard.insert(c1); leaderboard.insert(c2);

    eventsMap["Зимняя универсиада"] = "14.11.2026";
    eventsMap["Кубок Минска"] = "20.11.2026";

    teams.insert("Сборная БГУИР"); teams.insert("Сборная БГУ"); teams.insert("Динамо");

    achievements.insert({ 1, "Золото 100м" }); achievements.insert({ 1, "Серебро эстафета" });
    achievements.insert({ 2, "Золото многоборье" }); achievements.insert({ 3, "Бронза 50м в/с" });

    logs.insert("Система инициализирована"); logs.insert("Загружен реестр спортсменов"); logs.insert("Обновлены данные команд");

    events[0].addAthlete(new Athlete("Иванов А.П.", 22, "Беларусь", "Легкая атлетика", 5, 3));
    events[0].addAthlete(new Athlete("Смирнов Д.О.", 19, "Россия", "Плавание", 3, 1));
    events[0].addCoach(new Coach("Соколов В.И.", 45, "Беларусь", "Плавание", 20));

    cout << "  Тестовые данные загружены!\n";
}
