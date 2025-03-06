# so2_pro

## 1st project

### Prerequsites

cmake make ninja

### How to build 
make
make clean -> cleans build

### Project structure
Philosophers are named by indexes, they eat only when they have acquired both forks, then they take some time to think, and try to eat again.
Forks are mutexes, that are named by indexes.

### Why this even works
When philosophers want to take they need to get lower indexed fork first, only then they can reach out and take higher indexed fork. This makes sure that each philosopher can not have only one fork. If that thing happens no one can reach for second fork and have a meal.
After philosophers have a meal, they put down the forks and once they fulfilled their needs they think. Thinking makes sure, that no philosopher can infinitely eat, making it possible for other philosophers to eat.
