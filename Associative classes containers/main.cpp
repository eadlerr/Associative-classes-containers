#include <iostream>
#include "SportsCommittee.h"
#include "FileManager.h"
#include "Menu.h"
#include "Utils.h"

#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#endif

using namespace std;

int main() {
#ifdef _WIN32
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
#endif
    SportsCommittee reg;

    if (!loadAll(reg)) {
        reg.fillDemo();
        saveAll(reg);
        cout << "  [Файл не найден] Созданы тестовые данные и сохранены в " << DATA_FILE << "\n";
    }
    else {
        cout << "  [OK] Данные успешно загружены из файла " << DATA_FILE << "\n";
    }

    while (true) {
        cout << "\n===== СПОРТИВНЫЙ РЕЕСТР =====\n";
        cout << "1. Участники (map)\n";
        cout << "2. Виды спорта (set)\n";
        cout << "3. Составы команд (multimap)\n";
        cout << "4. Рейтинг (multiset)\n";
        cout << "5. Мероприятия (unordered_map)\n";
        cout << "6. Команды (unordered_set)\n";
        cout << "7. Достижения (unordered_multimap)\n";
        cout << "8. Журнал (unordered_multiset)\n";
        cout << "9. Сбросить/загрузить тест заново\n";
        cout << "10. Показать соревнования (Event из 3 лабы)\n";
        cout << "0. Выход\n";

        int c = askInt("Выбор: ", 0, 10);
        if (c == 0) break;
        if (c == 1) menuParticipants(reg);
        if (c == 2) menuSports(reg);
        if (c == 3) menuTeamMembers(reg);
        if (c == 4) menuLeaderboard(reg);
        if (c == 5) menuEvents(reg);
        if (c == 6) menuTeams(reg);
        if (c == 7) menuAchievements(reg);
        if (c == 8) menuLogs(reg);
        if (c == 9) { reg.fillDemo(); saveAll(reg); }
        if (c == 10) menuEventsClass(reg);
    }
    return 0;
}