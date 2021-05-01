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

        enum Parts {
            ObjectsScripting = 0,
            Rendering,
            UI,
            AsyncScripting,
            EventPolling,
            COUNT
        };

        struct PartDurations {

            using DurationPair = std::pair<std::string, duration<double>>;

            std::vector<DurationPair> recordedTimes {};

            duration<double> time {};

            void reset()
            {
                this->recordedTimes.clear();
                time = duration<double>::zero();
            }

            void addTime(const std::string &name, const duration<double> &t)
            {
                this->time += t;
                this->recordedTimes.emplace_back(name, t);
            }

        };

        struct Data {

            duration<double> frameTime {};

            PartDurations recordedTimes[Parts::COUNT];

            Data() = default;

        };

    public:

        Data resetTimer()
        {
            const auto now = system_clock::now();
            Data data {};
            data.frameTime = now - this->start;

            for (auto i = 0; i < Parts::COUNT; i++)
                data.recordedTimes[i] = this->recordedTimes[i];

            this->start = now;

            for (auto &d : this->recordedTimes)
                d.reset();

            return data;
        }

        inline void recordTime(Parts part, const std::string &name, const duration<double> &time)
        {
            this->recordedTimes[part].addTime(name, time);
        }

        inline void recordTime(Parts part, const duration<double> &time)
        {
            this->recordedTimes[part].time += time;
        }

    protected:

        system_clock::time_point start {};

        PartDurations recordedTimes[Parts::COUNT];

    };

}


