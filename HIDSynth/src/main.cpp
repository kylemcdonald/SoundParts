#include "ofMain.h"
#include "HIDKeyboardEvent.h"

class ofApp : public ofBaseApp, public HIDKeyboardReceiver {
public:
    ofSoundStream ss;
    bool pressed = false;
    
    void setup() {
        ofSoundStreamSettings settings;
        settings.sampleRate = 48000;
        settings.numInputChannels = 0;
        settings.numOutputChannels = 2;
        settings.bufferSize = 32;
        settings.numBuffers = 0;
        settings.setOutListener(this);
        ss.setup(settings);
        
        setupHIDCallback(this);
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
    void HIDKeyboardEvent(int charcode, bool press) {
        printf("%c %s\n", charcode, press ? "down" : "up");
        if(press) {
            pressed = true;
        }
    }
};

int main() {
    ofGLFWWindowSettings settings;
    settings.width = 256;
    settings.height = 256;
    settings.windowMode = OF_WINDOW;
    ofCreateWindow(settings);
    ofRunApp(new ofApp());
}
