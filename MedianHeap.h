#ifndef _MEDIANHEAP_H_
#define _MEDIANHEAP_H_

#include <iostream>
using namespace std;


//array based, starts at index 1
template <class T>
class Heap{
public:
  Heap();
  Heap(int size, bool(*lt)(const T&, const T&)
       , bool(*gt)(const T&, const T&));
  ~Heap();
  
  const Heap<T>& operator=(const Heap<T>& rhs);
  void insert(T item);
  void dump();
  T deleteRoot();
  void sort(bool(*fptr) (const T&, const T&));

  bool deleteItem(T& givenItem, bool (*equalTo)
                  (const T&, const T&) ) ;

  //comparison functions
  //  bool lt(const T&, const T&);
  //  bool gt(const T&, const T&);
  //size of min/max heap
  int m_size;
  //array of numbers in the heap
  T *m_array;
  
private:
  int m_capacity;
  bool (*m_lt) (const T&, const T&);
  bool (*m_gt) (const T&, const T&);

};



template <typename T> class MedianHeap{
public:

  //constructor
  //first param returns true if first param > second
  //second param returns if first param < second
  MedianHeap( bool (*lt) (const T&, const T&), bool (*gt)
	      (const T&, const T&), int cap=100 ) ;

  //copy constructor
  MedianHeap(const MedianHeap<T>& otherH) ;

  //destructor
  ~MedianHeap()  ;

  //overloaded assignment operator
  const MedianHeap<T>& operator=(const MedianHeap<T>& rhs)  ;

  //returns total number of items in MedianHeap
  //must run in constant time
  int size() ;

  //returns capacity of MedianHeap
  //must run in constant time
  int capacity() ;

  //inserts items to MedianHeap
  //must run in O(log n) time, if capacity is reached throw error
  void insert(const T& item) ;

  T getMedian() ;
  
  T getMin() ;
  
  T getMax() ;

  void setMin();
  void setMax();

  //deletes item based on givenItem being equal to some key
  //if no item is found, returns false
  //must run in O(n)
  bool deleteItem(T& givenItem, bool (*equalTo)
		  (const T&, const T&) ) ;
  
  //dump fxn
  void dump() ;
  
  int maxHeapSize() ;
  
  int minHeapSize() ;
  
  T locateInMaxHeap(int pos) ;
  
  T locateInMinHeap(int pos) ;

  
private:
  Heap<T>* m_minHeap;
  Heap<T>* m_maxHeap;

  bool (*m_lt) (const T&, const T&);
  bool (*m_gt) (const T&, const T&);

  T m_max;
  T m_min;
  int m_size;
  int m_capacity;
};




#include "MedianHeap.cpp"
#endif
