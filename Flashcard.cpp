#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;


class FlashCard {
protected:
    string quest;
    string ans;
    int score;

public:
    FlashCard(string q, string a, int s = 0) : quest(q), ans(a), score(s) {}

    void showCard()
    {
        cout << "Q: " << quest << endl;
    }

    bool checkAnswer(const string& userAnswer)
    {
        return userAnswer == ans;
    }

    void adjustScore(bool correct)
    {
        score += (correct ? 1 : -1);
        if (score < 0) score = 0;
    }

    string getQuestion() const { return quest; }
    string getAnswer() const { return ans; }
    int getScore() const { return score; }

    void saveToFile(ofstream& out) const
    {
        out << quest << "|" << ans << "|" << score << endl;
    }

    static FlashCard loadFromLine(const string& line)
    {
        size_t p1 = line.find('|');
        size_t p2 = line.find('|', p1 + 1);
        string q = line.substr(0, p1);
        string a = line.substr(p1 + 1, p2 - p1 - 1);
        int s = stoi(line.substr(p2 + 1));
        return FlashCard(q, a, s);
    }
};
