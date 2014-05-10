#define PLAYER_MOVE 'x'
#define ARDUINO_MOVE 'o'
#define IN_PIN 13

#define LED_RED 12
#define LED_GREEN 7
#define LED_BLUE 8

#define IN_PROGRESS 0
#define WAIT_FOR_RESTART 1 

#define WINNER_NONE 0
#define WINNER_PLAYER 1
#define WINNER_ARDUINO 2
#define WINNER_TIE 3

byte game_state;
byte winner;

char board[3][5] = {
  {'-', '-', '-', '\n', '\0'},
  {'-', '-', '-', '\n', '\0'},
  {'-', '-', '-', '\n', '\0'}
};

void draw_board() {
  Serial.println("");
  for (byte line = 0; line < 3; line ++) {
    Serial.print(board[line]);
  }
  Serial.println("");
}

void update_board(byte input, boolean is_user) {
  const char sign = is_user ? PLAYER_MOVE : ARDUINO_MOVE;
  byte line = (input - 1) / 3;
  byte col = (input - 1) % 3;
      
  board[line][col] = sign;
  
  draw_board();
  
  if (is_user) {
    Serial.println("Hmm... Let me think.");
  } else {
    Serial.println("It's your turn now!");
  }
}

boolean is_taken(byte input) {
  byte line = (input - 1) / 3;
  byte col = (input - 1) % 3;
  
  return ((board[line][col] == PLAYER_MOVE) || (board[line][col] == ARDUINO_MOVE));
}

byte validate_input(int input) {
  if ((input >= 49) && (input <= 57)) {
    input -= 48;
    if (is_taken(input)) {
      Serial.println("You can't move there. It's already taken!");
      return 0;
    }
    
    return input;
  }  
  
  Serial.println("Choose an integer between 1 and 9 inclusive.");
  return 0;
}

byte read_input() {
  if (Serial.available() > 0) {
    byte incoming = validate_input(Serial.read());
    if (incoming) {
      Serial.print("Your move: "); Serial.println(incoming, DEC);
      return incoming;
    } else {
      Serial.println("Your move is invalid! Try again.");
    }
  }
  
  return 0;
}

void reset_board() {
  for (byte i = 0; i < 3; i++) {
    for (byte j = 0; j < 3; j++) {
      board[i][j] = 49 + j + i * 3;
    }
  }
  
  draw_board();
  Serial.println("You start...");    
}

boolean check_for_victory() {
  // check the lines
  short sum_diag_1 = 0, sum_diag_2 = 0;
  byte _available = 0;
  
  for (byte i = 0; i < 3; i++) {
    short sum_line = 0, sum_col = 0;

    for (byte j = 0; j < 3; j++) {
      sum_line += board[i][j];
      sum_col += board[j][i];
      
      if (i == j) sum_diag_1 += board[i][j];
      if (2 - i == j) sum_diag_2 += board[i][j];
      
      if ((board[i][j] != PLAYER_MOVE) && (board[i][j] != ARDUINO_MOVE))
        _available ++;
    }
    
    if ((sum_line == 360) || (sum_col == 360)) {
      Serial.println("You win! Press the switch to play again.");
      winner = WINNER_PLAYER;
      return true;
    }
      
    if ((sum_line == 333) || (sum_col == 333)) {
      winner = WINNER_ARDUINO;      
      Serial.println("I win! Press the switch to play again.");
      return true;
    }    
  }
  
  if ((sum_diag_1 == 360) || (sum_diag_2 == 360)) {
    winner = WINNER_PLAYER;
    Serial.println("You win! Press the switch to play again.");
    return true;
  }
      
  if ((sum_diag_1 == 333) || (sum_diag_2 == 333)) {
    winner = WINNER_ARDUINO;      
    Serial.println("I win! Press the switch to play again.");
    return true;
  }
  
  if (!_available) {
    winner = WINNER_TIE;      
    Serial.println("It's a tie! Press the switch to play again.");
    return true;
  }

  return false;  
}

byte arduino_move() {
  byte moves_left[8];
  byte idx = 0;
  
  for (byte i = 0; i < 3; i++) {
    for (byte j = 0; j < 3; j++) {
      if ((board[i][j] != PLAYER_MOVE) && (board[i][j] != ARDUINO_MOVE)) {
        moves_left[idx++] = (board[i][j] - 48);
      }
    }
  }
    
  byte _move = moves_left[random(idx)];
  Serial.print("My move: "); Serial.println(_move);
  
  return _move;
}

void updateLedColor() {
  switch (winner) {
    case WINNER_NONE:
      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_BLUE, LOW);    
    break;
    case WINNER_PLAYER:
      digitalWrite(LED_GREEN, HIGH);
    break;
    case WINNER_ARDUINO:
      digitalWrite(LED_RED, HIGH);
    break;
    case WINNER_TIE:
      digitalWrite(LED_BLUE, HIGH);    
    break;
  }
}

void setup() {                	
  Serial.begin(9600);	
  randomSeed(analogRead(0));
  pinMode(IN_PIN, INPUT);
  pinMode(LED_RED, OUTPUT);    
  pinMode(LED_BLUE, OUTPUT);    
  pinMode(LED_GREEN, OUTPUT);    
  
  Serial.println("Let's play a nice game of Tic-Tac-Toe!");

  reset_board();
  game_state = IN_PROGRESS;
  winner = WINNER_NONE;
}

void loop() {
  updateLedColor();
  
  if (game_state == WAIT_FOR_RESTART) {
    int value = digitalRead(IN_PIN);
    delay(100);
      
    if (value == HIGH) {
      while(Serial.available())
        Serial.read();
        
      Serial.println("Great, we play again!");
      reset_board();
      game_state = IN_PROGRESS;
      winner = WINNER_NONE;
    }
    
    return;
  }
    
  if (game_state == IN_PROGRESS) {
    byte player_move = read_input();

    if (player_move) {
      update_board(player_move, true);
      if (check_for_victory()) {
        game_state = WAIT_FOR_RESTART;
        return;
      }
  
      update_board(arduino_move(), false);  
      if (check_for_victory()) {
        game_state = WAIT_FOR_RESTART;
        return;
      }
    }
    
    return;
  }
}
