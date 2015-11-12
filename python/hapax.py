from os import listdir
from os.path import isfile, join
from sys import argv

def hapax(words):
  res={}
  
  for word in words:
    if word in res:
      res[word] = False
    else: 
      res[word] = True

  return [k for k,v in res.iteritems() if v]

pt = argv[1]
for f in listdir(pt):
  if isfile(join(pt, f)):
    words=hapax([e for line in [l.replace('\n', '').lower() for l in open(join(pt, f)).readlines() if l != '\n'] for e in line.split(' ')])
    print f + ';'+' '.join(words)
    
#Format resultats pour evaluation:
# nomFichierFr [tab] X [tab] nomFichierEn [tab] Y [tab] Score d'appariement [tab] nomRun
# Proposer plusieurs appariement pour un meme fichier (TOP 10)

