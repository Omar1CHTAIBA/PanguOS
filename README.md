# PanguOS

A lightweight, 32-bit graphical operating system kernel built from scratch in C and Assembly. it features a custom Login screen, Welcoming animation, GUI window manager, Start menu, native PS/2 mouse/keyboard drivers, a command-line terminal with an integrated math parser, and standalone retro games.
## Features

* **Custom Graphical User Interface (GUI):** A 320x200 VGA graphical desktop environment with draggable windows. The UI is deliberately streamlined—specifically omitting minimize/close buttons on certain static windows—to reduce overhead and save CPU cycles.
* **Hardware Drivers:** Custom-written PS/2 keyboard and mouse drivers, featuring real-time mouse packet parsing and shift-state tracking with dual keyboard maps.
* **PanguOS Terminal:** A functional, double-buffered command-line interface featuring:
  * Persistent command history and screen scrolling.
  * A recursive descent math parser for real-time mathematical calculations (`CALCULATE <expr>`).
  * On-the-fly account management commands (`CHANGE --P TO <pass>`).
* **Integrated Applications:**
  * **Snake:** A fully playable classic snake implementation with collision detection and dynamic memory handling for growth.
  * **Tic-Tac-Toe:** A mouse-driven 2-player logic game with automated win-state detection.
* **Security & Boot:** A user authentication gate required upon boot, followed by a custom, timed boot animation sequence.

## Architecture & Technical Details

* **Language:** C (Kernel logic) and x86 Assembly (Bootloader/Interrupts).
* **Display:** VGA Mode 13h (320x200, 256 colors) utilizing direct memory access (`0xA0000`).
* **Interrupt Handling:** A custom Interrupt Descriptor Table (IDT) maps and handles hardware timers, keyboard interrupts, and mouse packets natively.
* **Graphics Rendering:** Implements double-buffering (`0x50000` to `0xA0000`) to eliminate screen tearing during window movement and state redraws.
* **Typography & Assets:** Employs a custom 8x8 hexadecimal font rendering engine alongside static 16x16 pixel-art bitmap arrays for system icons.

## Usage & Controls

- **Boot:** Enter your chosen Username and Password to register the session.
- **Navigation:** Use the mouse to drag windows via their title bars. The Start Menu located at the bottom left provides system power options and quick access to built-in apps.
- **Terminal:** Open via the Start Menu or desktop icon. Use standard keyboard input to execute registered commands.

## Author

**Omar Chtaiba** *Independent Developer & Researcher*
