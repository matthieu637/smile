smile
=====

SeMi-supervIsed Learning agEnt

keywords : reinforcement learning, learning from demonstration, TORCS



Nous devrons développer une bibliothèque indépendante du domaine dans le-
quel l’agent évolue. Elle fournira plusieurs algorithmes de base de l’apprentissage
par renforcement (Q-Learning, SARSA, . . .), plusieurs critères de performance, ainsi
qu’une ouverture sur l’apprentissage semi-supervisé : c’est à dire avec les retours de
l’expert pris en compte.
Dans un premier temps, l’expert pourra simplement compenser la fonction de
récompense en précisant si l’agent a bien ou mal agit. Dans un second temps, si
le temps le permet, l’expert pourra également agir sur le choix de l’action à en-
treprendre lors de l’exploration de l’agent, ou encore dire à l’agent s’il est temps
d’exploiter ou d’explorer.
Parallèlement au développement de la bibliothèque, afin d’avoir une application
pratique de la théorie, on utilisera TORCS qui simule des courses de voiture en 3D
dans lequelles on intégrera nos propres conducteurs. Nous modifierons également
l’interface TORCS pour intégrer les retours positifs ou négatifs de l’expert.
