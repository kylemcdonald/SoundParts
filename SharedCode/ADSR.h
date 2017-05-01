#pragma once

class ADSR {
private:
    
    struct adr {
        float attack = 0;
        float decay = 0;
        float release = 0;
    };
    
    adr ref, lim;
    float sustain = 1;
    
    void limit();
    
public:
    
    ADSR() { }
    
    // not normalized to length
    float get_envelope(float time, float noteoff, float length, bool* done);
    // normalized to length
    float get_envelope(float time, float noteoff, bool* done);
    
    void set_attack(float attack);
    void set_decay(float decay);
    void set_sustain(float sustain);
    void set_release(float release);
};
