/*
  Filename   : DivideAndConquer.hpp
  Author     : Jeremy Foxx
  Course     : CSCI 362
  Description:  implementing various divide and conquer algorithms
*/

#ifndef DIVIDE_AND_CONQUER_HPP_
#define DIVIDE_AND_CONQUER_HPP_

#include <iterator>
#include <utility>
#include <algorithm> 
#include <vector> 

// NOTE: you are forbidden from using anything from <algorithm> for this assignment
//       EXCEPT for std::copy

namespace SortUtils
{

// [9]
// Given a RandomAccessRange [first, last), determine where the "midpoint"
// would be and perform the following steps:
// order *first, *mid, *std::prev(last) in such a way such that
//   *first <= *mid <= *std::prev(last)
//
// returns an iterator to mid -- a.k.a. the median
//
template<typename Iter>
Iter
median3 (Iter first, Iter last)
{
  // TODO
  Iter mid = first; 
  std::advance (mid, std::distance (first, last) / 2); 
  --last;
  Iter iterators [3] = {first, mid, last};

  if (*iterators [0] > *iterators [1])
  {
    std::iter_swap (iterators [0], iterators [1]); 
  }

  if (*iterators [0] > *iterators [2])
  {
    std::iter_swap (iterators [0], iterators [2]); 
  }

  if (*iterators [1] > *iterators [2])
  {
    std::iter_swap (iterators [1], iterators [2]);
  }

  return iterators [1]; 

}

// [10]
// Takes two sorted ranges [first1, last1) and [first2, last2)
// and "merges" them by copying values into the iterator starting
// at "out". Uses operator< for comparing values
//
// Returns the iterator of one-past-the-last where we wrote to out
//
template<typename Iter1, typename Iter2, typename OIter>
OIter
merge (Iter1 first1, Iter1 last1, Iter2 first2, Iter2 last2, OIter out)
{
  // TODO
  if (first1 == last1 && first2 == last2)
  {
    return out; 
  }

  if (first1 == last1)
  {
    return std::copy (first2, last2, out); 
  } 
  else if (first2 == last2){
    return std::copy (first1, last1, out); 
  }

  if (*first1 < *first2)
  {
    *out = *first1; 
    ++out; 
    ++first1; 
  } 
  else {
    *out = *first2; 
    ++out; 
    ++first2;
  } 

  return SortUtils::merge (first1, last1, first2, last2, out); 
}

// [15]
// Takes a RandomAccessRange [first, last) and partitions the data into
// three groups -- this should be accomplished in a SINGLE PASS Big-O: O(N)
//
// Group 1: all values in [first, last) < pivot
// Group 2: all values in [first, last) == pivot
// Group 3: all values in [first, last) > pivot
//
// [ ... Group 1 ... | ... Group 2 ... | ... Group 3 ... ]
//                   ^                 ^
//                   p1                p2
//
// Returns a pair of iterators pointing to "p1" and "p2" above
//
template<typename Iter, typename Value>
std::pair<Iter, Iter>
partition (Iter first, Iter last, Value const& pivot)
{
  // TODO
  Iter l = first; 
  Iter eq = first; 
  Iter h = --last; 

  while (eq != h)
  {

    if (*eq < pivot) 
    {
      std::swap (*l, *eq);
      ++l;
      ++eq;
    } 
    else if (*eq > pivot){
      std::swap (*h, *eq);
      --h; 
    } 
    else {
      ++eq;
    }
  }

  if (*eq < pivot)
  {
    std::swap (*l, *eq); 
    ++l;
  } 
  else if (*eq > pivot) {
    std::swap (*h, *eq);
    --h;
  }

  ++h; 

  return std::make_pair (l, h); 
}

// [10]
// Given a RandomAccessRange, recursively call partition on either the
// left half or right half until you have found the nth largest element
//
// A call to nth_element (v.begin(), v.end(), 0) will return the min
// A call to nth_element (v.begin(), v.end(), v.size() - 1) will return the max
// A call to nth_element (v.begin(), v.end(), v.size() / 2) will return the median
//
// Precondition:
//   std::distance (begin, end) > n
//
// Hints:
//  - n will change if you need to recurse on the right half
//  - No recursion happens if "index of" n is between "index of" p1 and p2
//    remember: p1 and p2 are the return values to partition.
//  - call median3 to get a pivot value
//  - when calling partition, remember to dereference the iterator returns by median3
//
template<typename Iter>
Iter
nth_element (Iter first, Iter last, size_t n)
{
  // auto [p1, p2] = SortUtils::partition (...);
  if (std::distance (first, last) < 2)
  {
    return first;
  }

  auto const pivot = *SortUtils::median3 (first, last); 
  auto [p1, p2] = SortUtils::partition (first, last, pivot); 

  if (n < std::distance (first, p1))
  {
    return nth_element (first, p1, n); 
  } 
  else if (n < std::distance (first, p2)) {
    return first + n;
  } 
  else {
    return nth_element (p2, last, n - std::distance (first, p2)); 
  }
}

// [10]
// Given a RandomAccessRange, sort using merge sort
//
// Precondition:
//   std::distance (begin, end) > 0
//
// Hints:
//   - You will need a vector to act as a temporary buffer.
//
template<typename Iter>
void
merge_sort (Iter first, Iter last)
{
  // T is the type of data we are sorting
  const size_t N = std::distance (first, last); 
  Iter mid = first + N/2; 
  using T = std::remove_reference_t<decltype (*std::declval<Iter> ())>;

  if (N < 2)
  {
    return; 
  }

  SortUtils::merge_sort (first, mid); 
  SortUtils::merge_sort (mid, last); 
  std::vector <T> buff (N); 
  SortUtils::merge (first, mid, mid, last, buff.begin ());

  std::copy_n (buff.begin (), N, first); 
}

// [10]
// Given a RandomAccessRange, sort using quick sort
//
// Precondition:
//   std::distance (begin, end) > 0
//
// Hints:
//   - median3 will be called to find the pivot
//   - remember to dereference the iterator returned by median3 to get the pivot value
//   - partition should be called
//
template<typename Iter>
void
quick_sort (Iter first, Iter last)
{
  // T is the type of data we are sorting
  const auto N = std::distance (first, last); 
  using T = std::remove_reference_t<decltype (*std::declval<Iter> ())>;

  if (N < 2) return; 

  auto pivot = *SortUtils::median3 (first, last); 
  auto [p1, p2] = SortUtils::partition (first, last, pivot);

  SortUtils::quick_sort (first, p1);
  SortUtils::quick_sort (p2, last); 
}

template<typename Iter>
void
insertion_sort (Iter first, Iter last)
{
  for (Iter i = first; i != last; ++i)
  {
    for (Iter j = i; j != first; --j)
    {
      if (*(j - 1) > *j)
      {
        std::iter_swap (j - 1, j);
      }
      else
      {
        break;
      }
    }
  }
}

} // end namespace util

#endif
