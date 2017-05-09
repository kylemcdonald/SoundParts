#pragma once
#include <vector>
#include <utility>

#include "ADSR.h"
#include "NoteScheduler.h"
#include "BinaryDataSync.h"

#define USE_ASYNC

#ifdef USE_ASYNC
#include "BinaryDataAsync.h"
#endif

class Multisampler : public NoteQueueCollection, public ADSR {
private:
    size_t nrows, ncols;
    unsigned int samplerate;
    float position = 0;
    float volume = 1;
    uint64_t previous_time = 0;
    float average_time_available = 0;
    float average_time_used = 0;
    
protected:
#ifdef USE_ASYNC
    BinaryDataAsync<int16_t> sources;
#else
    BinaryDataSync<int16_t> sources;
#endif
    BinaryDataSync<float> metadata;
    std::vector<std::pair<int, float>> lookup;
    
    virtual void load_audio(std::string filename);
    virtual void load_metadata(std::string filename);
    static std::vector<std::pair<int, float>> buildMultisampleLookup(const std::vector<std::pair<int, int>>& centers);
    
public:
    void setup(int rows, int cols, int samplerate);
    void load(std::string filename);
    size_t rows() const;
    size_t cols() const;
    void on(int note);
    void set_position(float position);
    void set_volume(float volume);
    void audio_loop(std::vector<float>& audio, unsigned int samplerate);
    float get_performance() const;
};
