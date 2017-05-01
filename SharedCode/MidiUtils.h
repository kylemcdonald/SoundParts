#pragma once

/// Get the frequency in Hertz of a MIDI note.
float getFrequency(int midiNote);

/// Clamp a note to the range [0, 127]
int clampMidiNote(int note);

/// Clamp an octave to the range [-1, 7]
int clampMidiOctave(int octave);

/// Convert a key and octave to a MIDI note.
/// Examples: 0, 4 => 60 ; 0, -1 => 0
int keyAndOctaveToMidiNote(int key, int octave);
