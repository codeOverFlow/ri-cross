#!/bin/bash

for f in appariements_en_vers_fr_top1.score appariements_en_vers_fr_top5.score appariements_en_vers_fr_top10.score appariements_fr_vers_en_top1.score appariements_fr_vers_en_top5.score appariements_fr_vers_en_top10.score
do
	scores=`cat c++/save/baseline/score/$f`
	echo $f

	ret=`echo "$scores" | grep 'num_ret' | cut -f 3`
	rel=`echo "$scores" | grep 'num_rel\b' | cut -f 3`
	rel_ret=`echo "$scores" | grep 'num_rel_ret' | cut -f 3`

	echo $ret

	echo "$scores" | grep 'num_ret'
	echo "$scores" | grep 'num_rel'
	echo "$scores" | grep '^map'

	pre=`echo "($rel_ret)/($ret)" | bc -l`
	rec=`echo "($rel_ret)/($rel)" | bc -l`
	echo "precision	$pre"
	echo "recall		$rec"

	echo
done
