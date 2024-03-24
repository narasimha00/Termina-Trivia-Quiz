#include "additional-header-files/json.hpp"
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>
#include <unistd.h>
#include <vector>

#ifdef __linux__
#include <unistd.h>
#elif __WIN32__
#include < conio.h>
#endif

using namespace std;
using json = nlohmann::json;

fstream file("questions.json", ios::in);

class QuestionInterface {
private:
  vector<string> Options;
  string Question;
  int correct_option;

public:
  void displayQuestion() { cout << Question; }
  int displayOptions() {
    cout << endl;
    int op_counter = 1;
    for (const auto &element : Options) {
      cout << op_counter << ". " << element << endl;
      op_counter++;
    }
    return correct_option;
  }
  void setQuestion(string question) { Question = question; }
  void setOptions(vector<string> _options, int choice) {
    Options = _options;
    correct_option = choice;
  }

  int readOption() {
    int x;
    while (1) {
      cout << endl << "Enter your choice number : ";
      cin >> x;
      if (x > 0) {
        return x;
      } else {
        cout << "Error! there is no such choice !" << endl;
        continue;
      }
    }
  }
};

class player {
private:
  int lives;
  string name;
  bool wonGame = false;

public:
  player() { lives = 3; }
  int getLives() { return lives; }
  void decreaseLife() { lives--; }
  void setWonGame(bool state) { wonGame = state; }
  bool getWonGame() { return wonGame; }
};

int displayQuestionPage(QuestionInterface q, player p, int ob, int tot) {
  cout << "Lifes : " << p.getLives() << "                                   " << "You got : " << ob << "/" << tot << endl;
  cout << "-- - -- " << endl;
  q.displayQuestion();
  cout << endl;
  int crt = q.displayOptions();
  return crt;
}

// TOOLS FUNCTIONS
// json parser initialization
json jsonData = json::parse(file);

// Define ANSI escape codes for text colors
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"
#define ORANGE "\033[38;5;214m"

// Welcome screen
void welcome() {
  cout << GREEN << "<<<< WELCOME TO >>>>" << RESET << endl;
  cout << "┌────────────────────────────┐" << endl;
  cout << "│                            │" << endl;
  cout << "│  mmmm           \"          │" << endl;
  cout << "│ m\"  \"m m   m  mmm    mmmmm │" << endl;
  cout << "│ #    # #   #    #       m\" │" << endl;
  cout << "│ #    # #   #    #     m\"   │" << endl;
  cout << "│  #mm#\" \"mm\"#  mm#mm  #mmmm │" << endl;
  cout << "│     #                      │" << endl;
  cout << "│                            │" << endl;
  cout << "└────────────────────────────┘" << endl;
  cout << "PRESS ANY KEY TO CONTINUE !!" << endl;
  getchar();
  cout << endl;
}

// clear screen
void clrscr() {
#ifdef __linux__
  system("clear");
#elif _WIN32_
  system("cls");
#endif
}

// win function
void win(player p) {
  clrscr();
  cout << GREEN << "You nailed it !!, you did all right !!" << RESET << endl;
  p.setWonGame(true);
}

// gameplay stuff
void startGame() {
  player p;
  int length = jsonData["questions"].size();
  // cout << length;
  QuestionInterface q[length];

  string question;
  vector<string> options;
  int crt_choice;

  for (int i = 0; i < length; i++) {
    question = jsonData["questions"][i]["qstring"];
    options = jsonData["questions"][i]["options"];
    crt_choice = jsonData["questions"][i]["correct_choice"];

    q[i].setQuestion(question);
    q[i].setOptions(options, crt_choice);
  }

  // game logic begin
  clrscr();
  welcome();
  clrscr();
  int crt;
  int choice;
  int points = 0;
  int qCount = 0;
  while (qCount < length) {
    for (int i = 0; i < length; i++) {
      crt = displayQuestionPage(q[i], p,points,length);
      cout << "Enter your choice : ";
      cin >> choice;
      clrscr();
      if (choice != crt) {
        if (p.getLives() != 1) {
          cout << YELLOW << "OOPS !! YOU LOST A LIFE, TRY AGAIN !! " << RESET
               << endl;
          i--;
          p.decreaseLife();
        } else {
          cout << BLUE
               << "Sorry, better luck next time, YOU RAN OUT OF LIVES !!"
               << RESET << endl;
          cout << RED << "YOU LOST :(" << RESET << endl;
          p.decreaseLife();
          break;
        }

      } else {
        cout << GREEN << "CORRECT CHOICE !! CARRY ON !! " << RESET << endl;
        points++;
      }
      if (points == length) {
        win(p);
      }
      qCount++;
    }
  }
  if (p.getWonGame()) {
    cout << GREEN << "You nailed it !!, you did all right !!"<<RESET<<endl;
  }
}

int main() {
  if (!file.is_open()) {
    cout << "Error opening the file 'questions.json' !!" << endl;
    exit(0);
  }
  startGame();
  return 0;
}
