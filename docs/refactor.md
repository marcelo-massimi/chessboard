# 📋 Documento de Refactorización - Automated Chessboard
**Versión Actual:** V11.3  
**Fecha:** Diciembre 2025  
**Autor:** Marcelo Massimi  
**Proyecto:** Automated Chessboard - Open Source

---

## 📑 Tabla de Contenidos

1. [Resumen Ejecutivo](#resumen-ejecutivo)
2. [Análisis del Código Actual](#análisis-del-código-actual)
3. [Inventario de Componentes](#inventario-de-componentes)
4. [Funcionalidades Implementadas](#funcionalidades-implementadas)
5. [Problemas Identificados](#problemas-identificados)
6. [Arquitectura Propuesta](#arquitectura-propuesta)
7. [Plan de Refactorización](#plan-de-refactorización)
8. [Requerimientos de Desarrollo](#requerimientos-de-desarrollo)
9. [Estándares y Convenciones](#estándares-y-convenciones)
10. [Cronograma y Fases](#cronograma-y-fases)

---

## 1. Resumen Ejecutivo

### 🎯 Objetivo del Refactor
Reescribir completamente el código del proyecto Automated Chessboard manteniendo las mismas funcionalidades, hardware y librerías, pero con código optimizado, documentado y simplificado que sea:
- ✅ **Mantenible**: Código modular y bien estructurado
- ✅ **Legible**: Documentación clara y nombres descriptivos
- ✅ **Eficiente**: Optimizado en memoria y rendimiento
- ✅ **Escalable**: Fácil de extender con nuevas funcionalidades
- ✅ **Testeable**: Componentes independientes y verificables

### 📊 Estado Actual
- **Líneas de código:** ~1,632 líneas en .ino + archivos auxiliares
- **Versión:** V11.3
- **Calidad del código:** Funcional pero difícil de mantener
- **Documentación:** Fragmentada y mayormente en comentarios
- **Modularidad:** Baja (código monolítico)

### 🎯 Resultado Esperado
Código profesional con:
- Reducción estimada del 30-40% en líneas de código
- Modularización en componentes independientes
- Documentación completa y actualizada
- Sistema de pruebas básico
- Facilidad de mantenimiento y extensión

---

## 2. Análisis del Código Actual

### 2.1 Estructura de Archivos Actual

```
software/Automatic_Chessboard/
├── Automatic_Chessboard.ino  (1,632 líneas)
├── global.h                    (218 líneas)
├── Micro_Max.cpp              (427 líneas)
├── Micro_Max.h                (16 líneas)
└── desktop.ini
```

### 2.2 Análisis por Archivo

#### **Automatic_Chessboard.ino** (Archivo Principal)
**Responsabilidades actuales:**
- Control de máquina de estados del juego
- Gestión de sensores Hall
- Control de motores
- Control de electroimán
- Interfaz LCD
- Detección de movimientos
- Calibración del sistema
- Gestión de tiempo (countdown)
- Lógica de jugadas especiales (enroque, en passant)

**Problemas:**
- ❌ Archivo monolítico con múltiples responsabilidades
- ❌ Funciones muy largas (>200 líneas en algunos casos)
- ❌ Código repetido en múltiples lugares
- ❌ Nombres de funciones en español e inglés mezclados
- ❌ Comentarios desactualizados y en múltiples idiomas
- ❌ Variables globales difíciles de rastrear
- ❌ Lógica de negocio mezclada con hardware
- ❌ Delays bloqueantes en todo el código
- ❌ Gestión de errores inexistente

#### **global.h** (Configuración Global)
**Contenido actual:**
- Definiciones de hardware (pines, constantes)
- Variables globales del juego
- Matrices de estado de sensores
- Configuración de motores
- Parámetros de calibración
- Enumeraciones de estados

**Problemas:**
- ❌ Mezcla de configuración con variables de estado
- ❌ Comentarios con historial de versiones (debería estar en Git)
- ❌ Valores mágicos sin explicación
- ❌ Falta de organización lógica
- ❌ Múltiples sistemas de configuración

#### **Micro_Max.cpp/h** (Motor de Ajedrez)
**Descripción:**
Motor de ajedrez Micro-Max adaptado para Arduino

**Funcionalidades:**
- Validación de movimientos
- IA del oponente
- Detección de jaque mate
- Gestión del tablero interno

**Problemas:**
- ❌ Código de terceros sin refactorizar
- ❌ Variables con nombres de un solo carácter (difícil de entender)
- ❌ Sin documentación de la lógica interna
- ❌ Acoplamiento fuerte con el código principal
- ❌ Difícil de debuggear

### 2.3 Análisis de Complejidad

#### Complejidad Ciclomática (estimada)
- `black_player_movement()`: **Alta** (~15-20 paths)
- `detect_movement()`: **Media** (~8-10 paths)
- `motor()`: **Media** (~6-8 paths)
- `compare_hall_status()`: **Alta** (~12-15 paths)
- `Fix_Distance()`: **Baja** (~3-4 paths)

#### Acoplamiento
- **Alto:** Casi todas las funciones dependen de variables globales
- **Crítico:** El motor de ajedrez está acoplado con la lógica de movimiento físico

#### Cohesión
- **Baja:** Funciones con múltiples responsabilidades no relacionadas

### 2.4 Deuda Técnica Identificada

| Categoría | Descripción | Severidad |
|-----------|-------------|-----------|
| **Código duplicado** | Bloques de código repetidos para movimientos | 🔴 Alta |
| **Variables globales** | Más de 50 variables globales | 🔴 Alta |
| **Funciones largas** | Funciones con >100 líneas | 🔴 Alta |
| **Falta de modularidad** | Todo en un solo archivo | 🔴 Alta |
| **Nombres inconsistentes** | Mezcla español/inglés | 🟡 Media |
| **Comentarios obsoletos** | Historial de cambios en comentarios | 🟡 Media |
| **Sin gestión de errores** | No hay try-catch o validaciones | 🔴 Alta |
| **Delays bloqueantes** | Uso extensivo de delay() | 🟡 Media |
| **Números mágicos** | Constantes sin nombre | 🟡 Media |

---

## 3. Inventario de Componentes

### 3.1 Hardware Utilizado

#### Microcontrolador
- **Arduino Mega 2560**
  - Motivo: Mayor memoria que Arduino Nano
  - RAM: 8 KB
  - Flash: 256 KB
  - Pines: 54 digitales, 16 analógicos

#### Sensores
- **64 Sensores Hall Lineales (Hall Effect 49E TO92)**
  - Distribución: 8x8 (uno por casilla)
  - Función: Detectar presencia y polaridad de piezas
  - Valores de referencia:
    - Sin imán: ~571-670
    - Polo Norte: < 571 - hall_compare
    - Polo Sur: > 670 + hall_compare
  - Conexión: 4 Multiplexores HC4067

#### Actuadores
- **2 Motores Paso a Paso**
  - Configuración: Eje X y Eje Y (sistema cartesiano)
  - Micropasos configurables: 1, 2, 4, 8, 16
  - Control: Drivers de motor (TMC2208 o similares)
  
- **1 Electroimán con Puente H L298**
  - Control de polaridad: Para piezas blancas y negras
  - PWM: 0-230 (configurable)
  - Pines control: IN1, IN2, ENABLE

#### Interfaz de Usuario
- **Pantalla LCD I2C (16x2)**
  - Dirección: 0x27
  - Librería: LiquidCrystal_I2C

- **Matriz LED 8x8 (MAX7219)**
  - Función: Visualización del estado del tablero
  - Conexión SPI: DIN=51, CLK=52, CS=53

- **2 Botones Arcade**
  - Botón Blanco: Confirmar turno blancas / HvsH
  - Botón Negro: Confirmar turno negras / HvsC

- **2 Finales de Carrera (Limit Switches)**
  - Función: Calibración inicial del sistema
  - Ubicación: Esquinas del tablero

#### Otros
- **4 Multiplexores HC4067 (16 canales)**
  - Pines control: S0 (A3), S1 (A2), S2 (A1), S3 (A0)
  - Pines enable: 22, 23, 24, 25
  - Entrada analógica compartida: A8

### 3.2 Librerías Utilizadas

```cpp
// Librerías Estándar de Arduino
#include <Wire.h>              // Comunicación I2C

// Librerías de Terceros
#include <HC4067.h>            // Control de multiplexores
#include <LiquidCrystal_I2C.h> // Control LCD
#include <LedControl.h>        // Control matriz LED MAX7219

// Librerías del Proyecto
#include "global.h"            // Variables y configuración global
#include "Micro_Max.h"         // Motor de ajedrez
```

### 3.3 Configuración de Pines

```cpp
// Motores
MOTOR_WHITE_DIR   = 2    // Dirección motor X
MOTOR_WHITE_STEP  = 3    // Paso motor X
MOTOR_BLACK_DIR   = 4    // Dirección motor Y
MOTOR_BLACK_STEP  = 5    // Paso motor Y

// Electroimán (Puente H)
MAGNET = 6               // Enable PWM
IN1    = 14              // Control polaridad
IN2    = 15              // Control polaridad

// Botones
BUTTON_WHITE_SWITCH_MOTOR_WHITE = 11
BUTTON_BLACK_SWITCH_MOTOR_BLACK = 10

// Multiplexores
S0 = A3, S1 = A2, S2 = A1, S3 = A0  // Control común
Mux_Out = A8                         // Lectura analógica común
Enable: mux1=22, mux2=23, mux3=24, mux4=25

// Matriz LED (SPI)
DIN = 51
CLK = 52
CS  = 53
```

---

## 4. Funcionalidades Implementadas

### 4.1 Modos de Juego

#### 🎮 Modo HvsH (Humano vs Humano)
- Dos jugadores humanos
- Validación de movimientos por motor de ajedrez
- Control de tiempo individual por jugador
- Turnos alternados con confirmación por botón

#### 🤖 Modo HvsC (Humano vs Computadora)
- Jugador humano (blancas) vs IA (negras)
- Calibración automática del trolley al inicio
- IA ejecuta movimientos físicos automáticamente
- Detección de jaque mate con pantalla de victoria

### 4.2 Sistema de Detección

#### Detección de Movimientos
- **Lectura de 64 sensores Hall** mediante multiplexores
- **Calibración inicial** sin piezas para obtener valores de referencia
- **Detección de polaridad**: Distingue piezas blancas y negras
- **Comparación con memoria**: Detecta cambios entre estados

#### Tipos de Movimientos Detectados
1. **Movimiento simple** (2 sensores cambian)
2. **Captura normal** (1 sensor cambia)
3. **Captura al paso** (3 sensores cambian)
4. **Enroque** (4 sensores cambian)

### 4.3 Sistema de Movimiento Físico

#### Control de Trolley (Carro XY)
- **Sistema cartesiano** con 2 motores paso a paso
- **Movimientos soportados:**
  - Horizontales (A-H)
  - Verticales (1-8)
  - Diagonales (Alfil, Reina)
  - Movimiento en L (Caballo)

#### Calibración Automática
- Uso de finales de carrera
- Posición inicial programable
- Ajuste de pasos por casilla: `SQUARE_SIZE = 199 * microsteps`
- Factor diagonal: `DIAGONALFACTOR = 2.01`

#### Control de Electroimán
- **Polaridad variable**: Diferencia entre piezas blancas y negras
- **Control PWM**: 0-230 para ajustar fuerza
- **Secuencia de captura:**
  1. Posicionar sobre pieza a capturar
  2. Activar electroimán
  3. Mover a "cementerio" (fuera del tablero)
  4. Desactivar electroimán
  5. Retornar para mover pieza atacante

### 4.4 Movimientos Especiales

#### ♟️ Enroque (Castling)
- **Enroque corto (Kingside)**: e1-g1, h1-f1
- **Enroque largo (Queenside)**: e1-c1, a1-d1
- Secuencia de movimiento automática del Rey y Torre
- Corrección de posición con `FIXUP_POSITION`

#### ♟️ Captura al Paso (En Passant)
- Detección automática (3 sensores cambian)
- Eliminación de peón capturado
- Movimiento de peón atacante

#### ♟️ Promoción
- **Limitación actual**: Solo promoción a Reina
- Automática al llegar a la octava fila

### 4.5 Sistema de Gestión de Tiempo

#### Contador de Tiempo (Countdown)
- Tiempo configurable por jugador
- Actualización cada segundo
- Visualización en LCD
- **Game Over** por tiempo agotado

### 4.6 Interfaz de Usuario

#### Pantalla LCD
Muestra:
- Versión del software
- Modo de juego seleccionado
- Turno actual (White/Black)
- Tiempo restante (MM:SS)
- Mensajes de error
- Pantalla de victoria

#### Matriz LED 8x8
- Representación visual del tablero
- Ilumina casillas con piezas
- Actualización en tiempo real

#### Sistema de Botones
- **Botón Blanco**: Confirmar turno / Seleccionar HvsH
- **Botón Negro**: Confirmar turno / Seleccionar HvsC
- Sistema de rebote (debounce) de 100ms

### 4.7 Recuperación de Errores

#### Detección de Problemas
- Movimiento no válido
- Pieza no centrada correctamente
- Número incorrecto de piezas detectadas

#### Corrección Automática
- **Fix_Distance()**: Ajuste micro de posición
- Verificación posterior a movimiento negro

#### Corrección Manual
- **Set_The_Board()**: Permite al usuario reposicionar piezas
- Muestra tablero esperado en Serial Monitor
- Re-lectura de sensores tras corrección

### 4.8 Motor de Ajedrez (Micro-Max)

#### Capacidades
- Validación de movimientos legales
- IA con profundidad configurable
- Detección de jaque y jaque mate
- Evaluación de posición
- Representación interna del tablero (array `b[]`)

#### Niveles de Dificultad
- Variable `T` controla profundidad: `T = 0x3F`
- Tiempo de pensamiento variable

---

## 5. Problemas Identificados

### 5.1 Problemas Críticos 🔴

#### **P1: Arquitectura Monolítica**
- **Descripción**: Todo el código en un único archivo de 1,632 líneas
- **Impacto**: Dificulta mantenimiento, testing y comprensión
- **Solución**: Modularizar en componentes independientes

#### **P2: Exceso de Variables Globales**
- **Descripción**: Más de 50 variables globales compartidas
- **Impacto**: Estado compartido impredecible, difícil debugging
- **Solución**: Encapsular en clases/structs con acceso controlado

#### **P3: Funciones Excesivamente Largas**
- **Descripción**: Funciones con >200 líneas (`black_player_movement`, etc.)
- **Impacto**: Difícil de entender, probar y modificar
- **Solución**: Dividir en subfunciones con responsabilidad única

#### **P4: Código Duplicado**
- **Descripción**: Lógica repetida para movimientos, lecturas de sensores
- **Impacto**: Mantenimiento duplicado, inconsistencias
- **Solución**: Extraer a funciones reutilizables

#### **P5: Falta de Gestión de Errores**
- **Descripción**: No hay validaciones ni manejo de excepciones
- **Impacto**: El sistema puede quedar en estado inconsistente
- **Solución**: Implementar sistema de estados de error

#### **P6: Acoplamiento Alto**
- **Descripción**: Dependencias circulares entre componentes
- **Impacto**: Cambios en un módulo afectan a otros
- **Solución**: Interfaces bien definidas entre módulos

### 5.2 Problemas Importantes 🟡

#### **P7: Nomenclatura Inconsistente**
- **Problema**: Mezcla de español e inglés
  - Ejemplos: `black_player_movement()`, `hall_colone[]`, `pieces_on_board()`
- **Solución**: Estandarizar a inglés

#### **P8: Comentarios Obsoletos**
- **Problema**: Comentarios con historial de versiones en el código
- **Solución**: Usar Git para historial, comentarios solo para lógica compleja

#### **P9: Delays Bloqueantes**
- **Problema**: Uso extensivo de `delay()` bloquea ejecución
- **Solución**: Implementar delays no bloqueantes con `millis()`

#### **P10: Números Mágicos**
- **Problema**: Constantes sin nombre: `0x3F`, `799`, `0x10`, `0x08`
- **Solución**: Definir como constantes nombradas

#### **P11: Falta de Documentación de API**
- **Problema**: No hay documentación de parámetros y retornos
- **Solución**: Comentarios estructurados tipo Doxygen

#### **P12: Sin Sistema de Logging**
- **Problema**: Debug mediante Serial.print disperso
- **Solución**: Sistema de logging con niveles (DEBUG, INFO, ERROR)

### 5.3 Mejoras Deseables 🟢

#### **P13: Configuración Hardcodeada**
- **Problema**: Valores de calibración en el código
- **Solución**: Sistema de configuración persistente (EEPROM)

#### **P14: Sin Tests Automatizados**
- **Problema**: Testing solo manual
- **Solución**: Tests unitarios básicos para componentes críticos

#### **P15: Optimización de Memoria**
- **Problema**: Uso de memoria no optimizado
- **Solución**: Revisar uso de arrays y strings

---

## 6. Arquitectura Propuesta

### 6.1 Principios de Diseño

#### 🎯 Principios SOLID Adaptados a Arduino
1. **Single Responsibility**: Una clase/módulo = una responsabilidad
2. **Open/Closed**: Abierto a extensión, cerrado a modificación
3. **Interface Segregation**: Interfaces específicas, no genéricas
4. **Dependency Inversion**: Depender de abstracciones

#### 📐 Patrones de Diseño a Aplicar
- **State Machine**: Para gestión de estados del juego
- **Strategy**: Para diferentes modos de juego
- **Observer**: Para notificaciones de eventos
- **Command**: Para encapsular movimientos

### 6.2 Estructura Modular Propuesta

```
software/AutomatedChessboard/
├── AutomatedChessboard.ino          # Punto de entrada (mínimo)
├── config/
│   ├── Config.h                      # Configuración global
│   ├── HardwarePins.h               # Definición de pines
│   └── Constants.h                   # Constantes del sistema
├── hardware/
│   ├── HallSensorManager.h/cpp      # Gestión sensores Hall
│   ├── MotorController.h/cpp        # Control de motores
│   ├── ElectromagnetController.h/cpp # Control electroimán
│   ├── MultiplexerManager.h/cpp     # Gestión multiplexores
│   └── ButtonManager.h/cpp          # Gestión de botones
├── display/
│   ├── LCDDisplay.h/cpp             # Control LCD
│   └── LEDMatrix.h/cpp              # Control matriz LED
├── game/
│   ├── GameStateMachine.h/cpp       # Máquina de estados
│   ├── ChessEngine.h/cpp            # Motor de ajedrez (Micro-Max refactorizado)
│   ├── MoveDetector.h/cpp           # Detección de movimientos
│   ├── MoveExecutor.h/cpp           # Ejecución física de movimientos
│   └── GameTimer.h/cpp              # Gestión de tiempo
├── utils/
│   ├── Logger.h/cpp                  # Sistema de logging
│   ├── ErrorHandler.h/cpp           # Gestión de errores
│   └── Helpers.h/cpp                # Funciones auxiliares
└── types/
    ├── ChessTypes.h                  # Tipos de datos del juego
    └── HardwareTypes.h              # Tipos de datos hardware
```

### 6.3 Diagrama de Componentes

```
┌─────────────────────────────────────────────────────────┐
│              AUTOMATED CHESSBOARD SYSTEM                │
└─────────────────────────────────────────────────────────┘
                            │
                ┌───────────┴──────────┐
                │                      │
        ┌───────▼────────┐    ┌───────▼────────┐
        │  GAME LAYER    │    │  DISPLAY LAYER │
        │                │    │                │
        │ • GameStateMachine  │ • LCDDisplay  │
        │ • ChessEngine  │    │ • LEDMatrix   │
        │ • MoveDetector │    │                │
        │ • MoveExecutor │    │                │
        │ • GameTimer    │    │                │
        └───────┬────────┘    └────────────────┘
                │
        ┌───────▼────────┐
        │ HARDWARE LAYER │
        │                │
        │ • HallSensorMgr     │
        │ • MotorController   │
        │ • ElectromagnetCtrl │
        │ • MultiplexerMgr    │
        │ • ButtonManager     │
        └───────┬────────┘
                │
        ┌───────▼────────┐
        │  CONFIG LAYER  │
        │                │
        │ • Config       │
        │ • HardwarePins │
        │ • Constants    │
        └────────────────┘
```

### 6.4 Diseño de Clases Principales

#### **GameStateMachine**
```cpp
enum class GameState {
    STARTUP,
    MODE_SELECTION,
    CALIBRATION,
    WHITE_TURN,
    BLACK_TURN,
    GAME_OVER
};

class GameStateMachine {
private:
    GameState currentState;
    GameMode gameMode;
    
public:
    void initialize();
    void update();
    void changeState(GameState newState);
    GameState getCurrentState();
};
```

#### **HallSensorManager**
```cpp
struct SensorReading {
    int value;
    int8_t pieceType;  // -1=White, 0=Empty, 1=Black
};

class HallSensorManager {
private:
    SensorReading currentState[8][8];
    SensorReading previousState[8][8];
    int referenceValues[8][8];
    
public:
    void calibrate();
    void readAllSensors();
    SensorReading getSensor(uint8_t row, uint8_t col);
    bool detectChanges();
};
```

#### **MotorController**
```cpp
enum class Direction {
    A_TO_H,
    H_TO_A,
    F1_TO_F8,
    F8_TO_F1,
    DIAGONAL_AH_18,
    DIAGONAL_HA_81,
    DIAGONAL_AH_81,
    DIAGONAL_HA_18
};

class MotorController {
private:
    uint8_t currentX;
    uint8_t currentY;
    
public:
    void calibrate();
    void moveTo(uint8_t x, uint8_t y);
    void move(Direction dir, float distance);
    Position getCurrentPosition();
};
```

#### **MoveDetector**
```cpp
enum class MoveType {
    SIMPLE_MOVE,
    CAPTURE,
    EN_PASSANT,
    CASTLING_KINGSIDE,
    CASTLING_QUEENSIDE
};

struct DetectedMove {
    Position from;
    Position to;
    MoveType type;
    bool isValid;
};

class MoveDetector {
public:
    DetectedMove analyzeChanges(
        const SensorState& current,
        const SensorState& previous
    );
};
```

#### **MoveExecutor**
```cpp
class MoveExecutor {
private:
    MotorController& motor;
    ElectromagnetController& magnet;
    
public:
    bool executeMove(const Move& move);
    bool executeCapture(const Move& move);
    bool executeCastling(const Move& move);
    bool executeEnPassant(const Move& move);
};
```

### 6.5 Flujo de Datos

```
┌──────────────┐
│ User Input   │
│ (Buttons)    │
└──────┬───────┘
       │
       ▼
┌──────────────────┐
│ GameStateMachine │◄──────────┐
└──────┬───────────┘           │
       │                       │
       ▼                       │
┌──────────────────┐           │
│ HallSensorMgr    │           │
│ .readSensors()   │           │
└──────┬───────────┘           │
       │                       │
       ▼                       │
┌──────────────────┐           │
│ MoveDetector     │           │
│ .analyzeChanges()│           │
└──────┬───────────┘           │
       │                       │
       ▼                       │
┌──────────────────┐           │
│ ChessEngine      │           │
│ .validateMove()  │           │
└──────┬───────────┘           │
       │                       │
       ▼                       │
┌──────────────────┐           │
│ MoveExecutor     │           │
│ .executeMove()   │           │
└──────┬───────────┘           │
       │                       │
       ▼                       │
┌──────────────────┐           │
│ MotorController  │           │
│ Electromagnet    │───────────┘
└──────────────────┘
       │
       ▼
┌──────────────────┐
│ Display Update   │
│ (LCD, LED)       │
└──────────────────┘
```

---

## 7. Plan de Refactorización

### 7.1 Estrategia General

#### Enfoque: **Refactor Incremental y Modular**
1. **No reescritura completa de una vez**
2. **Refactorizar módulo por módulo**
3. **Mantener funcionalidad durante el proceso**
4. **Testing continuo de cada módulo**

#### Criterios de Éxito
- ✅ Código compila sin errores
- ✅ Todas las funcionalidades originales operativas
- ✅ Reducción de líneas de código (objetivo: -30%)
- ✅ Mejora en legibilidad (métricas de complejidad)
- ✅ Documentación completa

### 7.2 Fases del Refactor

#### **FASE 0: Preparación** (1-2 días)
- [ ] Crear branch de refactorización en Git
- [ ] Documentar estado actual (este documento)
- [ ] Establecer métricas de referencia
- [ ] Configurar entorno de desarrollo
- [ ] Crear estructura de carpetas propuesta

#### **FASE 1: Configuración y Constantes** (2-3 días)
**Objetivo**: Separar configuración de lógica

**Tareas**:
1. Crear `config/HardwarePins.h`
   - Mover todas las definiciones de pines
   - Documentar cada pin

2. Crear `config/Constants.h`
   - Extraer todas las constantes
   - Organizar por categoría
   - Documentar propósito de cada una

3. Crear `types/ChessTypes.h`
   - Definir enums (GameMode, GameState, Direction, etc.)
   - Definir structs (Position, Move, etc.)

4. Limpiar `global.h`
   - Mantener solo variables de estado necesarias
   - Documentar cada variable global restante

**Entregables**:
- Archivos de configuración organizados
- Documentación de constantes
- Reducción de números mágicos a 0

#### **FASE 2: Capa de Hardware** (5-7 días)
**Objetivo**: Encapsular todo el acceso a hardware

**Tareas**:
1. **MultiplexerManager** (1 día)
   ```cpp
   class MultiplexerManager {
   public:
       void initialize();
       int readChannel(uint8_t muxId, uint8_t channel);
   };
   ```

2. **HallSensorManager** (2 días)
   ```cpp
   class HallSensorManager {
   public:
       void calibrate();
       void readAllSensors();
       int8_t getPieceAt(uint8_t row, uint8_t col);
       bool hasChanged(uint8_t row, uint8_t col);
   };
   ```

3. **MotorController** (2 días)
   ```cpp
   class MotorController {
   public:
       void calibrate();
       void moveTo(Position pos);
       void move(Direction dir, float distance);
   };
   ```

4. **ElectromagnetController** (1 día)
   ```cpp
   class ElectromagnetController {
   public:
       void activate(PieceColor color);
       void deactivate();
       void setPower(uint8_t pwm);
   };
   ```

5. **ButtonManager** (1 día)
   ```cpp
   class ButtonManager {
   public:
       bool isPressed(Button button);
       void update();  // Non-blocking debounce
   };
   ```

**Entregables**:
- Clases de hardware independientes
- Tests básicos de cada componente
- Documentación de API

#### **FASE 3: Capa de Display** (2-3 días)
**Objetivo**: Abstraer interfaces de usuario

**Tareas**:
1. **LCDDisplay** (1 día)
   ```cpp
   class LCDDisplay {
   public:
       void showStartup();
       void showModeSelection();
       void showTurn(Player player, int minutes, int seconds);
       void showError(const char* message);
       void showWinner(Player winner);
   };
   ```

2. **LEDMatrix** (1 día)
   ```cpp
   class LEDMatrix {
   public:
       void updateFromBoard(const int8_t board[8][8]);
       void clear();
       void setPixel(uint8_t row, uint8_t col, bool state);
   };
   ```

**Entregables**:
- Interfaces de display desacopladas
- Mensajes centralizados

#### **FASE 4: Detección y Ejecución de Movimientos** (4-5 días)
**Objetivo**: Lógica de movimientos modular

**Tareas**:
1. **MoveDetector** (2 días)
   ```cpp
   class MoveDetector {
   public:
       DetectedMove analyzeChanges();
   private:
       MoveType classifyMove(int changedSensors);
   };
   ```

2. **MoveExecutor** (2-3 días)
   ```cpp
   class MoveExecutor {
   public:
       bool executeMove(const Move& move);
   private:
       void executeSimpleMove(const Move& move);
       void executeCapture(const Move& move);
       void executeCastling(const Move& move);
       void executeEnPassant(const Move& move);
       void fixPosition();
   };
   ```

**Entregables**:
- Detección de movimientos separada
- Ejecución física separada
- Reducción de código duplicado

#### **FASE 5: Motor de Ajedrez** (3-4 días)
**Objetivo**: Refactorizar Micro-Max

**Tareas**:
1. Renombrar variables a nombres descriptivos
2. Documentar funciones principales
3. Separar validación de IA
4. Crear interfaz clara con el resto del sistema

```cpp
class ChessEngine {
public:
    bool validateMove(const Move& move);
    Move calculateBestMove();
    bool isCheckmate();
    void resetBoard();
};
```

**Entregables**:
- Motor de ajedrez documentado
- Interfaz simplificada

#### **FASE 6: Máquina de Estados y Lógica de Juego** (4-5 días)
**Objetivo**: Centralizar control del flujo del juego

**Tareas**:
1. **GameStateMachine** (2 días)
   ```cpp
   class GameStateMachine {
   public:
       void update();
       void handleInput(GameEvent event);
   private:
       void handleStartup();
       void handleModeSelection();
       void handleCalibration();
       void handleWhiteTurn();
       void handleBlackTurn();
   };
   ```

2. **GameTimer** (1 día)
   ```cpp
   class GameTimer {
   public:
       void start();
       void update();
       bool hasExpired();
       TimeRemaining getTime(Player player);
   };
   ```

3. Integración de componentes (2 días)

**Entregables**:
- Máquina de estados completa
- Control de tiempo no bloqueante
- Sistema integrado funcional

#### **FASE 7: Utilidades y Pulido** (3-4 días)
**Objetivo**: Sistema de soporte

**Tareas**:
1. **Logger** (1 día)
   ```cpp
   class Logger {
   public:
       static void debug(const char* message);
       static void info(const char* message);
       static void error(const char* message);
   };
   ```

2. **ErrorHandler** (1 día)
   ```cpp
   class ErrorHandler {
   public:
       void handleError(ErrorCode code);
       void requestManualFix();
   };
   ```

3. Optimización de memoria (1-2 días)
4. Revisión general y limpieza (1 día)

**Entregables**:
- Sistema de logging
- Gestión de errores consistente
- Código optimizado

#### **FASE 8: Testing y Documentación** (3-4 días)
**Objetivo**: Calidad y mantenibilidad

**Tareas**:
1. Tests de componentes individuales
2. Tests de integración
3. Documentación de API (Doxygen)
4. README actualizado
5. Guía de usuario
6. Guía de mantenimiento

**Entregables**:
- Suite de tests
- Documentación completa
- Guías de usuario y desarrollador

### 7.3 Cronograma Estimado

| Fase | Duración | Dependencias |
|------|----------|--------------|
| FASE 0 | 1-2 días | - |
| FASE 1 | 2-3 días | FASE 0 |
| FASE 2 | 5-7 días | FASE 1 |
| FASE 3 | 2-3 días | FASE 2 |
| FASE 4 | 4-5 días | FASE 2 |
| FASE 5 | 3-4 días | FASE 1 |
| FASE 6 | 4-5 días | FASE 2, 3, 4, 5 |
| FASE 7 | 3-4 días | FASE 6 |
| FASE 8 | 3-4 días | FASE 7 |

**Duración Total Estimada**: 27-37 días (~5-7 semanas)

---

## 8. Requerimientos de Desarrollo

### 8.1 Requerimientos Funcionales

#### RF-001: Modos de Juego
- El sistema DEBE soportar modo Humano vs Humano
- El sistema DEBE soportar modo Humano vs Computadora
- La selección de modo DEBE hacerse mediante botones

#### RF-002: Detección de Movimientos
- El sistema DEBE detectar movimientos simples
- El sistema DEBE detectar capturas
- El sistema DEBE detectar enroque
- El sistema DEBE detectar captura al paso
- El sistema DEBE validar movimientos contra reglas de ajedrez

#### RF-003: Ejecución de Movimientos
- El sistema DEBE mover piezas físicamente
- El sistema DEBE soportar todos los tipos de movimiento de ajedrez
- El sistema DEBE capturar piezas y moverlas fuera del tablero
- El sistema DEBE corregir automáticamente posición de piezas

#### RF-004: Interfaz de Usuario
- El sistema DEBE mostrar estado actual en LCD
- El sistema DEBE mostrar tablero en matriz LED
- El sistema DEBE mostrar tiempo restante
- El sistema DEBE mostrar mensajes de error
- El sistema DEBE mostrar pantalla de victoria

#### RF-005: Gestión de Tiempo
- El sistema DEBE controlar tiempo individual por jugador
- El sistema DEBE declarar game over por timeout
- El sistema DEBE pausar/reanudar tiempo correctamente

#### RF-006: Calibración
- El sistema DEBE calibrar sensores Hall al inicio
- El sistema DEBE calibrar posición de trolley
- El sistema DEBE permitir recalibración manual

#### RF-007: Recuperación de Errores
- El sistema DEBE detectar movimientos inválidos
- El sistema DEBE permitir corrección manual del tablero
- El sistema DEBE validar cantidad correcta de piezas

### 8.2 Requerimientos No Funcionales

#### RNF-001: Rendimiento
- Lectura de sensores: < 500ms para 64 sensores
- Tiempo de respuesta UI: < 100ms
- Movimiento de pieza: < 30s para cualquier movimiento

#### RNF-002: Memoria
- Uso de RAM: < 6KB (75% de disponible)
- Uso de Flash: < 200KB (78% de disponible)

#### RNF-003: Confiabilidad
- El sistema NO DEBE quedar en estado inconsistente
- El sistema DEBE ser recuperable de errores
- Detección de piezas: >95% precisión

#### RNF-004: Mantenibilidad
- Código DEBE seguir estándares de codificación
- Funciones NO DEBEN exceder 50 líneas
- Complejidad ciclomática < 10 por función
- Todas las funciones públicas DEBEN estar documentadas

#### RNF-005: Portabilidad
- Compatible con Arduino Mega 2560
- Mismas librerías que versión original
- Mismo hardware que versión original

### 8.3 Restricciones

#### Hardware
- DEBE usar Arduino Mega 2560
- DEBE usar sensores Hall lineales 49E
- DEBE usar multiplexores HC4067
- DEBE usar puente H L298 para electroimán
- DEBE usar LCD I2C 16x2
- DEBE usar matriz LED MAX7219

#### Software
- DEBE usar Arduino IDE o PlatformIO
- DEBE compilar con estándar C++11 o superior
- DEBE usar las mismas librerías:
  - Wire.h
  - HC4067.h
  - LiquidCrystal_I2C.h
  - LedControl.h

#### Reglas de Juego
- DEBE seguir reglas estándar de ajedrez FIDE
- DEBE implementar todas las jugadas especiales
- Promoción: solo a Reina (limitación conocida)

---

## 9. Estándares y Convenciones

### 9.1 Convenciones de Código

#### Nomenclatura

```cpp
// Clases: PascalCase
class HallSensorManager { };
class MotorController { };

// Funciones y métodos: camelCase
void readSensor();
bool validateMove();

// Variables: camelCase
int sensorValue;
float distanceInSquares;

// Constantes: UPPER_SNAKE_CASE
const int MAX_SENSORS = 64;
const float SQUARE_SIZE = 199.0;

// Enums: PascalCase para tipo, UPPER_CASE para valores
enum class GameState {
    STARTUP,
    MODE_SELECTION,
    CALIBRATION
};

// Miembros privados: prefijo m_
class Example {
private:
    int m_privateValue;
    bool m_isReady;
};
```

#### Formato de Código

```cpp
// Llaves en nueva línea para funciones y clases
void functionName()
{
    // Código
}

// Llaves en misma línea para control de flujo
if (condition) {
    // Código
} else {
    // Código
}

// Indentación: 4 espacios (no tabs)
void example()
{
    if (condition) {
        doSomething();
        doAnother();
    }
}

// Longitud de línea máxima: 100 caracteres
const char* longMessage = "Este es un mensaje largo que "
                          "se divide en múltiples líneas";
```

### 9.2 Documentación

#### Comentarios de Archivo

```cpp
/**
 * @file HallSensorManager.h
 * @brief Gestión de sensores Hall para detección de piezas
 * @author Marcelo Massimi
 * @date Diciembre 2025
 * @version 2.0
 */
```

#### Comentarios de Clase

```cpp
/**
 * @class HallSensorManager
 * @brief Maneja la lectura y calibración de 64 sensores Hall
 * 
 * Esta clase es responsable de:
 * - Calibrar sensores al inicio
 * - Leer estado actual de todos los sensores
 * - Detectar cambios entre estados
 * - Distinguir entre piezas blancas y negras
 */
class HallSensorManager
{
    // ...
};
```

#### Comentarios de Función

```cpp
/**
 * @brief Lee todos los sensores Hall y actualiza el estado
 * 
 * Utiliza los multiplexores para leer secuencialmente
 * los 64 sensores y determina si hay pieza y su color.
 * 
 * @pre Los multiplexores deben estar inicializados
 * @post El array currentState está actualizado
 * 
 * @return true si la lectura fue exitosa, false en caso de error
 */
bool readAllSensors();
```

#### Comentarios Inline

```cpp
// Solo para lógica compleja o no obvia
// NO comentar código auto-explicativo

// MAL
int x = 5;  // Asignar 5 a x

// BIEN
// Ajuste empírico basado en calibración física
// Ver documentación de hardware para detalles
const float DIAGONAL_FACTOR = 2.01;
```

### 9.3 Gestión de Errores

#### Códigos de Error

```cpp
enum class ErrorCode {
    NONE = 0,
    SENSOR_READ_FAILED,
    INVALID_MOVE,
    PIECE_COUNT_MISMATCH,
    CALIBRATION_FAILED,
    TIMEOUT,
    HARDWARE_ERROR
};
```

#### Manejo de Errores

```cpp
// Retornar códigos de error
ErrorCode function()
{
    if (error) {
        Logger::error("Descripción del error");
        return ErrorCode::SENSOR_READ_FAILED;
    }
    return ErrorCode::NONE;
}

// Uso
ErrorCode result = function();
if (result != ErrorCode::NONE) {
    ErrorHandler::handle(result);
}
```

### 9.4 Control de Versiones

#### Commits
```
tipo(alcance): descripción corta

Descripción detallada si es necesaria

Fixes #123
```

Tipos:
- `feat`: Nueva funcionalidad
- `fix`: Corrección de bug
- `refactor`: Refactorización sin cambio de funcionalidad
- `docs`: Cambios en documentación
- `test`: Añadir o modificar tests
- `style`: Cambios de formato
- `perf`: Mejoras de rendimiento

Ejemplos:
```
feat(sensors): implementar HallSensorManager

Añade clase para gestionar lectura de sensores Hall
con calibración automática y detección de cambios.

Refs #12
```

#### Branches
- `main`: Código estable y probado
- `develop`: Desarrollo activo
- `feature/nombre`: Nueva funcionalidad
- `refactor/nombre`: Refactorización
- `fix/nombre`: Corrección de bugs

### 9.5 Testing

#### Estructura de Tests

```cpp
/**
 * @test Test_HallSensorManager_Calibration
 * @brief Verifica que la calibración se ejecute correctamente
 */
void test_calibration()
{
    HallSensorManager sensors;
    
    // Arrange (Preparar)
    sensors.initialize();
    
    // Act (Actuar)
    bool result = sensors.calibrate();
    
    // Assert (Verificar)
    assert(result == true);
    assert(sensors.isCalibrated() == true);
}
```

---

## 10. Cronograma y Fases

### 10.1 Resumen de Fases

| Fase | Nombre | Duración | Inicio | Fin | Estado |
|------|--------|----------|--------|-----|--------|
| 0 | Preparación | 1-2 días | - | - | ⏳ Pendiente |
| 1 | Configuración | 2-3 días | - | - | ⏳ Pendiente |
| 2 | Hardware | 5-7 días | - | - | ⏳ Pendiente |
| 3 | Display | 2-3 días | - | - | ⏳ Pendiente |
| 4 | Movimientos | 4-5 días | - | - | ⏳ Pendiente |
| 5 | Motor Ajedrez | 3-4 días | - | - | ⏳ Pendiente |
| 6 | Estado/Juego | 4-5 días | - | - | ⏳ Pendiente |
| 7 | Utilidades | 3-4 días | - | - | ⏳ Pendiente |
| 8 | Testing/Docs | 3-4 días | - | - | ⏳ Pendiente |

**Total**: 27-37 días (~5-7 semanas)

### 10.2 Hitos Principales

#### Hito 1: Infraestructura Básica ✅
- **Fecha objetivo**: Fin de FASE 1
- **Criterios**:
  - Estructura de carpetas creada
  - Configuración separada del código
  - Tipos de datos definidos
  - Documentación inicial completada

#### Hito 2: Hardware Abstraído ✅
- **Fecha objetivo**: Fin de FASE 2
- **Criterios**:
  - Todas las clases de hardware funcionando
  - Tests básicos pasando
  - Documentación de API completa
  - Lectura de sensores operativa

#### Hito 3: Sistema Integrado ✅
- **Fecha objetivo**: Fin de FASE 6
- **Criterios**:
  - Todos los módulos integrados
  - Funcionalidad completa del juego
  - Ambos modos operativos (HvsH, HvsC)
  - Máquina de estados funcional

#### Hito 4: Código Producción ✅
- **Fecha objetivo**: Fin de FASE 8
- **Criterios**:
  - Todas las funcionalidades testeadas
  - Documentación completa
  - Código optimizado
  - README y guías actualizadas

### 10.3 Checklist de Calidad

#### Por Cada Módulo
- [ ] Código compila sin warnings
- [ ] Funcionalidad original preservada
- [ ] Tests básicos implementados
- [ ] Documentación de API completa
- [ ] Sin números mágicos
- [ ] Nomenclatura consistente
- [ ] Complejidad ciclomática < 10
- [ ] Funciones < 50 líneas
- [ ] Sin código duplicado

#### Al Finalizar
- [ ] Todas las funcionalidades operativas
- [ ] Reducción de líneas de código lograda
- [ ] Memoria dentro de límites
- [ ] Performance aceptable
- [ ] Documentación completa
- [ ] README actualizado
- [ ] Guía de usuario creada
- [ ] Guía de desarrollador creada

---

## 📚 Apéndices

### A. Glosario

| Término | Definición |
|---------|-----------|
| **Trolley** | Carro XY que mueve el electroimán sobre el tablero |
| **Hall Sensor** | Sensor magnético para detectar presencia de piezas |
| **Multiplexer** | Circuito que permite leer múltiples sensores con un pin |
| **Enroque** | Movimiento especial de Rey y Torre simultáneo |
| **En Passant** | Captura especial de peón |
| **Micro-Max** | Motor de ajedrez minimalista usado en el proyecto |
| **PWM** | Modulación por ancho de pulso para control de potencia |

### B. Referencias

1. **Proyecto Original**
   - Greg06 Instructable: https://www.instructables.com/Automated-Chessboard/

2. **Micro-Max Chess Engine**
   - H.G. Muller's Micro-Max

3. **Librerías Arduino**
   - HC4067: Multiplexer control library
   - LiquidCrystal_I2C: LCD I2C control
   - LedControl: MAX7219 LED matrix

4. **Reglas de Ajedrez FIDE**
   - https://www.fide.com/fide/handbook.html?id=171&view=article

### C. Historial de Versiones del Código

| Versión | Fecha | Cambios Principales |
|---------|-------|---------------------|
| V11.3 | 2025 | Corrección problema after no_valid_move |
| V11.0 | Jul 2025 | Nuevo sistema comparación sensores |
| V10.3.8 | 2024 | Game Over por tiempo, fix black castle |
| V10.3.7 | 2024 | Pantalla final jaque mate |
| V10.3 | Oct 2024 | Fix posición de piezas |
| V9.2.7 | 2024 | Diagonal moves, En Passant, Castle |
| V9.2.6 | 2024 | Mejoras detección sensores |
| V7.1 | 2024 | PWM para electroimán |

### D. Configuración de Hardware Actual

```cpp
// Microcontrolador
Arduino Mega 2560 (ATmega2560)
- Clock: 16 MHz
- Flash: 256 KB
- SRAM: 8 KB
- EEPROM: 4 KB

// Motores
Tipo: Stepper (paso a paso)
Drivers: TMC2208 o similares
Micropasos: 8 (configurable)
Velocidad lenta: 3500/microsteps µs
Velocidad rápida: 2000/microsteps µs

// Sensores
64x Hall Effect 49E TO92
Conexión: 4x HC4067 (16 canales c/u)
Rango operación: 571-670 (sin imán)

// Electroimán
Control: Puente H L298
PWM: 0-230
Polaridad: Variable (blancas/negras)

// Display
LCD: 16x2 I2C (0x27)
LED Matrix: 8x8 MAX7219
```

### E. Matriz de Trazabilidad

| Requisito | Módulo Responsable | Test | Documentación |
|-----------|-------------------|------|---------------|
| RF-001 | GameStateMachine | test_game_modes | GameStateMachine.h |
| RF-002 | MoveDetector | test_move_detection | MoveDetector.h |
| RF-003 | MoveExecutor | test_move_execution | MoveExecutor.h |
| RF-004 | LCDDisplay, LEDMatrix | test_display | Display.h |
| RF-005 | GameTimer | test_timer | GameTimer.h |
| RF-006 | HallSensorMgr, MotorCtrl | test_calibration | Calibration.md |
| RF-007 | ErrorHandler | test_error_handling | ErrorHandler.h |

---

## 📝 Notas Finales

### Para el Desarrollador

Este documento es una guía viva. Se espera que se actualice conforme avanza el refactor. Cada fase completada debe marcar las tareas como finalizadas y documentar cualquier desviación del plan original.

### Contacto

Para preguntas o clarificaciones sobre este documento:
- **Autor**: Marcelo Massimi
- **Proyecto**: github.com/marcelo-massimi/chessboard
- **Fecha**: Diciembre 2025

---

**🎯 ¡El objetivo es código limpio, mantenible y profesional!**

**Versión del Documento**: 1.0  
**Última Actualización**: Diciembre 9, 2025
