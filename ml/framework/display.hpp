//
// Created by Jeremy S on 2017-11-19.
//

#pragma once

#include "config.hpp"
#include "../tsafe_ptr.hpp"

namespace ml {

    struct p_report {
        size_t thread_id;
        size_t round_num;
        double best;
        double average;
        double delta_best;
        double delta_average;
    };




    // Display Singleton
    class display {
    private:

        long next_index = 0;

        std::vector<std::vector<p_report>> data{};

        std::string format_f (const char *format, double val) {
            char str[32];
            std::string prefix = val < 0 ? "-" : " ";
            sprintf(str, format, util::abs(val));
            return prefix + std::string{str};
        }

        std::string format_i (const char *format, int val) {
            auto str = new char[32];
            sprintf(str, format, util::abs(val));
            return std::string{str};
        }

    public:
        static lang::tsafe_ptr<display> interface;

        bool locked = false;

        // Setup the rows for the data matrix.
        void setup () {
            for (size_t i = 0; i < NUM_THREADS; ++i) {
                data.emplace_back(std::vector<p_report>{});
            }
        }

        // Adds a report to the specific row based on the thread id enclosed in the report.
        void add (p_report const &report) {
            data.at(report.thread_id).push_back(report);
        }

        void print_report () {

            locked = true;
            std::this_thread::sleep_for(std::chrono::milliseconds{1});

            std::string indent = "          ";
            std::string separator = "  ";

            const char *numericalFormat = "%.5f";
            char EOL = '\n';

            std::string title;
            title = format_i("Report %d", (int)next_index);
            title += " - Elapsed Time: " + util::as_str(flags::global->clock.elapsed());

            std::string header = indent;
            std::string underline = indent;
            for (size_t i = 0; i < data.size(); ++i) {
                header += format_i("Thread %d", (int)i) + separator;
                underline += "--------" + separator;
            }

            std::string rowBest     = "Best    : ";
            std::string rowAverage  = "Average : ";
            std::string rowDeltaB   = "Delta B : ";
            std::string rowDeltaA   = "Delta A : ";

            for (auto row : data) {
                if (!row.empty()) {
                    rowBest += format_f(numericalFormat, row.back().best) + separator;
                    rowAverage += format_f(numericalFormat, row.back().average) + separator;
                    rowDeltaB += format_f(numericalFormat, row.back().delta_best) + separator;
                    rowDeltaA += format_f(numericalFormat, row.back().delta_average) + separator;
                } else {
                    rowBest += "********" + separator;
                    rowAverage += "********" + separator;
                    rowDeltaB += "********" + separator;
                    rowDeltaA += "********" + separator;
                }
            }

            for (auto d : data) {
                d.clear();
            }
            locked = false;

            std::string masterString =
                    title + EOL
                    + header + EOL
                    + underline + EOL
                    + rowBest + EOL
                    + rowAverage + EOL
                    + rowDeltaB + EOL
                    + rowDeltaA + EOL;

            ++next_index;

            std::cout << masterString << std::endl;
        }

    }; // class display

    lang::tsafe_ptr<display> display::interface{new display{}};

    inline void display_add_data (p_report const &data) {
        display::interface.exec<p_report>(data, [](display &src, p_report const &arg) {
            src.add(arg);
        });
    }

}
