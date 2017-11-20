#pragma once

// A simple and clean clock that may be used to track durations.
// Provides a nice and simple way to print the duration (h:m:s)
// to the screen.

#include <cstdlib>
#include <ctime>
#include <ratio>
#include <chrono>
#include <string>
#include <ostream>

namespace util {

    // A data type to hold a time value in hours, minutes, and seconds.
    struct time_t {

        // Constructor from hours, minutes, and seconds.
        time_t (size_t h, size_t m, size_t s)
                : __hours(h), __minutes(m), __seconds(s)
        {
            distribute();
        }

        // Constructor form only seconds.
        // Will distribute values properly over the measurements.
        //  e.g. time_t{62} -> 00:01:02
        explicit time_t (size_t s)
                : __hours(0), __minutes(0), __seconds(s)
        {
            distribute();
        }

        // Default constructor, init to 0
        time_t ()
                : __hours(0), __minutes(0), __seconds(0)
        {}

        // Geters
        size_t hours () const {
            return __hours;
        }

        size_t minutes () const {
            return __minutes;
        }

        size_t seconds () const {
            return __seconds;
        }


        // Adds a given number of seconds to the time object.
        // Distributes the values after adding them.
        void add_seconds (size_t sec) {
            __seconds += sec;
            distribute();
        }

        // Adds a given number of minutes to the time object and distributes.
        void add_minutes (size_t min) {
            __minutes += min;
            distribute();
        }

        // Adds a given number of hours to the time object.
        void add_hours (size_t h) {
            __hours += h;
        }

        // Converts the time object into a scalar value measured in seconds.
        //  e.g. 00:02:00 -> 120
        size_t condence () {
            return (__hours * 60 * 60) + (__minutes * 60) + __seconds;
        }

    private:

        size_t __hours;
        size_t __minutes;
        size_t __seconds;

        // Distributes the values over the measurments.
        void distribute () {
            while (__seconds >= 60) {
                __seconds -= 60;
                __minutes += 1;
            }
            while (__minutes >= 60) {
                __minutes -= 60;
                __hours += 1;
            }
        }
    };


    // Converts a number into a 2 digit string.
    //  e.g. 1 -> "01"
    inline std::string as_2_digit_str (size_t x) {
        std::string str;
        if (x < 10) {
            str += "0";
        }
        str += std::to_string(x);
        return str;
    }


    // Converts a time_t object into a string
    //  Format: "<h>:<m>:<s>"
    inline std::string as_str (time_t t) {
        return as_2_digit_str(t.hours()) + ":" 
             + as_2_digit_str(t.minutes()) + ":"
             + as_2_digit_str(t.seconds());
    }


    // A simple clock that can be used to measure the duration of tasks.
    struct clock {

        std::chrono::steady_clock::time_point start_time;

        // Initalizes start_time to the current time.
        clock ()
            : start_time(std::chrono::steady_clock::now())
        {}

        // Sets start_time to the current time.
        void set_start () {
            start_time = std::chrono::steady_clock::now();
        }

        // Returns the elapsed time since start_time as a time_t object.
        time_t elapsed () {
            std::chrono::steady_clock::time_point call_time = std::chrono::steady_clock::now();
            std::chrono::duration<size_t> duration = 
                std::chrono::duration_cast<std::chrono::duration<size_t>>(call_time - start_time);
            return time_t{duration.count()};
        }

    };

}


inline std::ostream &operator<< (std::ostream &os, util::time_t const &t) {
    os << util::as_str(t);
    return os;
}
