#pragma once
#include <string>
#include <list>
#include <iostream>

using namespace std;


class Editor {
public:
    Editor()
        : pos_(str_.end()) {
    }

    // сдвинуть курсор влево
    void Left() {
        if (pos_ != str_.begin()) {
            --pos_;
        }
    }

    // сдвинуть курсор вправо 
    void Right() {
        if (pos_ != str_.end()) {
            ++pos_;
        }
    }

    // вставить символ token
    void Insert(char token) {
        str_.insert(pos_, token);
    }

    // вырезать не более tokens символов, начиная с текущей позиции курсора
    void Cut(size_t tokens = 1) {
        textbuffer_.resize(0);
        for (auto& it = pos_; tokens != 0;) {
            if (it == str_.end()) {
                break;
            }
            textbuffer_.push_back(*it);
            pos_ = str_.erase(pos_);
            --tokens;
        }
    }

    // cкопировать не более tokens символов, начиная с текущей позиции курсора
    void Copy(size_t tokens = 1) {
        textbuffer_.resize(0);
        for (auto it = pos_; tokens != 0; ++it) {
            if (it == str_.end()) {
                break;
            }
            textbuffer_.push_back(*it);
            --tokens;
        }
    }

    // вставить содержимое буфера в текущую позицию курсора
    void Paste() {
        str_.insert(pos_, textbuffer_.begin(), textbuffer_.end());
    }

    // получить текущее содержимое текстового редактора
    string GetText() const {
        return { str_.begin(), str_.end() };
    }

private:
    list<char> str_;
    list<char> textbuffer_;
    list<char>::iterator pos_;
};

// Тест проверяет методы класса Editor
void TestEditor() {
    Editor editor;
    const string text = "hello, world"s;
    for (char c : text) {
        editor.Insert(c);
    }
    // Текущее состояние редактора: `hello, world|`
    for (size_t i = 0; i < text.size(); ++i) {
        editor.Left();
    }
    // Текущее состояние редактора: `|hello, world`
    editor.Cut(7);
    // Текущее состояние редактора: `|world`
    // в буфере обмена находится текст `hello, `
    for (size_t i = 0; i < 5; ++i) {
        editor.Right();
    }
    // Текущее состояние редактора: `world|`
    editor.Insert(',');
    editor.Insert(' ');
    // Текущее состояние редактора: `world, |`
    editor.Paste();
    // Текущее состояние редактора: `world, hello, |`
    editor.Left();
    editor.Left();
    //Текущее состояние редактора: `world, hello|, `
    editor.Cut(3);  // Будут вырезаны 2 символа
    // Текущее состояние редактора: `world, hello|`
    for (size_t i = 0; i < 7; ++i) {
        editor.Left();
    }
    editor.Copy(7);
    for (size_t i = 0; i < 7; ++i) {
        editor.Right();
    }
    editor.Paste();
    for (size_t i = 0; i < editor.GetText().size(); ++i) {
        editor.Left();
    }
    editor.Copy(5);
    for (size_t i = 0; i < editor.GetText().size(); ++i) {
        editor.Right();
    }
    editor.Insert(',');
    editor.Insert(' ');
    editor.Paste();
    editor.Insert('\n');
    std::cout << editor.GetText();
}
