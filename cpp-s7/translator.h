#pragma once
#include <map>
#include <deque>
#include <string>
#include <string_view>
#include <cassert>

using namespace std;


class Translator {
public:
    Translator() = default;
    void Add(string_view source, string_view target) {
        auto word = FindWord(source);
        auto tr_word = FindWord(target);
        ru_eng[word] = tr_word;
        eng_ru[tr_word] = word;
    }

    string_view TranslateForward(string_view source) const {
        return ru_eng.count(source) ? ru_eng.at(source) : string_view();
    }

    string_view TranslateBackward(string_view target) const {
        return eng_ru.count(target) ? eng_ru.at(target) : string_view();
    }

private:
    map<string_view, string_view> ru_eng;
    map<string_view, string_view> eng_ru;
    deque<string> data_;

    string_view FindWord(string_view value) {
        if (ru_eng.count(value)) {
            return ru_eng.find(value)->first;
        }
        else if (ru_eng.count(value)) {
            return eng_ru.find(value)->first;
        }
        return data_.emplace_back(value);
    }
};

void TestSimple() {
    Translator translator;
    translator.Add(string("okno"s), string("window"s));
    translator.Add(string("stol"s), string("table"s));

    assert(translator.TranslateForward("okno"s) == "window"s);
    assert(translator.TranslateBackward("table"s) == "stol"s);
    assert(translator.TranslateForward("table"s) == ""s);
}