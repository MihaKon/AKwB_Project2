import random

def write_list(all_sums, filename):
    with open(filename, "w") as file:
        for sum in all_sums:
            file.write(f"{sum} ")

def generate_map(n):
    result = []
    for _ in range(1, n+1):
        result.append(random.randint(1, 14))
    return result

def generate_all_sums(nums):
    all_sums = []
    for i in range(len(nums)):
        for j in range(i, len(nums)):
            all_sums.append(sum(nums[i:j+1]))

    return all_sums


def main():
    for i in range(5,15,3):
        print(f"Generating instance for n = {i}")
        map = generate_map(i)
        all_sums = generate_all_sums(map)
        for j in range(2):
            if(j == 0):
                all_sums = sorted(all_sums)
                write_list(all_sums, f"instance_{i}_asc.txt")
            if(j == 1):
                all_sums = sorted(all_sums, reverse=True)
                write_list(all_sums, f"instance_{i}_desc.txt")
        write_list(map, f"instance_{i}_answ.txt")


if __name__ == "__main__":
    main()  