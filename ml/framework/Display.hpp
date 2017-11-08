#pragma once

#include <memory>
#include <vector>
#include <string>
#include <cmath>
#include <cstdio>
#include "Config.hpp"

namespace ml {

    struct ProgressReport {
        int threadId;
        int roundNum;
        double best;
        double average;
        double changeBest;
        double changeAverage;
    };


    class Display {
    private:

        long nextIndex = 0;

        std::vector<std::vector<ProgressReport>> data{};

        // Checks to see if each thread has given enough
        // data to print out the next report.
        bool isReadyToPrint () {
            for (size_t i = 0; i < data.size(); ++i) {
                if (long(data[i].size()) - 1 < nextIndex) {
                    return false;
                }
            }
            return true;
        }

        // Returns a formated string for a floating point number.
        // Special case is handeled for negative numbers so that
        // formating is correct.
        //  e.g.
        //      " 0.123"
        //      "-0.123"
        std::string formatFloatingPoint (const char* format, double val) {
            char *str = nullptr;
            std::string prefix = val < 0 ? "-" : " ";
            sprintf(str, format, abs(val));
            return prefix + std::string{str};
        }

        std::string formatInteger (const char* format, int val) {
            char *str = nullptr;
            sprintf(str, format, val);
            return std::string{str};
        }

    public:

        // Singleton instance
        static std::shared_ptr<Display> instance;

        // Setup the data matrix.
        void setup (size_t numThreads) {
            for (size_t i = 0; i < numThreads; ++i) {
                data.push_back(std::vector<ProgressReport>{});
            }
        }

        // Inserts some data into the data set.
        // Assumes setup(int) has been called.
        void addData (ProgressReport const &d) {
            data[d.threadId].push_back(d);
        }

        // Returns a progress report as a string that may be printed to the screen.
        // Returns nullptr if unable to create a progress report.
        std::shared_ptr<std::string> getNextReport (bool force = false) {

            if (!isReadyToPrint() && !force) {
                return nullptr;
            }

            std::string indent{10, ' '};
            std::string separator{2, ' '};

            const char* numericalFormat = "%.5f";
            char EOL = '\n';
            
            std::string title;
            if (force) {
                title = formatInteger("Report %d", (int)nextIndex);
            } else {
                title = "Report (forced)";
            }

            std::string header = indent;
            std::string underline = indent;
            for (size_t i = 0; i < data.size(); ++i) {
                header += formatInteger("Thread %d", (int)i) + separator;
                underline += "--------" + separator;
            }

            std::string rowBest     = "Best    : ";
            std::string rowAverage  = "Average : ";
            std::string rowDeltaB   = "Delta B : ";
            std::string rowDeltaA   = "Delta A : ";

            for (size_t i = 0; i < data.size(); ++i) {
                if (!force) {
                    rowBest += formatFloatingPoint(numericalFormat, data[i][nextIndex].best) + separator;
                    rowAverage += formatFloatingPoint(numericalFormat, data[i][nextIndex].average) + separator;
                    rowDeltaB += formatFloatingPoint(numericalFormat, data[i][nextIndex].changeBest) + separator;
                    rowDeltaA += formatFloatingPoint(numericalFormat, data[i][nextIndex].changeAverage) + separator;
                } else {
                    rowBest += formatFloatingPoint(numericalFormat, data[i].back().best) + separator;
                    rowAverage += formatFloatingPoint(numericalFormat, data[i].back().average) + separator;
                    rowDeltaB += formatFloatingPoint(numericalFormat, data[i].back().changeBest) + separator;
                    rowDeltaA += formatFloatingPoint(numericalFormat, data[i].back().changeAverage) + separator;
                }
            }

            std::shared_ptr<std::string> masterString = std::shared_ptr<std::string>{new std::string {
                          title + EOL 
                        + header + EOL
                        + underline + EOL
                        + rowBest + EOL
                        + rowAverage + EOL
                        + rowDeltaB + EOL
                        + rowDeltaA + EOL
            }};

            if (!force) {
                nextIndex += Config::global->outputInterval;
            } 

            return masterString;
        }


    };

}


std::shared_ptr<ml::Display> ml::Display::instance{new ml::Display{}};
