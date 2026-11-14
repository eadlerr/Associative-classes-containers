#pragma once
#include <iostream>
#include "Event.h"
#include "SportsCommittee.h"

void printTableHeader(std::ostream& os);
void printEventTable(std::ostream& os, const Event& e);
void saveAll(const SportsCommittee& reg);
bool loadAll(SportsCommittee& reg);
