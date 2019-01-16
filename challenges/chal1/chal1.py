def count(string, arr, length):
    longest = 0
    if len(string) == 0:
        return length
    else:
        if string[0] not in arr:
            length += 1
            arr.append(string[0])
        else:
            return length
        return count(string[1:], arr, length)


string = input()
length = 0
for i in range(len(string)):
    arr = []
    tmp = count(string[i:], arr, 0)
    if length < tmp:
        length = tmp
print(length)
