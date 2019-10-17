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

|Baseline| 1 thread | 2 threads | 4 threads | 8 threads | 16 threads |
|--------|----------|-----------|-----------|-----------|------------|
| 2.483s | 	2.471s	|	1.541s	|	1.326s	|	1.243s	|	1.264	 |

As the CPU has 4 cores and multithreaded, little benefit can be expected after 8 threads. This seem to fit with the difference in measurments of 8 and 16 threads. The difference between baseline and 1 thread might be due to fewer processes run on the hos system, as the job queue and pthread code should only create overhead and not speed up the application. The most gain is between 1 and 2 threads. Probably as there is less conflict over mutexes. The speedup might be greater with different blocksizes and divisions..

## Task 3
I added omp_get_wtime(); at the beginning and end of serial_mxm. All that is within the function was used for the matrix multiplication. We want to test the time of the matrix multiplication, not the time it takes to setput the matrix or the printout.

The time was: 0.56532
## Task 4
|Baseline| 1 thread | 2 threads | 4 threads | 8 threads | 16 threads |
|--------|----------|-----------|-----------|-----------|------------|
| 0.5632s| 	0.562s	|	0.291s	|	0.236s	|	0.228s	| 	0.225s 	 |

The speedup is significant with two threads but does not improve much beyond that. Would have expected more improvement between 2 and 4 threads.
The default OMP_NUM_THREADS was 4 threads and that is also what was shown with opm_get_num_threads(). 

## Task 5
The cblas function took 0.318s. I would have expected it to be faster, as it is specificly made for matrix computation and intel should have more knowledge about utilizing the cpu for this specific usecase. The OpenMP version is parallelizing a generic for loop and should therefore not have as much domain specific knowledge. I guess the reason for blas to be slower is that it does not know what type of matrix it is, requiering scalars for multiplying the matrixes and row/column major information.
