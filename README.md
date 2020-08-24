# emuW65C02S
`emuW65C02S` is a simple emulator and text user interface (TUI) viewer for the W65C02S microprocessor manufactured by The Western Design Center, Inc. The data sheet can be found here: https://eater.net/datasheets/w65c02s.pdf. Please refer to it for more information.

## Dependancies
The only external library used is `ncurses`, for the TUI.

## Usage
The emulator can be built with CMake. Once built, it can be run via
`./emuW65C02S rom.bin`,
where `rom.bin` is the raw binary file which contains the program you want the emulator to run.

## Design
The emulator is made up of two classes: `W65C02S` and `Monitor`. As the name suggest, the former is a full implementation of the W65C02S, containing all the data and functions needed to emulate the microprocessor, while the latter is a terminal-based viewer which displays the contents of RAM, the stack, registers, flags. The real W65C02S can access 65536 bytes of memory via a 16-bit address bus. These lines are usually connected to various EEPROMS chips, RAM modules, I/O units, etc. `emuW65C02S` 'connects' these 'lines' to a RAM module represented by a 65536 byte C-array.

## File structure
### main.c
In `main.c`, the emulator loads the 65536 byte C-array with the contents of the provided binary file. An instance of class `W65C02S` takes in a pointer to the start of this array to use it for reading instructions, storing data, etc. An instance of class `Monitor` takes in this instance of `W65C02S`, as well as the same pointer, to display it. Once set up, the `W65C02S` instance starts up its `run()` function, which is an infinite fetch-decode-execute loop. Simultaneously, in another thread, the `Monitor` instance starts up its `start()` function, a similar infinite loop which constantly reads from the RAM, registers, etc. and prints them to screen. These loops will continue forever unless the user quits or an error occurs.

### W65C02S
The files `W65C02S.h`, `W65C02S.cpp`, `decoder.cpp`, and `executors.cpp` fully implement the W65C02S class.
 - `W65C02S.h` defines the microprocessor in full, including all it's registers, flags, etc. For convinence's sake, each flag is represented seperately as a boolean, unlike the real W65C02S.
 - `W65C02S.cpp` most importantly contains the implementation of the fetch-decode-execute cycle in `run()`. It also has some other helper functions.
 - `decoder.cpp` contains a single C++ map mapping each of the W65C02S's 212 1-byte opcodes to a pointer to the function which implements its instruction. There are 70 such instructions (LDA, ADC, STA, etc.), and thus, 70 such functions. Since these functions execute the instruction, they are called 'executors'.
 - `executors.cpp`, as the name suggests, implements all the executor functions. It also implements functions for the W65C02S's 16 addressing modes (see datasheet).
 
 ### Monitor
 The files `Monitor.h` and `Monitor.cpp` fully implement the Monitor class.
  - `Monitor.h` declares the TUI's many windows and their dimensions and positions. Also of note is the C++ thread which runs the Monitor's main loop.
  - `Monitor.cpp` implements the Monitor's main loop in `start()`. This loop repetedly calls separate helper functions for each window which read the concerned register/memory location, format it properly, and print it to the screen in the right location.
