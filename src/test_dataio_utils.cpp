/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Unit tests for DATAIO utility functions.
 *
 * Copyright (C) 2016-2026 designed, written and mantained by Giuseppe Marco Randazzo <gmrandazzo@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include "DATAIO.h"
#include <iostream>
#include <vector>
#include <string>
#include <cassert>

void test_STrim() {
    std::cout << "Testing STrim..." << std::endl;
    // Note: The current implementation of STrim in DATAIO.cpp only trims trailing whitespace
    assert(DATAIO::STrim("  hello  ") == "  hello"); 
    assert(DATAIO::STrim("hello\n\r\t") == "hello");
    assert(DATAIO::STrim("   ") == "");
    assert(DATAIO::STrim("") == "");
    assert(DATAIO::STrim("no_spaces") == "no_spaces");
    std::cout << "STrim tests passed!" << std::endl;
}

void test_split() {
    std::cout << "Testing split..." << std::endl;
    
    // Test normal split
    std::string s1 = "apple,banana,cherry";
    std::vector<std::string> r1 = DATAIO::split(s1, ",", false);
    assert(r1.size() == 3);
    assert(r1[0] == "apple");
    assert(r1[1] == "banana");
    assert(r1[2] == "cherry");

    // Test multiple delimiters (space and tab)
    std::string s2 = "1.0\t2.0 3.0";
    std::vector<std::string> r2 = DATAIO::split(s2, " \t", false);
    assert(r2.size() == 3);
    assert(r2[0] == "1.0");
    assert(r2[1] == "2.0");
    assert(r2[2] == "3.0");

    // Test keep_empty = true
    std::string s3 = "a,,b";
    std::vector<std::string> r3 = DATAIO::split(s3, ",", true);
    assert(r3.size() == 3);
    assert(r3[1] == "");

    // Test keep_empty = false
    std::vector<std::string> r4 = DATAIO::split(s3, ",", false);
    assert(r4.size() == 2);
    assert(r4[0] == "a");
    assert(r4[1] == "b");

    // Test empty string
    std::vector<std::string> r5 = DATAIO::split("", ",", false);
    assert(r5.empty());

    std::cout << "split tests passed!" << std::endl;
}

int main() {
    try {
        test_STrim();
        test_split();
        std::cout << "\nAll DATAIO utility tests passed successfully!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
