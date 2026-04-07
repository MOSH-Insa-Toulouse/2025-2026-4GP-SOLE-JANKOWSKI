# 2025-2026-4GP-SOLE-JANKOWSKI

## *Low-tech graphite-based tactile sensor*
### Content list
•[Contact the developers](https://github.com/MOSH-Insa-Toulouse/2025-2026-4GP-SOLE-JANKOWSKI/edit/main/README.md#contact-the-developers)

•[Introduction](https://github.com/MOSH-Insa-Toulouse/2025-2026-4GP-SOLE-JANKOWSKI/edit/main/README.md#context)

•[Deliverables](https://github.com/MOSH-Insa-Toulouse/2025-2026-4GP-SOLE-JANKOWSKI/edit/main/README.md#deliverables)

•[Components used for the project](https://github.com/MOSH-Insa-Toulouse/2025-2026-4GP-SOLE-JANKOWSKI/edit/main/README.md#components-used-for-the-project)

•[I. LTSpice and Simulations](I. LTSpice and Simulations)
### Contact the developers
Wiktor JANKOWSKI : jankowski@insa-toulouse.fr   
Joan SOLÉ : sole-lohan@insa-toulouse.fr  

### Introduction  
The aim of this project is to study a low-tech graphite-based tactile sensor. This work is inspired by the "[Pencil Drawn Strain Gauges and Chemiresistors on Paper](https://www.nature.com/articles/srep03812)" by Cheng-Wei Lin, Zhibo Zhao, Jaemyung Kim and Jiaxing Huang.

The research states that graphite pencil traces left on paper can act as strain gauges. As the strain is applied, the graphite structure changes, which in turn modifies the electrical resistance in the material.

A PCB was developed from scratch in order to study the graphite-based sensor which was then controlled via an Arduino UNO. The main purpose of the board is to amplify the very weak current flowing through the graphite-based sensor. Furthermore, a digital potentiometer was integrated into the circuit, allowing fine tuning of the measurement setup. In addition, a commercial flex sensor was added, whose resistance varies with the angle, providing a comparative reference for our low-tech counterpart. For practical reasons, a Bluetooth component will serve as a medium of sharing the data collected throughout the usage, as well as an OLED screen that will display (...) 

This project encompasses the full development cycle, including electronic simulations in LTSpice, PCB design with KiCad, Arduino programming, application development using MIT App Inventor, and the creation of a technical datasheet.

### Deliverables 
  • A PCB shield connected to an Arduino UNO, which includes : an amplifier circuit, a graphite-based sensor, an OLED screen, a Bluetooth module, a commercial flex sensor and a rotary encoder.\
  • An Arduino code which controls the PCB.\
  • A mobile application which collects data from the Arduino.\
  • A datasheet detailing the characteristics of the graphite-based sensor.\
  • A test bench for the writing of the datasheet.

### Materials used in this project:
• Arduino Uno  
• Bluetooth module (HC-05)  
• Capacitors  
• Digital potentiometer (MCP41050)  
• Flex sensor  
• Graphite-based paper sensor  
• OLED screen  
• Operational amplifier (LTC1050)  
• Resistors  
• Rotary encoder (KY-040)  

### Circuit simulations on LTSpice
As the objective of our project is to study the graphite-sensor, the measurement of the current flowing thorugh it will be rather low (nA), with a variable resistence in the order of gigaohm (GΩ) for a potential of 5V from the Arduino. Hence, we have implemented a transimpedence amplifier circuit which will give us the  


