# ICFP Programming Contest
## Physical Specs
### Platter
An unsigned 32-bit number, uint.
### Registers
8 general purpose registers, uint
Identified by a 3-bit id
### Arrays
Collection of arrays with a 32-bit id, int
*0* is reserved for **program**
### IO
Ascii-encoding
input output of chars.
## Behaviour
### Initialization
The program is read from a file. See **Reading programs**. All registers are set
to 0. PP = 0.
### Reading programs
Programs are encoded as bytes. A series of four bytes, A,B,C,D should be
interpreted as 0bABCD => 32 uint.
### Operator
The four most significant bits are opcodes. There are three registers specified,
A (the first three lsb), B (fourth to sixth lsb) and C (seventh to ninth lsb)
where lsb is Least-Significant-Byte. Before the operator is executed, the program
pointer should be advanced.
#### opcodes
* 0x0 - Conditional move (A = B if C) =>
* 0x1 - Array index (A = C[B]) =>
* 0x2 - Array Amendment (A[B] = C) =>
* 0x3 - Addition A = (B + C) % 2^32
* 0x4 - Multiplication A = (B*C) % 2^32
* 0x5 - Division A=B/C
* 0x6 - Not-And A=!(B & C)
* 0x7 - Halt program
* 0x8 - Allocation (Creates array with size C and with id = B, where B != 0 and is unique)
* 0x9 - Abandonment (Kills the array with address C)
* 0xA - Output (output C if 0 <= C <=255)
* 0xB - Input (C = input if input <= input <= 255, 0xFFFFFFFF if EOF is reached)
* 0xC - Load program (*program* = B.copy(), PP = C).
Notes on the load program. As indicated by the instructions, usually B is the
*program* array and if B = 0 then *program* should not be copied and rather the
program pointer should be set to the value of C.
#### Special opcode, Orthography 0xD
opcode is still retrieved from the same location as before, but the A register
is identified by the 26th to 28th lsb. The remaining 25-bites are immediately
loaded into the register specified by A.
### Failures
* If the opcode is invalid and/or a value stored at a register is not valid in the
context of an operation
* If an array is referenced but does not exist and/or if the supplied offset is
out of bounds of the allocated array. Includes attempt to load a program.
* If attempting to abandon the *program* array.
* Division by zero
* Outputting a value larger than 255
* If the execution finger aims outside of the capacity of the *program* array.


# UM Architecture

Consist of a main function that will handle initialising the UM, interpret each cycle and solve IO. Will be responsible for cleaning up the program. 

Main loop has the following parameters

UMMainLoop()
