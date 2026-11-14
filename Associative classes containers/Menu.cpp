#include "Menu.h"
#include "Utils.h"
#include "FileManager.h"
#include <iostream>
#include <vector>

using namespace std;

void menuParticipants(SportsCommittee& reg) {
    while (true) {
        cout << "\n--- Участники (map) ---\n 1. Показать\n 2. Добавить\n 3. Редактировать (по ID)\n 4. Удалить\n 0. Назад\n";
        int c = askInt("Выбор: ", 0, 4);
        if (c == 0) break;

        if (c == 1) {
            printTableHeader(cout);
            int idx = 1;
            for (auto& p : reg.participants) p.second->printRow(cout, idx++);
            cout << "  " << string(T::W_MAIN, '-') << "\n  Записей: " << reg.participants.size() << "\n";
        }
        else if (c == 2) {
            int t = askInt("Тип (1-Спортсмен, 2-Тренер): ", 1, 2);
            int id = reg.participants.empty() ? 1 : reg.participants.rbegin()->first + 1;

            string name = askNonEmptyString("ФИО: ");
            int age = askPositiveInt("Возраст: ");
            string country = askNonEmptyString("Страна: ");
            string sport = askNonEmptyString("Вид спорта: ");
            int exp = askPositiveInt("Опыт (лет): ");
            int rating = askInt("Рейтинг (0-100): ", 0, 100);

            Person* p = nullptr;
            if (t == 1) {
                int medals = askInt("Медали: ", 0, 100);
                p = new Athlete(name, age, country, sport, exp, medals, id, rating);
            }
            else {
                p = new Coach(name, age, country, sport, exp, id, rating);
            }

            reg.participants.insert({ id, p });
            reg.leaderboard.insert(p);
            if (!p->getSport().empty()) reg.sports.insert(p->getSport());
            cout << "  [OK] Добавлено с ID " << id << "\n";
            saveAll(reg);
        }
        else if (c == 3) {
            int id = askInt("ID для редактирования: ", 1, 10000);
            Person* p = reg.findParticipant(id);
            if (p) {
                // Удаляем из рейтинга ПЕРЕД редактированием, чтобы multiset не сломался
                for (auto it = reg.leaderboard.begin(); it != reg.leaderboard.end(); ++it) {
                    if (*it == p) { reg.leaderboard.erase(it); break; }
                }

                p->edit();

                // Возвращаем в рейтинг (встанет на новое место по рейтингу)
                reg.leaderboard.insert(p);
                if (!p->getSport().empty()) reg.sports.insert(p->getSport());

                cout << "  [OK] Обновлено!\n";
                saveAll(reg);
            }
            else cout << "  [Ошибка] Нет такого ID\n";
        }
        else if (c == 4) {
            if (reg.participants.empty()) { cout << "  Список пуст.\n"; continue; }
            vector<Person*> list;
            cout << "\n";
            for (auto& p : reg.participants) {
                cout << "  " << list.size() + 1 << ". " << p.second->getName() << " (ID: " << p.first << ")\n";
                list.push_back(p.second);
            }
            int choice = askInt("Введите № для удаления (0 - отмена): ", 0, (int)list.size());
            if (choice > 0) {
                Person* toDelete = list[choice - 1];
                int delId = toDelete->getId();

                for (auto aIt = reg.teamMembers.begin(); aIt != reg.teamMembers.end(); ) {
                    if (aIt->second == toDelete) aIt = reg.teamMembers.erase(aIt);
                    else ++aIt;
                }
                for (auto lIt = reg.leaderboard.begin(); lIt != reg.leaderboard.end(); ) {
                    if (*lIt == toDelete) lIt = reg.leaderboard.erase(lIt);
                    else ++lIt;
                }
                for (auto achIt = reg.achievements.begin(); achIt != reg.achievements.end(); ) {
                    if (achIt->first == delId) achIt = reg.achievements.erase(achIt);
                    else ++achIt;
                }
                delete toDelete;
                reg.participants.erase(delId);
                cout << "  [OK] Удалено\n";
                saveAll(reg);
            }
        }
    }
}

void menuSports(SportsCommittee& reg) {
    while (true) {
        cout << "\n--- Виды спорта (set) ---\n 1. Показать\n 2. Добавить\n 3. Удалить\n 0. Назад\n";
        int c = askInt("Выбор: ", 0, 3);
        if (c == 0) break;
        if (c == 1) {
            cout << "  " << string(30, '-') << "\n";
            for (auto& s : reg.sports) cout << "  " << s << "\n";
            cout << "  " << string(30, '-') << "\n  Записей: " << reg.sports.size() << "\n";
        }
        else if (c == 2) { reg.sports.insert(askNonEmptyString("Вид спорта: ")); saveAll(reg); }
        else if (c == 3) {
            if (reg.sports.empty()) { cout << "  Список пуст.\n"; continue; }
            vector<string> list(reg.sports.begin(), reg.sports.end());
            for (size_t i = 0; i < list.size(); ++i) cout << "  " << i + 1 << ". " << list[i] << "\n";
            int choice = askInt("Введите № для удаления (0 - отмена): ", 0, (int)list.size());
            if (choice > 0) {
                reg.sports.erase(list[choice - 1]);
                cout << "  [OK] Удалено\n";
                saveAll(reg);
            }
        }
    }
}

void menuTeamMembers(SportsCommittee& reg) {
    while (true) {
        cout << "\n--- Составы команд (multimap) ---\n 1. Показать\n 2. Добавить\n 3. Редактировать\n 4. Удалить\n 0. Назад\n";
        int c = askInt("Выбор: ", 0, 4);
        if (c == 0) break;

        if (c == 1) {
            if (reg.teamMembers.empty()) { cout << "  Составы команд пусты.\n"; continue; }
            string currentTeam; bool first = true; int idx = 1;
            for (const auto& p : reg.teamMembers) {
                if (first || p.first != currentTeam) {
                    if (!first) cout << "\n";
                    currentTeam = p.first; first = false; idx = 1;
                    cout << "  === Команда: " << currentTeam << " ===\n";
                    printTableHeader(cout);
                }
                p.second->printRow(cout, idx++);
            }
            cout << "  " << string(T::W_MAIN, '-') << "\n  Всего записей: " << reg.teamMembers.size() << "\n";
        }
        else if (c == 2) {
            string team = askNonEmptyString("Название команды: ");
            int id = askInt("ID участника (из общего списка): ", 1, 10000);
            Person* p = reg.findParticipant(id);
            if (p) { reg.teamMembers.insert({ team, p }); saveAll(reg); }
            else cout << "  [Ошибка] Нет такого ID\n";
        }
        else if (c == 3 || c == 4) {
            string team = askNonEmptyString("Название команды: ");
            auto range = reg.teamMembers.equal_range(team);
            if (range.first == range.second) {
                cout << "  В команде нет участников.\n";
                continue;
            }
            cout << "  === Команда: " << team << " ===\n";
            printTableHeader(cout);
            int idx = 1;
            vector<multimap<string, Person*>::iterator> iters;
            for (auto it = range.first; it != range.second; ++it) {
                it->second->printRow(cout, idx++);
                iters.push_back(it);
            }
            cout << "  " << string(T::W_MAIN, '-') << "\n";

            if (c == 3) {
                int choice = askInt("Введите № для редактирования (0 - отмена): ", 0, (int)iters.size());
                if (choice > 0) {
                    Person* p = iters[choice - 1]->second;

                    for (auto it = reg.leaderboard.begin(); it != reg.leaderboard.end(); ++it) {
                        if (*it == p) { reg.leaderboard.erase(it); break; }
                    }

                    cout << "\n  === РЕДАКТИРОВАНИЕ ===\n";
                    p->edit();

                    reg.leaderboard.insert(p);
                    if (!p->getSport().empty()) reg.sports.insert(p->getSport());

                    cout << "  [OK] Данные обновлены!\n";
                    saveAll(reg);
                }
            }
            else if (c == 4) {
                int choice = askInt("Введите № для удаления (0 - отмена): ", 0, (int)iters.size());
                if (choice > 0) {
                    reg.teamMembers.erase(iters[choice - 1]);
                    cout << "  [OK] Участник удален из команды.\n";
                    saveAll(reg);
                }
            }
        }
    }
}

void menuLeaderboard(SportsCommittee& reg) {
    while (true) {
        cout << "\n--- Рейтинг (multiset) ---\n 1. Узнать рейтинг участника (список)\n 2. Показать общий рейтинг\n 3. Добавить в рейтинг\n 4. Удалить\n 0. Назад\n";
        int c = askInt("Выбор: ", 0, 4);
        if (c == 0) break;

        if (c == 1) {
            if (reg.teamMembers.empty()) { cout << "  Составы команд пусты.\n"; continue; }
            vector<Person*> list;
            string currentTeam = "";
            int idx = 1;

            for (const auto& p : reg.teamMembers) {
                if (p.first != currentTeam) {
                    currentTeam = p.first;
                    cout << "\n  === Команда: " << currentTeam << " ===\n";
                }
                cout << "  " << idx << ". " << p.second->getName() << "\n";
                list.push_back(p.second);
                idx++;
            }

            cout << "  " << string(50, '-') << "\n";
            int choice = askInt("Выберите номер участника для просмотра рейтинга (0 - отмена): ", 0, (int)list.size());
            if (choice > 0) {
                Person* p = list[choice - 1];
                cout << "\n========================================\n";
                cout << " Участник: " << p->getName() << "\n";
                cout << " Тип:     " << (p->getType() == 1 ? "Спортсмен" : "Тренер") << "\n";
                cout << " Рейтинг: " << p->getRating() << "\n";

                bool inLeaderboard = false;
                for (auto lp : reg.leaderboard) {
                    if (lp == p) { inLeaderboard = true; break; }
                }
                cout << " Статус:  " << (inLeaderboard ? "Включен в общий рейтинг" : "Отсутствует в рейтинге") << "\n";
                cout << "========================================\n";
            }
        }
        else if (c == 2) {
            cout << "  " << string(50, '-') << "\n";
            for (auto p : reg.leaderboard)
                cout << "  " << padR(p->getName(), 25) << " (Рейтинг: " << p->getRating() << ")\n";
            cout << "  " << string(50, '-') << "\n  Записей: " << reg.leaderboard.size() << "\n";
        }
        else if (c == 3) {
            int id = askInt("ID участника: ", 1, 10000);
            Person* p = reg.findParticipant(id);
            if (p) { reg.leaderboard.insert(p); cout << "  [OK] Добавлено в рейтинг.\n"; saveAll(reg); }
            else cout << "  [Ошибка] Нет такого ID\n";
        }
        else if (c == 4) {
            if (reg.leaderboard.empty()) { cout << "  Рейтинг пуст.\n"; continue; }
            vector<Person*> list(reg.leaderboard.begin(), reg.leaderboard.end());
            for (size_t i = 0; i < list.size(); ++i) {
                cout << "  " << i + 1 << ". " << list[i]->getName() << " (Рейтинг: " << list[i]->getRating() << ")\n";
            }
            int choice = askInt("Введите № для удаления (0 - отмена): ", 0, (int)list.size());
            if (choice > 0) {
                for (auto it = reg.leaderboard.begin(); it != reg.leaderboard.end(); ++it) {
                    if (*it == list[choice - 1]) { reg.leaderboard.erase(it); break; }
                }
                cout << "  [OK] Удалено\n";
                saveAll(reg);
            }
        }
    }
}

void menuEvents(SportsCommittee& reg) {
    while (true) {
        cout << "\n--- Мероприятия (unordered_map) ---\n 1. Показать\n 2. Добавить\n 3. Удалить\n 0. Назад\n";
        int c = askInt("Выбор: ", 0, 3);
        if (c == 0) break;
        if (c == 1) {
            cout << "  " << string(50, '-') << "\n";
            for (auto& p : reg.eventsMap) cout << "  " << padR(p.first, 25) << " -> " << p.second << "\n";
            cout << "  " << string(50, '-') << "\n  Записей: " << reg.eventsMap.size() << "\n";
        }
        else if (c == 2) {
            string name = askNonEmptyString("Название: ");
            string date = askNonEmptyString("Дата (дд.мм.гггг): ");
            reg.eventsMap[name] = date;
            saveAll(reg);
        }
        else if (c == 3) {
            if (reg.eventsMap.empty()) { cout << "  Список пуст.\n"; continue; }
            vector<pair<string, string>> list(reg.eventsMap.begin(), reg.eventsMap.end());
            for (size_t i = 0; i < list.size(); ++i) {
                cout << "  " << i + 1 << ". " << list[i].first << " (" << list[i].second << ")\n";
            }
            int choice = askInt("Введите № для удаления (0 - отмена): ", 0, (int)list.size());
            if (choice > 0) {
                reg.eventsMap.erase(list[choice - 1].first);
                cout << "  [OK] Удалено\n";
                saveAll(reg);
            }
        }
    }
}

void menuTeams(SportsCommittee& reg) {
    while (true) {
        cout << "\n--- Команды (unordered_set) ---\n 1. Показать\n 2. Добавить\n 3. Удалить\n 0. Назад\n";
        int c = askInt("Выбор: ", 0, 3);
        if (c == 0) break;
        if (c == 1) {
            cout << "  " << string(30, '-') << "\n";
            for (auto& t : reg.teams) cout << "  " << t << "\n";
            cout << "  " << string(30, '-') << "\n  Записей: " << reg.teams.size() << "\n";
        }
        else if (c == 2) { reg.teams.insert(askNonEmptyString("Название команды: ")); saveAll(reg); }
        else if (c == 3) {
            if (reg.teams.empty()) { cout << "  Список пуст.\n"; continue; }
            vector<string> list(reg.teams.begin(), reg.teams.end());
            for (size_t i = 0; i < list.size(); ++i) cout << "  " << i + 1 << ". " << list[i] << "\n";
            int choice = askInt("Введите № для удаления (0 - отмена): ", 0, (int)list.size());
            if (choice > 0) {
                reg.teams.erase(list[choice - 1]);
                cout << "  [OK] Удалено\n";
                saveAll(reg);
            }
        }
    }
}

void menuAchievements(SportsCommittee& reg) {
    while (true) {
        cout << "\n--- Достижения (unordered_multimap) ---\n 1. Показать\n 2. Добавить\n 3. Удалить\n 0. Назад\n";
        int c = askInt("Выбор: ", 0, 3);
        if (c == 0) break;
        if (c == 1) {
            cout << "  " << string(50, '-') << "\n";
            for (auto& p : reg.achievements) cout << "  ID " << p.first << ": " << p.second << "\n";
            cout << "  " << string(50, '-') << "\n  Записей: " << reg.achievements.size() << "\n";
        }
        else if (c == 2) {
            int id = askInt("ID участника: ", 1, 10000);
            reg.achievements.insert({ id, askNonEmptyString("Достижение: ") });
            saveAll(reg);
        }
        else if (c == 3) {
            if (reg.achievements.empty()) { cout << "  Список пуст.\n"; continue; }
            vector<pair<int, string>> list(reg.achievements.begin(), reg.achievements.end());
            for (size_t i = 0; i < list.size(); ++i) {
                cout << "  " << i + 1 << ". ID " << list[i].first << ": " << list[i].second << "\n";
            }
            int choice = askInt("Введите № для удаления (0 - отмена): ", 0, (int)list.size());
            if (choice > 0) {
                auto range = reg.achievements.equal_range(list[choice - 1].first);
                for (auto it = range.first; it != range.second; ++it) {
                    if (it->second == list[choice - 1].second) { reg.achievements.erase(it); break; }
                }
                cout << "  [OK] Удалено\n";
                saveAll(reg);
            }
        }
    }
}

void menuLogs(SportsCommittee& reg) {
    while (true) {
        cout << "\n--- Журнал (unordered_multiset) ---\n 1. Показать\n 2. Добавить\n 3. Удалить\n 0. Назад\n";
        int c = askInt("Выбор: ", 0, 3);
        if (c == 0) break;
        if (c == 1) {
            cout << "  " << string(50, '-') << "\n";
            for (auto& e : reg.logs) cout << "  " << e << "\n";
            cout << "  " << string(50, '-') << "\n  Записей: " << reg.logs.size() << "\n";
        }
        else if (c == 2) { reg.logs.insert(askNonEmptyString("Событие: ")); saveAll(reg); }
        else if (c == 3) {
            if (reg.logs.empty()) { cout << "  Список пуст.\n"; continue; }
            vector<string> list(reg.logs.begin(), reg.logs.end());
            for (size_t i = 0; i < list.size(); ++i) cout << "  " << i + 1 << ". " << list[i] << "\n";
            int choice = askInt("Введите № для удаления (0 - отмена): ", 0, (int)list.size());
            if (choice > 0) {
                auto it = reg.logs.find(list[choice - 1]);
                if (it != reg.logs.end()) reg.logs.erase(it);
                cout << "  [OK] Удалено\n";
                saveAll(reg);
            }
        }
    }
}

void menuEventsClass(SportsCommittee& reg) {
    cout << "\n  Выберите соревнование (класс Event из 3 лабы):\n";
    for (size_t i = 0; i < reg.getEvents().size(); ++i) {
        cout << "    " << (i + 1) << ". " << reg.getEvents()[i].getEventName() << "\n";
    }
    int i = askInt("  Номер: ", 1, 3);
    printEventTable(cout, reg.getEvents()[i - 1]);
}