from os import listdir
from os.path import isfile, join
from sys import argv
import cPickle as pickle

#Usage : python ranking.py path/to/hapaxFr path/to/hapaxEn
#Format hapax : 1 fichier par ligne, nomFile;token token token...

fr_file = file(argv[1])
en_file = file(argv[2])

fr = {}
en = {}
appariements = {}
inverse = {}

print "Fr"
for w in fr_file.readlines():
  s=w.split(';')
  doc = s[0]
  words = ' '.join(s[1:])
  fr[doc] = words.split(' ')
  appariements[doc] = {}


print "En"
for w in en_file.readlines():
  s=w.split(';')
  doc = s[0]
  words = ' '.join(s[1:])
  en[doc] = words.split(' ')
  
  for k,v in fr.iteritems():
    score = len([a for a in en[doc] if a in fr[k]])
    appariements[k][doc] = score
        
    if score not in inverse:
      inverse[score] = [(k, doc)]
    else:
      inverse[score].append((k, doc))

print appariements
print inverse

for k in inverse.keys():
  inverse[k] = sorted(inverse[k])
      
pickle.dump(appariements, file('save/appariements_fr-en','w'))
pickle.dump(inverse, file('save/inverse_fr-en', 'w'))


    
#Format resultats pour evaluation:
# nomFichierFr [tab] X [tab] nomFichierEn [tab] Y [tab] Score d'appariement [tab] nomRun
# Proposer plusieurs appariement pour un meme fichier (TOP 10)

