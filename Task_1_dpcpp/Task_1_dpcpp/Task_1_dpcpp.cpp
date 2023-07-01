#include <fstream>

class Printable
{
public:
    virtual ~Printable() = default;

    virtual std::string printAsHTML() const = 0;
    virtual std::string printAsText() const = 0;
    virtual std::string printAsJSON() const = 0;
};

class Data : public Printable { 
public:
    enum class Format
    {
        kText,
        kHTML,
        kJSON
    };

    Data(std::string data, Format format)
        : data_(std::move(data)), format_(format) {}

    std::string printAsHTML() const override
    {
        if (format_ != Format::kHTML) {
            throw std::runtime_error("Invalid format!");
        }
        return "<html>" + data_ + "<html/>";
    }
    std::string printAsText() const override
    {
        if (format_ != Format::kText) {
            throw std::runtime_error("Invalid format!");
        }
        return data_;
    }
    std::string printAsJSON() const override
    {
        if (format_ != Format::kJSON) {
            throw std::runtime_error("Invalid format!");
        }
        return "{ \"data\": \"" + data_ + "\"}";
    }

private:
    std::string data_;
    Format format_;
};

void saveTo(std::ofstream& file, const Printable& printable, Data::Format format)
{
    switch (format)
    {
    case Data::Format::kText:
        file << printable.printAsText();
        break;
    case Data::Format::kJSON:
        file << printable.printAsJSON();
        break;
    case Data::Format::kHTML:
        file << printable.printAsHTML();
        break;
    }
}

void saveToAsHTML(std::ofstream& file, const Printable& printable) {
    saveTo(file, printable, Data::Format::kHTML);
}

void saveToAsJSON(std::ofstream& file, const Printable& printable) {
    saveTo(file, printable, Data::Format::kJSON);
}

void saveToAsText(std::ofstream& file, const Printable& printable) {
    saveTo(file, printable, Data::Format::kText);
}

//1). Принцип единственной ответственности(Single Responsibility Principle - SRP) : 
//    Класс Data нарушает SRP, так как он отвечает не только за хранение данных, 
//    но и за их форматирование и преобразование в различные форматы (Text, HTML, JSON).
//    Это приводит к смешиванию различных ответственностей в одном классе.

//2). Принцип подстановки Барбары Лисков(Liskov Substitution Principle - LSP) : 
//    Методы printAsHTML(), printAsText() и printAsJSON() в классе Data могут 
//    выбросить исключение std::runtime_error, что является нарушением LSP.
//    В базовом классе Printable нет информации о возможных исключениях, 
//    которые могут быть выброшены в производных классах.

//3). Принцип открытости / закрытости(Open / Closed Principle - OCP) : 
//    Функция saveTo() нарушает OCP, так как она должна быть изменена каждый раз, 
//    когда добавляется новый формат для сохранения данных.
//    Это приводит к нарушению принципа, который говорит, 
//    что классы должны быть открыты для расширения, но закрыты для модификации.

//4). Принцип разделения интерфейса (Interface Segregation Principle - ISP) : 
//    Интерфейс Printable имеет несколько методов, 
//    которые могут быть неиспользуемыми или нерелевантными для некоторых классов, 
//    реализующих данный интерфейс.
//    Это нарушает ISP, так как клиенты должны зависеть только от тех методов, 
//    которые необходимы им в конкретном контексте.

//5). Принцип инверсии зависимостей(Dependency Inversion Principle - DIP) : 
//    Классы Data и Printable жестко зависят друг от друга, что нарушает DIP.
//    Вместо того, чтобы зависеть от конкретных классов, 
//    классы должны зависеть от абстракций. Например, 
//    класс Data может зависеть от абстракции Printer, 
//    а класс Printable может зависеть от абстракции Formatter.
//    Это позволит легко внедрять различные реализации Printer 
//    и Formatter в зависимости от контекста.

//6). Также стоит отметить, что в коде отсутствует проверка успешности открытия файла 
//    для записи в функциях, которые сохраняют данные в файлы (saveToAsHTML(), 
//    saveToAsJSON(), saveToAsText()).
//    Это может привести к ошибкам выполнения, 
//    если файл не может быть открыт для записи.