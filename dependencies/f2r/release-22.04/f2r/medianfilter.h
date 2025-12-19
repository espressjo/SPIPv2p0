/*!
 * \brief Median Filter
 * Class to perform median filtering on a 1-D array.
 */
#ifndef MEDIANFILTER_H
#define MEDIANFILTER_H

#include <cstdlib>

template <class dtype>

class medianfilter {
public:
  medianfilter(size_t length);
  dtype findMedian(dtype arr[], int n);
  void filtering(dtype *ref, unsigned short filter_size);

private:
  size_t dim;

  void MedianUtil(dtype arr[], int l, int r, int k, dtype &a, dtype &b);
  int randomPartition(dtype arr[], int l, int r);
  void swap(dtype *a, dtype *b);
  int Partition(dtype arr[], int l, int r);
};
template <class dtype>
medianfilter<dtype>::medianfilter(size_t length)
/*!
 * \brief description
 * must be initialize with the length of the array and datatype used
 * e.g., medianfilter<unsigned short> mf(1024);
 */
{
  dim = length;
}
template <class dtype>
void medianfilter<dtype>::filtering(dtype *ref, unsigned short filter_size)
/*!
 * \brief description
 * filtering applies a median filter, with filter size, to the
 * array pointed by *ref. Mode of the filter median is reflect.
 */
{

  // copy ref to 0 padded array.
  dtype med;
  int n;
  dtype *cpRef = new dtype[dim]{0};
  for (int i = 0; i < static_cast<int>(dim); i++) {

    cpRef[i] = ref[i];
  }
  int check = 0;
  int halff = (filter_size - 1) / 2;
  dtype *buff = new dtype[filter_size]{0};
  for (int i = 0; i < static_cast<int>(dim); ++i) {
    n = 0;
    check = 0;
    for (int ii = -1 * halff; ii <= halff; ii++) {
      if ((i + ii) >= 0 && (i + ii) < static_cast<int>(dim)) {
        buff[n] = cpRef[i + ii];
        n += 1;
      }

      if ((i + ii) < 0) {
        check = -1;
      }
      if ((i + ii) >= static_cast<int>(dim)) {
        check = 1;
      }
    }

    // reflect mode in scipy.ndimage.median_filter

    if ((filter_size - (n)) != 0 && check == -1) {
      int N = filter_size - (n);
      for (int iii = 1; iii <= N; ++iii) {
        buff[(n - 1) + iii] = buff[iii - 1];
      }

    } else if ((filter_size - (n)) != 0 && check == 1) {
      int N = filter_size - (n);
      for (int iii = 1; iii <= N; ++iii) {
        buff[(n - 1) + iii] = buff[n - iii];
      }
    }

    med = findMedian(buff, filter_size);
    // std::cout<<med<<std::endl;
    ref[i] = med;
  }
  delete[] buff;
  delete[] cpRef;
}
template <class dtype>
dtype medianfilter<dtype>::findMedian(dtype arr[], int n)
/*!
 * \brief description
 * Return the median of the array.
 */
{
  dtype ans, a = -1, b = -1;

  // If n is odd

  ans = b;
  if (n % 2 == 1) {
    MedianUtil(arr, 0, n - 1, n / 2, a, b);
    ans = b;
  }

  // If n is even
  else {
    MedianUtil(arr, 0, n - 1, n / 2, a, b);
    ans = (a + b) / 2;
  }

  // Print the Median of arr[]
  return ans;
}

template <class dtype>
void medianfilter<dtype>::MedianUtil(dtype arr[], int l, int r, int k, dtype &a,
                                     dtype &b) {

  // if l < r
  if (l <= r) {

    // Find the partition index
    int partitionIndex = randomPartition(arr, l, r);

    // If partion index = k, then
    // we found the median of odd
    // number element in arr[]
    if (partitionIndex == k) {
      b = arr[partitionIndex];
      if (a != -1)
        return;
    }

    // If index = k - 1, then we get
    // a & b as middle element of
    // arr[]
    else if (partitionIndex == k - 1) {
      a = arr[partitionIndex];
      if (b != -1)
        return;
    }

    // If partitionIndex >= k then
    // find the index in first half
    // of the arr[]
    if (partitionIndex >= k)
      return MedianUtil(arr, l, partitionIndex - 1, k, a, b);

    // If partitionIndex <= k then
    // find the index in second half
    // of the arr[]
    else
      return MedianUtil(arr, partitionIndex + 1, r, k, a, b);
  }

  return;
}

template <class dtype>
int medianfilter<dtype>::randomPartition(dtype arr[], int l, int r) {
  int n = r - l + 1;
  int pivot = rand() % n;
  swap(&arr[l + pivot], &arr[r]);
  return Partition(arr, l, r);
}
template <class dtype> void medianfilter<dtype>::swap(dtype *a, dtype *b) {
  dtype temp = *a;
  *a = *b;
  *b = temp;
}
template <class dtype>
int medianfilter<dtype>::Partition(dtype arr[], int l, int r) {
  dtype lst = arr[r];
  int i = l, j = l;
  while (j < r) {
    if (arr[j] < lst) {
      swap(&arr[i], &arr[j]);
      i++;
    }
    j++;
  }
  swap(&arr[i], &arr[r]);
  return i;
}

#endif // MEDIANFILTER_H
