/**
 * @file
 * @author Max Godefroy
 * @date 17/03/2021.
 */

#pragma once


#include <iostream>
#include <sstream>


namespace Utils {

    /**
     * @brief An helper struct for generating unique labels linked to a specific object
     */
    struct ImGuiLabel {

        /// The label string
        std::string value;

        /**
         * @brief Builds a Dear ImGui label string.
         *
         * @param label     The base label string
         * @param object    A pointer to the object this label is linked to.
         * @param hidden    Set to true to hide the label
         */
        ImGuiLabel(const std::string &label, void *object, bool hidden = false)
        {
            std::stringstream fmt;

            if (hidden)
                fmt << "##";

            fmt << label << "##" << std::hex << object;

            this->value = fmt.str();
        }

        operator const char *() const
        {
            return value.c_str();
        }

    };

}