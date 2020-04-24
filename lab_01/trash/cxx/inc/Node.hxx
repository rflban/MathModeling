#ifndef MMLAB01_NODE_HXX_
#define MMLAB01_NODE_HXX_

template <typename _T>
struct Node
{
    Node<_T> *next;
    _T data;
};

#endif // MMLAB01_NODE_HXX_

