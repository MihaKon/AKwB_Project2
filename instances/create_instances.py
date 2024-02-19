def read_fasta(file_name):
    result = {}
    with open(file_name, 'r') as f:
        for line in f:
            if line[0] == '>':
                name = line[1:15]
                result[name] = {}
            else:
                result[name]["seq"] = line.strip()
    return result

def read_quality(file_name, result):
    with open(file_name, 'r') as f:
        for line in f:
            if line[0] == '>':
                name = line[1:15]
            else:
                result[name]["qual"] = line.strip()
    return result

def create_instance(sequences, ind):
    instance = {}
    for i in range(5):
        index = ((ind+1)*10+(i+1))
        name = list(sequences.keys())[index]
        instance[name] = {}
        instance[name]["seq"] = sequences[name]["seq"]
        instance[name]["qual"] = sequences[name]["qual"]
    return instance

def write_instance(instance, ind):
    with open(f"instance_{ind}.txt", 'w') as f:
        for name in instance.keys():
            f.write(">" + name + "\n")
            for q in instance[name]["qual"].split(" "):
                if int(q) < 10:
                    f.write("0"+q+" ")
                else:
                    f.write(q+" ")
            f.write("\n")
            f.write(instance[name]["seq"] + "\n")

def main():
    fasta_file = "fasta.txt"
    sequences = read_fasta(fasta_file)
    quality_file = "qual.txt"
    sequences = read_quality(quality_file, sequences)
    for i in range(6):
        instance = create_instance(sequences, i)
        write_instance(instance, i)


if __name__ == "__main__":
    main()