// MinimaxNoughtsAndCrosses.cpp : Defines the entry point for the application.
//

#include "MinimaxNoughtsAndCrosses.h"

//X - 88
//O - 79
const char ai = 79;
const char player = 88;

void drawGrid(sf::RenderWindow& window, const int& size)
{
    sf::Vertex line[] = {
        sf::Vertex(sf::Vector2f(0, 0)),
        sf::Vertex(sf::Vector2f(0, 0)) 
    };
    for (int i = 0; i < size; i += (size/3)) {
        line[0] = sf::Vertex(sf::Vector2f(i, 0), sf::Color::Black);
        line[1] = sf::Vertex(sf::Vector2f(i, size), sf::Color::Black);
        window.draw(line, 2, sf::Lines);
        line[0] = sf::Vertex(sf::Vector2f(0, i), sf::Color::Black);
        line[1] = sf::Vertex(sf::Vector2f(size, i), sf::Color::Black);
        window.draw(line, 2, sf::Lines);
    }
}

void drawBoard(sf::RenderWindow& window, char board[9], const int& size) {
    sf::Font font;
    if (!font.loadFromFile("../../../../MinimaxNoughtsAndCrosses/arialbd.ttf"))
    {
        std::cout << "ERROR LOADING FONT\n";
    }

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(size/3);

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            text.setFillColor(sf::Color(255, 254, 186));
            text.setString(board[3 * j + i]);
            if ((text.getString() == 'X') || (text.getString() == 'O')) { text.setFillColor(sf::Color::Black);; }
            text.setPosition(i * (size / 3) + (size / 20), j * (size / 3) - (size / 20));
            window.draw(text);
        }
    }
}

void printBoard(char board[9]) {
    std::cout << "----------------------------\n";
    std::cout << board[0] << '|' << board[1] << '|' << board[2] << '\n'
        << "-+-+-\n"
        << board[3] << '|' << board[4] << '|' << board[5] << '\n'
        << "-+-+-\n"
        << board[6] << '|' << board[7] << '|' << board[8] << '\n';

    /*for (int i = 0; i < 9; i++) {
        std::cout << int(board[i]) << ' ';
    }*/
    std::cout << "\n----------------------------\n";
}

void mouseClick(sf::RenderWindow& window, char *board, const int& size, bool& playerPlayed) {
    bool valid = false;
    int x = -1;
    int y = -1;
    while (!valid) {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            x = sf::Mouse::getPosition(window).x;
            y = sf::Mouse::getPosition(window).y;
            x = floor((3 * x) / size);
            y = floor((3 * y) / size);
            //std::cout << x << ' ' << y << '\n';
            valid = ((board[(y * 3) + x] == 'X') || (board[(y * 3) + x] == 'O')) ? false : true;
        }
    }
    board[(y * 3) + x] = player;
    playerPlayed = true;
}

std::vector<int> emptyIndexes(const char board[9]) {
    std::vector<int> empty;
    for (int i = 0; i < 9; i++) {
        if ((board[i] != 'X') && (board[i] != 'O')) { empty.push_back(i); }
    }
    return empty;
}

bool checkWin(const char board[9], const char mark) {
    //std::cout << mark << '\n';
    std::string win[8] = {"012", "048", "036", "147", "258", "246", "345", "678"};
    for (auto& s : win) {
        if ((board[s[0] - '0'] == mark) && (board[s[1] - '0'] == mark) && (board[s[2] - '0'] == mark)) { return true; }
    }
    return false;
}

std::pair<int, int> minimax(char board[9], char mark) {
    std::vector<int> free = emptyIndexes(board);

    //Check for any wins
    //If player wins return -1 as bad option
    // If ai wins return +1 as good option
    // If draw return 0
    // Heuristic values so could be -100 and +100 and wouldnt make a difference
    if (checkWin(board, player)) {
        return std::make_pair(-1, - 1);
    }
    else if (checkWin(board, ai)) {
        return std::make_pair(-1, 1);
    }
    else if (free.size() == 0) {
        return std::make_pair(-1, 0);
    }

    //Vector to store outcomes of each potential move
    //Stores pair (index, score)
    std::vector<std::pair<int, int>> outcomes;

    //Loop over each available move
    for (auto& m : free) {
        //std::cout << m << '\n';
        int currentIndex = m;
        int currentScore = 0;
        board[m] = mark;
        
        //Recursively call for all positions
        if (mark == ai) {
            std::pair<int, int> result = minimax(board, player);
            currentScore = result.second;
        }
        else {
            std::pair<int, int> result = minimax(board, ai);
            currentScore = result.second;
        }
        board[m] = currentIndex + 48;
        outcomes.push_back(std::make_pair(currentIndex, currentScore));
    }

    std::pair<int, int> best;
    int bestIndex = -1;
    if (mark == ai) {
        int bestScore = -INT_MAX;
        for (int b = 0; b < outcomes.size(); b++) {
            if (outcomes[b].second > bestScore) {
                bestScore = outcomes[b].second;
                bestIndex = b;
            }
        }
    }
    else {
        int bestScore = INT_MAX;
        for (int b = 0; b < outcomes.size(); b++) {
            if (outcomes[b].second < bestScore) {
                bestScore = outcomes[b].second;
                bestIndex = b;
            }
        }
    }
    return outcomes[bestIndex];
}

int main()
{
    const int WINDOWSIZE = 900;
    sf::RenderWindow window(sf::VideoMode(WINDOWSIZE, WINDOWSIZE), "MinimaxNoughtsAndCrosses");

    std::cout << "Current path is " << std::filesystem::current_path() << '\n';

    char board[9] = { '0', '1', '2', '3', '4', '5', '6', '7', '8' };
    bool playerPlayed = (player == 'O') ? false : true;
    bool over = false;

    printBoard(board);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                
            }
        }

        window.clear(sf::Color(252, 250, 134));
        drawGrid(window, WINDOWSIZE);
        drawBoard(window, board, WINDOWSIZE);
        //printBoard(board);

        window.display();

        if ((emptyIndexes(board).size() != 0) && (!over)) {
            if (!playerPlayed) {
                bool valid = false;
                int x = -1;
                int y = -1;
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    x = sf::Mouse::getPosition(window).x;
                    y = sf::Mouse::getPosition(window).y;
                    x = floor((3 * x) / WINDOWSIZE);
                    y = floor((3 * y) / WINDOWSIZE);
                    //std::cout << x << ' ' << y << '\n';
                    valid = ((board[(y * 3) + x] == 'X') || (board[(y * 3) + x] == 'O')) ? false : true;
                }
                if (valid) {
                    board[(y * 3) + x] = player;
                    playerPlayed = true;
                }

                if (checkWin(board, player)) {
                    std::cout << "PLAYER WINS\n";
                    over = true;
                }
            }
            else {
                std::pair<int, int> best = minimax(board, ai);
                std::cout << "Best Index: " << best.first << "  Best Score: " << best.second << '\n';
                board[best.first] = ai;
                playerPlayed = false;
                if (checkWin(board, ai)) {
                    std::cout << "AI WINS\n";
                    over = true;
                }
            }
        }
    }
}
