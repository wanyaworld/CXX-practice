f = open("mbox.txt")
cnt = 0
for line in f.readlines():
    if line.startswith('From'):
        print(line.split()[1])
        cnt += 1
print ("there was {} people.".format(cnt))
