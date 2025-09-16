#ifndef TERMO_GAME_H
#define TERMO_GAME_H

#define WORD_LENGTH 6 // 'thunder' e 'ratz' têm 6 caracteres com o terminador null
#define MAX_ATTEMPTS 6

typedef enum {
    GAME_ONGOING,
    GAME_WON,
    GAME_LOST
} GameState;

typedef enum {
    LETTER_ABSENT,  // Letra não está na palavra
    LETTER_PRESENT, // Letra está na palavra, mas na posição errada
    LETTER_CORRECT  // Letra está na palavra e na posição correta
} LetterStatus;

typedef struct {
    char word[WORD_LENGTH];
    LetterStatus status[WORD_LENGTH - 1]; // Status para cada letra da palavra (excluindo null terminator)
} Guess;

// Variáveis globais do jogo
extern const char* secret_words[];
extern const int num_secret_words;
extern char current_secret_word[WORD_LENGTH];
extern Guess player_guesses[MAX_ATTEMPTS];
extern char current_input_buffer[WORD_LENGTH];
extern int current_input_pos;
extern int current_attempt;
extern GameState game_state;
extern void kprint_char(char c, int row, int col, char color);
// Funções do jogo
void termo_init_game();
void termo_process_input(char key_char);
void termo_update_screen();
void termo_check_guess();
void termo_display_keyboard_status();
void termo_reset_game();

#endif // TERMO_GAME_H