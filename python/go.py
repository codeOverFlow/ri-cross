from sys import argv
import cPickle as pickle

#Usage : python go.py path/to/appariements path/to/inverse_index

appariements = pickle.load(file(argv[1], 'r'))
inverse = pickle.load(file(argv[2], 'r'))

results={}

for k in sorted(inverse.keys(), reverse=True):
  for (f,e) in inverse[k]:
    results[f] = e
    for a,b in inverse.iteritems():
      inverse[a] = [c for c in b if e not in c and f not in c]
      
    print e + '\tX\t' + f + '\tY\t1.0\tA'
    
    
#Compte-rendu
# lundi 7 décembre 2015
# 2 pages (11pt, times)
# décrire la méthode
#   Résultats
# améliorations
#   Apariement 1-1 -> Résultats
