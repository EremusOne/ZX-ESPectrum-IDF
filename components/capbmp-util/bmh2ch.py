# bmp header to c header
# writes bmp header at "bmp8.header" to stdout as c literal data
# using custom palette for SSBBGGRR (6 bit bitluni ESP32 VGA pixelformat)

f = open("bmp8.header", "rb")
bs = f.read()
f.close()

s = "unsigned char header[" + str(len(bs)+1024) + "] = {"

def myhex(n):
    s = hex(n)
    if len(s) == 3:
        s = '' + s[0] + s[1] + '0' + s[2]
    return s

count = 16

for b in bs:
    if count == 16:
        s += "\n    ";
        count = 0
    s += myhex(int(b)) + ","
    count += 1

pal = []
lumas = [0, 127, 191, 255]
for sync in [0,1,2,3]:
    for b in [0,1,2,3]:
        for g in [0,1,2,3]:
            for r in [0,1,2,3]:
                pal.append(lumas[b])
                pal.append(lumas[g])
                pal.append(lumas[r])
                pal.append(0)
                
for e in pal:
    if count == 16:
        s += "\n    ";
        count = 0
    #print(s, e, type(s), type(e))
    s += myhex(e) + ","
    count += 1    

if s[-1] == ',':
    s = s[0:-1]
s += "\n};"     

print(len(bs))
print(bs)
print()
print(s)

