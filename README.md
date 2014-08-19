smile
=====

SeMi-supervIsed Learning agEnt

keywords : reinforcement learning, learning from demonstration, TORCS



Nous devrons développer une bibliothèque indépendante du domaine dans lequel l’agent évolue. 
Elle fournira plusieurs algorithmes de base de l’apprentissage
par renforcement (Q-Learning, SARSA, . . .), plusieurs critères de performance, ainsi
qu’une ouverture sur l’apprentissage semi-supervisé : c’est à dire avec les retours de
l’expert pris en compte.
Dans un premier temps, l’expert pourra simplement compenser la fonction de
récompense en précisant si l’agent a bien ou mal agit. Dans un second temps, si
le temps le permet, l’expert pourra également agir sur le choix de l’action à entreprendre lors de l’exploration de l’agent, ou encore dire à l’agent s’il est temps
d’exploiter ou d’explorer.
Parallèlement au développement de la bibliothèque, afin d’avoir une application
pratique de la théorie, on utilisera TORCS qui simule des courses de voiture en 3D
dans lequelles on intégrera nos propres conducteurs. Nous modifierons également
l’interface TORCS pour intégrer les retours positifs ou négatifs de l’expert.

Démo :
https://www.youtube.com/watch?v=MirqnEToUVg

<hr>
Code in library/ have been used in <br>

[Teacher-Student Framework: A Reinforcement Learning Approach](https://matthieu-zimmer.net/publications/ARMS2014.pdf)<br>
Zimmer, Matthieu; Viappiani, Paolo; Weng, Paul <br>
AAMAS Workshop Autonomous Robots and Multirobot Systems 2014
