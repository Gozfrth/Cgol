#include <stdio.h>
#include <iostream>
#include <ctime>
#include <thread>
#include <SFML/Graphics.hpp>

using namespace std;

const int rows = 200;
const int cols = 200;
const int cellSize = 3;
const int delay = 2;
const bool buttoned = false;

sf::Font font;
sf::RectangleShape buttonShape(sf::Vector2f(150, 50));


// Utility function to copy one bool array with another.
void copyArray(bool source[rows][cols], bool destination[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            destination[i][j] = source[i][j];
        }
    }
}


// Initialize Board with random values.
void initializeBoard(bool board[rows][cols]){
	srand(static_cast<unsigned>(time(0)));
	for (int i = 0; i < rows; i++){
		for(int j = 0; j < cols; j++){
			board[i][j] = false;
		}
	}
	board[10][10] = true;
	board[10][11] = true;
	board[11][10] = true;
	board[11][11] = true;
	board[10][20] = true;
	board[11][20] = true;
	board[12][20] = true;
	board[9][21] = true;
	board[13][21] = true;
	board[8][22] = true;
	board[8][23] = true;
	board[14][22] = true;
	board[14][23] = true;
	board[11][24] = true;
	board[9][25] = true;
	board[13][25] = true;
	board[10][26] = true;
	board[11][26] = true;
	board[12][26] = true;
	board[11][27] = true;
	board[8][30] = true;
	board[9][30] = true;
	board[10][30] = true;
	board[8][31] = true;
	board[9][31] = true;
	board[10][31] = true;
	board[7][32] = true;
	board[11][32] = true;
	board[6][34] = true;
	board[7][34] = true;
	board[11][34] = true;
	board[12][34] = true;
	board[8][44] = true;
	board[9][44] = true;
	board[8][45] = true;
	board[9][45] = true;


}


//Draw different states of the board at each 'int delay' ms delay
void drawBoard(sf::RenderWindow& window, bool board[rows][cols]){
	sf::Text buttonText("Toggle Pause", font, 24);
	buttonText.setFillColor(sf::Color::White);
	
	buttonShape.setFillColor(sf::Color(128, 128, 128));
	buttonShape.setPosition(250, 0);

	window.clear();
	for(int i=0; i<rows; i++){
		for(int j=0; j<cols; j++){
			sf::RectangleShape cell(sf::Vector2f(cellSize, cellSize));
			cell.setPosition(j * cellSize, i * cellSize);
			cell.setFillColor(board[i][j] ? sf::Color::White : sf::Color::Black);
			window.draw(cell);
		}
	}
	if(buttoned){
		window.draw(buttonShape);

        	sf::FloatRect textBounds = buttonText.getLocalBounds();
        	buttonText.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
        	buttonText.setPosition(buttonShape.getPosition() + sf::Vector2f(buttonShape.getSize().x / 2.0f, buttonShape.getSize().y / 2.0f));
        	window.draw(buttonText);
	}
	window.display();

}


// Updates the state of the board based on the Rules of Conway's Game of Life.
void updateBoard(bool board[rows][cols]){
	bool nextBoard[rows][cols];

	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
            		int liveNeighbors = 0;
            		for (int x = -1; x <= 1; ++x) {
                		for (int y = -1; y <= 1; ++y) {
                    			if (x == 0 && y == 0) {
                        			continue;
        	            		}

                	    		// Check if the neighbor is within the bounds of the board
                    			int neighborX = i + x;
                    			int neighborY = j + y;
                    			if (neighborX >= 0 && neighborX < rows && neighborY >= 0 && neighborY < cols) {
                        		liveNeighbors += board[neighborX][neighborY] ? 1 : 0;
                    			}
                		}
			}

            		if (board[i][j]) { // Cell is alive. 
                		if (liveNeighbors < 2 || liveNeighbors > 3) {
                    			// Rule 1 and Rule 3
					nextBoard[i][j] = false;
                		} else {
					// Rule 2
                    			nextBoard[i][j] = true;
                		}
            		} else { // Cell is dead
                		if (liveNeighbors == 3) {
                    			// Rule 4
                    			nextBoard[i][j] = true;
                		} else {
                    			nextBoard[i][j] = false;
                		}
            		}
        	}
    	}
	copyArray(nextBoard, board);

}

int main() {
	font.loadFromFile("/home/abybinoy/cProjects/Questrial-Regular.ttf");
	int currentDelay = delay;

	bool isPaused = false;
	bool board[rows][cols];
	initializeBoard(board);
	sf::RenderWindow window(sf::VideoMode(cols * cellSize, rows * cellSize), "Game of Life");
	
	while (window.isOpen()){	
		sf::Event event;
		while (window.pollEvent(event)){
			if(event.type == sf::Event::Closed){
				window.close();
			}else if(event.type == sf::Event::MouseButtonPressed && buttoned){
				sf::Vector2 mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
				if(buttonShape.getGlobalBounds().contains(mousePos)){
					isPaused = !isPaused;
				}
			}else if(event.type == sf::Event::KeyPressed){
				if(event.key.code == sf::Keyboard::Space){
					isPaused = !isPaused;
				}else if(event.key.code == sf::Keyboard::Up){
					currentDelay = max(currentDelay - 10, 1);
				}else if(event.key.code == sf::Keyboard::Down){
					currentDelay += 10;
				}
			}
		}
		
		if(!isPaused)
		{
			updateBoard(board);
			drawBoard(window, board);
		}

		sf::sleep(sf::milliseconds(currentDelay));
	}

	return 0;
}
