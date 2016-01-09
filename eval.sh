#!/bin/bash

for f in appariements_en_vers_fr_top1.format appariements_en_vers_fr_top5.format appariements_en_vers_fr_top10.format appariements_fr_vers_en_top1.format appariements_fr_vers_en_top5.format appariements_fr_vers_en_top10.format appariements_intersect_top1.format
do
	e=`echo $f | sed "s/.format/.score/"`
	trec_eval/trec_eval data/fr-en-train.qrels c++/save/res/$f >  c++/save/score/$e
done
