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

class FileManager {
public:
    static void save(const string& filename, const vector<FlashCard>& cards) {
        ofstream out(filename);
        for (const auto& card : cards) {
            card.saveToFile(out);
        }
        cout << "Cards saved to file.\n";
    }

    static vector<FlashCard> load(const string& filename) {
        vector<FlashCard> cards;
        ifstream in(filename);
        string line;
        while (getline(in, line)) {
            cards.push_back(FlashCard::loadFromLine(line));
        }
        cout << "Loaded" << cards.size() << " cards loaded.\n";
        return cards;
    }
};

class User {
private:
    string name;
    int totalCorrect = 0;
    int totalAttempts = 0;

public:
    User(string uname) : name(uname) {}

    void updateScore(bool correct)
    {
        totalAttempts++;
        if (correct) totalCorrect++;
    }

    void showScore() const {
        cout << " User: " << name << "\n";
        cout << " Score: " << totalCorrect << " / " << totalAttempts << " correct\n";
    }
};

class FlashCardSet {
private:
    vector<FlashCard> cards;

public:
    void addCard(const FlashCard& card)
    {
        cards.push_back(card);
    }

    void reviewCards(User& user)
    {
        cout << "\n-- Review Session --\n";
        if (cards.empty()) {
            cout << "No flashcards to review.\n";
            return;
        }

        sort(cards.begin(), cards.end(), [](FlashCard a, FlashCard b) {
            return a.getScore() < b.getScore();
        });

        for (auto& card : cards)
        {
            card.showCard();
            cout << "Your Answer: ";
            string ans;
            getline(cin, ans);
            bool correct = card.checkAnswer(ans);
            if (correct) {
                cout << "Correct!\n";
            } else {
                cout << "Incorrect! Answer: " << card.getAnswer() << endl;
            }
            card.adjustScore(correct);
            user.updateScore(correct);
        }

        user.showScore();
    }

    vector<FlashCard>& getCards() { return cards; }
};
