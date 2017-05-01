/* Copyright 2017 Kyle McDonald */
#include "KeycodeToMidi.h"
#include "MidiUtils.h"

#include <map>
#include <set>

// Ableton style keyboard => MIDI layout
//  w e   t y u   o p
// a s d f g h j k l ; '
const std::map<int, int> keycodeToMidiLookup = {
    {'a', 0},
    {'w', 1},
    {'s', 2},
    {'e', 3},
    {'d', 4},
    {'f', 5},
    {'t', 6},
    {'g', 7},
    {'y', 8},
    {'h', 9},
    {'u', 10},
    {'j', 11},
    {'k', 12},
    {'o', 13},
    {'l', 14},
    {'p', 15},
    {';', 16},
    {'\'', 17}
};

const std::set<int> keycodeSet = {
    'a','w','s','e','d','f','t','g','y','h','u','j','k','o','l','p',';','\''
};

int keycodeToMidiNote(int keycode, int octave) {
    if(keycodeSet.find(keycode) == keycodeSet.end()) {
        return -1;
    }
    return keyAndOctaveToMidiNote(keycodeToMidiLookup.at(keycode), octave);
}
