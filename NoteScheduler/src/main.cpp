/* Copyright 2017 Kyle McDonald */
#include "ofMain.h"
#include "KeycodeToMidi.h"
#include "NoteScheduler.h"

class ofApp : public ofBaseApp {
public:
    int octave = 0;
    NoteQueueCollection queues;
    
    ofTrueTypeFont font;
    
    void setup() {
        ofBackground(0);
        queues.setup();
        font.load(OF_TTF_MONO, 8);
    }
    void update() {
        queues.update();
    }
    void draw() {
        ofTranslate(10, 20);
        ofSetColor(255);
        
        int i = 0;
        for(const auto& queue : queues.queues) {
            font.drawString(ofToString(i), 0, 0);
            int j = 0;
            for(const auto& note : queue.queue) {
                font.drawString(ofToString(j), 0, 40 + j * 20);
                j++;
            }
            ofTranslate(20, 0);
            i++;
        }
    }
    void keyPressed(int key) {
        int note = keycodeToMidiNote(key, octave);
        if(note > -1) {
            int source = 0;
            float rate = 1;
            // we need to get these two from the sources
            queues.on(note, source, rate);
        }
    }
    void keyReleased(int key) {
        int note = keycodeToMidiNote(key, octave);
        if(note > -1) {
            queues.off(note);
        }
    }
};

int main() {
    ofSetupOpenGL(1280, 720, OF_WINDOW);
    ofRunApp(new ofApp());
}
