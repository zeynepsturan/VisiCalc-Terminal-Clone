# VisiCalc-Terminal-Clone
![Build Status](https://github.com/profanity-im/profanity/workflows/CI/badge.svg)

A retro-inspired, Linux terminal-based clone of the legendary [VisiCalc spreadsheet](https://www.pcjs.org/software/pcx86/app/other/visicalc/1981), rebuilt in C++ with modern OOP design principles

<img width="1026" height="698" alt="visicalc-photo" src="https://github.com/user-attachments/assets/6e16b5b0-3add-4469-be81-19976bc83765" />

This project is a terminal-based clone of the original VisiCalc — the first-ever spreadsheet software.
Written in C++ with modern object-oriented principles and polymorphism, it supports cell formulas, functions (SUM, AVER, MAX, MIN, STDDEV), and absolute cell referencing.
Designed for retro computing fans, programming students, and anyone curious about how spreadsheet engines work under the hood.

## Features

Absolute cell referencing (e.g., A1)

Formula parsing (e.g., A1=SUMB1..B10)

Basic arithmetic operations: +, -, *, /

Function support: SUM, AVER, MAX, MIN, STDDEV

Terminal navigation with custom key bindings

Save/Load to/from .csv files functionality

## Installation

Open the Linux terminal in the project folder and run:
```
chmod +x run_visicalc.sh
```
Then run this:
```
./run_visicalc.sh
```

Now you can start the program any time using:
```
./visicalc
```
