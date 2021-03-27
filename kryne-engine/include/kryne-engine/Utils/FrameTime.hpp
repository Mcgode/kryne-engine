/**
 * @file
 * @author Max Godefroy
 * @date 27/03/2021.
 */

#pragma once


#include <chrono>
#include <unordered_map>
#include <string>


namespace Utils {

    using std::chrono::system_clock;
    using std::chrono::duration;


    class FrameTime {

    public:

        struct Data {

            duration<double> frameTime {};

            std::unordered_map<std::string, duration<double>> recordedTimes {};

            Data() = default;

        };

    public:

        Data resetTimer()
        {
            const auto now = system_clock::now();
            Data data {};
            data.frameTime = now - this->start;
            data.recordedTimes = this->recordedTimes;
            this->start = now;
            this->recordedTimes.clear();
            return data;
        }

        void recordTime(const std::string &name, const duration<double> &time)
        {
            this->recordedTimes.emplace(name, time);
        }

    protected:

        system_clock::time_point start {};

        std::unordered_map<std::string, duration<double>> recordedTimes;

    };

}


