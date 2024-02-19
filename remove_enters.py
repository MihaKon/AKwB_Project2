

def get_file_content(file_name, qual=False):
    edit = False
    result = []

    with open(file_name, "r") as file:
        for line in file:
            if line.startswith(">"):
                edit = True
                result.append(line)
            elif edit:
                if qual:
                    result.append(line.replace("\n", " "))
                else:
                    result.append(line.replace("\n", ""))
                result[-1] += line
                edit = False
    
    return result
            

def write_file_content(file_name, content):
    with open(file_name, "w") as file:
        for line in content:
            file.write(line)

def main():
    file_name = "AKwB_Project3/sample.fasta"
    file_content = get_file_content(file_name)
    write_file_content("AKwB_Project3/fasta.txt", file_content)

    file_name = "AKwB_Project3/sample.qual"
    file_content = get_file_content(file_name, True)
    write_file_content("AKwB_Project3/qual.txt", file_content)

if __name__ == "__main__":
    main()