/* Copyright 2017 Kyle McDonald */
#include "ofMain.h"
#include "ofxMidi.h"

class ofApp : public ofBaseApp, public ofxMidiListener  {
public:
    ofSoundStream ss;
    ofxMidiIn midiIn;
    bool pressed = false;
    
    void setup() {
        ofBackground(0);
        ofSetColor(255);
        ofSetLineWidth(2);
        
        midiIn.listPorts();
        midiIn.openPort("Q25");
        midiIn.addListener(this);
        
        ofSoundStreamSettings settings;
        settings.sampleRate = 48000;
        settings.numInputChannels = 0;
        settings.numOutputChannels = 2;
        settings.bufferSize = 32;
        settings.numBuffers = 0;
        settings.setOutListener(this);
        ss.setup(settings);
    }
    void update() {
    }
    void draw() {
    }
    void audioOut(ofSoundBuffer& buffer) {
        if(pressed) {
            buffer.getBuffer()[0] = +1;
            buffer.getBuffer()[1] = +1;
            buffer.getBuffer()[2] = -1;
            buffer.getBuffer()[3] = -1;
            pressed = false;
        }
    }
    void newMidiMessage(ofxMidiMessage& msg) {
        if(msg.status == MIDI_NOTE_ON) {
            cout << "on: " << msg.pitch << " " << msg.velocity << endl;
            pressed = true;
        } else if(msg.status == MIDI_NOTE_OFF) {
            cout << "off: " << msg.pitch << endl;
        }
    }
};

int main() {
    ofSetupOpenGL(1024, 256, OF_WINDOW);
    ofRunApp(new ofApp());
}
