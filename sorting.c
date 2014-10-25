/* file:          sorting.c */
/* authors:       Wiebe Marten Wijnja (wiebemarten@gmx.com) */
/*                Tomas Karamazen (t.j.karamazen@student.rug.nl) */
/* date:          24th Oct 2014 */
/* version:       10 */



#include <stdio.h>
#include <stdlib.h>

#define FALSE 0
#define TRUE  1

void* safeMalloc(int size) {
  void *ptr = malloc(size);
  if (ptr == NULL) {
    printf("Error: memory allocation failed. Out of memory?\n");
    exit(-1);
  }
  return ptr;
}

void* safeCalloc(int length, int size) {
  void *ptr = calloc(length, size);
  if (ptr == NULL) {
    printf("Error: memory allocation failed. Out of memory?\n");
    exit(-1);
  }
  return ptr;
}


void swap(int i, int j, int arr[]) {
  int h = arr[i];
  arr[i] = arr[j];
  arr[j] = h;

  #ifdef VERBOSE
  printf("%d %d\n", i, j);
  #endif
}
  
/******************* Selection Sort ******************/

void selectionSort(int length, int arr[]) { 
  int i, j, minimum;
  for (i=0; i < length; i++){
    minimum = i;
    for (j=i+1; j < length; j++) {
      if (arr[j] < arr[minimum]) {
        minimum = j;
      }
    }
    swap(i, minimum, arr);
  }
}

/******************* Insertion Sort ******************/
  
int searchIndex(int length, int arr[], int value) {
  int i = 0;
  while ((i < length) && (arr[i] <= value)) {
    i++;
  }
  return i;
}

void shiftRight(int length, int arr[], int idx) {
  /* shift the elements with indices from [idx,length) one position to the right. */
  int i;
  for (i=length; i > idx; i--) {
    arr[i] = arr[i-1];
  }
}

void insert(int length, int arr[], int value) {
  /* insert value in sorted array */
  int idx = searchIndex(length, arr, value);
  shiftRight(length, arr, idx);
  arr[idx] = value;
}

void insertionSort(int length, int arr[]) {
  int i;
  for (i=1; i < length; i++) {
    insert(i, arr, arr[i]);
  }
}
  
/******************* Bubble Sort ******************/

void bubbleSort(int length, int arr[]) { 
  int i, j;
  for (i=0; i < length; i++) {
    for (j=0; j+1 < length; j++) {
      if (arr[j] > arr[j+1]) {
        swap(j, j+1, arr);
      }
    }
  }
}

/******************* Improved Bubble Sort ******************/

void bubbleSort2(int length, int arr[]) { 
  int i, j, a;
  int alreadySorted;
  
  i=0;
  for (i=0; i < length; i++) {
	alreadySorted=1;
	j=0;
    while(j+1 < length-i) {
	  a=1;
	  while(j+a < length && arr[j]==arr[j+a]){ /*If multiple elements are the same, bubble them all at once. */
		  a++;
	  }
		
      if (arr[j] > arr[j+a]) {/*Compares the current element with the next element that is not equal. If the current element is bigger, these two need to be swapped. */
        swap(j, j+a, arr);
        alreadySorted=0;/*We had to perform a swap. Array not yet sorted.*/
        
      }else{
		  j+=a;
	  }
      /*Increment by 1, or by the amount of elements that are the same if there are (so we skip over all occurrences of the same elements, as these dont need to move in relation to each other) */
      
    }
    i+=a-1; /*If a is bigger than 1 at the last iteration of the inner loop, then a sequence of multiple items at the end is sorted. Thus, we can subtract this whole sequence from the list we still need to check. */
    
    if(alreadySorted){
		break;/*If, after looping over all yet-to-sort elements, no swaps were performed, the array is already fully sorted: We're done in this case. Exit prematurely*/
	}
  }
}

/******************* Merge Sort ******************/

int *copySubArray(int left, int right, int arr[]) {
  int i;
  int *copy = safeMalloc((right - left)*sizeof(int));
  for (i=left; i < right; i++) {
    copy[i - left] = arr[i];
  }
  return copy;
}

void mergeSort(int length, int arr[]) {
  int l, r, mid, idx, *left, *right;
  if (length <= 1) { /* base case */
    return;
  }
  /* recursive case: length >= 2 implies 0 < length/2 < length */
  mid = length/2;
  /* make copies of subarrays and sort them (recursion) */
  left = copySubArray(0, mid, arr);
  right = copySubArray(mid, length, arr);
  mergeSort(mid, left);
  mergeSort(length - mid, right);
  /* merge sorted subarrays */
  idx = 0;
  l = r = 0;
  while ((l < mid) && (r < length - mid)) {
    if (left[l] < right[r]) {
      arr[idx] = left[l];
      l++;
    } else {
      arr[idx] = right[r];
      r++;
    }
    idx++;
  }
  /* process trailing elements: either l < mid or r < length - mid */
  while (l < mid) {
    arr[idx] = left[l];
    idx++;
    l++;
  }
  while (r < length - mid) {
    arr[idx] = right[r];
    idx++;
    r++;
  }
  /* deallocate memory */
  free(left);
  free(right);
}

/******************* Quick Sort ******************/

int partition(int length, int arr[]) {
  int left = 0;
  int right = length;
  int pivot = arr[0];

  while (left < right) {
    while ((left < right) && (arr[left] <= pivot)) {
      left++;
    }
    while ((left < right) && (pivot < arr[right-1])) {
      right--;
    }
    if (left < right) {
      /* (arr[left] > pivot) && (arr[right-1] <= pivot) : swap */
      right--;
      swap(left, right, arr);
      left++;
    }    
  }
  /* put pivot in right location: swap(0,left-1,arr) */
  left--;
  arr[0] = arr[left];
  arr[left] = pivot;
  return left;
}

void quickSort(int length, int arr[]) {
  int boundary;
  if (length <= 1) {
    return; /* empty or singleton array: nothing to sort */
  }
  boundary = partition(length, arr);
  quickSort(boundary, arr);
  quickSort(length - boundary - 1, &arr[boundary + 1]);
}



/******************* Improved Quick Sort (increased pivot for input set) This is actually a 'quick radix sort' ******************/
/***WARNING!! Only works for integers [0,1024) ***/

int partition2(int length, int arr[], int mask) {
  int left = 0;
  int right = length;
  
  while (left < right) {
    while ((left < right) && (arr[left] & mask) == 0) {
      left++;
    }
    while ((left < right) && ( arr[right-1] & mask) != 0) {
      right--;
    }
    if (left < right-1) {
      /* (arr[left] > pivot) && (arr[right-1] <= pivot) : swap */
      right--;
      swap(left, right, arr);
      left++;
    } 
  }
  return left;
}

void quickSort2Rec(int length, int arr[], int mask) {
  int boundary;
  if (length <= 1 || mask==0) {
    return; /* empty or singleton array: nothing to sort */
  }
  boundary = partition2(length, arr, mask);
  
  quickSort2Rec(boundary, arr, mask/2);
  quickSort2Rec(length - boundary, &arr[boundary], mask/2); /*Exact same boundary as left side, as there is no 'pivot' */
}


void quickSort2(int length, int arr[]) {
	quickSort2Rec(length, arr, 512);
}
/******************* Quick Sort using a Random Pivot ******************/

int partitionRand(int length, int arr[]) {
  int left = 0;
  int right = length;
  int pivot = arr[rand() % length];/*A random element in the array.*/

  while (left < right) {
    while ((left < right) && (arr[left] <= pivot)) { /*Skip all values to the left of the pivot that sould be to the left.*/
      left++;
    }
    while ((left < right) && (pivot < arr[right-1])) {/*Skip all values to the right of the pivot that sould be to the right.*/
      right--;
    }
    if (left < right) {
      /* (arr[left] > pivot) && (arr[right-1] <= pivot) : swap */
      right--;
      swap(left, right, arr);
      left++;
    }    
  }
  /* put pivot in right location: swap(0,left-1,arr) */
  left--;
  arr[0] = arr[left];
  arr[left] = pivot;
  return left;
}

void quickSortRand(int length, int arr[]) {
  int boundary;
  if (length <= 1) {
    return; /* empty or singleton array: nothing to sort */
  }
  boundary = partitionRand(length, arr);
  quickSortRand(boundary, arr);
  quickSortRand(length - boundary - 1, &arr[boundary + 1]);
}


/******************* Counting Sort ******************/
 
void countingSort(int length, int arr[]) {
  int min, max, range;
  int i, j, idx, *count;
  /* compute range of values */
  min = max = arr[0];
  for (i=1; i < length; i++) {
    min = (arr[i] < min ? arr[i] : min);
    max = (arr[i] > max ? arr[i] : max);
  }
  range = 1 + max - min;
  /* create histogram and fill it with zeroes (i.e. use calloc) */
  count = safeCalloc(range, sizeof(int));
  /* count occurrences */
  for (i=0; i < length; i++) {
    count[arr[i] - min]++;
  }
  /* generate sorted array */
  idx = 0;
  for (i=0; i < range; i++) {
    for (j=0; j < count[i]; j++) {
      arr[idx] = min + i;
      idx++;
    }
  }
  /* release memory used for histogram */
  free(count);
}



/******************* Binary(Base 2) Radix Sort ******************/

/* Uses bit shifting for increased efficiency */
void binRadixSort(int length, int arr[]) {
	int max=arr[0];
	int exp=1;
	int *barr = safeMalloc(length * sizeof(int)); /* Array that will store the two buckets */

	int startZeroesBucket;
	int startOnesBucket;
	void *swapPtr;
	
	int i;
	for(i=1;i<length;i++){/* Calculate max so we know when to stop comparing binary digits.*/
		if(arr[i]>max){
			max=arr[i];
		}
	}
	
	while((max+1)/exp > 0){/*As long as the digits we are comparing are still inside the maximum number.*/
		
		startZeroesBucket = 0;
		startOnesBucket = 0;
		
		for(i=0;i<length;i++){/*Increase the start of the Ones bucket each time a number is found that should fit in the Zeroes bucket. */
			if((arr[i]&exp)==0){
				startOnesBucket++;
			}
		}
		
		/*If startOnesBucket is 0 or length, then all elements fall in the same bucket (end with the same digit), and we can skip this sorting iteration. */
		if(startOnesBucket!=0&&startOnesBucket!=length){
		
		
			for(i=0;i<length;i++){/*Put elements in their respective buckets in the new array */
				if((arr[i]&exp)==0){
					barr[startZeroesBucket++] = arr[i];
				}else{
					barr[startOnesBucket++] = arr[i];
				}
			}
			
			/*Now, swap arr[] and barr[] to preserve space. (So we dont need to keep allocating new space) */
			swapPtr = arr;
			arr = barr;
			barr = swapPtr;
			

		}
		
		exp <<=1; /*Exponent times two, e.g. next binary digit.*/
	}
	free(barr);
	
	/* arr[] is now sorted. */
}

/******************* Heap Sort *******************/

/*Function that moves a node that has been inserted at the root, to its correct position in a binary tree by swapping it with the childs until it is in the correct place.*/
void moveNodeDown(int start,int end, int arr[]){
	int root = start;
	int child;
	int swapIndex;
	while(root*2+1 <=end){
		child = root*2+1;
		swapIndex = root;
		if(arr[swapIndex] < arr[child]){ /*If left child is bigger than root*/
			swapIndex = child;
		}
		if(child+1 <=end && arr[swapIndex] < arr[child+1]){ /*If there is a right child and this right child is bigger than left child, or is bigger than root (if left child is not) */
			swapIndex = child+1;
		}
		
		if(swapIndex != root){ /*If we need to swap the root with something, perform this swap, and move down in the tree to consider the position the root ended up as as the new 'root' (so consider the subtree) */
			swap(root, swapIndex, arr);
			root = swapIndex;
		}else{
			return;
		}
	}
	
}

/* Changes an unsorted list to a proper binary tree by repeatedly inserting the next array element in the tree, until we've moved through the whole array. */
void makeHeap(int length, int arr[]){
	int start;
	
	/*Get array index where last 'parent' will be when transforming the array into a binary tree*/
	start = (length-2/2);
	
	/*Sort whole array into binary tree */
	while(start >=0){

		moveNodeDown(start, length-1, arr);
		start--;
	}
}

void heapSort(int length, int arr[]){
	int end;
	end = length-1;
	makeHeap(length, arr); /*Create the binary tree */
	
	/*Now, extract largest number from tree until array is in order. */
	while(end >0){
		/*Root node is the highest current number, swap with end.*/
		swap(0,end,arr);
		end--;
		/*Restore binary tree, because new root might not be the largest anymore.*/
		moveNodeDown(0,end,arr);
	}
}





/**** Smooth Sort, an optimized HeapSort that uses Leonardo Trees instead of Binary ones****/
/*** Very smooth, very hard to understand. With thanks to Dijkstra, who came up with this Algorithm and idea. ***/
/**** Is an Adaptive, In-Place algorithm that needs only O(1) additional space. Unfortunately is not stable.  ****/

/* Precomputed list of Leonardo numbers below 2^31. (The unsigned range of ANSI C longs) 
 * For the rest, see oeis.org/A001595/b001595.txt */

unsigned long LEONARDO[] = {1u, 1u, 3u, 5u, 9u, 15u, 25u, 41u, 67u, 109u, 177u, 287u, 465u, 753u,     1219u, 1973u, 3193u, 5167u, 8361u, 13529u, 21891u, 35421u, 57313u, 92735u,    150049u, 242785u, 392835u, 635621u, 1028457u, 1664079u, 2692537u,     4356617u, 7049155u, 11405773u, 18454929u, 29860703u, 48315633u, 78176337u,    126491971u, 204668309u, 331160281u, 535828591u, 866988873u, 1402817465u,    2269806339u, 3672623805u};

typedef struct LeonardoHeap {
	int smallestTreeSize;
	int treesVector;
} LeonardoHeap;


/*
 * Gets the index of the right-most child when given the index of the root.
 * Assumes a well-formed tree with size > 1
 * */
int leonardoHeapGetRightChild(int root){
	return root - 1;
}
/*
 * Gets the index of the right-most child when given the index of the root and the size(e.g. order) of the tree.
 * For a tree of depth N, this is root - LEONARDO[N-2] - 1
 * Assumes a well-formed tree with size > 1
 * */
int leonardoHeapGetLeftChild(int root, int treeSize){
	return root - 1 - LEONARDO[treeSize - 2];
}

/* For a given root and size (e.g. order) of the tree, returns which child is larger.
 * Assumes a well-formed tree with size > 1
 * */
int leonardoHeapLargerChild(int root, int treeSize, int arr[]){
	int left = leonardoHeapGetLeftChild(root, treeSize);
	int right = leonardoHeapGetRightChild(root);
	return (arr[left] > arr[right] ? left : right);
}

/*
 * When a new value is added as the root of a heap that was sorted. (Thus the root is the only unsorted element)
 * We sort the heap using a bubble-down approach: Sort the root for the highest part of the tree, if it ends up swapped with one of its children, sort that subtree, etc.
 * We don't need to sort the WHOLE heap in this case, which saves a lot time.
 * */
void leonardoHeapRebalanceSingleHeap(int root, int treeSize, int arr[]){
	while(treeSize > 1){
		
		/* First, select the child that is the bigger of the two, and keep track of its internal tree size. */
		int left = leonardoHeapGetLeftChild(root, treeSize);
		int right = leonardoHeapGetRightChild(root);
		
		int largerChild;
		int childTreeSize;
		if(arr[left] > arr[right]){
			largerChild = left;
			childTreeSize = treeSize - 1; 
		}else{
			largerChild = right;
			childTreeSize = treeSize - 2; /* Right tree has order N - 2 */
		}
		if(arr[root] > arr[largerChild]){ /*If root is the biggest, we are done. */
			return;
		}
		/* Otherwise, swap root with largest child, and consider that to be the new tree to compare against */
		swap(root, largerChild, arr);
		root = largerChild;
		treeSize = childTreeSize;
	}
}

/*
 * Completely sorts an unsorted amount of values to become a proper leonardo heap.
 * */
void leonardoHeapRectify(int begin, int end, LeonardoHeap lHeap, int arr[]){
	int curElem = end; /* right-most position, will store the root of the main tree at some point.*/
	
	/*Keep track of how big a heap is. We need this because it will change once we insert a node on top of it.*/
	int lastHeapSize;
	
	/* We're done when we're done. */
	while(1){
		
		
		
		/* Cache the tree we're currently working on */
		lastHeapSize = lHeap.smallestTreeSize;
		
		/*If this is the first heap in the tree, we're done */
		if(curElem - begin == LEONARDO[lastHeapSize]-1){
			break;
		}
		
		int toCompare;/*Caching of element to compare against*/
		toCompare = curElem; /*We want to compare the current root, except when its children are bigger */
	
		int largerChild;
		int previousRoot;
		
		if(lHeap.smallestTreeSize > 1){
			largerChild = leonardoHeapLargerChild(curElem, lHeap.smallestTreeSize, arr);
			
			if(arr[largerChild] > arr[curElem]){
				toCompare = largerChild;
			}
		}
		
		/* Get the index of the root of the next heap, by going LEONARDO[lastHeapSize] steps to the left */
		previousRoot = curElem - LEONARDO[lastHeapSize];
		
		/*If the current root is bigger or equal than the previous one, it is positioned correctly. Also stop if previous root is nonexistent because we ran out of trees. (Prevent segmentation faults from happening)*/
		/*if(previousRoot < 0){
			break;
		}*/
		if(arr[previousRoot] < arr[toCompare] ){
			break;
		}
		/*Otherwise, swap current and previous root. */
		swap(previousRoot, curElem, arr);
		/*And this becomes the next case to look at. */
		curElem = previousRoot;
		
		/*Scan down in the bitvector until we find the next heap.
		 * Note that lHeap was passed by value, not by reference, so we can manipulate it all we want and it won't overwrite the main one.
		 * */
		do{
			lHeap.treesVector >>=1;
			++lHeap.smallestTreeSize;
		}while((lHeap.treesVector & 1)!=1);/* While there is not yet a vector in the last place, increment the smallest next tree. */
		
	}
	/* Finally, we only need to rebalance the heap the root ended up in. */
	leonardoHeapRebalanceSingleHeap(curElem, lastHeapSize, arr);
	
}
/*
 * Given an implicit leonardo heap spanning [0, end) in a range spanning [0, length), 
 * increases the size of that heap by one and inserts the node (currently at *end) at the correct position in the heap.
 * */
LeonardoHeap leonardoHeapAdd(LeonardoHeap lHeap, int end,int length, int arr[]){
	/*
	 * When adding, there are three cases to consider:
	 * (Case 0: No elements yet in heap, add one of length 1)
	 * Case 1: If the last two heaps have sizes that differ by one, merge the two heaps by adding the new element.
	 * Case 2: If the last heap is of order L1, add a singleton heap of order L0.
	 * Case 3: Otherwise, add a singleton heap of order L1.
	 * */
	 
	 
	 if(lHeap.treesVector == 0){ /*If 0, there were no trees yet.  Case 0*/
		 lHeap.treesVector |= 1;
		 lHeap.smallestTreeSize = 1;
		 
		 
	 }else if((lHeap.treesVector & 3) == 3){/* If the last two bits are both set, e.g. it has the shape (?11), we have case 1.*/
		 /* First, remove these two trees from the vector (as we're going to merge them) */
		 
		 lHeap.treesVector >>=2;
		 lHeap.smallestTreeSize +=2; /*Increase the smallest size by 2 */
		 lHeap.treesVector |=1; /*And set the last bit of the treesVector to 1 */
		 
		 
	 }else if(lHeap.smallestTreeSize == 1){/* Case 2. Add a singleton heap. */
		 lHeap.treesVector <<=1;
		 lHeap.treesVector |=1;
		 
		 lHeap.smallestTreeSize = 0;
		 
		 
	 }else{/* Case 3: Everything else */
		 lHeap.treesVector <<= lHeap.smallestTreeSize -1;
		 lHeap.treesVector |= 1;
		 lHeap.smallestTreeSize = 1;
		 
		 
	 }
	 
	 /*We now have our new tree. Now check if it will stay at this size or not. If not, we won't do a full rectify yet (efficiency).
	  * */
	  int isLast = 0;
	  switch(lHeap.smallestTreeSize){
		  case 0: /*If last heap has order 0, only stop if we're at the end of the list */
			if(end+1==length){
				isLast = 1;
			}
			break;
		  case 1: /* If last heap has order 1, its the last if its the last element or the penultimate element and is not about to be merged. */
			if(end+1==length || (end+2==length && ((lHeap.treesVector &2 ) != 2))){
				isLast = 1;
			}
			break;
		  default: /*Otherwise, in final position if not enough room for next leonardo number +1. */
			if(length-(end+1) < LEONARDO[lHeap.smallestTreeSize-1]+1){
				isLast = 1;
			}
			break;
	  }
	  if(!isLast){
			leonardoHeapRebalanceSingleHeap(end, lHeap.smallestTreeSize, arr);
	  }else{
			leonardoHeapRectify(0, end, lHeap, arr);
	  }
	 
	return lHeap;
}
LeonardoHeap leonardoHeapRemove(LeonardoHeap lHeap, int end, int arr[]){
	/* Order of the current heap */
	int curHeapOrder;
	
	/*Heaps when splitting the current heap */
	int leftHeap;
	int rightHeap;
	/*
	 * Two cases:
	 * Case 1: Last heap is of order L1 or L0. Removing the element wont change other heaps, and does not need rebalancing.
	 * Case 2: All other cases, we expose two heaps. These need rebalancing.
	 * */
	 
	 /* Case 1 */
	if(lHeap.smallestTreeSize<=1){

		do{
			lHeap.treesVector >>=1;
			++lHeap.smallestTreeSize;
		}while(lHeap.treesVector!=0 && (lHeap.treesVector&1) != 1);
		return lHeap;
	}
	
	/* Case 2
	 * Break open the heap to expose two heaps: order LN-1 and order LN-2
	 * This works by mapping (?1, n)   to (?011, n-2)
	 * */

	curHeapOrder = lHeap.smallestTreeSize;

	lHeap.treesVector &= ~1;/*Set last bit to 0 */

	lHeap.treesVector <<= 2;/*Shift up by two */
	lHeap.treesVector |= 1; /*Set last two bits to 1 */
	lHeap.treesVector |= 2; /*Set last two bits to 1 */
	
	lHeap.smallestTreeSize -= 2;
	leftHeap = leonardoHeapGetLeftChild(end, curHeapOrder);
	rightHeap = leonardoHeapGetRightChild(end);
		
	/*Rebalance left heap. For this, pretend there is one heap less than there actually is. Thus we need to make a separate LeonardoHeap struct */
	LeonardoHeap allButLast;
	allButLast = lHeap;
	++allButLast.smallestTreeSize;
	allButLast.treesVector >>=1;
	
	
	leonardoHeapRectify(0, leftHeap,  allButLast, arr);
	leonardoHeapRectify(0, rightHeap, lHeap, arr);
	
	return lHeap;
}

/* Main Smooth Sort function. */
void smoothSort(int length, int arr[]){
	int i;
	
	
	
	if(length < 2){
		return;/*No sorting for 0 or 1 array length */
	}
	LeonardoHeap lHeap;
	lHeap.smallestTreeSize=0;
	lHeap.treesVector=0;
	

	for(i=0;i<length;i++){
		lHeap = leonardoHeapAdd(lHeap, i, length, arr);
	}
	
	for(i=length-1;i>0;i--){
		lHeap = leonardoHeapRemove(lHeap, i, arr);
	}
	
	return;

}


/*** Improved Insertion Sort ***/
/*** Uses Binary Search to optimize comparisons ***/

int binarySearchLower(int left, int right, int key, int* a){
  int mid = (left+right)/2;
  if(left > right){
    /* We did not find our key, but we still need to place it. */
    /* Placing it at mid keeps the list sorted. */
    return mid;
  }
  if(key < a[mid]){
    /* Go lower. */
    return binarySearchLower(left, mid-1, key, a);
  }
  if(key > a[mid]){
    /* Go higher. */
    return binarySearchLower(mid+1, right, key, a);
  }
  if(key == a[mid]){
    /* We should place our key here and shift the array. */
    return mid;
  }
  return -1;
}


void insertionSort2(int length, int a[]) {
  int i;
  int r;
  int m;
  /* For each element. */
  for (i = 1; i < length; i++) {
    /* Take our current element for comparison. */
    /* We need to move. */
    if (a[i] < a[i-1]) {
      /* Find the final place for our current element. */
      r = binarySearchLower(0, i-1, a[i], a);
      if (a[r] < a[i]) {
		r++;
      }
      /* Make room. */
      for (m = i; m >= r; m--) {
	a[m] = a[m-1];
      }
      /* Swap. */
      swap(i, r, a);
    }
  }
}

/**** Flash Sort ****/
/***First sorts the array partially, then finishes sorting using insertionSort. ***/
void partialFlashSort(int length, int arr[]){
	int m;
	
	m = length/20;/* Change 20 by your favourite lucky number! The original paper used 42. But 20 seems to work best.*/
	int* l = safeCalloc(m, sizeof(int));
	
	int i,j,k;
	int amin,nmax;
	amin = arr[0]; /*For the minimum value, it is enough to know what the min is.*/
	nmax = 0; /*For the maximum value, we also want to know what array element it is pointing to */
	
	for(i=1;i<length;i++){
		if(arr[i]<amin){
			amin = arr[i];
		}
		if(arr[i]>arr[nmax]){
				nmax=i;
		}
	}
	if(amin == arr[nmax]){ /*Array where all values are the same. Already sorted, of course. Also the case when the length 0 or 1.*/
		return;
	}
	
	/*The next part is very similar to what happens in bucket sort. Get the range of a bucket that we need to distribute values in. */
	double c1 = ((double) m-1)/(arr[nmax]-amin);
	
	for(i=0;i<length;i++){/*Check how long each bucket becomes, e.g. how many elements should fit in there. */
		k = (int) (c1 * (arr[i]- amin) );
		l[k]++;
	}
	for(k=1;k<m;k++){/*Add length of previous buckets to the current one to get the index at which it starts */
		l[k] += l[k-1];
	}
	/*Bring max to start */
	swap(0,nmax,arr);
	
	int nmove = 0;
	int flash,hold;
	j=0;
	k= m-1;
	while(nmove < length - 1){/*Until all elements have been moved at least once */
		while(j > (l[k]-1)){ /*We are searching for a new cycle leader: This is a Repeat Until we find the first element where l[k] >= j*/
			j++; /*As long as we did not find it yet, increment the element we look at, and recalculate the bucket we look at.*/
			k = (int) (c1 * (arr[j]- amin) );
		}
		flash = arr[j]; /*Now make a copy of this element, as we're going to swap it soon.*/
		while(j!=l[k]){ /* While the bucket of the current cycle leader is not yet full, repeat.*/
			k = (int) (c1 * (flash- amin) );/* Recalculate bucket because the element in flash changes each iteration. */
			
			/* Now, swap flash with the element that was in there. We now have a new flash, and we keep looking. */
			hold = arr[l[k]-1];
			arr[l[k]-1] = flash;
			flash = hold;
			
			l[k]--;
			nmove++;
		}
	}
	
	
}

/* Slow Flash Sort that uses Smooth Sort. Smooth sort is adaptive, but its own overhead results in a program that is not optimally fast. */
void flashSort(int length, int arr[]){
	partialFlashSort(length, arr);
	smoothSort(length, arr);
}

/* Flash Sort that uses the optimized Insertion Sort. This is blazingly fast and beats many of the other algorithms. Using Insertion Sort here helps because it is fast on nearly-sorted lists, as it is adaptive.*/
void flashInsertSort(int length, int arr[]){
	partialFlashSort(length, arr);
	insertionSort2(length, arr);
}


/*** Bogo Sort, using the Fisher-Yates Shuffle algorithm to mix the array ***/

/*Basically, the first part of the array is considered as a hat*/
/*Now, pick a random number from the hat, and swap it with the part of the array outside of the hat. 
 * The amount of values in the hat is now one less.
 * Continue until no values in hat left. Array is now sorted.
 * Also, you can now put the metaphorical hat on your head!
 * */
void fisherYatesShuffle(int length, int arr[]){
	int i;
	int j;
	for(i=length;i>1;i--){
		j = rand()%i;
		swap(i-1,j,arr);
	}
	
}

/*Shuffle the array until it is sorted.
 * Best Case: Sorts in one shuffle.
 * Worst Case: Never finds a solution.
 * Will work for values up to +- 10 within reasonable time. After that: you will grow old before it is finished.
 * Therefore, a manual break after 999999999 iterations has been installed to not make your terminal hang. 
 * */
void bogoSort(int length, int arr[]){
	int i;
	int issorted;
	int numIterations=0;
	while(numIterations<99999999){
		numIterations++;
		issorted=1;
		for(i=1;i<length;i++){
			if(arr[i-1]>arr[i]){
				issorted = 0;
			}
		}
		if(issorted){
			return;
		}else{
			fisherYatesShuffle(length, arr);
		}
	}
	printf("Bogo Sort has been going on too long! Bye Bye\n");
	
}



/***************************************************/



/* 
 * arr: pointer to first element of left sequence.
 * barr: pointer to array to put sorted sequence in.
 * leftSeqLength: length of left set ( left set goes from arr[0] to arr[leftSeqLength-1] )
 * rightSeqLength: length of right set; (right set goes from arr[legtSeqLength] to arr[leftSeqLength+RightSeqLength-1]
 *  */
void mergeSubRoutine(int arr[],int barr[],int leftSeqLength,int rightSeqLength){
	int i,j,k;
	i=0;
	j=leftSeqLength;
	k=0;

	
	/*First, copy left sequence over to barr. This means we only need length/2 of extra memory.*/
	for(i=0;i<leftSeqLength;i++){
		barr[i] = arr[i];
	}
	
	i=0;
	while(i<leftSeqLength && j<leftSeqLength+rightSeqLength ){
		
		if(barr[i] < arr[j]){
			arr[k] = barr[i];
			i++;
		}else{
			arr[k] = arr[j];

			j++;
		}
		k++;
	}
	while(i<leftSeqLength){
		arr[k] = barr[i];
		i++;		
		k++;
	}
	while(j<leftSeqLength+rightSeqLength){
		arr[k] = arr[j];
		j++;
		k++;			
	}
	return;
}
/* 
 * In this version of the function, the right part subsequence is treated backwards, e.g. with the highest element first.
 * Thus it is also inserted from right-to-left.
 * arr: pointer to first element of left sequence.
 * barr: pointer to array to put sorted sequence in.
 * leftSeqLength: length of left set ( left set goes from arr[0] to arr[leftSeqLength-1] )
 * rightSeqLength: length of right set; (right set goes from arr[legtSeqLength] to arr[leftSeqLength+RightSeqLength-1]
 * 
 *  */
void mergeSubRoutineMirrored(int arr[],int barr[], int leftSeqLength,int rightSeqLength){


	int i,j,k;
	i=0;
	j=leftSeqLength+rightSeqLength-1;
	k=0;
	
	
	/*First, copy left sequence over to barr. This means we need length of extra memory.*/
	for(i=0;i<leftSeqLength+rightSeqLength;i++){
		barr[i] = arr[i];
	}
	
	i=0;
	while(i<leftSeqLength && j>leftSeqLength-1 ){
		if(barr[i] < barr[j]){
			arr[k] = barr[i];
			i++;
		}else{
			arr[k] = barr[j];
			j--;
		}
		k++;
	}
	while(i<leftSeqLength){
			arr[k] = barr[i];
			i++;		
			k++;
	}
	while(j>leftSeqLength-1){
			arr[k] = barr[j];
			j--;
			k++;			
	}
	return;
}




void weemsort(int length, int arr[]){
	/*First, get longest increasing sequence from the start of the array */
	int startSeq=1;
	int seqVector=1; /* A BitVector that counts what sequences are already sorted. Only uses powers of 2. */
	int smallestSeqExp=0; /* To keep smallest-list-size lookup in constant time, we remove the trailing zeroes, and instead increase this variable when merging two lists. */
	
	int* barr = safeMalloc( length * sizeof(int));/*Extra array that is used for merging. Only needs to be length/2 because we only need to copy the first half of the data we want to merge.*/
	
	while(startSeq < length && arr[startSeq-1]< arr[startSeq]){
		startSeq++;
	}
	
	
	
	if(startSeq==length){ /* Array already sorted. nothing left to do. */
		return;
	}
	
	/* Observe: If the first 7 elements are already sorted, this means that the first four elements are sorted, the next two elements are sorted, and the next 1 element is sorted.
	 * As we always want to merge lengths of powers of two, we split the first longest sequence like this.
	 * The way our vector is set up, means that we can just assign it the value of starting seqence length.
	 *  */
	seqVector = startSeq; 
	
	/* Now, remove trailing zeroes and save them in our exponent, so looking up the last sequence length can be done in O(1). */
	while((seqVector&1)==0){
		seqVector >>=1;
		smallestSeqExp +=1;
	}
	
	
	
	int ddd;
	for(ddd=0;ddd<length;ddd+=32){
		insertionSort2(32,&arr[ddd]);
	}
	
	int j;
	int i;
	int incSeqLen;
	int decSeqLen;
	int seqLen;
	int nextSmallestSeqExp;
	while((1 << smallestSeqExp) < length){/*while the smallest sorted sequence is smaller than the length of the array, e.g. while the list is not sorted*/
		j = seqVector << smallestSeqExp; /*The element right after the currently sorted part */
		i = 1;
		incSeqLen=1;
		decSeqLen=1;
		
		/*Find length of next already sorted sequence, either ascending or descending. */
		
		while(i< (1 << smallestSeqExp) && j+i < length && arr[j+i] == arr[j+i-1]){/* All values that are the same can be used for both ascending and descending sequences. Do not iterate over elements that are the same as the current twice. */
			incSeqLen++;
			decSeqLen++;
			i++;
		}
		while(i< (1 << smallestSeqExp) && j+i<length && arr[j+i] >= arr[j+i-1]){ /* Only loops when next elements are a strict increasing subsequence (e.g. this and the next while loop will never run right after eachother) */
			incSeqLen++;
			i++;
		}
		while(i< (1 << smallestSeqExp) && j+i<length && arr[j+i] <= arr[j+i-1]){/* Only loops when next elements are a strict decreasing subsequence (e.g. this and the previous while loop will never run right after eachother) */
			decSeqLen++;
			i++;
		}


		if(incSeqLen == (1 << smallestSeqExp) || j+incSeqLen>=length){/* Merge the smallest sequence together with this new sequence. The last sequence is either of the same length, or all that is left until the end of the array. */
			/* TODO: Recursive step */
			seqLen = incSeqLen;
			while((seqVector &1)==1){/*As long as we can combine the made sequence with the previous one */
				mergeSubRoutine(&arr[j-(1 << smallestSeqExp)],barr,(1 << smallestSeqExp), seqLen);
				j-= (1 << smallestSeqExp);

				seqVector >>=1;/* Now, the smallest sequence size is twice what it was, thus remove the last smallest */
				smallestSeqExp +=1;
				
				seqLen = (j+((1 << smallestSeqExp))>length?j+(1 << smallestSeqExp)-length:(1 << smallestSeqExp));/*Ensure that we dont overshoot the end of the array */

			}
			seqVector |=1;/*Set the final bit to one, as this is the size we end up with. */
			
		}else if(decSeqLen == (1 << smallestSeqExp) || j+decSeqLen>=length){ /* Merge the smallest sequence together with this new sequence, where this new sequence is to be handled backwards. The last sequence is either of the same length, or all that is left until the end of the array.*/
			/* TODO: Recursive step */
			seqLen = decSeqLen;
			
			mergeSubRoutineMirrored(&arr[j-(1 << smallestSeqExp)],barr,(1 << smallestSeqExp), seqLen);
				seqVector >>=1;/* Now, the smallest sequence size is twice what it was, thus remove the last smallest */
				j-= (1 << smallestSeqExp);

				smallestSeqExp +=1;
				
				seqLen = (j+((1 << smallestSeqExp))>length?length-j:(1 << smallestSeqExp));/*Ensure that we dont overshoot the end of the array */
				/*seqVector |=1;*//*Set the final bit to one, as this is the size we end up with. */
				
			while((seqVector &1)==1){/*As long as we can combine the made sequence with the previous one */
				mergeSubRoutine(&arr[j-(1 << smallestSeqExp)],barr,(1 << smallestSeqExp), seqLen);
				j-= (1 << smallestSeqExp);
				seqVector >>=1;/* Now, the smallest sequence size is twice what it was, thus remove the last smallest */
				smallestSeqExp +=1;
				
				
				seqLen = (j+((1 << smallestSeqExp))>length?length-j:(1 << smallestSeqExp));/*Ensure that we dont overshoot the end of the array */

			}
			seqVector |=1;/*Set the final bit to one, as this is the size we end up with. */
			
		}else{/* Append sequence to sequence list as new smallest sequence. Do this only for increasing subsequences!*/
			nextSmallestSeqExp=0; 
			while(incSeqLen > 1 ){ /* Calculate size of biggest power of two in next smallest sequence*/
				incSeqLen >>= 1;
				nextSmallestSeqExp+=1;
			}
			/* Shift vector and change exp size accordingly. */
			seqVector <<= (smallestSeqExp-nextSmallestSeqExp);
			seqVector |=1; /* Set final bit to 1 to register this new sequence in the vector */
			smallestSeqExp = nextSmallestSeqExp;
			
		}

		
		/*Only runs when we have reached the end. Travel back and merge everything that is left, irregardless of sequence size
		 * Now, empty the whole seqVector.
		 *  */
		if(seqLen < (1 << smallestSeqExp)){

			while(seqVector > 0){
				
				
				if((seqVector &1) == 1){
					j = seqVector << smallestSeqExp;
					seqLen = length-j;/*Ensure that we dont overshoot the end of the array */

					mergeSubRoutine(&arr[j-(1 << smallestSeqExp)],barr,(1 << smallestSeqExp), seqLen);
					
					
					
				}
				seqVector >>=1;
				smallestSeqExp+=1;
				
			}
			

		}
		
		
	}
	
	
	
	
	return;
}
