# Automatic Chessboard Firmware

Este directorio contiene el firmware para el tablero de ajedrez automático.

## Dependencias

### Librerías Arduino Requeridas

1. **HC4067** - Multiplexor Digital
   - Uso: Manejo de multiplexores para los sensores Hall
   - Instalación: A través del Gestor de Librerías de Arduino
   - Versión recomendada: última estable

2. **LedControl** 
   - Uso: Control de la matriz LED basada en MAX7219
   - Instalación: A través del Gestor de Librerías de Arduino
   - Versión recomendada: última estable

3. **LiquidCrystal_I2C**
   - Uso: Control de la pantalla LCD a través de I2C
   - Instalación: A través del Gestor de Librerías de Arduino
   - Versión recomendada: última estable

4. **Wire** (incluida con Arduino)
   - Uso: Comunicación I2C
   - Instalación: Incluida por defecto en Arduino IDE
   - No requiere instalación adicional

### Instalación de Dependencias

1. Abrir Arduino IDE
2. Ir a Herramientas > Administrar Bibliotecas...
3. Buscar e instalar cada una de las librerías mencionadas arriba
4. Para cada librería, seleccionar "Instalar"

## Estructura

- `Automatic_Chessboard.ino`: Archivo principal del proyecto Arduino
- `include/`: Archivos de cabecera y configuraciones
  - `global.h`: Configuraciones globales y constantes
  - `Micro_Max.h`: Definiciones para el motor de ajedrez
- `src/`: Código fuente
  - `Micro_Max.cpp`: Implementación del motor de ajedrez

## Hardware Requerido

- Arduino Mega 2560 (o compatible)
- Multiplexores HC4067
- Pantalla LCD con interfaz I2C
- Matriz LED con controlador MAX7219
- Sensores Hall lineales
- [Resto del hardware pendiente de documentar]

## Compilación

1. Instalar todas las dependencias listadas arriba
2. Abrir `Automatic_Chessboard.ino` en Arduino IDE
3. Seleccionar la placa Arduino correcta (Arduino Mega 2560)
4. Seleccionar el puerto COM correcto
5. Hacer clic en "Verificar" para comprobar que no hay errores
6. Hacer clic en "Subir" para cargar el programa en la placa

## Historial de Desarrollo

### Versión 11 (2025)
- Agregado soporte para configuración de sensores Hall
- Nuevo archivo hall_setup_V1.h para configuración de sensores

### Versión 10.x (2024-2025)
- **V10.3.5**
  - Variable b[] hecha global para acceso desde programa principal
  - Mejoras en integración con motor de ajedrez
- **V10.3**
  - Cambios en la librería LCD
  - Mejoras en la visualización
- **V10.1** (Agosto 2024)
  - Corrección de movimientos del alfil
  - Fix para movimientos diagonales (c8f5)
- **V10.0** (Junio 2024)
  - Actualización mayor del sistema

### Versión 9.x (2023-2024)
- **V9.2.7.2**
  - Coordenadas del enroque negro corregidas
- **V9.2.7.1**
  - Reintegración de movimientos diagonales
  - Factor diagonal restaurado
- **V9.2.6**
  - Optimización de sensores
  - Los sensores solo funcionan al finalizar el turno del jugador
  - Validación de movimientos antes de registrar posiciones
- **V9.2.5** (Marzo 2024)
  - Reorganización de multiplexores y columnas
  - Referencia: Sensor_Mux_Test_SK_V9_2_5
- **V9.2.4**
  - Redefinición de variables para step_number
- **V9.2.3** (Diciembre 2023)
  - Agregado Serial.print para debug de motores
  - Reubicación de la polarización de bobina
- **V9.2.2** (Diciembre 2023)
  - Implementación de biblioteca para multiplexores
  - Soporte para micropasos

### Versión 7.x
- **V7.1**
  - Implementación de PWM para electroimán

### Mejoras de Hardware
1. **Sistema de Sensores**
   - Migración a sensores Hall lineales
   - Implementación de multiplexores para gestión de sensores
   - Matriz LED 8x8 MAX7219 para monitoreo de sensores

2. **Sistema de Movimiento**
   - Integración de puente H L298
   - Control de polarización del electroimán
   - Soporte para piezas blancas/negras
   - Sistema PWM para control preciso

### Características Especiales

1. **Simulación**
   - Soporte dual para hardware real y SimulIDE
   - Configuraciones específicas para cada modo

2. **Diagnóstico**
   - Matriz LED para visualización de estado de sensores
   - Funciones de depuración vía puerto serie
   - Monitoreo en tiempo real de movimientos

3. **Motor de Ajedrez (Micro_Max)**
   - V3: Soporte para movimiento final antes de jaque mate
   - V2: Display para mostrar ganador
   - Sistema de validación de movimientos
   - Gestión global de estado del juego

## Configuración

[Pendiente: Documentar la configuración de pines y ajustes del sistema]

## Contribución

Si deseas contribuir al proyecto, por favor lee primero el archivo CONTRIBUTING.md en la raíz del repositorio.
