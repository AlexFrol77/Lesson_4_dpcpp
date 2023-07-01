#include <iostream>
#include <fstream>
#include <string>

class HTMLPrintable {
public:
    virtual ~HTMLPrintable() = default;
    virtual std::string printAsHTML() const = 0;
};

class JSONPrintable {
public:
    virtual ~JSONPrintable() = default;
    virtual std::string printAsJSON() const = 0;
};

class TextPrintable {
public:
    virtual ~TextPrintable() = default;
    virtual std::string printAsText() const = 0;
};

class Data : public HTMLPrintable, public JSONPrintable, public TextPrintable {
public:

    Data(std::string data) : data_(std::move(data)) {}

    std::string printAsHTML() const override {
        return "<html>" + data_ + "<html/>";
    }

    std::string printAsText() const override {
        return data_;
    }

    std::string printAsJSON() const override {
        return "{ \"data\": \"" + data_ + "\" }";
    }

private:
    std::string data_;
};

void saveTo(std::ofstream& file, const HTMLPrintable& printable) {
    if (file.good()) {
        file << printable.printAsHTML();
    }
    else {
        std::cerr << "Ошибка записи в файл!" << std::endl;
        return;
    }
}

void saveTo(std::ofstream& file, const JSONPrintable& printable) {
    if (file.good()) {
        file << printable.printAsJSON();
    }
    else {
        std::cerr << "Ошибка записи в файл!" << std::endl;
        return;
    }
}

void saveTo(std::ofstream& file, const TextPrintable& printable) {
    if (file.good()) {
        file << printable.printAsText();
    }
    else {
        std::cerr << "Ошибка записи в файл!" << std::endl;
        return;
    }
}

void saveToAsHTML(std::ofstream& file, const HTMLPrintable& printable) {
    saveTo(file, printable);
}

void saveToAsJSON(std::ofstream& file, const JSONPrintable& printable) {
    saveTo(file, printable);
}

void saveToAsText(std::ofstream& file, const TextPrintable& printable) {
    saveTo(file, printable);
}


int main() {
    
    setlocale(LC_ALL, "rus");

    std::string data = "Hello, Alex Frol!";
    Data myData(data);

    std::ofstream htmlFile("output.html", std::ios::app);
    if (!htmlFile.is_open()) {
        std::cerr << "Ошибка открытия файла!" << std::endl;
        return 1;
    }
    else {
        saveToAsHTML(htmlFile, myData);
    }
    htmlFile.close();

    std::ofstream jsonFile("output.json", std::ios::app);
    if (!jsonFile.is_open()) {
        std::cerr << "Ошибка открытия файла!" << std::endl;
        return 1;
    }
    else {
        saveToAsJSON(jsonFile, myData);
    }
    jsonFile.close();
    
    std::ofstream textFile("output.txt", std::ios::app);
    if (!textFile.is_open()) {
        std::cerr << "Ошибка открытия файла!" << std::endl;
        return 1;
    }
    else {
        saveToAsText(textFile, myData);
    }
    textFile.close();
    
    return 0;
}