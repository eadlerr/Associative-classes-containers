#include "Event.h"

void Event::clear() {
    for (Athlete* a : athletes) delete a;
    for (Coach* c : coaches) delete c;
    athletes.clear();
    coaches.clear();
}
