/*
 * File:   hourglass.h
 *
 * Use it if you want, but don't blame me for the result.. Stefan.
 */
#ifndef _HOURGLASS_H
#define	_HOURGLASS_H

#include <sys/time.h>
#include <string>

namespace utility
{

class Hourglass
{

public:
    ~Hourglass(){};

    Hourglass()
    {
      isStarted = false;
    }

// Methods
public:
    inline void start();
    inline void stop(std::string message);

private:
    struct timeval t1, t2;
    bool isStarted;
};

// Methods.

// Starts the timer, but only if it was not started before.. Perhaps the timer
// should be reset, but I like it this way..
inline void Hourglass::start()
{
    // Start time
    if(isStarted == false)
    {
      gettimeofday(&t1, NULL); // Get start time for this render
      isStarted = true;
    }

}

// Stops the timer and prints the supplied message with the recorded time in
// seconds added to it. The pipe simbols are there so it more easily imported in
// a spreadsheet program..
inline void Hourglass::stop(std::string message)
{
    // End time
    gettimeofday(&t2, NULL);
    float cost = (float) (((t2.tv_sec - t1.tv_sec) * 1000 * 1000) + (t2.tv_usec - t1.tv_usec));
    printf("TIMER REPORT| Message: %s | time: |%lf| sec\n", message.c_str(), cost / (1000 * 1000));
    isStarted = false;
}
};
#endif	/* _HOURGLASS_H */
