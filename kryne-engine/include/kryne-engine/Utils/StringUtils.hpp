/**
 * @file
 * @author Max Godefroy
 * @date 27/11/2020.
 */

#ifndef INC_3D_DEMOS_STRINGUTILS_HPP
#define INC_3D_DEMOS_STRINGUTILS_HPP

#include <iostream>


class StringUtils {

public:

    static std::istream& safeGetLine(std::istream& is, std::string& t)
    {
        t.clear();

        // The characters in the stream are read one-by-one using a std::streambuf.
        // That is faster than reading them one-by-one using the std::istream.
        // Code that uses streambuf this way must be guarded by a sentry object.
        // The sentry object performs various tasks,
        // such as thread synchronization and updating the stream state.

        std::istream::sentry se(is, true);
        std::streambuf* sb = is.rdbuf();

        for(;;) {
            int c = sb->sbumpc();
            switch (c) {
                case '\n':
                    return is;
                case '\r':
                    if(sb->sgetc() == '\n')
                        sb->sbumpc();
                    return is;
                case std::streambuf::traits_type::eof():
                    // Also handle the case when the last line has no line ending
                    if(t.empty())
                        is.setstate(std::ios::eofbit);
                    return is;
                default:
                    t += (char)c;
            }
        }
    }

};

#endif //INC_3D_DEMOS_STRINGUTILS_HPP
