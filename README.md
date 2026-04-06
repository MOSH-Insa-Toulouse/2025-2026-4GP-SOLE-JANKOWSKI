# 2025-2026-4GP-JANKOWSKI-SOLE

## *Low-tech graphite-based tactile sensor*
### Content
•[Contact the developers](https://github.com/MOSH-Insa-Toulouse/2025-2026-4GP-SOLE-JANKOWSKI/edit/main/README.md#contact-the-developers)

•[Context](https://github.com/MOSH-Insa-Toulouse/2025-2026-4GP-SOLE-JANKOWSKI/edit/main/README.md#context)

•[Deliverables](https://github.com/MOSH-Insa-Toulouse/2025-2026-4GP-SOLE-JANKOWSKI/edit/main/README.md#deliverables)

•

### Contact the developers
Wiktor JANKOWSKI : jankowski@insa-toulouse.fr   
Joan SOLÉ : sole-lohan@insa-toulouse.fr  

### Introduction  
This aim of this project is to study a low-tech graphite-based tactile sensor. This work is inspired by the "[Pencil Drawn Strain Gauges and Chemiresistors on Paper](https://www.nature.com/articles/srep03812)" by Cheng-Wei Lin, Zhibo Zhao, Jaemyung Kim and Jiaxing Huang.

The research states that graphite pencil traces drawn on print papers have the opportunity to act as strain gauges. As the strain is applied, the graphite structure changes, which in turn modifies the electrical resistance in the material. 

A PCB was developed from scratch in order to study the graphite-based sensor which was then controlled via an Arduino UNO. The main purpose of the board is to amplifie the very weak current flowing through the graphite-based sensor. Furthermore, a digital potentiometer was integrated into the circuit, allowing fine tuning of the measurement setup. In addition, a commercial flex sensor was added, whose resistance varies with the angle, providing a comparative reference for our low-tech counterpart. For practicality reasons, a Bluetooth component will serve as a medium of sharing the data collected throughout the usage, as well as an OLED screen that will display (...) 

This project encompasses the full development cycle, including electronic simulations in LTSpice, PCB design with KiCad, Arduino programming, application development using MIT App Inventor, and the creation of a technical datasheet.

### Deliverables 
  • A PCB shield connected to an Arduino UNO, which includes : an amplifier circuit, a graphite-based sensor, an OLED screen, a Bluetooth module, a commercial flex sensor and a rotary encoder.
  • An Arduino code which controls the PCB.
  • A mobile application which collects data from the Arduino.
  • A datasheet detailing the characteristics of the graphite-based sensor.
  • A bench of test for the writing of the datasheet.   

### Components used for the project
Materials used in this project: 
  • Arduino Uno
  • Bluetooth Module type *HC-05* 
  • Capacitors
  • Digital Potentiometer type *MCP41050* 
  • Flex sensor
  • Graphite-based paper sensor
  • OLED screen
  • Operational Amplifier type *LTC1050* 
  • Resistances
  • Rotary Encoder type *KY-040*






