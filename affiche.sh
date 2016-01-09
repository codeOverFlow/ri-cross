#!/bin/bash

for f in appariements_en_vers_fr_top1.score appariements_en_vers_fr_top5.score appariements_en_vers_fr_top10.score appariements_fr_vers_en_top1.score appariements_fr_vers_en_top5.score appariements_fr_vers_en_top10.score appariements_intersect_top1.score
do
	scores=`cat c++/save/score/$f`
	echo $f

	echo "$scores" | grep 'num_ret'
	echo "$scores" | grep 'num_rel'

	echo
done
