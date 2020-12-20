/**
 * @file
 * @author Max Godefroy
 * @date 27/11/2020.
 */

#ifndef INC_KRYNE_ENGINE_STRINGUTILS_HPP
#define INC_KRYNE_ENGINE_STRINGUTILS_HPP

#include <iostream>


using namespace std;


namespace Utils {

    /**
     * Sets up a safe line by line string iteration stream. Handles all line separators (`\n`, `\r\n` and `\r`)
     * @param is    The input string stream to read the lines from
     * @param t     The output string reference
     * @return A reference to the \p is parameter value
     */
    istream& safeGetLine(istream& is, string& t)
    {
        t.clear();

        // The characters in the stream are read one-by-one using a std::streambuf.
        // That is faster than reading them one-by-one using the std::istream.
        // Code that uses streambuf this way must be guarded by a sentry object.
        // The sentry object performs various tasks,
        // such as thread synchronization and updating the stream state.

        istream::sentry se(is, true);
        streambuf* sb = is.rdbuf();

        for(;;) {
            int c = sb->sbumpc();
            switch (c) {
                case '\n':
                    return is;
                case '\r':
                    if(sb->sgetc() == '\n')
                        sb->sbumpc();
                    return is;
                case streambuf::traits_type::eof():
                    // Also handle the case when the last line has no line ending
                    if(t.empty())
                        is.setstate(ios::eofbit);
                    return is;
                default:
                    t += (char)c;
            }
        }
    }

}

#endif //INC_KRYNE_ENGINE_STRINGUTILS_HPP
