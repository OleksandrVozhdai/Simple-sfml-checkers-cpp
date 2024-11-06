#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

using namespace std;

#define Size 8
int rows = 8;
int columns = 8;
bool RedTurn = 1;
int cellSize = 40;
int selectedRow = -1, selectedCol = -1;

void promoteToKing(int map[Size][Size], int row, int col) {
    if (row == 0 && map[row][col] == 3) {
        map[row][col] = 6; 
    }
    else if (row == Size - 1 && map[row][col] == 4) {
        map[row][col] = 7;  
    }
}

void checkForMoveRed(int map[Size][Size], int x, int y) {
    int direction = -1;

    for (int i = x + direction; i >= x + direction && i <= x + direction; ++i) {
        for (int j = y - 1; j <= y + 1; j += 2) {
            if (i >= 0 && i < Size && j >= 0 && j < Size) {
                if (map[i][j] == 2) {
                    map[i][j] = 5;
                }
                if (map[i][j] == 4 || map[i][j] == 7) { 
                    int targetX = i + direction;
                    int targetY = j + (j > y ? 1 : -1);
                    if (targetX >= 0 && targetX < Size && targetY >= 0 && targetY < Size && map[targetX][targetY] == 2) {
                        map[targetX][targetY] = 5;
                    }
                }
            }
        }
    }
}

void checkForMoveGreen(int map[Size][Size], int x, int y) {
    int direction = 1;

    for (int i = x + direction; i <= x + 1; ++i) {
        for (int j = y - 1; j <= y + 1; j += 2) {
            if (i >= 0 && i < Size && j >= 0 && j < Size) {
                if (map[i][j] == 2) {
                    map[i][j] = 5; 
                }
                if (map[i][j] == 3 || map[i][j] == 6) {
                    int targetX = i + direction;
                    int targetY = j + (j > y ? 1 : -1);
                    if (targetX >= 0 && targetX < Size && targetY >= 0 && targetY < Size && map[targetX][targetY] == 2) {
                        map[targetX][targetY] = 5; 
                    }
                }
            }
        }
    }
}

void moveForRedKing(int map[Size][Size], int x, int y) {
    int directions[4][2] = { {-1, -1}, {-1, 1}, {1, -1}, {1, 1} };

    for (auto& dir : directions) {
        int i = x + dir[0];
        int j = y + dir[1];
        if (i >= 0 && i < Size && j >= 0 && j < Size) {
            if (map[i][j] == 2) {
                map[i][j] = 5;
            }
            else if (map[i][j] == 4 || map[i][j] == 7) {
                int targetX = i + dir[0];
                int targetY = j + dir[1];
                if (targetX >= 0 && targetX < Size && targetY >= 0 && targetY < Size && map[targetX][targetY] == 2) {
                    map[targetX][targetY] = 5;
                }
            }
        }
    }
}

void moveForGreenKing(int map[Size][Size], int x, int y) {

    int directions[4][2] = { {-1, -1}, {-1, 1}, {1, -1}, {1, 1} };

    for (auto& dir : directions) {
        int i = x + dir[0];
        int j = y + dir[1];
        if (i >= 0 && i < Size && j >= 0 && j < Size) {
            if (map[i][j] == 2) {
                map[i][j] = 5;
            }
            else if (map[i][j] == 3 || map[i][j] == 6) {
                int targetX = i + dir[0];
                int targetY = j + dir[1];
                if (targetX >= 0 && targetX < Size && targetY >= 0 && targetY < Size && map[targetX][targetY] == 2) {
                    map[targetX][targetY] = 5;
                }
            }
        }
    }
}

void deleteHintMove(int map[Size][Size]) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            if (map[i][j] == 5) {
                map[i][j] = 2;
            }
        }
    }
}

bool hasCaptured = false;

void checkForAdditionalCaptures(int map[Size][Size], int row, int col) {
    deleteHintMove(map);
    int direction = (map[row][col] == 3) ? -1 : 1;

    for (int dr = -1; dr <= 1; dr += 2) {
        for (int dc = -1; dc <= 1; dc += 2) {
            int middleRow = row + dr;
            int middleCol = col + dc;
            int targetRow = row + 2 * dr;
            int targetCol = col + 2 * dc;

            if ((map[row][col] == 1 || map[row][col] == 3) && dr != direction) {
                continue;
            }

            if (middleRow >= 0 && middleRow < Size && middleCol >= 0 && middleCol < Size &&
                targetRow >= 0 && targetRow < Size && targetCol >= 0 && targetCol < Size &&
                map[middleRow][middleCol] != 2 && map[middleRow][middleCol] != map[row][col] &&
                map[targetRow][targetCol] == 2) {

                if (map[middleRow][middleCol] == (map[row][col] == 3 ? 4 : 3)) {
                    map[targetRow][targetCol] = 5;
                }
            }
        }
    }
}

void moveChecker(int map[Size][Size], int destRow, int destCol) {
    if (selectedRow != -1 && selectedCol != -1 && map[destRow][destCol] == 5) {
        int dRow = destRow - selectedRow;
        int dCol = destCol - selectedCol;

        int direction = (map[selectedRow][selectedCol] == 3) ? -1 : 1;
        if ((map[selectedRow][selectedCol] == 1 || map[selectedRow][selectedCol] == 3) && dRow * direction < 0) {
            return;
        }

        if (abs(dRow) == 2 && abs(dCol) == 2) {
            int capturedRow = selectedRow + dRow / 2;
            int capturedCol = selectedCol + dCol / 2;

            if (map[capturedRow][capturedCol] != 2 && map[capturedRow][capturedCol] != map[selectedRow][selectedCol]) {
                map[capturedRow][capturedCol] = 2;
                hasCaptured = true;
            }
            else {
                return;
            }
        }
        else {
            hasCaptured = false;
        }

        map[destRow][destCol] = map[selectedRow][selectedCol];
        map[selectedRow][selectedCol] = 2;

        promoteToKing(map, destRow, destCol);

        selectedRow = destRow;
        selectedCol = destCol;

        if (hasCaptured) {
            checkForAdditionalCaptures(map, selectedRow, selectedCol);
        }

        bool hasAdditionalMoves = false;
        for (int i = 0; i < Size; ++i) {
            for (int j = 0; j < Size; ++j) {
                if (map[i][j] == 5) {
                    hasAdditionalMoves = true;
                    break;
                }
            }
            if (hasAdditionalMoves) break;
        }

        if (!hasAdditionalMoves || !hasCaptured) {
            RedTurn = !RedTurn;
            selectedRow = selectedCol = -1;
            deleteHintMove(map);
        }
    }
}

int main() {
    int map[Size][Size] = { 1, 4, 1, 4, 1, 4, 1, 4,
                            4, 1, 4, 1, 4, 1, 4, 1,
                            1, 4, 1, 4, 1, 4, 1, 4,
                            2, 1, 2, 1, 2, 1, 2, 1,
                            1, 2, 1, 2, 1, 2, 1, 2,
                            3, 1, 3, 1, 3, 1, 3, 1,
                            1, 3, 1, 3, 1, 3, 1, 3,
                            3, 1, 3, 1, 3, 1, 3, 1 };

    sf::RenderWindow window(sf::VideoMode(columns * cellSize, rows * cellSize), "SFML Checkers");
    window.setFramerateLimit(60);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                int col = mousePos.x / cellSize;
                int row = mousePos.y / cellSize;

                if (row >= 0 && row < rows && col >= 0 && col < columns) {
                   
                    if (map[row][col] == 6 && RedTurn) {
                        deleteHintMove(map);
                        moveForRedKing(map, row, col);  
                        cout << "Red King move called at: " << row << ", " << col << endl;

                        selectedRow = row;
                        selectedCol = col;
                    }
                    else if (map[row][col] == 7 && !RedTurn) {
                        deleteHintMove(map);
                        moveForGreenKing(map, row, col); 
                        selectedRow = row;
                        selectedCol = col;
                    }
                    else if (map[row][col] == (RedTurn ? 3 : 4)) {
                        deleteHintMove(map);
                        if (RedTurn)
                            checkForMoveRed(map, row, col);
                        else
                            checkForMoveGreen(map, row, col);
                        selectedRow = row;
                        selectedCol = col;
                    }
                    else if (map[row][col] == 5) {
                        moveChecker(map, row, col);
                        promoteToKing(map, row, col); 
                    }
                }
            }
        }

        window.clear(sf::Color::Green);

        for (int row = 0; row < rows; ++row) {
            for (int col = 0; col < columns; ++col) {
                sf::RectangleShape cell(sf::Vector2f(cellSize, cellSize));
                cell.setPosition(col * cellSize, row * cellSize);

                if (map[row][col] == 1) {
                    cell.setFillColor(sf::Color::White);
                }
                else if (map[row][col] == 2) {
                    cell.setFillColor(sf::Color::Black);
                }
                else if (map[row][col] == 5) {
                    cell.setFillColor(sf::Color::Blue);
                }
                else {
                    cell.setFillColor(sf::Color::Black);
                }

                window.draw(cell);

                if (map[row][col] == 3 || map[row][col] == 4 || map[row][col] == 6 || map[row][col] == 7) {
                    sf::CircleShape checker(cellSize / 3);
                    if (map[row][col] == 6 || map[row][col] == 7) {
                        checker.setOutlineThickness(3);
                        checker.setOutlineColor(sf::Color::Yellow);  
                    }
                    checker.setFillColor((map[row][col] == 3 || map[row][col] == 6) ? sf::Color::Red : sf::Color::Green);
                    checker.setPosition(col * cellSize + (cellSize / 6), row * cellSize + (cellSize / 6));
                    window.draw(checker);
                }
            }
        }

        window.display();
    }

    return 0;
}

