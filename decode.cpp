#include "biginteger.hpp"
#include <iostream>
#include <fstream>
#include <string>

BigInteger findPow(BigInteger base, BigInteger exp, const BigInteger& mod) {

    BigInteger result(1);
    base = base % mod;

    while (exp != 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        exp = exp / 2;
        base = (base * base) % mod;
    }

    return result;
}

BigInteger parcer(const std::string& line) {

    size_t double_dotPos = std::string::npos;
    for (size_t i = 0; i < line.size(); ++i) {
        if (line[i] == ':') {
            double_dotPos = i;
            break;
        }
    }
    std::string hexString;
    for (size_t i = double_dotPos + 1; i < line.size(); ++i) {
        hexString += line[i];
    }

    size_t startPos = 0;
    while (startPos < hexString.size() && (std::isspace(hexString[startPos])!=0)) {
        startPos++;
    }
    hexString = hexString.substr(startPos);
    return BigInteger(hexString, biginteger_base::hex);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Wrang arguments" << std::endl;
        return 1;
    }

    std::ifstream FileForKey(argv[1]);
    if (!FileForKey.is_open()) {
        std::cerr << "Invalid key" << std::endl;
        return 1;
    }

    std::ifstream FileForMessage(argv[2]);
    if (!FileForMessage.is_open()) {
        std::cerr << "Unable to read message" << std::endl;
        return 1;
    }

    std::string line;
    BigInteger n;
    BigInteger d;

    while (std::getline(FileForKey, line)) {
        if (line.find("modulus:") != std::string::npos) {
            n = parcer(line);
        }
        else if (line.find("privateExponent:") != std::string::npos) {
            d = parcer(line);
        }
    }

    std::string secretCode;
    std::getline(FileForMessage, secretCode);

    BigInteger c(secretCode, biginteger_base::hex);

    BigInteger m = findPow(c, d, n);

    std::cout << to_hex(m);

    return 0;
}
