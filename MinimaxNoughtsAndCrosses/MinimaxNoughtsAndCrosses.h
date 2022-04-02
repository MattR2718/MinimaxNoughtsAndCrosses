// MinimaxNoughtsAndCrosses.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <vector>
#include <thread>
#include "SFML/Graphics.hpp"

#include <filesystem>

void drawGrid(sf::RenderWindow& window, const int& size);
void printBoard(char board[9]);
void drawBoard(sf::RenderWindow& window, char board[9], const int& size);
void mouseClick(sf::RenderWindow& window, char *board, const int& size, bool& playerPlayed);
std::vector<int> emptyIndexes(const char board[9]);
bool checkWin(const char board[9], const char mark);
std::pair<int, int> minimax(char board[9], char mark);

// TODO: Reference additional headers your program requires here.
