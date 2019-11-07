#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#define N 150000

enum InitType
{
	RANDOM,
	ASCENDING,
	DESCENDING
};

void init(int a[N], enum InitType initType)
{
	for (int i = 0; i < N; i++)
		a[i] = (initType == RANDOM ? ((rand() % N)+1) : (initType == ASCENDING ? i+1 : N-i));
}

/*
 * Sorting Algorithms
 */
void selectionSort(int a[N])
{
	int insertIdx = 0, minIdx, i, t;

	while (insertIdx < N)
	{
		minIdx = insertIdx;
		for (i = insertIdx + 1; i < N; i++)
			if (a[i] < a[minIdx])
				minIdx = i;

		t = a[minIdx];
		a[minIdx] = a[insertIdx];
		a[insertIdx] = t;

		insertIdx++;
	}
}

void insertionSort(int a[N])
{
	int i, j, k;

	for (i = 1; i < N; i++)
	{
		k = a[i];
		j = i;
		while (j > 0 && a[j-1] > k)
		{
			a[j] = a[j - 1];
			j--;
		}
		a[j] = k;
	}
}

void mergeSortInternal(int a[N], int b[N], int l, int r)
{
	int i, j, k, m;

	if (r > l)
	{
		m = (r + l) / 2;
		mergeSortInternal(a, b, l, m);
		mergeSortInternal(a, b, m + 1, r);

		for (i = m + 1; i > l; i--)
			b[i - 1] = a[i - 1];

		for (j = m; j < r; j++)
			b[r + m - j] = a[j + 1];

		for (k = l; k <= r; k++)
			a[k] = (b[i] < b[j]) ? b[i++] : b[j--];
	}
}
void mergeSort(int a[N])
{
	int b[N];
	mergeSortInternal(a, b, 0, N-1);
}

void quickSortInternal(int a[N], int l, int r)
{
	int v, i, j, t;

	if (r > l)
	{
		v = a[r];
		i = l - 1;
		j = r;

		while (1)
		{
			while (a[++i] < v) {};
			while (a[--j] > v) {};

			if (i >= j)
				break;

			t = a[i];
			a[i] = a[j];
			a[j] = t;
		}

		t = a[i];
		a[i] = a[r];
		a[r] = t;

		quickSortInternal(a, l, i - 1);
		quickSortInternal(a, i + 1, r);
	}
}
void quickSort(int a[N])
{
	quickSortInternal(a, 0, N-1);
}

void downheap(int a[N], int n, int k)
{
	int j, v;

	v = a[k];
	while (k <= n/2)
	{
		j = k + k;
		if (j < n && a[j] < a[j + 1])
			j++;

		if (v >= a[j])
			break;

		a[k] = a[j];
		k = j;
	}

	a[k] = v;
}
void heapSort(int a[N])
{
	int k, t, n=N-1;

	for (k = N / 2; k >= 0; k--)
		downheap(a, n, k);

	while (n > 0)
	{
		t = a[0];
		a[0] = a[n];
		a[n] = t;
		downheap(a, --n, 0);
	}
}

void distributionCountingSort(int a[N])
{
	int i, j, b[N], count[N];

	memset(count, 0, sizeof(int) * N);

	for (i = 0; i < N; i++)
		count[a[i]-1]++;

	for (j = 1; j < N; j++)
		count[j] += count[j - 1];

	for (i = N - 1; i >= 0; i--)
		b[--count[a[i]-1]] = a[i];

	memcpy(a, b, sizeof(int) * N);
}

/*
 * Measuring
 */
double measureSingle(int a[N], enum InitType initType, void(*sortFunc)(int*))
{
	clock_t starttime, endtime;

	init(a, initType);

	starttime = clock();
	sortFunc(a);
	endtime = clock();

	return ((double)endtime - starttime) / CLOCKS_PER_SEC;
}

void measureWithAverage(int a[N], enum InitType initType, void(*sortFunc)(int*))
{
	double runtime1, runtime2, runtime3, average;

	runtime1 = measureSingle(a, initType, sortFunc);
	printf("%6.2f s | ", runtime1);
	
	runtime2 = measureSingle(a, initType, sortFunc);
	printf("%6.2f s | ", runtime2);
	
	runtime3 = measureSingle(a, initType, sortFunc);
	printf("%6.2f s | ", runtime3);
	
	average = (runtime1 + runtime2 + runtime3) / 3;
	printf("%6.2f s\n", average);
}

void measureAlgorithm(int a[N], const char* name, void(*sortFunc)(int*))
{
	printf("\t%24s RANDOM | %7i | ", name, N);
	measureWithAverage(a, RANDOM, sortFunc);
	
	printf("\t%24s ASC    | %7i | ", name, N);
	measureWithAverage(a, ASCENDING, sortFunc);
	
	printf("\t%24s DESC   | %7i | ", name, N);
	measureWithAverage(a, DESCENDING, sortFunc);

	printf("\t--------------------------------------------------------------------------------------\n");
}

int main(int argc, char** argv)
{
	int a[N];
	
	srand(time(NULL));
	
	printf("\n\tAlgorithm                       |    N    |   1. Run |   2. Run |   3. Run |  Average");
	printf("\n\t--------------------------------------------------------------------------------------\n");
	
	measureAlgorithm(a, "SelectionSort",            selectionSort);
	measureAlgorithm(a, "InsertionSort",            insertionSort);
	measureAlgorithm(a, "MergeSort",                mergeSort);
	measureAlgorithm(a, "QuickSort",                quickSort);
	measureAlgorithm(a, "HeapSort",                 heapSort);
	measureAlgorithm(a, "DistributionCountingSort", distributionCountingSort);

	printf("\n");

	return system("pause");;
}
