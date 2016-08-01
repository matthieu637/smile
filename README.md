# smile

SeMi-supervIsed Learning agEnt

keywords : reinforcement learning, learning from demonstration, TORCS

<hr>
## How to build

##### Required packages :
* plib
* boost
* gcc
* cmake
* torcs (version 1.3.4 recommanded - doesn't work with >= 1.3.6)

The following works for ArchLinux paths :
```
git clone https://github.com/matthieu637/smile

#build library
cd smile/library
mkdir build
cd build
cmake ..
make -j4

#build driver for torcs
cd ../../driver/
mkdir build
cd build
cmake ..
make -j4

#copy lib to torcs
sudo make install
```
## How to run the driver
```
torcs
```
race -> quick race -> configure race -> accept -> add 33 smile_2 driver (with space)
accept -> accept -> new race

You should also remove the other drivers.

## How to learn (without display)
```
cp ~/.torcs/config/raceman/quickrace.xml ~/.torcs/config/dtmrace2.xml #copy config race
sudo chmod o+w /usr/share/games/torcs/ #allow to write the policy
cd ..
./learnIA 2 #wait at least 5 minutes than the perf is higher than 5000 several times
```

## Test my driver after learning
```
cp ~/.torcs/config/dtmrace2.xml ~/.torcs/config/raceman/quickrace.xml #copy config race
torcs
#race -> quick race -> new race
#Press F2 for a better view and "+" to accelerate time
#For better results you might condiser disable learning & e-greedy policy in this phase
#to do so changes learn to false in smile/driver/include/QLearnGen.hpp:65 + recompile/reinstall driver
```

## Troubleshooting
```
#you might need to remove some semaphores if you forced an exit
rm -rf /dev/shm/sem.*
```
<hr>

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

