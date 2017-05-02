#pragma once
#include <vector>
#include <utility>

#include "ADSR.h"
#include "NoteScheduler.h"
#include "BinaryDataSync.h"

class Multisampler : public NoteQueueCollection, public ADSR {
private:
    size_t nrows, ncols;
    unsigned int samplerate;
    float position = 0;
    uint64_t previous_time = 0;
    float average_time_available = 0;
    float average_time_used = 0;
    
protected:
    BinaryDataSync<int16_t> sources;
    BinaryDataSync<float> metadata;
    std::vector<std::pair<int, float>> lookup;
    
public:
    void setup(int rows, int cols, int samplerate);
    void load(std::string filename);
    size_t rows() const;
    size_t cols() const;
    void on(int note);
    void set_position(float position);
    void audio_loop(std::vector<float>& audio, unsigned int samplerate);
    float get_performance() const;
};
