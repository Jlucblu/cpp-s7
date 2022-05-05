#pragma once
#include <algorithm>
#include <cassert>
#include <string>
#include <vector>
#include <iterator>

using namespace std;


// ��������� Sentence<Token> ��� ������������� ���� Token
// ��������� vector<Token>.
// ��������� ����� � �������� ������������� ��������
// ������� ����� ������� �� ������������ ������ ��������,
// � ������ ����������� � vector<Sentence<Token>>.
template <typename Token>
using Sentence = vector<Token>;

template <typename TokenForwardIt>
TokenForwardIt FindSentenceEnd(TokenForwardIt tokens_begin, TokenForwardIt tokens_end) {
    const TokenForwardIt before_sentence_end
        = adjacent_find(tokens_begin, tokens_end, [](const auto& left_token, const auto& right_token) {
        return left_token.IsEndSentencePunctuation() && !right_token.IsEndSentencePunctuation();
            });
    return before_sentence_end == tokens_end ? tokens_end : next(before_sentence_end);
}

// ����� Token ����� ����� bool IsEndSentencePunctuation() const
template <typename Token>
vector<Sentence<Token>> SplitIntoSentences(vector<Token> tokens) {
    vector<Sentence<Token>> result;
    if (tokens.begin() == tokens.end()) {
        return result;
    }
    auto it = tokens.begin();
    //Sentence<Token> buffer;
    while (it != tokens.end()) {
        const auto it_end = FindSentenceEnd(it, tokens.end());
        /*if (!it->IsEndSentencePunctuation()) {
            buffer.push_back(move(*it));
        }
        else {
            buffer.push_back(move(*it));
            while (next(it) != tokens.end() && next(it)->IsEndSentencePunctuation()) {
                ++it;
                buffer.push_back(move(*it));
            }
            result.push_back(buffer);
            buffer.clear();
        }
        ++it;
    }
    if (!buffer.empty()) {
        result.push_back(make_move_iterator(buffer.begin()), (buffer.end()));
    }*/
        result.push_back(Sentence<Token>(make_move_iterator(it), make_move_iterator(it_end)));
        it = it_end;
    }

    return result;
}

struct TestToken {
    string data;
    bool is_end_sentence_punctuation = false;

    bool IsEndSentencePunctuation() const {
        return is_end_sentence_punctuation;
    }
    bool operator==(const TestToken& other) const {
        return data == other.data && is_end_sentence_punctuation == other.is_end_sentence_punctuation;
    }
};

ostream& operator<<(ostream& stream, const TestToken& token) {
    return stream << token.data;
}

// ���� �������� ����������� �������� ������ TestToken.
// ��� �������� ���������� ����������� � ������� SplitIntoSentences
// ���������� �������� ��������� ����.
void TestSplitting() {
    assert(SplitIntoSentences(vector<TestToken>({ {"Split"s}, {"into"s}, {"sentences"s}, {"!"s} }))
        == vector<Sentence<TestToken>>({ {{"Split"s}, {"into"s}, {"sentences"s}, {"!"s}} }));

    assert(SplitIntoSentences(vector<TestToken>({ {"Split"s}, {"into"s}, {"sentences"s}, {"!"s, true} }))
        == vector<Sentence<TestToken>>({ {{"Split"s}, {"into"s}, {"sentences"s}, {"!"s, true}} }));

    assert(SplitIntoSentences(vector<TestToken>(
        { {"Split"s}, {"into"s}, {"sentences"s}, {"!"s, true}, {"!"s, true}, {"Without"s}, {"copies"s}, {"."s, true} }))
        == vector<Sentence<TestToken>>({
            {{"Split"s}, {"into"s}, {"sentences"s}, {"!"s, true}, {"!"s, true}},
            {{"Without"s}, {"copies"s}, {"."s, true}},
            }));
}