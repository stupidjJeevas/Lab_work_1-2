#include <iostream>
#include <vector>
#include <stdexcept>
#include <string>
#include <algorithm>

class BinaryNumber {
private:
    std::vector<int> bits;
    int size; // Размер числа в битах (8, 16, 32)

    void fromDecimal(int decimal);
    int toDecimal() const;
    void addBinary(const BinaryNumber& other);
    void subtractBinary(const BinaryNumber& other);
    void multiplyBinary(const BinaryNumber& other);

public:
    BinaryNumber(int decimal, int size);
    BinaryNumber(const std::vector<int>& bits);

    BinaryNumber operator+(const BinaryNumber& other) const;
    BinaryNumber operator-(const BinaryNumber& other) const;
    BinaryNumber operator*(const BinaryNumber& other) const;

    std::string toBinaryString() const;
    int toDecimalSigned() const;
};

void BinaryNumber::fromDecimal(int decimal) {
    bits.resize(size, 0);
    bool isNegative = decimal < 0;
    if (isNegative) decimal = -decimal;

    for (int i = 0; i < size; ++i) {
        bits[i] = decimal % 2;
        decimal /= 2;
    }

    if (isNegative) {
        // Преобразование в дополнительный код
        for (int i = 0; i < size; ++i) {
            bits[i] = 1 - bits[i];
        }
        // Добавить 1 к числу
        int carry = 1;
        for (int i = 0; i < size; ++i) {
            int sum = bits[i] + carry;
            bits[i] = sum % 2;
            carry = sum / 2;
        }
    }
}

int BinaryNumber::toDecimal() const {
    int decimal = 0;
    int base = 1;
    for (int i = 0; i < size; ++i) {
        decimal += bits[i] * base;
        base *= 2;
    }
    return decimal;
}

int BinaryNumber::toDecimalSigned() const {
    if (bits[size - 1] == 0) {
        return toDecimal();
    }
    else {
        std::vector<int> invertedBits = bits;
        for (int i = 0; i < size; ++i) {
            invertedBits[i] = 1 - invertedBits[i];
        }
        int carry = 1;
        for (int i = 0; i < size; ++i) {
            int sum = invertedBits[i] + carry;
            invertedBits[i] = sum % 2;
            carry = sum / 2;
        }
        BinaryNumber invertedNumber(invertedBits);
        return -invertedNumber.toDecimal();
    }
}

BinaryNumber::BinaryNumber(int decimal, int size) : size(size) {
    fromDecimal(decimal);
}

BinaryNumber::BinaryNumber(const std::vector<int>& bits) : bits(bits), size(bits.size()) {}

std::string BinaryNumber::toBinaryString() const {
    std::string result;
    for (int i = size - 1; i >= 0; --i) {
        result += std::to_string(bits[i]);
    }
    return result;
}

BinaryNumber BinaryNumber::operator+(const BinaryNumber& other) const {
    BinaryNumber result(0, size);
    int carry = 0;
    for (int i = 0; i < size; ++i) {
        int sum = bits[i] + other.bits[i] + carry;
        result.bits[i] = sum % 2;
        carry = sum / 2;
    }
    return result;
}

BinaryNumber BinaryNumber::operator-(const BinaryNumber& other) const {
    // Для вычитания нужно прибавить отрицательное значение
    std::vector<int> invertedBits = other.bits;
    for (int i = 0; i < size; ++i) {
        invertedBits[i] = 1 - invertedBits[i];
    }
    int carry = 1;
    for (int i = 0; i < size; ++i) {
        int sum = invertedBits[i] + carry;
        invertedBits[i] = sum % 2;
        carry = sum / 2;
    }
    BinaryNumber negativeOther(invertedBits);
    return *this + negativeOther;
}

BinaryNumber BinaryNumber::operator*(const BinaryNumber& other) const {
    BinaryNumber result(0, size);
    for (int i = 0; i < size; ++i) {
        if (other.bits[i] == 1) {
            BinaryNumber temp = *this;
            for (int j = 0; j < i; ++j) {
                temp.bits.insert(temp.bits.begin(), 0);
                temp.bits.pop_back();
            }
            result = result + temp;
        }
    }
    return result;
}


void validateInput(int& num1, int& num2, char& op) {
    std::cout << "Введите первое число: ";
    std::cin >> num1;
    std::cout << "Введите второе число: ";
    std::cin >> num2;
    std::cout << "Введите операцию (+, -, *): ";
    std::cin >> op;
    if (op != '+' && op != '-' && op != '*') {
        throw std::invalid_argument("Неверная операция");
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    try {
        int num1, num2;
        char op;
        validateInput(num1, num2, op);

        const int SIZE = 32;
        BinaryNumber binNum1(num1, SIZE);
        BinaryNumber binNum2(num2, SIZE);
        BinaryNumber result(0, SIZE);

        if (op == '+') {
            result = binNum1 + binNum2;
        }
        else if (op == '-') {
            result = binNum1 - binNum2;
        }
        else if (op == '*') {
            result = binNum1 * binNum2;
        }

        std::cout << "Первое число в двоичном формате: " << binNum1.toBinaryString() << std::endl;
        std::cout << "Второе число в двоичном формате: " << binNum2.toBinaryString() << std::endl;
        std::cout << "Результат в двоичном формате: " << result.toBinaryString() << std::endl;
        std::cout << "Результат в десятичном формате: " << result.toDecimalSigned() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }
    return 0;
}