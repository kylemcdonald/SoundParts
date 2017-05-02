/* Copyright 2017 Kyle McDonald */
#include "ofMain.h"
#include "ofxGui.h"
#include "ADSR.h"

class ofApp : public ofBaseApp {
public:
    ofxPanel gui;
    ofxFloatSlider a, d, s, r, off;
    
    ADSR adsr;
    
    void setup() {
        ofBackground(0);
        ofSetColor(255);
        ofSetLineWidth(2);
        
        gui.setup();
        gui.add(a.setup("Attack", 0.25, 0, 1));
        gui.add(d.setup("Decay", 0.25, 0, 1));
        gui.add(s.setup("Sustain", 0.75, 0, 1));
        gui.add(r.setup("Release", 0.25, 0, 1));
        gui.add(off.setup("Note Off", 0, 0, 1));
    }
    void update() {
        adsr.set_attack(a);
        adsr.set_decay(d);
        adsr.set_sustain(s);
        adsr.set_release(r);
    }
    void draw() {
        gui.draw();
        
        int w = ofGetWidth();
        int h = ofGetHeight();
        ofTranslate(0, h);
        ofScale(1, -h);
        ofNoFill();
        ofBeginShape();
        bool prevdone = false;
        float length = 3 * w / 4;
        float noteoff = off * length;
        for(int i = 0; i < w; i++) {
            bool done;
            float env = adsr.get_envelope(i, i < noteoff ? 0 : noteoff, length, &done);
            if(done != prevdone) {
                ofDrawLine(i, 0, i, ofGetHeight());
            }
            prevdone = done;
            ofVertex(i, env);
        }
        ofEndShape();
        if(off > 0) {
            ofDrawLine(noteoff, 0, noteoff, h);
        }
    }
};

int main() {
    ofSetupOpenGL(1024, 256, OF_WINDOW);
    ofRunApp(new ofApp());
}
