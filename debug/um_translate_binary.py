fileName = input("Filepath: ")

with open(fileName, "r") as f:
    tmp = f.readlines()

with open("out/binaryOutput.txt","w") as f:
    for line in tmp:
        f.write("{}\n".format(str(bin(int(line.strip())))))
    