# GEI1064_Laboratoire_A

Synthèse haut niveau du filtre FIR-LMS en C

Introduction
L’algorithme LMS est un filtre adaptatif souvent utilisé en traitement du signal afin d’adresser le problème d’égalisation. Ce phénomène se produit lorsqu’un canal de communication introduit de la distorsion au signal qu’il transmet. L’algorithme tente de retrouver le signal transmis en diminuant l’erreur quadratique moyenne (mean square error) entre le signal désiré et le signal reçu. 

Le filtre est initialisé avec ses coefficient à 0. Pour chaque nouveau signal d’entrée, la sortie est calculée en multipliant les coefficients actuels aux signaux actuels et précédents. L’erreur est ensuite calculée entre la sortie et la sortie attendue afin d’Ajuster les coefficients. Le filtre LMS est peu complexe, le rendant propice à des applications en temps réel comme en télécommunication. Également, le filtre va souvent converger vers une solution dans le cas où le pas d’adaptation est paramétré correctement. Ce pas d’adaptation doit être choisi délicatement puisque s’il est trop grand, l’algorithme peut diverger alors que s’il est trop faible, la convergence sera longue.

Dans les systèmes de communication, l’égalisation est le processus qui permet de compenser pour la distorsion et de retrouver le signal original. Il existe plusieurs types d’égaliseur et le filtre LMS est souvent utilisé pour des filtres adaptatifs linéaires. Dans ce contexte, le filtre est utilisé pour s’ajuster en fonction du canal et estimer le signal reçu. 

Validation et comparaison avec Matlab
Pour le laboratoire A1, la validation du filtre s’est faite par le test bench dans Vivado HLS. Ce dernier lit les fichiers générés par Matlab et place les variables dans différents tableaux. Ensuite, une boucle for appelle la fonction FIR_LMS avec les valeurs précédemment lues une par une. Lorsque le nombre d’itérations atteint 19, le pas de convergence est changé de 0,52 à 0,22. Également, à la 200e itération, la variable isTraining est changée à false. Ceci permet de répliquer le comportement du filtre sur Matlab. Une fois que toutes les valeurs sont passées au filtre, le test bench vérifie les erreurs de calculs et affiche les résultats dans la console et dans un fichier de sortie afin de pouvoir comparer les résultats. Un message est ensuite affiché en fonction de l’erreur accumulée. La somme des erreurs absolues entre les sorties Matlab et le filtre est de 0.00052930 pour les 2000 échantillons, ce qui donne une erreur moyenne de 0,00000026.
Pour le laboratoire A2, le banc de test est très similaire à celui du laboratoire A1 à l’exception de l’utilisation des nouveaux types de variables et de la lecture des fichiers .txt en convertissant implicitement les valeurs en point fixe. Également, au lieu de comparer l’erreur accumulé le banc de test compare individuellement chaque sortie du filtre. On peut remarquer que l’erreur accumulée est bien plus grande dans ce cas puisqu’on utilise des variables à virgule fixe. Cependant, comme l’erreur entre chaque sortie et sortie attendue est moins de 0,01, aucune erreur n’est reportée. Ceci confirme le bon fonctionnement du filtre en virgule fixe. 

Dans le cadre du laboratoire A3, Les 10 premières valeurs des sorties Matlab ont été écrites dans les tableaux du test bench afin de procéder à la vérification RTL. Comme l’erreur entre les valeurs attendues et les sorties du filtre étaient plus petite que l’epsilon, on conclut que le filtre optimisé et synthétisé est bel et bien fonctionnel dans ce cas aussi. 



Analyse des performances et des ressources en virgule flottante vs fixe
Les différences en termes de ressources et de latence entre la synthèse en virgule fixe et en virgule flottante est bien présente. En effet, la latence a été réduite à moins de la moitié, passant de 54 – 96 coups d’horloge à 21 – 31 cycles d’horloge comme le démontre les figures suivantes (virgule flottante à gauche, virgule fixe à droite). 

  

Également, l’utilisation des ressources a grandement diminuée. On peut voir le nombre de DSP48 passer de 5 pour la virgule flottante à 3 pour la virgule fixe et les Flips-Flops et les Look Up Tables passent respectivement de 688 et 699 pour le filtre à virgule flottante à 123 et 208 pour le filtre à virgule fixe. En parallélisant les boucles dans le laboratoire suivant, on pourrait s’attendre à voir le nombre de ressources utilisées augmenter puisque cette technique d’optimisation a pour conséquence d’augmenter l’utilisation des ressources matérielles. 

 

Ces gains de performance et la diminution de l’utilisation des ressources est possible puisque les opérations en point fixes sont plus efficaces. En effet, on n’a pas besoin de la logique complexe permettant de traiter les opérations en virgule flottante ce qui rend le design plus compacte et rapide. Les opérations de bases tel que les additions, les multiplications et les soustractions nécessitent donc un matériel plus simple puisqu’il n’y a pas d’exposant à décaler ni de mantisse à aligner.

Impacts de l’optimisation sur la vitesse et les ressources
Le code en C a été optimisé à l’aide des directives HLS de trois façons : optimisation des boucles seulement, optimisation des fonctions seulement et optimisation complète. Voici l’estimation de performance et l’utilisation des ressources du filtre sans optimisation :
  







Optimisation des boucles
La solution afin d’obtenir les meilleurs résultats en termes de latence et d’intervalle a été d’ajouter les directives Pipeline et Unroll au niveau des boucles Shift_Accum_Loop et Update_Weights_Loop. Ces directives ont permis d’obtenir une latence et un intervalle minimal de 2 cycles et maximale de 4 cycles. 
  

Optimisation des fonctions
Il a été également possible d’obtenir des résultats similaires en optimisant les fonctions au lieu des boucles. Le résultat de la synthèse comporte toute fois des différences au niveau des ressources utilisées. Les directives de Pipeline ont été utilisées sur les fonctions FIR et LMS et les tableaux utilisés dans ces mêmes fonctions ont été partitionné. Cette optimisation a permis d’obtenir les mêmes performances en termes de latence et d’intervalle mais, à un cout plus élevé en matière de ressources. Le nombre de Flip-Flops et de Look up Tables utilisés est plus important dans ce scénario d’optimisation. Il est important de souligner que la directive Dataflow a aussi été utilisée mais, celle-ci offrait de moins bonne performance en termes d’intervalle et de latence que la directive Pipeline. 
  

Optimisation complète
Lors de l’optimisation complète, les résultats de la synthèse sont identiques aux résultats précédents de l’optimisation des fonctions seulement. Encore dans ce cas-ci, l’optimisation a été faite de sorte à obtenir les meilleurs résultats en matière de performance. Les instructions utilisées dans la fonction principale FIR_LMS sont simplement que deux directives d’array partition pour les tableaux input et updated_c. Ensuite, les sous fonctions FIR et LMS ont été pipelinées, leurs tableaux c et shift_reg ont été partitionnés et leur boucle interne ont été également pipelinées et dépliées. 
  


Meilleure optimisation
Parmi les trois choix d’optimisation présenté, la meilleure solution à choisir serait celle de l’optimisation des boucles seulement. Ce choix est basé sur les performances du filtre ainsi que son utilisation des ressources. Puisque la latence des trois optimisations est équivalente, l’utilisation moindre des ressources de ce profil d’optimisation le rend le plus avantageux.
