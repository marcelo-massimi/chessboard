/**
 * @file Micro_Max.h
 * @brief Interfaz del motor de ajedrez Micro-Max
 * 
 * Contiene las definiciones y prototipos necesarios para la integración
 * del motor de ajedrez con el sistema automatizado.
 * 
 * Modificaciones principales:
 * - Array `b[]` declarado como global para acceso desde programa principal
 * - Integración con sistema de visualización
 * - Adaptación para modo humano vs humano y humano vs computadora
 */

// Funciones del motor de ajedrez
unsigned short   myrand(void);     // Generador de números aleatorios
void            gameOver(void);    // Manejo de fin de juego
void            bkp(void);        // Backup del estado
void            serialBoard(void); // Envío del estado por serial
void            AI_HvsH(void);    // Modo Humano vs Humano
void            AI_HvsC(void);    // Modo Humano vs Computadora

// Variables compartidas con el programa principal
extern char     mov[];            // Buffer de movimiento actual
extern byte     sequence;         // Secuencia de juego
extern boolean  no_valid_move;    // Indicador de movimiento inválido

// Estado global del tablero
extern char     b[];              // Representación del tablero y puntuación

// **************************** Make game_mode Global V8
extern byte game_mode;
