#pragma once

#if __cplusplus == 202002L
    #include <format>
#else
    #include <memory>
    #include <string>
    #include <stdexcept>
#endif

namespace seam {
    // credits to: https://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
    template<typename ... Args>
    std::string string_format(const std::string& format, Args... args) {
#if __cplusplus == 202002L
        return std::format(format, args);
#else
        int size = snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
        if( size <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
        std::unique_ptr<char[]> buf( new char[ size ] ); 
        snprintf( buf.get(), size, format.c_str(), args ... );
        return std::string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
#endif
    }
}