import sys

if __name__ == "__main__":
    if (len(sys.argv) != 2):
        print("Invalid arguments, {}".format(sys.argv[1:]))
        quit()

    with open(sys.argv[1],"r") as f:
        source = f.readlines()

    opcodes = {
            "ifmv":0,
            "read":1,
            "put":2,
            "add":3,
            "mul":4,
            "div":5,
            "nand":6,
            "halt":7,
            "allc":8,
            "clr":9,
            "out":10,
            "in":11,
            "load":12,
            "set":13
        }

    registers = {"rega":0,"regb":1,"regc":2,"regd":3,"rege":4,"regf":5,"regg":6,"regh":7}
    program = []

    for line in source:
        instruction = [x for x in line.strip().split(" ") if not x == ""]
        opcode = instruction[0]
        if opcode in ("load", "allc"):
            program.append(opcodes[opcode] << 4)
            b, c = instruction[1:3]
            program += [0,0]
            program.append(registers[b] << 3 | registers[c])
        elif opcode in ("clra", "out", "in"):
            program.append(opcodes[opcode] << 4)
            c = instruction[1]
            program += [0,0]
            program.append(registers[c])
        elif opcode == "halt":
            program.append(opcodes[opcode] << 4)
            program += [0,0,0]
        elif opcode == "set":
            a, val = instruction[1:3]
            if val.isnumeric():
                program.append(opcodes[opcode] << 4 | registers[a] << 1 | 0b1 & (int(val) >> 24))
                program.append(0xFF & (int(val) >> 16))
                program.append(0xFF & (int(val) >> 8))
                program.append(0xFF & int(val))
            else:
                print("non-numeric")
                quit()
        elif opcode in opcodes: # The rest
            program.append(opcodes[opcode] << 4)
            a, b, c = instruction[1:4]
            program.append(0)
            program.append(registers[a]>>2)
            program.append(0xff & (registers[a] << 6 | registers[b] << 3 | registers[c]))

with open("debug/test.umz", "wb") as f:
    f.write(bytes(bytearray(program)))