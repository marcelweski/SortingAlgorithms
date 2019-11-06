#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define N 150000

void randomize(int a[N])
{
	for (int i = 0; i < N; i++)
		a[i] = rand() % N;
}

enum SortType
{
	ASCENDING,
	DESCENDING
};

void selectionSort(int *a, enum SortType sortType)
{
	int insertIdx = 0, minIdx, i, t;

	while (insertIdx < N)
	{
		minIdx = insertIdx;
		for (i = insertIdx + 1; i < N; i++)
			if (sortType == ASCENDING ? a[i] < a[minIdx] : a[i] > a[minIdx])
				minIdx = i;

		t = a[minIdx];
		a[minIdx] = a[insertIdx];
		a[insertIdx] = t;

		insertIdx++;
	}
}

void insertionSort(int *a, enum SortType sortType)
{
	int i, j, k;

	for (i = 1; i < N; i++)
	{
		k = a[i];
		j = i;
		while (j > 0 && (sortType == ASCENDING ? a[j-1] > k : a[j-1] < k))
		{
			a[j] = a[j - 1];
			j--;
		}
		a[j] = k;
	}
}

void mergeSortInternal(int *a, int *b, int l, int r, enum SortType sortType)
{
	int i, j, k, m;

	if (r > l)
	{
		m = (r + l) / 2;
		mergeSortInternal(a, b, l, m, sortType);
		mergeSortInternal(a, b, m + 1, r, sortType);

		for (i = m + 1; i > l; i--)
			b[i - 1] = a[i - 1];

		for (j = m; j < r; j++)
			b[r + m - j] = a[j + 1];

		for (k = l; k <= r; k++)
			a[k] = (sortType == ASCENDING ? b[i] < b[j] : b[i] > b[j]) ? b[i++] : b[j--];
	}
}

void mergeSort(int* a, enum SortType sortType)
{
	int* b = (int*)malloc(sizeof(int) * N);
	mergeSortInternal(a, b, 0, N-1, sortType);
	free(b);
}

void quickSortInternal(int *a, int l, int r, enum SortType sortType)
{
	int v, i, j, t;

	if (r > l)
	{
		v = a[r];
		i = l - 1;
		j = r;

		while (1)
		{
			while (sortType == ASCENDING ? a[++i] < v : a[++i] > v);
			while (sortType == ASCENDING ? a[--j] > v : a[--j] < v);

			if (i >= j)
				break;

			t = a[i];
			a[i] = a[j];
			a[j] = t;
		}

		t = a[i];
		a[i] = a[r];
		a[r] = t;

		quickSortInternal(a, l, i - 1, sortType);
		quickSortInternal(a, i + 1, r, sortType);
	}
}

void quickSort(int *a, enum SortType sortType)
{
	quickSortInternal(a, 0, N - 1, sortType);
}

void downheap(int *a, int n, int k, enum SortType sortType)
{
	int j, v;

	v = a[k];
	while (k <= n/2)
	{
		j = k + k;
		if (j < n && (sortType == ASCENDING ? a[j] < a[j + 1] : a[j] > a[j + 1]))
			j++;

		if (sortType == ASCENDING ? v >= a[j] : v <= a[j])
			break;

		a[k] = a[j];
		k = j;
	}

	a[k] = v;
}

void heapSort(int* a, enum SortType sortType)
{
	int k, t, n = N-1;

	for (k = N / 2; k >= 0; k--)
		downheap(a, n, k, sortType);

	while (n > 0)
	{
		t = a[0];
		a[0] = a[n];
		a[n] = t;
		downheap(a, --n, 0, sortType);
	}
}

double measureSingle(int *a, enum SortType sortType, void(*sortFunc)(int*,enum SortType))
{
	clock_t starttime, endtime;

	randomize(a);

	starttime = clock();
	sortFunc(a, sortType);
	endtime = clock();

	return ((double)endtime - starttime) / CLOCKS_PER_SEC;
}

void measureSortType(int *a, enum SortType sortType, void(*sortFunc)(int*, enum SortType))
{
	double runtime1, runtime2, runtime3, average;

	runtime1 = measureSingle(a, sortType, sortFunc);
	printf("%6.2f s | ", runtime1);
	
	runtime2 = measureSingle(a, sortType, sortFunc);
	printf("%6.2f s | ", runtime2);
	
	runtime3 = measureSingle(a, sortType, sortFunc);
	printf("%6.2f s | ", runtime3);
	
	average = (runtime1 + runtime2 + runtime3) / 3;
	printf("%6.2f s\n", average);
}

void measureAlgorithm(int *a, const char* name, void(*sortFunc)(int*, enum SortType))
{
	// ascending
	printf("\t%13s ASC  | %7i | ", name, N);
	measureSortType(a, ASCENDING, sortFunc);
	
	// descending 
	printf("\t%13s DESC | %7i | ", name, N);
	measureSortType(a, DESCENDING, sortFunc);
}

int main(int argc, char** argv)
{
	int *a;

	srand(time(NULL));

	a = (int*)malloc(sizeof(int) * N);

#define LINE "\t------------------------------------------------------------------------\n"
	
	printf("\n");
	printf("\tAlgorithm          | N       | 1. Run   | 2. Run   | 3. Run   | Average\n");
	printf(LINE);

	measureAlgorithm(a, "SelectionSort", selectionSort);
	
	printf(LINE);
	
	measureAlgorithm(a, "InsertionSort", insertionSort);
	
	printf(LINE);

	measureAlgorithm(a, "MergeSort", mergeSort);

	printf(LINE);

	measureAlgorithm(a, "QuickSort", quickSort);

	printf(LINE);

	measureAlgorithm(a, "HeapSort", heapSort);

	printf("\n");

	// for (int i = 0; i < N; i++)
	// 	printf("%i ", a[i]);
	// printf("\n");
	
	free(a);
	system("pause");
	return 0;
}
