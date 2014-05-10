Tic-Tac-Toe game on Arduino
===========================

Play a simple game of tic-tac-toe via Arduino's serial interface.

What you need:
* 1 Arduino Uno board
* 2 or 4 220ohm resiztors (dependning on whether you have an RGB-LED or not).
* 1 push-button
* 3 LEDs or 1 RGB-LED

## Instructions 

1. Schematic is available at the follwing location: http://123d.circuits.io/circuits/213703-tic-tac-toe-with-arduino/view?version=0
2. Upload the project on your Arduino Uno board
3. Open the _serial monitor_ window.
4. Follow the on-screen instructions. You can make you move by selecting a number from 1 to 9 corresponding to each cell on the board.
5. There's no AI. Arduino's moves are random.
6. If you win the GREEN led is lit. If you lose the RED led is lit. If it's a tie the BLUE led is lit.
7. You can replay the game by pressing the push-button.
