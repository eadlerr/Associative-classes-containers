#pragma once
#include <string>

const std::string DATA_FILE = "sport_data.txt";
const std::string REPORT_FILE = "sport_report.txt";

namespace T {
    const int NUM = 4;
    const int NAME = 22;
    const int AGE = 8;
    const int COUNTRY = 15;
    const int SPORT = 18;
    const int EXP = 12;
    const int MEDALS = 10;
    const int RATING = 10;
    const int SEP = 2;

    const int W_MAIN = NUM + NAME + AGE + COUNTRY + SPORT + EXP + MEDALS + RATING + 7 * SEP;
}

std::string padR(const std::string& s, size_t w);
int askInt(const std::string& prompt, int minV = -999999, int maxV = 999999);
int askPositiveInt(const std::string& prompt);
std::string askNonEmptyString(const std::string& prompt);
bool askBool(const std::string& prompt);
