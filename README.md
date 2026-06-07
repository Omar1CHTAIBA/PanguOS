# PanguOS

**A 32-Bit Bare-Metal Graphical Operating System**

PanguOS is a lightweight monolithic operating system written entirely from scratch in C and x86 Assembly. It runs directly on bare-metal hardware without relying on an existing operating system, standard libraries, or external runtime environments.

The project focuses on low-level systems programming, graphical user interface design, hardware interrupt management, and custom kernel-side application development within a highly constrained environment.

---

## Overview

PanguOS provides a complete graphical desktop environment featuring:

- User authentication system
- Animated boot experience
- Graphical desktop and window manager
- Integrated terminal emulator
- Built-in calculator parser
- Retro games (Snake and Tic-Tac-Toe)
- PS/2 keyboard and mouse support
- VGA graphics with double buffering
- Event-driven kernel architecture

All components operate directly inside the kernel without external dependencies.

---

## Demonstration

---

# Features

## Authentication System

The operating system starts with a dedicated login screen that controls access to the desktop environment.

### Capabilities

- Username and password input fields
- Password masking
- Keyboard navigation between fields
- Direct kernel-level credential validation
- Focus-aware input handling

The authentication process is implemented entirely within kernel space without relying on standard library functions.

---

## Welcome Animation

After successful authentication, PanguOS displays a startup animation before entering the desktop.

### Highlights

- Driven by the Programmable Interval Timer (PIT)
- Non-blocking execution
- Dynamic text centering
- Smooth state transitions
- Interrupt-controlled timing

The animation runs independently of the rendering loop, ensuring responsive system behavior.

---

## Desktop Environment

PanguOS includes a graphical desktop environment designed specifically for low-resource systems.

### Desktop Features

- Windowed applications
- Mouse-driven interaction
- Draggable windows
- Start menu
- Desktop icons
- Layered rendering system
- Real-time cursor tracking

### Window Manager

Each window is managed through an internal structure that stores:

- Position
- Size
- Visibility state
- Drag status
- Window capabilities

The rendering pipeline draws windows sequentially to simulate depth ordering.

---

## Start Menu

The start menu acts as the primary application launcher.

### Functions

- Open system applications
- Launch built-in games
- Restart session
- Shutdown system

### System Control

#### Restart

Resets kernel state variables and returns execution to the login environment.

#### Shutdown

Uses direct ACPI power management instructions to power off the machine.

---

## Terminal

The integrated terminal provides command-line interaction directly within the graphical environment.

### Features

- Text scrolling
- Command processing
- Screen clearing
- User management commands
- System information commands

### Built-in Calculator

PanguOS includes a recursive-descent mathematical parser capable of evaluating expressions with:

- Addition
- Subtraction
- Multiplication
- Division
- Parentheses
- Operator precedence

Example:

```text
CALCULATE (5 + 3) * 2
```

Output:

```text
16
```

---

## Built-in Games

### Snake

Classic Snake implementation featuring:

- Keyboard controls
- Food spawning
- Collision detection
- Score progression
- Timer-based game loop

### Tic-Tac-Toe

Graphical Tic-Tac-Toe implementation featuring:

- Mouse interaction
- Board state validation
- Win detection
- Turn management

---

# System Architecture

```text
+-------------------------------------------------------------+
|                       PanguOS GUI                           |
|       Login Screen -> Welcome Animation -> Desktop          |
+-------------------------------------------------------------+
|    Window Manager     |   Terminal   |     Retro Games      |
|  (GUI Subsystem)      | (Parser)     |   (Snake/Tic-Tac)    |
+-------------------------------------------------------------+
|             Monolithic Kernel Core                          |
|  Double Buffering | Font Renderer | Command Router          |
+-------------------------------------------------------------+
|            Hardware Abstraction Layer                       |
|     IDT / ISRs | PIT Timer | PS/2 Keyboard & Mouse          |
+-------------------------------------------------------------+
|                     x86 Hardware                            |
+-------------------------------------------------------------+
```

---

# Technical Details

## Event-Driven Execution

The kernel uses the x86 `hlt` instruction while idle.

### Benefits

- Near-zero idle CPU usage
- Lower power consumption
- Reduced heat generation
- Interrupt-driven wake-up

---

## Double Buffering

Rendering occurs in an off-screen buffer before being copied to VGA memory.

### Buffer Layout

| Address | Purpose |
|----------|----------|
| `0x50000` | Back Buffer |
| `0xA0000` | VGA Frame Buffer |

### Benefits

- Eliminates visible tearing
- Produces smoother graphics
- Reduces rendering artifacts

---

## Font Rendering

PanguOS uses a custom 8×8 bitmap font.

### Features

- Direct pixel rendering
- Bitwise glyph extraction
- Fast memory writes
- Minimal memory overhead

---

## Keyboard and Mouse Support

Input devices communicate directly through PS/2 hardware interrupts.

### Keyboard

- Scan code processing
- Shift-state tracking
- Character translation
- Real-time input handling

### Mouse

- Packet decoding
- Cursor movement
- Click detection
- Window interaction

---

# Memory Layout

```text
0x00000000  Interrupt Vector Table (IVT)
0x00000500  BIOS Data Area
0x00050000  PanguOS Back Buffer
0x000A0000  VGA Frame Buffer
```

---

# Commands

| Command | Description |
|----------|-------------|
| `CLS` | Clear terminal |
| `CALCULATE <expression>` | Evaluate a mathematical expression |
| `CHANGE --P TO <password>` | Change password |
| `PANGU CREATOR` | Display project author information |

---

# Technologies

- C
- x86 Assembly
- VGA Mode 13h Graphics
- PS/2 Hardware Interface
- PIT Timer
- Interrupt Descriptor Table (IDT)
- Interrupt Service Routines (ISRs)

---

# Project Goals

PanguOS was created as a learning and research project focused on:

- Operating system development
- Kernel architecture
- Computer graphics
- Hardware communication
- Low-level memory management
- Bare-metal programming

The project serves as a practical exploration of how operating systems are built from the hardware level upward.

---

# Author

**Omar Chtaiba**

A Passionate Independent Systems Developer

### Areas of Interest

- Low-Level Programming
- Computer Architecture
- Computability Theory
- Quantum Computing
- Theoretical Physics

---

# License

This project is licensed under the MIT License.

Copyright (c) 2026 Omar Chtaiba

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.


This project is currently distributed for educational and research purposes.
