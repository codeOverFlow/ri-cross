#!/usr/bin/env python
# -*- encoding: utf-8 -*-


#
# param:
#   - file appax fr
#   - new file appax fr
#   - file appax en
#   - new file appax en
#   - nb threads

import codecs
import sys
from subprocess import call


def purge(fr, fw, out):
    with codecs.open(fr, 'r', 'utf-8') as frr:
        with codecs.open(fw, 'w', 'utf-8') as frw:
            tab = [x for x in frr.readlines() if x.split(';')[0] not in out]
            tab.remove(tab[0])
            frw.write(str(len(tab))+'\n')
            for l in tab:
                frw.write(l)


stop = False
ite = 1
print 'BEGIN...'
call('rm -fr save/saves/', shell=True)
call('mkdir save/saves', shell=True)
out_fr = []
out_en = []
while not stop:
    print '...',
    add = False
    # fr
    purge(sys.argv[1], sys.argv[2], out_fr)
    call('./indexinverse ' + sys.argv[2], shell=True)
    # en
    purge(sys.argv[3], sys.argv[4], out_en)
    call('./ranking save/inverse_appax_per_file_fr.txt ' + sys.argv[4] + ' ' + sys.argv[5], shell=True)
    # parse results
    with codecs.open('save/appariements_fr-en.txt', 'r', 'utf-8') as fr:
        for l in fr.readlines():
            tab = l.split()
            if len(tab) == 3:
                out_fr.append(tab[0])
                out_fr.append(tab[1])
                add = True
    if not add:
        stop = True
    else:
        call('cp save/appariements_fr-en.txt save/saves/appar_ite' + str(ite) + '.txt', shell=True)

    ite += 1

print 'END...'
    
