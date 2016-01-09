# Identification d'articles parallèles sur Wikipedia

## Pour créer les fichiers d'hapax :

Nécessite de créer un dossier c++/save

```
c++/appax DOSSIER_CONTENANT_LES_FICHIERS
c++/inverseindex FICHER_D_HAPAX FICHIER_RESULTAT
```

Le script appax va créer le fichier d'appax dans le dossier c++/save

## Pour créer les appariements :

Pour le sens fr vers en:

```
c++/ranking FICHIER_INVERSE_EN FICHIER_HAPAX_FR NB_THREAD
```

Pour le sens en vers fr :

```
c++/ranking FICHIER_INVERSE_FR FICHIER_HAPAX_EN NB_THREAD
```

Le nombre de thread permet de paralléliser la tâche.

## Pour analyser :

Créer les dossiers c++/save/res et c++/save/score

### Convertir au format pour trec_eval (le top permet de proposer plusieurs appariements pour un même fichier, maximum 10)

Pour le sens fr vers en :

`̀̀`̀ 
python convertFormat.py FICHIER_APPARIEMENTS NOM_RUN TOP inverse > c++/save/res/appariements_fr_vers_en_top[TOP].format
```

Pour le sens en vers fr :

```
python convertFormat.py FICHIER_APPARIEMENTS NOM_RUN TOP > c++/save/res/appariements_en_vers_fr_top[TOP].format
```

### Evaluer

Les résultats complets des évaluations sont stockés dans c++/save/score

Nécessite de placer le dossier trec_eval à la racine du projet

``̀ 
./eval.sh 
./affiche.sh
``̀

## Pour faire l'intersection

```
./intersect.sh FICHIER1 FICHIER2
```

## Pour faire l'itération

```
python c++/iter.py FICHIER_HAPAX_FR FICHIER_HAPAX_EN NB_THREAD
```
