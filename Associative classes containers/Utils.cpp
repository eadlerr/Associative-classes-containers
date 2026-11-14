#include "Utils.h"
#include <iostream>

using namespace std;

string padR(const string& s, size_t w) {
    if (s.length() >= w) return s;
    return s + string(w - s.length(), ' ');
}

int askInt(const string& prompt, int minV, int maxV) {
    string input;
    while (true) {
        cout << "  " << prompt;
        getline(cin, input);
        try {
            size_t pos;
            int val = stoi(input, &pos);
            if (pos == input.length()) {
                if (val >= minV && val <= maxV) return val;
                cout << "  [Ошибка] Введите число от " << minV << " до " << maxV << "\n";
            }
        }
        catch (...) {}
        cout << "  [Ошибка] Введите целое число!\n";
    }
}

int askPositiveInt(const string& prompt) {
    while (true) {
        int val = askInt(prompt, 1, 999999);
        if (val > 0) return val;
    }
}

string askNonEmptyString(const string& prompt) {
    string s;
    while (true) {
        cout << "  " << prompt;
        getline(cin, s);
        if (!s.empty()) return s;
        cout << "  [Ошибка] Строка не может быть пустой!\n";
    }
}

bool askBool(const string& prompt) {
    string input;
    while (true) {
        cout << "  " << prompt << " (1-да, 0-нет): ";
        getline(cin, input);
        if (input == "1") return true;
        if (input == "0") return false;
        cout << "  [Ошибка] Введите 1 или 0.\n";
    }
}
