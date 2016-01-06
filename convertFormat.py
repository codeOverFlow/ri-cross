from sys import argv
import os

lines = [l.strip().split(' ') for l in open(argv[1]).readlines()]

top = int(argv[3])
assert(top <= 10 and top > 0)
isInverted = len(argv) > 4 and argv[4]=="inverse"

sortlines = {}

os.write(2,"trie\n")
for l in lines:
  if len(l) == 3 and l[2] != "0":
    if l[1] in sortlines:
       sortlines[l[1]].append(l)
    else:
       sortlines[l[1]] = [l]

os.write(2,"sort\n")
for k,s in sortlines.iteritems():
  sortlines[k] = sorted(s, key=lambda (a,b,c): int(c), reverse=True)
#sortlines = [sorted([l for l in lines if len(l) == 3 and l[0] == a], key=lambda (l1,l2,l3): l3) for a in set([l[0] for l in lines])]

os.write(2,"In file\n")
for k,l in sortlines.iteritems():
  for s in l[:top]:
    if len(s) == 3 and s[2] != "0":
      if isInverted:
        print s[1] + "	X	" + s[0] + "	Y	" + s[2] + "	" + argv[2]
      else:
	print s[0] + "	X	" + s[1] + "	Y	" + s[2] + "	"  + argv[2]
  

