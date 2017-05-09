#include "Multisampler.h"
#include "MidiUtils.h"
#include <cmath>
#include <iostream>
#include <chrono>

uint64_t get_time_ms() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}
uint64_t get_time_ns() {
    using namespace std::chrono;
    return duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
}

// This could be optimized by sorting the input then stepping.
// Or only compute the pitches once.
/// Input: vector of N (note, index) pairs
/// Output: vector of 128 (index, rate) pairs
std::vector<std::pair<int, float>> Multisampler::buildMultisampleLookup(const std::vector<std::pair<int, int>>& centers) {
    std::vector<std::pair<int, float>> lookup(128);
    for(int i = 0; i < 128; i++) {
        int nearestDistance = -1;
        for(size_t j = 0; j < centers.size(); j++) {
            int note = centers[j].first;
            int curDistance = std::abs(i - note);
            if(nearestDistance == -1 || curDistance < nearestDistance) {
                nearestDistance = curDistance;
                float target = getFrequency(i);
                float source = getFrequency(note);
                lookup[i].first = centers[j].second;
                lookup[i].second = target / source;
            }
        }
    }
    return lookup;
}

void Multisampler::setup(int rows, int cols, int samplerate) {
    this->nrows = rows;
    this->ncols = cols;
    this->samplerate = samplerate;
    NoteQueueCollection::setup();
}

void Multisampler::load_audio(std::string filename) {
    auto start = get_time_ms();
    sources.load(filename, nrows, ncols);
    auto stop = get_time_ms();
    std::cout << "load time: " << (stop - start) << "ms" << std::endl;
}

void Multisampler::load_metadata(std::string filename) {
    metadata.load(filename, nrows, 1);
    std::vector<std::pair<int, int>> centers;
    for(size_t i = 0; i < nrows; i++) {
        centers.emplace_back(metadata.get_element(i, 0), i);
    }
    lookup = buildMultisampleLookup(centers);
}

void Multisampler::load(std::string filename) {
    clear();
    load_audio(filename + ".audio.bin");
    load_metadata(filename + ".meta.bin");
}

size_t Multisampler::rows() const {
    return nrows;
}

size_t Multisampler::cols() const {
    return ncols;
}

void Multisampler::on(int note) {
    std::pair<int, float>& settings = lookup[note];
    unsigned int offset = position * ncols;
    NoteQueueCollection::on(note, settings.first, settings.second, offset);
}

void Multisampler::set_position(float position) {
    if(position < 0) position = 0;
    if(position > 1) position = 1;
    this->position = position;
}

void Multisampler::set_volume(float volume) {
    if(volume < 0) volume = 0;
    if(volume > 1) volume = 1;
    this->volume = volume;
}

void Multisampler::audio_loop(std::vector<float>& audio, unsigned int samplerate) {
    auto start = get_time_ns();
    std::fill(audio.begin(), audio.end(), 0);
    size_t cols = sources.cols();
    float samplerate_ratio = float(this->samplerate) / samplerate;
    float normalization = 1./std::numeric_limits<int16_t>::max();
    for(auto& queue : queues) {
        for(auto& note : queue.get()) {
            const auto& source = sources[note.source];
            
            size_t samples = audio.size();
            float start_time = note.time;
            float stop_time = start_time + samples;
            
            bool done = false;
            unsigned int length = cols - note.offset;
            float amp_start = normalization * get_envelope(start_time, note.noteoff, length, &done);
            float amp_stop = normalization * get_envelope(stop_time, note.noteoff, length, &done);
            float amp_step = (amp_stop - amp_start) / samples;
            float amp = amp_start;
            
            float i_start = (note.offset + start_time) * note.rate * samplerate_ratio;
            float i_stop = (note.offset + stop_time) * note.rate * samplerate_ratio;
            float i_step = (i_stop - i_start) / samples;
            float i = i_start;
            
            for(auto& sample : audio) {
                if(i+1 >= cols) {
                    break;
                }
                
                // linear interpolation for resampling
                unsigned int i0 = i;
                float t = i - i0;
                float interpolated = source[i0] * (1-t) + source[i0+1] * t;
                sample += interpolated * amp;

                i += i_step;
                amp += amp_step;
            }
            note.time += samples;
            
            if(done) {
                note.set_done();
            }
        }
    }
    if(volume < 1) {
        for(auto& sample : audio) {
            sample = volume;
        }
    }
    clear_done();
    auto stop = get_time_ns();
    if(previous_time > 0) {
        auto cur_time_used = stop - start;
        average_time_used = average_time_used*.99 + cur_time_used*.01;
        auto cur_time_available = start - previous_time;
        average_time_available = average_time_available*.99 + cur_time_available*.01;
    }
    previous_time = start;
}

float Multisampler::get_performance() const {
    return average_time_used / average_time_available;
}
