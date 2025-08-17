/**
 * @file global.h
 * @brief Configuración y variables globales del sistema
 * 
 * Centraliza la configuración y variables compartidas del sistema:
 * 
 * Definiciones principales:
 * - Matriz de sensores: Configuración [7][7] para tablero 8x8
 * - Parámetros de control: Motores y electroimán
 * - Calibración: Umbrales para sensores Hall
 * - Modos de operación: Hardware real vs Simulación
 * 
 * Variables de estado:
 * - Posiciones del tablero
 * - Estado de los motores
 * - Lecturas de sensores
 * 
 * @note Configuración del modo de operación:
 *       Buscar "Simulide" en el código para ajustar parámetros
 *       según se use en hardware real o simulación.
 */
#ifndef GLOBAL_H
#define GLOBAL_H

//-----------------------------------------------------------------------------
// Configuración de Hardware
//-----------------------------------------------------------------------------

// Configuración de motores y micropasos
// SimulIDE: 1, RB: 16, SK Big: 16, SK Small: 8
// TMC2208 MS2,MS1: 00:1/8, 01:1/2, 10:1/4, 11:1/16
const int   MICROSTEPS          = 8;    

// Configuración de imanes
// Tipo: N32 6x3mm, polarización diferente para B/N
int hall_sensor_status [8][8]; //V10 al definir [8][8] significa que es de 8x8, pero va de 0 a 7
int hall_sensor_record [8][8]; //V10 al definir [8][8] significa que es de 8x8, pero va de 0 a 7
int hall_sensor_status_memory [8][8]; //V10 al definir [8][8] significa que es de 8x8, pero va de 0 a 7
int hall_value[8][8]; //V10 al definir [8][8] significa que es de 8x8, pero va de 0 a 7       //  For Debug only

int hall_compare = 30; // Valor a sumar o restar para definir valor de comparacion Norte o Sur.
int hall_error = 30; // Valor de error aceptable entre 2 lecturas de un mismo sensor.
int difference_hall=500; // Valor donde guardo el error real entre 2 lecturas de un mismo sensor. Inicialmente seteado en un valor grande, para poder recalcularlo hasta llegar a un error aceptable.


int hall_value_no_magnet[8][8]; // Aqui se guarda el ULTIMO valor de cada sensor sin imanes cerca
int hall_value_no_magnet_memory[8][8]; // Aqui se guarda el ANTERIOR valor de cada sensor sin imanes cerca.
int hall_value_no_magnet_difference[8][8]; // Aqui verifico la diferencia entre ULTIMO y ANTERIOR para ver el error de medicion de cada sensor contra si mismo sin imanes
int hall_value_north_magnet[8][8]; // Guardo el valor, RESTANDO el hall_compare. (Recordar Sensor49E TO92 es orientado al Sur)
int hall_value_south_magnet[8][8]; // Guardo el valor, SUMANDO el hall_compare. (Recordar Sensor49E TO92 es orientado al Sur)


char mov [4] = {0, 0, 0, 0};
boolean no_valid_move = false;
byte hall_colone[2];
byte hall_line[2];

//  Game setting
enum {start_up, start, calibration, player_white, player_black};
byte sequence = start_up;


enum {A_H,H_A,F1_F8,F8_F1,AH_18, HA_81, AH_81, HA_18, calibrate_speed};	// V9.2.7.1 
const char* directionNames[] = {"A_H", "H_A", "F1_F8", "F8_F1", "AH_18", "HA_81", "AH_81", "HA_18", "calibrate_speed"}; // V9.2.3 for Serial Print the name

// T=Top  -  B=Bottom  -  L=Left  -  R=Right
byte game_mode = 0;
enum {HvsH, HvsC};

//  Electromagnet - H Bridge L298

int IN1 = (14);
int IN2 = (15);
int MAGNET (6);  // Enable or not the H-Bridge
//bool white_piece = 0; // For White Movement =true=1 H-Bdridge not Finished
// Define el rango del PWM del Electroiman
const int pwmMax = 230;  // Valor máximo para el PWM // SK Small 180  // SK Big 220  // SK Big xxx for 1 magnet D6x3mm + 2 Magnet D8x1mm
const int incrementDelay = 10;  // Retraso para el incremento de PWM
//const int holdTime = 500;  // Tiempo en milisegundos para mantener el PWM al máximo
   

//  Countdown
byte second = 60;
byte minute = 9;
byte second_white = second;
byte second_black = second;
byte minute_white = minute;
byte minute_black = minute;
unsigned long timer = 0;
boolean start_black = true;
boolean new_turn_countdown = false;

//  Motor
const byte MOTOR_WHITE_DIR (2);
const byte MOTOR_WHITE_STEP (3);
const byte MOTOR_BLACK_DIR (4);
const byte MOTOR_BLACK_STEP (5);
const float SQUARE_SIZE = 199*microsteps; //float for make the result of division float too // *** Defined on Calibrate_Trolley_XY *** // // 187 RB // 199 SK Big // 188 SK Small // The number are the Microsteps for make 1 square
const float DIAGONALFACTOR = 2.01; // *** Defined on Calibrate_Trolley_Diagonal *** // 1.99 RB // 2.01 SK Big // 1.98 SK Small // Math 1.414 // V3.3 If diagonal moves are not calibrated, modify this value !! Greg Original 1.44 // Ideal SQRT(2) but steps are not an integer number // for V9.2.7.1
const int SPEED_SLOW (3500/microsteps); // ***** previous 4000
const int SPEED_FAST (2000/microsteps); // ***** previous 1000

// Parameter for Fix posible error if the piece not arrives at the center
	// On Calibrate_Trolley_XY you define the Steps necessary to move the Trolley 1 square.
	// Here add or subtract steps of the piece, if the piece not get to the center, even the Trolley does.
const float FIXUP_POSITION = 5.0*microsteps/SQUARE_SIZE;  // Global V6 // change the number and put the one you need. If nothing to fix, put 0.0
byte Last_Direction;  // Global V7 added for have the Last direction move of the Motor
byte Undo_Last_Direction; // Global V7

// H7 Start position after Calibrate V5 ***
const float TROLLEY_START_POSITION_X = (352.0)*microsteps/SQUARE_SIZE; //X= F1_F8 //0.95 RB // 352 SK Big 2025  // 351 SK Big // SK Small 234.0*microsteps/SQUARE_SIZE // after square_size.ino test // v3.2 Distance in squares, from Microswitch X to E7. Measure your Board (If Square is 37mm, Distance X in mm/37mm) 
const float TROLLEY_START_POSITION_Y = (134.0)*microsteps/SQUARE_SIZE; //Y= A_H //4.84 RB //   134 SK Big 2025  // 126 SK Big // SK Small 321.0*microsteps/SQUARE_SIZE // fter square_size.ino test // v3.2 Distance in squares, from Microswitch Y to E7. Measure your Board (If Square is 37mm, Distance Y in mm/37mm)
byte trolley_coordinate_Letter = 7; // 4=e, So X= Column -- see Analisis de las Matrices_V9_2_7.xlsx on the excel folder --
byte trolley_coordinate_Number = 6; // 6=7, So Y= Row

//  Multiplexer Adapted for library use V3.1

int hall_min = 571; // 650 RB // 571 SK Big // 571 SK Small // If Simulide 200  // Test your Hall Sensor. One pole works fine under this value  // For D6x3mmF32 Magnet
int hall_max = 670; // 825 RB // 730 SK Big // 670 SK Small //If Simulide 800 // Test your Hall Sensor. Other pole works fine upper this value //  For D6x3mmF32 Magnet
int hallMeasure;
const byte S0 = A3;
const byte S1 = A2;
const byte S2 = A1;
const byte S3 = A0;
byte multiplex = 0;
int Mux_Out = A8;
byte column = 0; // V3.2 change type of variable int8_t
byte row = 0; // V3.2 change type of variable int8_t
HC4067 mux1(S0, S1, S2, S3, 22); // Check your pin connection. Greg06 put on 7
HC4067 mux2(S0, S1, S2, S3, 23); // Check your pin connection. Greg06 put on 8
HC4067 mux3(S0, S1, S2, S3, 24); // Check your pin connection. Greg06 put on 9
HC4067 mux4(S0, S1, S2, S3, 25); // Check your pin connection. Greg06 put on 13
// V3.4 for make switch with the number of sensor changed and detect white castle
int number_sensor_change=0;  // V9.2.7.2

// Matrices en C++
//[row][column]
// Row = 1 to 8
// Column= a to H
// [0][0]->[1][a]
// [0][1]->[1][b]
// [7][7]->[8][h]
// Greg put [column][row] for similar Human thinking
// I Changed it to [row][column] for not confuse with C++

//  Button - Switch
const byte BUTTON_WHITE_SWITCH_MOTOR_WHITE (11);
const byte BUTTON_BLACK_SWITCH_MOTOR_BLACK (10);
enum {WHITE, BLACK};

//  MicroMax
extern char lastH[], lastM[]; // SPK is The information MicroMax needs


// V8 Counter number of pieces
int pieces_on_micromax = 0;
int pieces_on_hall_status =0;

// V9 Add GameOver as Extern for acces it from .ino
extern void gameOver(); // Declaración de la función que está en el .cpp


#endif
