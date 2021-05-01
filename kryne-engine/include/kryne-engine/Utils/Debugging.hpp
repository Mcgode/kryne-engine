/**
 * @file
 * @author Max Godefroy
 * @date 26/02/2021.
 */

#pragma once


#include <string>
#include <sstream>
#include <vector>
#include <regex>
#include <iostream>


using namespace std;


namespace Utils {

    void parseAndDebugShaderInfoLog(const string &infoLog, const vector<string>& code)
    {
        istringstream stream(infoLog);
        string infoLine;

        regex lineRegex("([0-9]+)\\)");
        smatch results;

        while (getline(stream, infoLine))
        {
            cerr << endl;
            cerr << infoLine << endl;

            if (regex_search(infoLine, results, lineRegex))
            {
                size_t line = stoi(results[1].str());
                for (size_t l = max((size_t) 0, line - 2); l < min(line + 2, code.size()); l++)
                    cerr << "    " << code[l] << endl;
            }
        }
    }

}