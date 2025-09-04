import random


def generate_values_for_test():
    text = ""
    map_size = random.randint(1, 1000)
    planes = random.randint(1, 100)
    text += str(map_size) + "\n"
    text += str(planes) + "\n"
    for i in range(planes):
        text += str(random.uniform(0.000, map_size)) + " "
        text += str(random.uniform(0.000, map_size)) + " "
        text += str(random.uniform(0.000, map_size)) + " "
        text += str(random.uniform(0.000, map_size)) + "\n"
    return text


f = open("test/INPUT_1.txt", "w")
text = generate_values_for_test()
f.write(text)
f.close()

