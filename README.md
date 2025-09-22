# VisiCalc-Terminal-Clone
A retro-inspired, terminal-based clone of the legendary [VisiCalc spreadsheet](https://www.pcjs.org/software/pcx86/app/other/visicalc/1981), rebuilt in C++ with modern OOP design

This project is a terminal-based clone of the original VisiCalc — the first-ever spreadsheet software.
Written in C++ with modern object-oriented principles and polymorphism, it supports cell formulas, functions (SUM, AVER, MAX, MIN, STDDEV), and absolute cell referencing.
Designed for retro computing fans, programming students, and anyone curious about how spreadsheet engines work under the hood.

Features:

Formula parsing without parentheses (e.g., =SUM A1:A10 + B2)

Absolute cell referencing (e.g., $A$1)

Basic arithmetic operations: +, -, *, /

Function support: SUM, AVER, MAX, MIN, STDDEV

Terminal navigation with custom key bindings

OOP design with polymorphism for cell types

Future plans include relative addressing, dependency graph evaluation, and save/load functionality.
