f = open("romeo.txt")
dic = {}
for lines in f.readlines():
    words = lines.split()
    for w in words:
        dic[w] = 1
print(sorted(dic))
