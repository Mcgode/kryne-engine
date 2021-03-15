/**
 * @file
 * @author Max Godefroy
 * @date 15/03/2021.
 */

#pragma once


#include <vector>
#include <memory>


using namespace std;


namespace Utils {

    template <class T>
    struct UniquePtrVector {

        using VectorType = vector<unique_ptr<T>>;
        using IteratorType = typename VectorType::iterator;
        using ConstIteratorType = typename VectorType::const_iterator;

    public:

        inline unique_ptr<T> &emplace_back(T *pointer) { return _vector.emplace_back(pointer); }


        inline IteratorType begin() { return _vector.begin(); }


        inline ConstIteratorType begin() const { return _vector.begin(); }


        inline IteratorType end() { return _vector.end(); }


        inline ConstIteratorType end() const { return _vector.end(); }


        [[nodiscard]] inline size_t size() const { return _vector.size(); }


        /**
         * @brief Inserts a unique pointer to the vector
         *
         * @details
         * Will insert the unique pointer by right shifting the unique pointers values (using swap()) until reaching the
         * end of the iterator, upon which a new unique pointer will be emplaced with the last value.
         *
         * Complexity is linear.
         *
         * @param position  The iterator pointing to the insert position.
         * @param value     The unique pointer to insert into the vector.
         *
         * @returns An iterator pointing to the newly inserted element.
         */
        IteratorType insert(IteratorType position, unique_ptr<T> value)
        {
            // Save original position. Iterator will still be valid by then since the unique pointer structs stay in
            // place (only their content is swapped)
            const auto originalPosition = position;

            // Right shift
            while (position != _vector.end())
            {
                swap(value, *position);
                position++;
            }

            _vector.emplace_back(value.release());

            return originalPosition;
        }

        /**
         * @brief Erases a unique pointer from the vector
         *
         * @details
         * Will erase the unique pointer by left shifting the unique pointers values (using swap()) until reaching the
         * end of the iterator, upon which the last element will be popped.
         *
         * Complexity is linear.
         *
         * @param position  The iterator pointing to the erase position.
         *
         * @returns An iterator pointing to the element following .
         */
        IteratorType erase(IteratorType position)
        {
            if (position == _vector.end())
                return _vector.end();

            const auto originalPosition = position;
            auto previous = position++;
            while (position != _vector.end())
            {
                swap(*previous, *position);
                previous = position++;
            }

            _vector.pop_back();

            return originalPosition;
        }

    private:

        /// Internal vector
        VectorType _vector;

    };

}