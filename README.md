## NOPE sign design files  

**All files are (c) 2025, Gary Marsh (gmarsh @ github)
Released under a CC BY-SA V4.0 license.**

All files here are old, poorly organized and unsupported. Use at your own risk :)

You'll need the following tools:
 
- Microcontroller code:
-- STM32CubeIDE for editing/compiling the source code
-- STM32CubeProg for programming the sign over USB DFU
-- Both of the above are available free from ST Microelectronics (http://www.st.com)
- Schematic and PCB design: Kicad (http://www.kicad.org)
- 3D design: OpenSCAD (http://openscad.org)

Directory contents:
- /3d_design/ = STL files for sign
- /3d_design/openscad = OpenSCAD source code for generating the STL files. The code also generates a few SVG files (board outline, etc) to be used in Kicad.
- /kicad/ = Kicad project for the sign electronics
- /kicad/gerbers/ = Gerber files ready to order
- /kicad/production/ = JLCPCB files needed to do a production run. Not all parts are guaranteed to be in stock.
- /software/ = Source code for the STM32 microcontroller.
