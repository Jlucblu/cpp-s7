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

    // �������� ������ �����
    void Left() {
        if (pos_ != str_.begin()) {
            --pos_;
        }
    }

    // �������� ������ ������ 
    void Right() {
        if (pos_ != str_.end()) {
            ++pos_;
        }
    }

    // �������� ������ token
    void Insert(char token) {
        str_.insert(pos_, token);
    }

    // �������� �� ����� tokens ��������, ������� � ������� ������� �������
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

    // c���������� �� ����� tokens ��������, ������� � ������� ������� �������
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

    // �������� ���������� ������ � ������� ������� �������
    void Paste() {
        str_.insert(pos_, textbuffer_.begin(), textbuffer_.end());
    }

    // �������� ������� ���������� ���������� ���������
    string GetText() const {
        return { str_.begin(), str_.end() };
    }

private:
    list<char> str_;
    list<char> textbuffer_;
    list<char>::iterator pos_;
};

// ���� ��������� ������ ������ Editor
void TestEditor() {
    Editor editor;
    const string text = "hello, world"s;
    for (char c : text) {
        editor.Insert(c);
    }
    // ������� ��������� ���������: `hello, world|`
    for (size_t i = 0; i < text.size(); ++i) {
        editor.Left();
    }
    // ������� ��������� ���������: `|hello, world`
    editor.Cut(7);
    // ������� ��������� ���������: `|world`
    // � ������ ������ ��������� ����� `hello, `
    for (size_t i = 0; i < 5; ++i) {
        editor.Right();
    }
    // ������� ��������� ���������: `world|`
    editor.Insert(',');
    editor.Insert(' ');
    // ������� ��������� ���������: `world, |`
    editor.Paste();
    // ������� ��������� ���������: `world, hello, |`
    editor.Left();
    editor.Left();
    //������� ��������� ���������: `world, hello|, `
    editor.Cut(3);  // ����� �������� 2 �������
    // ������� ��������� ���������: `world, hello|`
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
