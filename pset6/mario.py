height = 0;
print("height:", end="")
while(True):
    height = int(input())
    if height >= 0 and height <= 23:
        break
    else:
        print("retry")

for i in range (height):
    for j in range (height-1-i):
        print(" ", end="")
    for j in range (i+1):
        print("#", end="")
    print("  ", end="")  #middle space
    for j in range(i+1):
        print("#", end="")
    print()
    