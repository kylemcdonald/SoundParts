#include "ofMain.h"
#include "KeycodeToMidi.h"
#include "MidiUtils.h"
#include "Multisampler.h"

class ofApp : public ofBaseApp {
public:
    int octave = 5;
    ofTrueTypeFont smallFont, bigFont;
    Multisampler synth;
    ofSoundStream ss;
    vector<float> monoBuffer;
    unsigned int sampleRate;
    
    void setup() {
        ofBackground(0);
        setupSound();
        setupSynth();
        smallFont.load(OF_TTF_MONO, 8);
        bigFont.load(OF_TTF_MONO, 32);
        ofSetLineWidth(3);
    }
    void setupSound() {
        ofSoundStreamSettings settings;
        sampleRate = 48000;
        settings.sampleRate = sampleRate;
        settings.numInputChannels = 0;
        settings.numOutputChannels = 2;
        settings.bufferSize = 32;
        settings.numBuffers = 0;
        settings.setOutListener(this);
        ss.setup(settings);
    }
    void setupSynth() {
        int rows = 8;
        int cols = 64000;
        synth.setup(rows, cols, 16000);
        synth.set_attack(0.01);
        synth.set_decay(0.01);
        synth.set_sustain(0.90);
        synth.set_release(1.00);
        synth.load(ofToDataPath("../../../SharedData/samples"));
    }
    void update() {
        synth.set_position(mouseX / float(ofGetWidth()));
    }
    void draw() {
        ofSetColor(255);
        
        ofPushMatrix();
        ofTranslate(10, 30);
        int i = 0;
        for(auto& queue : synth.get()) {
            float x = ofMap(i, 0, 128, 0, ofGetWidth() - 10);
            ofPushMatrix();
            ofTranslate(x + 7, 0);
            ofRotateZDeg(-90);
            smallFont.drawString(ofToString(i), 0, 0);
            ofPopMatrix();
            int j = 0;
            for(auto& note : queue.get()) {
                ofDrawRectangle(x, 20 + j * 20, 5, 5);
                j++;
            }
            i++;
        }
        ofPopMatrix();
        
        ofPushMatrix();
        ofBeginShape();
        ofNoFill();
        int n = monoBuffer.size();
        ofTranslate(0, ofGetHeight() / 2);
        ofScale(ofGetWidth() / float(n-1), ofGetHeight() / 2);
        for(int i = 0; i < n; i++) {
            ofVertex(i, monoBuffer[i]);
        }
        ofEndShape();
        ofPopMatrix();
        
        float performance = synth.get_performance();
        bigFont.drawString(ofToString(int(100 * performance)) + "%", 32, ofGetHeight() - 64);
    }
    void keyPressed(int key) {
        int note = keycodeToMidiNote(key, octave);
        if(note > -1) {
            synth.on(note);
        }
        if(key == 'z') {
            octave--;
        } else if(key == 'x') {
            octave++;
        }
        octave = clampMidiOctave(octave);
    }
    void keyReleased(int key) {
        int note = keycodeToMidiNote(key, octave);
        if(note > -1) {
            synth.off(note);
        }
    }
    void audioOut(ofSoundBuffer& buffer) {
        int n = buffer.size() / 2;
        monoBuffer.resize(n);
        synth.audio_loop(monoBuffer, sampleRate);
        int j = 0;
        for(int i = 0; i < n; i++) {
            buffer[j++] = monoBuffer[i];
            buffer[j++] = monoBuffer[i];
        }
    }
};

int main() {
    ofSetupOpenGL(1280, 720, OF_WINDOW);
    ofRunApp(new ofApp());
}
