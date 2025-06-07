#pragma once
#include <chrono>
#include <cstdint>
#include <iostream>
#include <string>
#include <thread>
#include <mutex>

class ProgressBar {
  public:
    ProgressBar (std::size_t max_ticks,
                 std::size_t bar_width = 50,
                 const std::string& delimeter = "#",
                 const std::string& empty_delimeter = " ") {
        _moment_of_creation = std::chrono::steady_clock::now();
        _all_ticks = max_ticks;
        _bar_width = bar_width;
        _delimeter = delimeter;
        _empty_delimeter = empty_delimeter;
    }

    void tick() {
        std::lock_guard<std::mutex> lock(_mutex);
        _current_ticks += 1;
        _show_status();
    }

  private:
    std::chrono::time_point<std::chrono::steady_clock> _moment_of_creation;
    std::size_t _all_ticks;
    std::size_t _current_ticks = 0;
    std::size_t _bar_width;
    std::string _delimeter;
    std::string _empty_delimeter;
    std::mutex _mutex;

    struct Time_interval {
        float time_delta;
        std::string time_unit;
    };

    Time_interval _get_time_interval(std::size_t time_in_nanoseconds) {
        Time_interval time_interval{time_in_nanoseconds, "ns"};
        if (time_interval.time_delta > 1000) {
            time_interval.time_delta = time_interval.time_delta / 1000.0;
            time_interval.time_unit = "um";
            if (time_interval.time_delta > 1000 ) {
                time_interval.time_delta = time_interval.time_delta / 1000.0;
                time_interval.time_unit = "miliseconds";
                if (time_interval.time_delta > 1000) {
                    time_interval.time_delta = time_interval.time_delta / 1000.0;
                    time_interval.time_unit = "seconds";
                    if (time_interval.time_delta > 60) {
                        time_interval.time_delta = time_interval.time_delta / 60.0;
                        time_interval.time_unit = "minutes";
                        if (time_interval.time_delta > 60) {
                            time_interval.time_delta = time_interval.time_delta / 60.0;
                            time_interval.time_unit = "hours";
                            if (time_interval.time_delta > 24) {
                                time_interval.time_delta = time_interval.time_delta / 24.0;
                                time_interval.time_unit = "days";
                                if (time_interval.time_delta > 7) {
                                    time_interval.time_delta = time_interval.time_delta / 7.0;
                                    time_interval.time_unit = "weeks";
                                }
                            }
                        }
                    }
                }
            }
        }
        return time_interval;
    }

    void _show_status() {
        float current_percent = static_cast<float>(_current_ticks) / static_cast<float>(_all_ticks);
        std::size_t number_of_elements_to_show = static_cast<float>(_bar_width) * current_percent;
        std::string progress_bar = "|";
        for (std::size_t i = 0; i < _bar_width; i++) {
            if (i <= number_of_elements_to_show) {
                progress_bar += _delimeter;
            } else {
                progress_bar += _empty_delimeter;
            }
        }
        progress_bar += std::string("| ") + std::to_string(current_percent*100).substr(0,5) + std::string("%, ");
        auto time_passed = (std::chrono::steady_clock::now() - _moment_of_creation).count();
        auto estimated_time = (time_passed / current_percent) - time_passed;
        auto time_interval = _get_time_interval(estimated_time);
        progress_bar += std::to_string(time_interval.time_delta).substr(0, 5) + std::string(" ") + time_interval.time_unit + std::string(" left");
        std::cout << progress_bar << std::endl;
    }
};