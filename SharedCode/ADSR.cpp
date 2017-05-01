#include "ADSR.h"

float remap(const float& value, const float& istart, const float& istop, const float& ostart, const float& ostop) {
    return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
}

const float& clamp(const float& v, const float& lo, const float& hi) {
    if(v < lo) return lo;
    if(v > hi) return hi;
    return v;
}

void ADSR::limit() {
    float sum = ref.attack + ref.decay + ref.release;
    if(sum > 1) {
        lim.attack = ref.attack / sum;
        lim.decay = ref.decay / sum;
        lim.release = ref.release / sum;
    } else {
        lim.attack = ref.attack;
        lim.decay = ref.decay;
        lim.release = ref.release;
    }
}

// not normalized to length
float ADSR::get_envelope(float time, float noteoff, float length, bool* done) {
    return get_envelope(time / length, noteoff / length, done);
}

// normalized to length
float ADSR::get_envelope(float time, float noteoff, bool* done) {
    *done = false;
    if(noteoff > 0) { // release (noteoff)
        if((time - noteoff) >= lim.release) { // done
            *done = true;
            return 0;
        }
        return sustain * (time - noteoff / lim.release); // releasing
    }
    if(time <= lim.attack) { // attack
        return time / lim.attack;
    }
    if((time - lim.attack) <= lim.decay) { // decay
        return remap(time - lim.attack, 0, lim.decay, 1, sustain);
    }
    if((1 - time) <= lim.release) { // release
        if(time >= 1) {
            *done = true;
            return 0;
        }
        return remap(time, 1 - lim.release, 1, sustain, 0);
    }
    return sustain; // sustain, default
}

void ADSR::set_attack(float attack) {
    ref.attack = clamp(attack, 0, 1);
    limit();
}

void ADSR::set_decay(float decay) {
    ref.decay = clamp(decay, 0, 1);
    limit();
}

void ADSR::set_sustain(float sustain) {
    this->sustain = clamp(sustain, 0, 1);
}

void ADSR::set_release(float release) {
    ref.release = clamp(release, 0, 1);
    limit();
}
