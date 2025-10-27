# Line Drawing and Clipping Visualization App

This project is a simple OpenGL-based graphical application written in C for visualizing foundational computer graphics algorithms. It demonstrates Bresenham's line drawing, thick lines, concentric circles with gradients, and the Liang-Barsky line clipping algorithm.

## Features

- **Task 1a: Standard Line Drawing**
  - Implements Bresenham's algorithm for drawing lines between two points.

- **Task 1b: Thick Line Drawing**
  - Draws thick/bold lines using parallel offset lines and 8-way symmetry.

- **Task 2: Concentric Circles with Gradient**
  - Uses the Midpoint Circle Algorithm to render concentric circles with a color gradient from red to blue.

- **Task 3: Liang-Barsky Line Clipping**
  - Demonstrates the Liang-Barsky algorithm for clipping lines against a rectangular window.
  - Displays both unclipped (red) and clipped (green) line segments.

## Controls

- **Right-click**: Open menu to switch between tasks/visualizations.
- **`+` / `-` keys**: Increase or decrease line thickness (only in Thick Line mode).
- **`Q`**: Quit the application.

## Building and Running

### Prerequisites

- OpenGL and GLUT libraries.
- C compiler such as `gcc`.

#### On Linux

Install dependencies:
```sh
sudo apt-get install freeglut3-dev build-essential
```

Build:
```sh
gcc line_drawing_app.c -o line_drawing_app -lGL -lGLU -lglut -lm
```

Run:
```sh
./line_drawing_app
```

#### On macOS

Install Xcode Command Line Tools and GLUT (if needed):
```sh
xcode-select --install
```

Build:
```sh
gcc line_drawing_app.c -o line_drawing_app -framework OpenGL -framework GLUT -lm
```

Run:
```sh
./line_drawing_app
```

## File Overview

- **line_drawing_app.c**: Main application source code implementing all graphics algorithms and user interface.

## Algorithms Used

- **Bresenham's Line Algorithm**: Efficient rasterization of lines.
- **Midpoint Circle Algorithm**: Efficient circle rasterization.
- **Liang-Barsky Clipping**: Fast line clipping against an axis-aligned rectangle.

## License

This project is provided for academic and learning purposes.

---

**CSE 426 Lab Assignment**  
Author: Samiul Bashir
