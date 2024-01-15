lines = []

with open("final_op", "r") as f:
    lines = f.readlines()

current = []
fin = []

for line in lines:
    if line == "\n":
        if (len(current)!=0):
            fin.append(current)
        current = []
    else:
        current.append(line)

idx = 0

for content in fin:
    with open("dot_files/final_op_"+str(idx) + ".dot", "w") as f:
        f.write("digraph G {\n")
        for line in content:
            line = line.replace("0x", "x")
            f.write(line)
        f.write("}")

    idx = idx + 1

