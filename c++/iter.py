#!/usr/bin/env python
# -*- encoding: utf-8 -*-


#
# param:
#   - file appax fr
#   - file appax en
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



fileAppaxFr=sys.argv[1]
fileAppaxEn=sys.argv[2]
nbThread=sys.argv[3]

dir='save/iterate'

stop = False
ite = 1
print 'BEGIN...'
call('rm -fr ' + dir, shell=True)
call('mkdir ' + dir, shell=True)
call('mkdir ' + dir + '/saves/', shell=True)

out_fr = []
out_en = []

while not stop:
    print '...Ite '+str(ite)+'...'

    dirIte=dir+'/saves/'+str(ite)

    call('mkdir '+dirIte, shell=True)

    add = False
    print '	...Purge & InverseIndex...'
    # fr
    purge(fileAppaxFr, dir+'/appax_fr.txt', out_fr)
    call('./indexinverse '+dir+'/appax_fr.txt '+dir+'/inverse_appax_fr' , shell=True)
    # en
    purge(fileAppaxEn, dir+'/appax_en.txt', out_en)
    call('./indexinverse '+dir+'/appax_en.txt '+dir+'/inverse_appax_en', shell=True)

    print '	...Appariements En vers Fr...'
    #appariements
    call('./ranking '+dir+'/inverse_appax_fr.txt '+dir+'/appax_en.txt ' + nbThread, shell=True)
    call('mv save/appariements_fr-en.txt '+dirIte+'/appariements_en_vers_fr_top10.txt', shell=True)
    call('python ../convertFormat.py '+dirIte+'/appariements_en_vers_fr_top10.txt Run'+str(ite)+' 1 > '+dirIte+'/appariements_en_vers_fr_top1.format', shell=True)

    print '	...Appariements Fr vers En...'
    call('./ranking '+dir+'/inverse_appax_en.txt '+dir+'/appax_fr.txt ' + nbThread, shell=True)
    call('mv save/appariements_fr-en.txt '+dirIte+'/appariements_fr_vers_en_top10.txt',shell=True)
    call('python ../convertFormat.py '+dirIte+'/appariements_fr_vers_en_top10.txt Run'+str(ite)+' 1 inverse > '+dirIte+'/appariements_fr_vers_en_top1.format', shell=True)


    print '	...Intersect...'
    #intersect
    call('../intersect.sh '+dirIte+'/appariements_en_vers_fr_top1.format '+dirIte+'/appariements_fr_vers_en_top1.format > '+dirIte+'/intersect_top1.format', shell=True)

    print '	...ParseResults...'
    # parse results
    with codecs.open(dirIte+'/intersect_top1.format', 'r', 'utf-8') as fr:
        for l in fr.readlines():
            tab = l.split()
            if len(tab) == 6:
                out_fr.append(tab[0])
                out_en.append(tab[2])
                add = True
    if not add:
        stop = True

    ite += 1

print 'END...'

