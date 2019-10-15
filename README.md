# TDT4200_assignment 5

## Task 1

a) Get baseline measurments

Baseline measurments were taken with the following command:
```Bash
time ./mandel/mandel -x 0.5 -y 0.5 -s 1 -r 1920 -i 2048 -c 1 -b 9 -d 9 -p 9 -m -o ./output/mandel.bmp
```

The system used was a Dell LATITUDE E5450 with Intel(R) Core(TM) i5-5300U CPU @ 2.30GHz.

The real time of the application is about **2.483s**.

b) See how different parameters change the execution time and take special note of the area being rendered and Mariani-Silver borders.

- r (Resolution) has a significant effect on runtime, higher numbers takes longer.
- i (Itterations) has a significant effect on runtime, higher numbers takes longer,
- b (block dimension) has an effect on runtime, especially at larger numbers, but smaler numbers does not necessarily mean lower execution time.
- d (subdivision) Has a significant effect on runtime, higher numbers seem to take longer. It seems that lower subdivision allows the algorithm to check fewer sections before seperating a section with more dwell values into one without and one with.
- p (threads) has noot been implemented and therefore has no effect
- x,y,s (location) Has a significant effect. For instance, it the scale is high and the location is on a spot with no dwell values, the execution time can be 0.310s. The algorithm does not need to compute much as the entire border is outside of the set and the innside is therefore also outside the set. Other places require more computation.

```Bash
time mandel/mandel -x 0.49389 -y 0.36953 -s 0.0168 -r 1920 -i 2048 -c 1 -b 9 -d 9 -p 9 -m -o output/mandel.bmp
```
Section requiering more computation than baseline:
<img width="800" height="800" src="pthread/doc/img/highComputeMandel.bmp">
```Bash
time mandel/mandel -x 0.5 -y 0.5 -s 0.25 -r 1920 -i 2048 -c 1 -b 9 -d 9 -p 9 -m -o output/mandel.bmp
```
Section requiering less computation than baseline:

<img width="800" height="800" src="pthread/doc/img/lowComputeMandel.bmp" />

The first image require more time as there are more sections that can not be filled as their bord
er does not have the same dwell value. The size of the sections are greater in the second image as the algorithm can exit earlier

## Task 2

b)

## Task 3

## Task 4

## Task 5
