/* file:          sorting.c */
/* author:		  Dr. A. Meijster */       
/* with major improvements by:
 * 				  Wiebe Marten Wijnja (wiebemarten@gmx.com) */
/*                Tomas Karamazen (t.j.karamazen@student.rug.nl) */
/* date:          24th Oct 2014 */
/* version:       10 */

#include <stdio.h>
#include <stdlib.h>
#include "timer.h"

#define NUMBEROFALGORITHMS 16
/* I'm going to forget that CriminalSort existed */

char algorithm[NUMBEROFALGORITHMS][16] = {
  "selectionSort",
  "insertionSort",
  "bubbleSort",
  "mergeSort",
  "quickSort",
  "countingSort",
  "bubbleSort2",
  "quickSort2",
  "quickSortRand",
  "binRadixSort",
  "heapSort",
  "smoothSort",
  "insertion2Sort",
  "flashSort",
  "flashInsertSort",
  "bogoSort"
  
};

void selectionSort(int length, int arr[]);
void bubbleSort(int length, int arr[]);
void insertionSort(int length, int arr[]);
void mergeSort(int length, int arr[]);
void quickSort(int length, int arr[]);
void countingSort(int length, int arr[]);
void bubbleSort2(int length, int arr[]);
void quickSort2(int length, int arr[]);
void quickSortRand(int length, int arr[]);
void binRadixSort(int length, int arr[]);
void heapSort(int length, int arr[]);
void smoothSort(int length, int arr[]);
void flashSort(int length, int arr[]);
void insertionSort2(int length, int arr[]);
void flashInsertSort(int length, int arr[]);
void bogoSort(int length, int arr[]);



void *makeDynamicIntArray(int length) {
  void *ptr = malloc(length*sizeof(int));
  if (ptr == NULL) {
    printf("\nMalloc failed: out of memory?\n");
    exit(-1);
  }
  return ptr;
}

int chooseMode() {
  int choice;
  do {
    printf("Available modes:\n");
    printf("  0 - manual\n");
    printf("  1 - automatic\n");
    printf("  2 - automatic(fast only)\n");
    printf("  3 - automatic(pgfplots output)\n");
    printf("  4 - automatic(fast only, pgfplots output)\n");
    
    printf("Which operating mode do you want to use? : ");
    scanf("%d", &choice);
  } while ((choice > 4) || (choice < 0));
  printf("\n");
  return choice;
}

int chooseAlgorithm() {
  int i, choice;
  do {
    printf("Available sorting algorithms:\n");
    for (i=0; i<NUMBEROFALGORITHMS; i++) {
      printf("  %d - %s\n", i, algorithm[i]);
    }
    printf("Which algorithm do you want to use? : ");
    scanf("%d", &choice);
  } while ((choice < 0) || (choice >= NUMBEROFALGORITHMS));
  printf("\n");
  return choice;
}

int chooseInputSet(){
	int choice;
  do {
    printf("Available input sets:\n");
    printf("  0 - random in range [0-1023]\n");
    printf("  1 - random in range [0-INT_MAX]\n");
    printf("  2 - random in range [0-10]\n");
    printf("  3 - already sorted list of length N\n");
    printf("  4 - Repeated sequence of 0-1024 \n");
    
    printf("Which input set do you want to use? : ");
    scanf("%d", &choice);
  } while ((choice > 4) || (choice < 0));
  printf("\n");
  return choice;
}

void sort(int length, int arr[], int algorithm) {
  switch (algorithm) {
    case 0: /* selection sort */
      selectionSort(length, arr);
      break;
    case 1: /* insertion sort */
      insertionSort(length, arr);
      break;
    case 2: /* bubble sort */
      bubbleSort(length, arr);
      break;
    case 3: /* merge sort */
      mergeSort(length, arr);
      break;
    case 4: /* quick sort */
      quickSort(length, arr);
      break;
    case 5: /* counting sort */
      countingSort(length, arr);
      break;
    case 6: /* improved bubble sort */
      bubbleSort2(length, arr);
      break;
    case 7: /* improved quick sort */
      quickSort2(length, arr);
      break;
    case 8: /* improved quick sort */
      quickSortRand(length, arr);
      break;
    case 9: /* Binary (base 2) radix sort */
      binRadixSort(length, arr);
      break;
    case 10: /* Heap Sort */
      heapSort(length, arr);
      break;
    case 11:
	  smoothSort(length, arr);
	  break;
	case 12:
	  flashSort(length, arr);
	  break;
	case 13:
	  insertionSort2(length, arr);
	  break;
	case 14:
	  flashInsertSort(length, arr);
	  break;

	case 15:
	  bogoSort(length, arr);
	  break;
  }
}

void sortFastOnly(int length, int arr[], int algorithm) {
  switch (algorithm) {
	  
    case 3: /* merge sort */
      mergeSort(length, arr);
      break;
    case 5: /* counting sort */
      countingSort(length, arr);
      break;
	case 7: /* quick sort */
      quickSort2(length, arr);
      break;
    case 8: /* improved quick sort */
      quickSortRand(length, arr);
      break;
    case 9: /* Binary (base 2) radix sort */
      binRadixSort(length, arr);
      break;
    case 10: /* Heap Sort */
      heapSort(length, arr);
    case 11: /* Smooth Sort */
      smoothSort(length, arr);
      break;
    case 12:/*FlashSort*/
	  flashSort(length, arr);
	  break;
	case 13:
	  insertionSort2(length, arr);
	  break;
	case 14:
	  flashInsertSort(length, arr);
	  break;

  }
}

/*
 * The function sortTime() returns the time in milliseconds that
 * the chosen sorting algorithm takes to sort the given array.
 *
 * parameters:  length     Length of the array that needs sorting
 *              arr        An array (with integers) to sort
 *              algorithm  The number of the sorting algorithm that should be used
 *
 * returns:     time in milliseconds
 */
int sortTime(int length, int arr[], int algorithm, int fastMode) {
  Timer stopwatch;
  startTimer(&stopwatch);
  if(fastMode==1){
	  sortFastOnly(length, arr, algorithm);
  }else{
	  sort(length, arr, algorithm);
  }
  stopTimer(&stopwatch);
  return milliseconds(&stopwatch);
}

/*
 * The function generateNumbers() allocates an array
 * of the given length and fills it with random
 * integers from the interval [0..1024).
 *
 * parameter:  length    Length of the array
 *
 * returns:    Array of integers
 */
int *generateNumbers(int length) {
  int i, *arr = makeDynamicIntArray(length);
  for (i=0; i<length; i++) {
    arr[i] = rand() % 1024;
  }
  return arr;
}
/*
 * The function generateNumbers() allocates an array
 * of the given length and fills it with random
 * integers from the interval [0-10].
 *
 * parameter:  length    Length of the array
 *
 * returns:    Array of integers
 */
int *generateNumbersBelowTen(int length) {
  int i, *arr = makeDynamicIntArray(length);
  for (i=0; i<length; i++) {
    arr[i] = (rand() %10);
  }
  return arr;
}
/*
 * The function generateNumbers() allocates an array
 * of the given length and fills it a list going from 0 to 1023 and then starting again at 0.
 * It thus contains many subsequences that are locally sorted.
 *
 * parameter:  length    Length of the array
 *
 * returns:    Array of integers
 */
int *generateNumberSequence(int length) {
  int i, *arr = makeDynamicIntArray(length);
  for (i=0; i<length; i++) {
    arr[i] = i % 1024;
  }
  return arr;
}
/*
 * The function generateNumbers() allocates an array
 * of the given length and fills it with random
 * integers from the interval [0..RAND_MAX).
 * 
 *
 * parameter:  length    Length of the array
 *
 * returns:    Array of integers
 */
int *generateScatteredNumbers(int length) {
  int i, *arr = makeDynamicIntArray(length);
  for (i=0; i<length; i++) {
    arr[i] = rand()/2;
  }
  return arr;
}
/*
 * The function generateNumbers() allocates an array
 * of the given length and fills it with sequential
 * integers from the interval [0..length).
 * 
 * These kinds of sequences test if adaptive sorting algorithms work better.
 * 
 * parameter:  length    Length of the array
 *
 * returns:    Array of integers
 */
int *generateNumbersUpToN(int length) {
  int i, *arr = makeDynamicIntArray(length);
  for (i=0; i<length; i++) {
    arr[i] = i;
  }
  return arr;
}

/*
 * The function copyArray() copies the values from the array
 * `src` to the array `dest`.
 *
 * parameters: length  Length of the array `from` and `to`.
 *             src     Source array of integers
 *             dest    Destination array of integers
 */
void copyArray(int length, int dest[], int src[]) {
  int i;
  for (i=0; i<length; i++) {
    dest[i] = src[i];
  }
}

void verifySorting(int length, int arr[]) {
  int i;
  for (i=1; i < length; i++) {
    if (arr[i] < arr[i-1]) {
      printf("ERROR: arr[%d](%d) > arr[%d](%d)\n", i-1,arr[i-1], i,arr[i]);
      /*exit(-1);*/
      break;
    }
  }
}

/*
 * The function testAlgorithm() is used to test a
 * sorting algorithm. First it reads the number of the
 * algorithm you want to use, then it reads the length of
 * the array followed by the values for that array.
 *
 * That array will be sorted by the chosen sorting algorithm
 * and the result will be printed.
 */
void testAlgorithm() {
  int algorithm, i, length, *arr;
  /* Choose the sorting algorithm */
  algorithm = chooseAlgorithm();

  /* Read the array from the input */
  printf("array length? ");
  scanf("%d", &length);
  arr = makeDynamicIntArray(length);
  printf("array elements: ");
  for (i=0; i < length; i++) {
    scanf("%d", &arr[i]);
  }
  
  /* Sort the array using the chosen sorting algorithm */
  sortTime(length, arr, algorithm,0);
    
  /* Output the sorted array */
  for (i = 0; i < length - 1; i++) {
    printf("%d ", arr[i]);
  }
  printf("%d\n", arr[i]);

  /* Check if the array is correctly sorted */
  verifySorting(length, arr);
  
  /* Free used memory */
  free(arr);
}

/*
 * The function timeAlgorithms() prints the runtime of the 
 * sorting algorithms. For a fair comparison, each algorithm sorts
 * the same array. The timing results are printed on the screen.
 *
 * paremeter: length   Length of the array that is used to perform the timing
 */
void timeAlgorithms(int length, int fastMode, int pgfplotmode, int inputset,int startlength) {
  int *arr, *arr2;
  int alg, t;
  
  if(!pgfplotmode){
	printf("\n");
  }
  arr = makeDynamicIntArray(length);
  
  switch(inputset){

		case 1:
			arr2 = generateScatteredNumbers(length);
			break;
		case 2:
			arr2 = generateNumbersBelowTen(length);
			break;
		case 3:
			arr2 = generateNumbersUpToN(length);
			break;
		case 4:
			arr2 = generateNumberSequence(length);
			break;
  		case 0:
		default:
			arr2 = generateNumbers(length);
			break;
  }
  if(pgfplotmode){
	  if(length == startlength){
		  printf("length ");
	  }else{
		  printf("%8d ", length);  
	  }
	}
  for (alg=0; alg < NUMBEROFALGORITHMS; alg++) {
    copyArray(length, arr, arr2);
    if( (fastMode==0 && ((( (inputset==0||inputset==2) || (alg != 5 && alg != 7))/*Only counting sort and quicksort2 when input set correct */ && (alg!=15 || length<10)) /*Only use bogosort in VERY small cases */) )
    || (fastMode==1 && (alg==3 || /*alg==4 || alg==5 ||  alg==7 || alg==9 || */alg==8 || alg==10 || alg==11 /*|| alg==12 */ || alg==14 || (inputset==0 && (alg==5 || alg==7))/*Only use quicksort2 and counting sort when correct range */) )
    || (fastMode==2 && (alg==4 || alg==7 || alg==8))
    || (alg==9 && inputset != 3 && inputset != 4) /*BinRadixSort still has some problems that might crash the program */
    || ((inputset==0 || inputset==2) && (alg == 5 || alg==7))
    ){ /* ONLY FAST ALGORITHMS */
		
		
		if(length==startlength && pgfplotmode){/*In this case, only print the name. */
			printf("%17s ",algorithm[alg]);
		}else{
			
			
			t = sortTime(length, arr, alg, fastMode);
			if(pgfplotmode){/*Only print millisecond in times, as we use columns to indicate algorithm */
				printf("%17d ",t);
			}else{
				printf("%20s (with length %8d) takes %8d milliseconds\n",
				   algorithm[alg], length, t);
			}
			/* Check if the array is correctly sorted */
			verifySorting(length, arr);
		}
	}
  }
  
  if(length > 1000){
	  free(arr2);
	  free(arr);
	}
	printf("\n");
	if(length==startlength){/*Now call with first iteration of values */
		timeAlgorithms(length, fastMode, pgfplotmode, inputset, startlength+1);
	}
	
   
}



void sortingTester() {
  /* Choose manual mode or automatic mode */
  int mode;
  mode = chooseMode();
  if (mode == 0) {
    /* manual mode:
     *
     * Choose a sorting algorithm and provide it with
     * an array of integers on the input.
     */
    testAlgorithm();
  } else {
    /* automatic mode:
     * Compare the execution times for the sorting algorithms
     * to sort an arrays with 1000, 10000 and 100000 elements.
     */
     
    int inputset =  chooseInputSet();
	int startlength = 16;
	if(inputset == 3) {
		startlength = 8192;/*For some reason, some algorithms do not like it when running with low values on already-sorted lists. Memory leak somewhere?*/
	}
     
    int length;
    if(mode == 1){
		for (length=startlength; length <= 8000000; length*=2) {
		  timeAlgorithms(length, 0, 0, inputset,startlength);
		}
	}else{
		if(mode == 2){
			for (length=startlength; length <= 8000000; length*=2) {
			  timeAlgorithms(length, 1,0, inputset,startlength);
			}
		}else{
			if(mode == 3){
				

				for (length=startlength; length <= 8000000; length*=2) {
				  timeAlgorithms(length, 0, 1, inputset,startlength);
				  
				}
			}else{
				for (length=startlength; length <= 8000000; length*=2) {
				  
				  timeAlgorithms(length, 1, 1, inputset,startlength);
				}
			}
		}
	}
  }
}

int main(int argc, char *argv[]) {
  sortingTester();
  return 0;
}
