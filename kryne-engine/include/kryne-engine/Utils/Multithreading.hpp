/**
 * @file
 * @author Max Godefroy
 * @date 28/03/2021.
 */

#pragma once


#include <mutex>


namespace Utils {

    /**
     * A BasicLockable structure, for locking and unlocking two mutexes at the same time without deadlock.
     */
    struct double_lock {

        using std::mutex;

        /// First mutex to lock/unlock
        mutex &m1;

        /// Second mutex to lock/unlock
        mutex &m2;

        /**
         * @brief Initializes and locks the lock.
         *
         * @param m1 First mutex to lock/unlock
         * @param m2 Second mutex to lock/unlock
         */
        double_lock(mutex &m1, mutex &m2): m1(m1), m2(m2) { lock(); }

        /**
         * @brief unlock on delete
         */
        ~double_lock() { unlock(); }

        double_lock(const double_lock &) = delete;
        double_lock operator=(const double_lock &) = delete;

        /**
         * @brief Locks both mutexes with no deadlock
         */
        void lock() { std::lock(m1, m2); }

        /**
         * @brief Unlocks both mutexes
         */
        void unlock() { m1.unlock(); m2.unlock(); }

    };

}