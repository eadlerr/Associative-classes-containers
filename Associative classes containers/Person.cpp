#include "Person.h"
#include "Utils.h"

using namespace std;

Person::Person(string n, int a, string c, string s, int exp, int id, int r)
    : name(n), age(a), country(c), sport(s), experience(exp), id(id), rating(r) {
}

void Person::printRow(ostream& os, int num) const {
    os << "  "
        << padR(to_string(num), T::NUM) << string(T::SEP, ' ')
        << padR(name, T::NAME) << string(T::SEP, ' ')
        << padR(to_string(age), T::AGE) << string(T::SEP, ' ')
        << padR(country, T::COUNTRY) << string(T::SEP, ' ')
        << padR(sport, T::SPORT) << string(T::SEP, ' ')
        << padR(to_string(experience), T::EXP) << string(T::SEP, ' ')
        << padR(getMedalsStr(), T::MEDALS) << string(T::SEP, ' ')
        << padR(to_string(rating), T::RATING) << "\n";
}

void Person::edit() {
    cout << "  Текущее ФИО: " << name << "\n";
    if (askBool("Изменить ФИО?")) name = askNonEmptyString("Новое ФИО: ");
    cout << "  Текущий возраст: " << age << "\n";
    if (askBool("Изменить возраст?")) age = askPositiveInt("Новый возраст: ");
    cout << "  Текущая страна: " << country << "\n";
    if (askBool("Изменить страну?")) country = askNonEmptyString("Новая страна: ");
    cout << "  Текущий вид спорта: " << sport << "\n";
    if (askBool("Изменить вид спорта?")) sport = askNonEmptyString("Новый вид спорта: ");
    cout << "  Текущий опыт: " << experience << "\n";
    if (askBool("Изменить опыт?")) experience = askPositiveInt("Новый опыт (лет): ");
    cout << "  Текущий рейтинг: " << rating << "\n";
    if (askBool("Изменить рейтинг?")) rating = askInt("Новый рейтинг (0-100): ", 0, 100);
}

Athlete::Athlete(string n, int a, string c, string s, int exp, int m, int id, int r)
    : Person(n, a, c, s, exp, id, r), medals(m) {
}

void Athlete::edit() {
    Person::edit();
    cout << "  Текущие медали: " << medals << "\n";
    if (askBool("Изменить медали?")) medals = askInt("Новое количество медалей: ", 0, 100);
}

Coach::Coach(string n, int a, string c, string s, int exp, int id, int r)
    : Person(n, a, c, s, exp, id, r) {
}
