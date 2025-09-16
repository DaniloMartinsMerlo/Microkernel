#include "termo_game.h"
#include "string_utils.h"


#define COLOR_DEFAULT 0x07 // Cinza claro sobre preto
#define COLOR_CORRECT   0x27 // Verde sobre preto
#define COLOR_PRESENT   0x67 // Amarelo sobre preto
#define COLOR_ABSENT    0x87 // Cinza escuro sobre preto

//Palavras secretas
const char* secret_words[] = {"thunder", "ratz"};
const int num_secret_words = sizeof(secret_words) / sizeof(secret_words[0]);
char current_secret_word[WORD_LENGTH];

// Estado do jogo
Guess player_guesses[MAX_ATTEMPTS];
char current_input_buffer[WORD_LENGTH];
int current_input_pos = 0;
int current_attempt = 0;

GameState game_state = GAME_ONGOING;

// Funções auxiliares para o kernel
extern void clear_screen(void);
extern void kprint(const char *str);
extern void kprint_char(char c, int row, int col, char color);
extern void set_cursor(int row, int col);

void termo_init_game() {
    clear_screen();
    strcpy(current_secret_word, secret_words[0]);

    // Inicializa as tentativas do jogador
    for (int i = 0; i < MAX_ATTEMPTS; i++) {
        for (int j = 0; j < WORD_LENGTH; j++) {
            player_guesses[i].word[j] = '\0';
            player_guesses[i].status[j] = LETTER_ABSENT; // Ou um estado inicial 'vazio'
        }
    }

    // Limpa o buffer de entrada
    for (int i = 0; i < WORD_LENGTH; i++) {
        current_input_buffer[i] = '\0';
    }
    current_input_pos = 0;
    current_attempt = 0;
    game_state = GAME_ONGOING;
}


void termo_process_input(char key_char) {
    if (game_state != GAME_ONGOING) {
        if (key_char == '\n') {
            termo_reset_game();
        }
        return;
    }

    if (key_char >= 'a' && key_char <= 'z') {
        // Adiciona a letra ao buffer se houver espaço
        if (current_input_pos < strlen(current_secret_word)) {
            current_input_buffer[current_input_pos++] = key_char;
            current_input_buffer[current_input_pos] = '\0'; // Garante que seja uma string válida
        }
        
    } else if (key_char == '\b') { // Backspace
        // Remove a última letra do buffer
        if (current_input_pos > 0) {
            current_input_buffer[--current_input_pos] = '\0';
        }

    } else if (key_char == '\n') { // Enter
        // Se a palavra digitada tiver o tamanho correto, verifica a tentativa
        if (current_input_pos == strlen(current_secret_word)) {
            // Copia a palavra do buffer para a tentativa atual
            strncpy(player_guesses[current_attempt].word, current_input_buffer, strlen(current_secret_word));
            player_guesses[current_attempt].word[strlen(current_secret_word)] = '\0';
            termo_check_guess(); // Verifica a palavra

            // Limpa o buffer para a próxima tentativa
            for (int i = 0; i < WORD_LENGTH; i++) {
                current_input_buffer[i] = '\0';
            }
            current_input_pos = 0;

            // Avança para a próxima tentativa, se o jogo ainda estiver em andamento
            if (game_state == GAME_ONGOING) {
                current_attempt++;
                if (current_attempt >= MAX_ATTEMPTS) {
                    game_state = GAME_LOST; 
                }
            }
        }
    }
}

void termo_update_screen() {
    clear_screen();

    // Desenha as tentativas anteriores
    for (int i = 0; i < MAX_ATTEMPTS; i++) {
        for (int j = 0; j < strlen(current_secret_word); j++) {
            char display_char = player_guesses[i].word[j];
            char color = COLOR_DEFAULT;

            if (i < current_attempt) { // Se esta é uma tentativa já submetida
                switch (player_guesses[i].status[j]) {
                    case LETTER_CORRECT:
                        color = COLOR_CORRECT;
                        break;
                    case LETTER_PRESENT:
                        color = COLOR_PRESENT;
                        break;
                    case LETTER_ABSENT:
                        color = COLOR_ABSENT;
                        break;
                }
            } else if (i == current_attempt && j < current_input_pos) { // Palavra atual sendo digitada
                color = COLOR_DEFAULT;
            } else {
                display_char = '_'; // Espaços vazios para letras não digitadas
            }
            kprint_char(display_char, i, j * 2, color); // Multiplica por 2 para espaçamento
        }
    }

    // Exibe a palavra atual sendo digitada
    for (int j = 0; j < current_input_pos; j++) {
        kprint_char(current_input_buffer[j], current_attempt, j * 2, COLOR_DEFAULT);
    }

    // Exibe mensagens de estado do jogo
    if (game_state == GAME_WON) {
        kprint("Voce GANHOU! Pressione Enter para jogar novamente.");
    } else if (game_state == GAME_LOST) {
        kprint("Voce PERDEU! A palavra era: ");
        kprint(current_secret_word);
        kprint(". Pressione Enter para jogar novamente.");
    }

    // Posiciona o cursor para a próxima entrada
    set_cursor(current_attempt, current_input_pos * 2);
}

void termo_check_guess() {
    int correct_letters = 0;
    char temp_secret[WORD_LENGTH];
    strcpy(temp_secret, current_secret_word); // Cria uma cópia para não modificar a palavra secreta original

    // Primeiro, verifica letras corretas e na posição certa
    for (int i = 0; i < strlen(current_secret_word); i++) {
        if (player_guesses[current_attempt].word[i] == temp_secret[i]) {
            player_guesses[current_attempt].status[i] = LETTER_CORRECT;
            temp_secret[i] = '_';
            correct_letters++;
        } else {
            player_guesses[current_attempt].status[i] = LETTER_ABSENT;
        }
    }

    // Se todas as letras estiverem corretas, o jogo é ganho
    if (correct_letters == strlen(current_secret_word)) {
        game_state = GAME_WON;
        return;
    }

    // Em seguida, verifica letras corretas, mas na posição errada (LETTER_PRESENT)
    for (int i = 0; i < strlen(current_secret_word); i++) {
        if (player_guesses[current_attempt].status[i] == LETTER_CORRECT) {
            continue;
        }

        for (int j = 0; j < strlen(current_secret_word); j++) {
            if (player_guesses[current_attempt].word[i] == temp_secret[j]) {
                player_guesses[current_attempt].status[i] = LETTER_PRESENT;
                temp_secret[j] = '_'; 
                break;
            }
        }
    }
}

void termo_reset_game() {
    termo_init_game();
    termo_update_screen();
}