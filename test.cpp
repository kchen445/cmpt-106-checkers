#include <iostream>
#include <iomanip>

const std::string ansii_null = "\e[0m";
const std::string ansii_white = "\e[47m";
const std::string ansii_red = "\e[41m";
const std::string ansii_a = "\e[30m";
const std::string ansii_b = "\e[30m";

std::string w = ansii_white + ansii_a + "   " + ansii_null;
std::string b = ansii_black + ansii_b + "   " + ansii_null;

const std::string tab = std::string(28, ' ');
const std::string header = std::string(8, '\n');
const std::string footer = std::string(7, '\n');

struct Point {
    int x, y;
};

bool should_put_piece_a (Point const &pos) {
    return 
        (pos.x == 0 && pos.y == 0) ||
        (pos.x == 2 && pos.y == 0) ||
        (pos.x == 4 && pos.y == 0) ||
        (pos.x == 6 && pos.y == 0) ||
        (pos.x == 1 && pos.y == 1) ||
        (pos.x == 3 && pos.y == 1) ||
        (pos.x == 5 && pos.y == 1) ||
        (pos.x == 7 && pos.y == 1);

}

bool should_put_piece_b (Point const &pos) {
    return 
        (pos.x == 0 && pos.y == 6) ||
        (pos.x == 2 && pos.y == 6) ||
        (pos.x == 4 && pos.y == 6) ||
        (pos.x == 6 && pos.y == 6) ||
        (pos.x == 1 && pos.y == 7) ||
        (pos.x == 3 && pos.y == 7) ||
        (pos.x == 5 && pos.y == 7) ||
        (pos.x == 7 && pos.y == 7);
}

void print_board_pos (Point const &pos, bool is_w, bool is_pa) {
    std::string ansii;

    if (is_w) {
        ansii = ansii_white;
    } else {
        ansii = ansii_black;
    }

    if (is_pa) {
        ansii += ansii_a;
    } else {
        ansii += ansii_b;
    }

    if (should_put_piece_a(pos)) {
        std::cout << ansii << " 0 " << ansii_null;
    } else if (should_put_piece_b(pos)) {
        std::cout << ansii << " O " << ansii_null;
    } else {
        std::cout << ansii << "   " << ansii_null;
    }
}

void erase_screen () {
    std::cout << "\e[H";
    std::cout << "\e[J";
}

int main () {
    // std::cout << (char)153 << std::endl; // error
    // std::cout << "\u00D6" << std::endl; // ok

    erase_screen();

    bool print_w = true;
    bool is_pa = true;
    Point pos = {0, 0};

    std::cout << header;
    std::cout << tab;

    for (size_t i = 1; i <= 64; ++i) {

        print_board_pos(pos, print_w, is_pa);

        print_w = !print_w;

        if (i == 32) {
            is_pa = false;
        }

        if (i % 8 == 0) {
            std::cout << std::endl << tab;
            print_w = !print_w;
            ++pos.y;
            pos.x = 0;
        } else {
            ++pos.x;
        }
    }

    std::cout << footer;

}
