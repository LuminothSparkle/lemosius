#ifndef INOUT_H
#define INOUT_H

#include<filesystem>
#include<fstream>
#include<vector>

std::vector<char> read_file(const std::filesystem::path& path) {
    std::ifstream input;
    input.exceptions( std::ifstream::failbit | std::ifstream::badbit );
    input.open( path );
    auto len = std::filesystem::file_size( path );
    std::vector<char> buf( len + 1, '\0' );
    input.read( buf.data( ), len );
    return buf;
}

#endif
