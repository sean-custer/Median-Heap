#include "MedianHeap.h"
#ifndef MEDIANHEAP_CPP
#define MEDIANHEAP_CPP


template <typename T>
MedianHeap<T>::MedianHeap( bool (*lt) (const T&, const T&), bool (*gt)
			   (const T&, const T&), int cap ) {
  
  m_minHeap = new Heap<T>(cap/2+2, lt, gt);
  m_maxHeap = new Heap<T>(cap/2+2, lt, gt);
  
  m_lt = lt;
  m_gt = gt;
  
  m_capacity = cap;
  m_size = 0;
  
}
template <typename T>
MedianHeap<T>::~MedianHeap(){

  delete m_maxHeap;
  delete m_minHeap;
}

//copy construktohr
template <typename T>
MedianHeap<T>::MedianHeap(const MedianHeap<T>& otherH) {
  m_lt = otherH.m_lt;
  m_gt = otherH.m_gt;
  m_capacity = otherH.m_capacity;
  m_size = otherH.m_size;

  m_minHeap = otherH.m_minHeap;
  m_maxHeap = otherH.m_maxHeap;
  //do i need to copy m_min and m_max?
}


template <typename T>
const MedianHeap<T>& MedianHeap<T>::operator=(const MedianHeap<T>& rhs){
  m_lt = rhs.m_lt;
  m_gt = rhs.m_gt;
  m_capacity = rhs.m_capacity;
  m_size = rhs.m_size;
  //deallocate current heaps
  delete m_minHeap;
  delete m_maxHeap;

  m_minHeap = rhs.m_minHeap;
  m_maxHeap = rhs.m_maxHeap;
  return *this;
}

template <typename T>
const Heap<T>& Heap<T>::operator=(const Heap<T>& rhs){
  m_size = rhs.m_size;
  m_capacity = rhs.m_capacity;
  m_array = new T [rhs.m_size];
  m_lt = rhs.m_lt;
  m_gt = rhs.m_gt;
  
  //copy values
  for(int i = 1; i <=m_size; i++)
    {
      m_array[i] = rhs.m_array[i];
    }

}

template <typename T>
int MedianHeap<T>::maxHeapSize(){
  return m_maxHeap->m_size;
}

template <typename T>
int MedianHeap<T>::minHeapSize(){
  return m_minHeap->m_size;
}

template <typename T>
int MedianHeap<T>::size(){
  return m_size;
}

template <typename T>
int MedianHeap<T>::capacity(){
  return m_capacity;
}

template <typename T>
T MedianHeap<T>::locateInMaxHeap(int pos){
  //if index does not exist or is out of range
  if((pos > m_maxHeap->m_size) or (pos == 0)){
    throw out_of_range("Error: Position does not exist in Max Heap!");
  }
  return m_maxHeap->m_array[pos];
}


template <typename T>
T MedianHeap<T>::locateInMinHeap(int pos){
  //if index does not exist or is out of range
  if((pos > m_minHeap->m_size) or (pos == 0)){
    throw out_of_range("Error: Position does not exist in Min Heap!");
  }
  return m_minHeap->m_array[pos];
}


template <typename T>
T MedianHeap<T>::getMin(){

  //bounds check
  if(m_maxHeap->m_size != 0)
     {
       return m_min;
     }
  else
    {
      //if minHeap is not empty, but maxHeap is, then minHeap must
      //only have 1 item in it due to our balancing checks.
      //Therefore, m_max should be the only item in the heap
      if(m_minHeap->m_size != 0)
	return m_max;
      throw out_of_range("Error: Heap is empty!");
    }

}

template <typename T>
T MedianHeap<T>::getMax(){

  //bounds check
  if(m_minHeap->m_size != 0)
     {
       return m_max;
     }
  else
    {
      //if maxHeap is not empty, but minHeap is, then maxHeap must
      //only have 1 item in it due to our balancing checks.
      //Therefore, m_min should be the only item in the heap      
      if(m_maxHeap->m_size != 0)
	return m_min;
      throw out_of_range("Error: Heap is empty!");
    }

}


template <typename T>
void MedianHeap<T>::insert(const T& item) {
  //check to see if MedianHeap is empty, if so add to maxHeap

  if(m_size == 0)
    {
      m_maxHeap->insert(item);
      this->setMin();
    }
  
  //if item is less than Median, add to m_maxHeap
  else if(m_lt(item, getMedian()))
    {
      m_maxHeap->insert(item);
      this->setMin();
      
    }
  
  //if item is greater than Median, add to m_minHeap
  else if(m_gt(item, getMedian()))
    {
      m_minHeap->insert(item);
      this->setMax();
      
    }
  //if item is equivalent to Median (if they were to add the same number)
  //add to m_maxHeap by default
  else
    {
      m_maxHeap->insert(item);
      this->setMin();
    }
  
  m_size++;
  
  //BALANCING CASES
  //if m_minHeap is greater than 1 size larger, remove root
  //and add to m_maxHeap
  if(m_minHeap->m_size - m_maxHeap->m_size > 1)
    {
      T root = m_minHeap->deleteRoot();
      m_maxHeap->insert(root);
    }
  //if m_maxHeap is greater than 1 size larger, remove root
  //and add to m_minHeap
  else if(m_maxHeap->m_size - m_minHeap->m_size > 1)
    {
      T root = m_maxHeap->deleteRoot();
      m_minHeap->insert(root);
    }
  
  //SORTING CASES
  //if m_maxHeap has at least two items in it, sort them.

  if(m_maxHeap->m_size > 1)
    {
      m_maxHeap->sort(m_gt);
    }

  
  //if m_minHeap has at least two items in it, sort them.

  if(m_minHeap->m_size > 1)
    {
      m_minHeap->sort(m_lt);
    }

}

template <typename T>
void Heap<T>::sort(bool(*fptr) (const T&, const T&)){

  //iterate from last to first
  for(int i = m_size; i >= 1; i--)
    {
      //check bounds for children
      if(i/2 >= 1)
        {
          //compare parent to the left child, swap
          if((fptr(this->m_array[i], this->m_array[i/2])))
            {
              T temp;
              temp = m_array[i/2];
              m_array[i/2] = m_array[i];
              m_array[i] = temp;
            }
        }
    }
}

template <typename T>
bool MedianHeap<T>::deleteItem(T& givenItem, bool (*equalTo)
			       (const T&, const T&)){
  //iterate through maxHeap for item.
  if(m_maxHeap->deleteItem(givenItem, *equalTo))
    {
      //BALANCING CASES
      //if m_minHeap is greater than 1 size larger, remove root
      //and add to m_maxHeap
      if(m_minHeap->m_size - m_maxHeap->m_size > 1)
	{
	  T root = m_minHeap->deleteRoot();
	  m_maxHeap->insert(root);
	}
      //if m_maxHeap is greater than 1 size larger, remove root
      //and add to m_minHeap
      else if(m_maxHeap->m_size - m_minHeap->m_size > 1)
	{
	  T root = m_maxHeap->deleteRoot();
	  m_minHeap->insert(root);
	}
      
      //SORTING CASES
      //if m_maxHeap has at least two items in it, sort them.      
      if(m_maxHeap->m_size > 1)
	{
	  m_maxHeap->sort(m_gt);
	}      
      
      //if m_minHeap has at least two items in it, sort them.      
      if(m_minHeap->m_size > 1)
	{
	  m_minHeap->sort(m_lt);
	}
      //once we have checked all the cases, decrement overall size
      //and return true
      m_size--;
      return true;
    }

  //if not in maxHeap, check minHeap
  else if((m_minHeap->deleteItem(givenItem, *equalTo)))
    {
      //BALANCING CASES
      //if m_minHeap is greater than 1 size larger, remove root
      //and add to m_maxHeap
      if(m_minHeap->m_size - m_maxHeap->m_size > 1)
        {
          T root = m_minHeap->deleteRoot();
          m_maxHeap->insert(root);
        }
      //if m_maxHeap is greater than 1 size larger, remove root
      //and add to m_minHeap
      else if(m_maxHeap->m_size - m_minHeap->m_size > 1)
        {
          T root = m_maxHeap->deleteRoot();
          m_minHeap->insert(root);
        }

      //SORTING CASES
      //if m_maxHeap has at least two items in it, sort them.
      if(m_maxHeap->m_size > 1)
        {
          m_maxHeap->sort(m_gt);
        }

      //if m_minHeap has at least two items in it, sort them.
      if(m_minHeap->m_size > 1)
        {
          m_minHeap->sort(m_lt);
        }
      m_size--;
      return true;
    }
  //if neither heap has the item
  return false;
}

template <typename T>
void MedianHeap<T>::dump(){
  cout << "------------Max Heap------------\n";
  m_maxHeap->dump();
  cout << "------------Min Heap------------\n";
  m_minHeap->dump();
}

template <typename T>
void MedianHeap<T>::setMin(){
  //check m_maxHeap, set m_min
  if(m_maxHeap->m_size >= 1)
    {
      //if maxHeap only has one item in it, set that as min
      if(m_maxHeap->m_size == 1)
	m_min = m_maxHeap->m_array[1];
      //otherwise, compare item just inserted with m_min
      //if item just inserted is less than m_min, set m_min
      else
	{
	  if(m_lt(m_maxHeap->m_array[m_size], m_min))
	    m_min = m_maxHeap->m_array[m_size];
	}
    }
}

template <typename T>
void MedianHeap<T>::setMax(){
  //check m_minHeap, set m_max
  if(m_minHeap->m_size < 1)
    throw out_of_range("Error: Heap is empty!");
  else if(m_minHeap->m_size >= 1)
    {
      //if minHeap only has 1 item in it, set m_max to it
      if(m_minHeap->m_size == 1)
	m_max = m_minHeap->m_array[1];
      else
	{
	  //compare m_max to each new item
	  if(m_gt(m_minHeap->m_array[m_size], m_max))
	    m_max = m_minHeap->m_array[m_size];
	}
    }  
}

template <typename T>
T MedianHeap<T>::getMedian(){
  //if both arrays have at least one item in them
  if((m_maxHeap->m_size != 0) or (m_minHeap->m_size != 0))
     {
       //if maxHeap is bigger than minHeap by 1, return
       //root of maxHeap
       if(m_maxHeap->m_size - m_minHeap->m_size == 1)
	 {
	   return m_maxHeap->m_array[1];
	 }
       //if minHeap is bigger than maxHeap by 1, return
       //root of minHeap
       else if(m_minHeap->m_size - m_maxHeap->m_size == 1)
	 {
	   return m_minHeap->m_array[1];
	 }
       
       //if both sizes are equivalent, return maxHeap root
       else if(m_minHeap->m_size == m_maxHeap->m_size)
	 {
	   return m_maxHeap->m_array[1];
	 }
     }
  else
    {
      throw out_of_range("Error: Heap is empty!");
    }
}
 
//HEAP FUNCTIONS

template <typename T>
bool Heap<T>::deleteItem(T& givenItem, bool (*equalTo)
			 (const T&, const T&)){
    //iterate through the heap, check if item is in there.
  for(int i = 1; i <= m_size; i++)
    {
      //if we find item
      if(equalTo(givenItem, m_array[i]))
        {
	  //replace it with the farthest right child (last index in array)
	  m_array[i] = m_array[m_size];
	  this->m_size--;
          return true;
        }
      //if item not found, returns false
      else
	{
	  return false;
	}

    }
}


template <typename T>
T Heap<T>::deleteRoot(){
  
  T root = m_array[1];
  //shift entire array up one index, decrement size of heap
  for(int i = 1; i < m_size; i++)
    {
      m_array[i] = m_array[i+1];
    }
  m_size--;
  return root;
}

template <typename T>
void Heap<T>::dump(){
  for(int i = 1; i <= m_size; i++){
    cout << "Heap[" << i << "] = ";
    cout << m_array[i] << endl;
  }
}

template <typename T>
Heap<T>::~Heap(){
  delete m_array;
}


template <typename T>
Heap<T>::Heap(){}


template <typename T>
Heap<T>::Heap(int cap, bool(*fxn1)(const T&, const T&),
	      bool(*fxn2)(const T&, const T&)){
  //set function pointers to comparison functions
  m_lt = fxn1;
  m_gt = fxn2;
  m_capacity = cap;
  m_array = new T [m_capacity];
  m_size = 0;
  
}

template <typename T>
void Heap<T>::insert(T item){
  //if m_array is not full
  if(m_size != m_capacity)
    {
      //insert new item at the last index available
      m_size++;
      m_array[m_size] = item;
   
    }
  else
    {
      throw out_of_range("Error: Heap is full!");
    }
}





#endif
