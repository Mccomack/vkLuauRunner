export module utf8;
import std;

export namespace utf8 {
    std::string index(std::string_view str, std::size_t& i);
}

std::string utf8::index(std::string_view str, std::size_t& i) {
    unsigned char c = str[i];
    std::size_t len = 1;
    if ((c & 0x80) == 0x00)
        len = 1;  // ASCII
    else if ((c & 0xE0) == 0xC0)
        len = 2;
    else if ((c & 0xF0) == 0xE0)
        len = 3;
    else if ((c & 0xF8) == 0xF0)
        len = 4;

    std::string ch = std::string(str.substr(i, len));
    i += len;
    return ch;
}