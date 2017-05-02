/* Copyright 2017 Kyle McDonald */
#pragma once

#include <deque>
#include <vector>

class Note {
private:
    bool done = false;
    
public:
    float rate = 1;
    unsigned int source = 0;
    unsigned int time = 0;
    unsigned int noteoff = 0;
    
    Note(int source=0, float rate=1)
    :source(source)
    ,rate(rate) {
    }
    
    /// Called when the note has been released.
    void off() {
        noteoff = time;
    }
    /// Called when the note is no longer playing.
    void set_done() {
        done = true;
    }
    /// True when the note is no longer needed.
    bool is_done() const {
        return done;
    }
};

template <class Container, class UnaryPredicate>
void erase_and_remove_if(Container& container, UnaryPredicate predicate) {
    container.erase(std::remove_if(container.begin(),
                                   container.end(),
                                   predicate),
                    container.end());
}

/// Don't call on() and update() from different threads.
class NoteQueue {
private:
    std::deque<Note> queue;
    
public:
    void on(const Note& note) {
        queue.push_front(note);
    }
    void off() {
        if(queue.empty()) {
            // this happens when a note has been removed
            // after playing out, but the key was never lifted
            // until now
            return;
        }
        queue.front().off();
    }
    void clear_done() {
        erase_and_remove_if(queue, [](Note& note){
            return note.is_done();
        });
    }
    void clear() {
        queue.clear();
    }
    std::deque<Note>& get() {
        return queue;
    }
};

class NoteQueueCollection {
private:
    int minNote, maxNote;
    std::vector<NoteQueue> queues;
    
public:
    void setup(int minNote=0, int maxNote=127) {
        this->minNote = minNote;
        this->maxNote = maxNote;
        queues.resize(maxNote - minNote);
    }
    void clear_done() {
        for(auto& queue : queues) {
            queue.clear_done();
        }
    }
    void on(int note, int source, float rate=1) {
        if(note < minNote || note >= maxNote) {
            return;
        }
        int i = note - minNote;
        queues[i].on(Note(source, rate));
    }
    void off(int note) {
        if(note < minNote || note >= maxNote) {
            return;
        }
        int i = note - minNote;
        queues[i].off();
    }
    void clear() {
        for(auto& queue : queues) {
            queue.clear();
        }
    }
    std::vector<NoteQueue>& get() {
        return queues;
    }

};
