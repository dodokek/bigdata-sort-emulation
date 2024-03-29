#include <linux/limits.h>
#include <fstream>
#include <string>
#include <variant>

// Heap realisation :)

struct MinHeapNode { 
	int element; 

	int tape_index; 
}; 

class MinHeap { 
	MinHeapNode* heap_array; 

	int heap_size; 

public: 
    MinHeap(MinHeapNode a[], int size) 
    { 
        heap_size = size; 
        heap_array = a; // store address of array 
        int i = (heap_size - 1) / 2; 
        while (i >= 0) { 
            MinHeapify(i); 
            i--; 
        } 
    } 
	
    void MinHeapify(int i) 
    { 
        int l = left(i); 
        int r = right(i); 
        int smallest = i; 
        
        if (l < heap_size && heap_array[l].element < heap_array[i].element) 
            smallest = l; 
        
        if (r < heap_size 
            && heap_array[r].element < heap_array[smallest].element) 
            smallest = r; 
        
        if (smallest != i) { 
            std::swap(heap_array[i], heap_array[smallest]); 
            MinHeapify(smallest); 
        } 
    } 

	int left(int i) { return (2 * i + 1); } 

	int right(int i) { return (2 * i + 2); } 

	MinHeapNode getMin() { return heap_array[0]; } 

	void replaceMin(MinHeapNode x) 
	{ 
		heap_array[0] = x; 
		MinHeapify(0); 
	} 
}; 