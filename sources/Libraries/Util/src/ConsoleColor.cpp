// Реализация функций управления цветом консоли Windows.
// Использует SetConsoleTextAttribute для вывода цветного текста.

#include "ConsoleColor.h"
#include <algorithm>
#include <windows.h>

namespace TalesOfPirate::Utils::Console {

    int stoc(std::string a) {
        // Приведение к нижнему регистру, замена '_' и '-' на пробелы
        std::transform(a.begin(), a.end(), a.begin(), [](char c) {
            if ('A' <= c && c <= 'Z')
                c = c - 'A' + 'a';
            else if (c == '_' || c == '-')
                c = ' ';
            return c;
        });

        return (CODES.find(a) != CODES.end()) ? CODES.at(a) : BAD_COLOR;
    }

    int stoc(std::string a, std::string b) {
        return itoc(stoc(a), stoc(b));
    }

    std::string ctos(int c) {
        return (0 <= c && c < 256) ? "(text) " + NAMES.at(c % 16) + " + " +
                                         "(background) " + NAMES.at(c / 16)
                                   : "BAD COLOR";
    }

    int get() {
        CONSOLE_SCREEN_BUFFER_INFO i;
        return GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &i) ? i.wAttributes : BAD_COLOR;
    }

    int get_text() {
        return (get() != BAD_COLOR) ? get() % 16 : BAD_COLOR;
    }

    int get_background() {
        return (get() != BAD_COLOR) ? get() / 16 : BAD_COLOR;
    }

    void set(int c) {
        if (is_good(c))
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);
    }

    void set(int a, int b) {
        set(a + b * 16);
    }

    void set(std::string a, std::string b) {
        set(stoc(a) + stoc(b) * 16);
    }

    void set_text(std::string a) {
        set(stoc(a), get_background());
    }

    void set_background(std::string b) {
        set(get_text(), stoc(b));
    }

    void reset() {
        set(DEFAULT_COLOR);
    }

    int invert(int c) {
        if (is_good(c)) {
            int a = c % 16;
            int b = c / 16;
            return b + a * 16;
        } else
            return BAD_COLOR;
    }

    std::ostream &reset(std::ostream &os) {
        reset();
        return os;
    }

    std::ostream &black(std::ostream &os) {
        set_text("k");
        return os;
    }

    std::ostream &blue(std::ostream &os) {
        set_text("b");
        return os;
    }

    std::ostream &green(std::ostream &os) {
        set_text("g");
        return os;
    }

    std::ostream &aqua(std::ostream &os) {
        set_text("a");
        return os;
    }

    std::ostream &bright_white_on_light_yellow(std::ostream &os) {
        set("bw", "ly");
        return os;
    }

    std::ostream &bright_white_on_bright_white(std::ostream &os) {
        set("bw", "bw");
        return os;
    }

}
