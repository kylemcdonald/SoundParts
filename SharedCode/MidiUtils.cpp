/* Copyright 2017 Kyle McDonald */
#include "MidiUtils.h"
#include <cmath>

float getFrequency(int midiNote) {
    const static float frequencyA4 = 440.;
    const static int midiNoteA4 = 69;
    return frequencyA4 * pow(2, (midiNote - midiNoteA4) / 12.);
}

int clampMidiNote(int note) {
    if(note < 0) {
        return 0;
    } else if(note > 127) {
        return 127;
    }
    return note;
}

int clampMidiOctave(int octave) {
    if(octave < -1) {
        return -1;
    }
    if(octave > 9) {
        return 9;
    }
    return octave;
}

int keyAndOctaveToMidiNote(int key, int octave) {
    octave = clampMidiOctave(octave);
    int note = 12 * (octave + 1) + key;
    return clampMidiNote(note);
}
