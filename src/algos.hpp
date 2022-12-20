#pragma once
#include <climits>
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>
using std::string;

template <class T> void validate(T &a) {
  bool valid;
  do {
    valid = 1;
    std::cout << ">>";
    std::cin >> a;
    if (!std::cin) {
      valid = 0;
      std::cout << "Invalid input!\n";
      std::cin.clear();
      std::cin.ignore(INT_MAX, '\n');
    }
  } while (!valid);
}

int selectNum(int lBound, int hBound);
int selectChoice(string choices[], int size);
bool yN();

void parse(string, char, std::vector<string> &);

template <class Node> size_t listSize(Node *&head) {
  size_t size = 0;
  for (Node *current = head; current != nullptr; current = current->next) {
    size++;
  }
  return size;
}

template <class Node> void singlyInsertAtTail(Node *&current, Node *newNode) {
  if (current == nullptr) {
    current = newNode;
    return;
  }
  if (current->next == nullptr) {
    current->next = newNode;
    return;
  }
  singlyInsertAtTail(current->next, newNode);
}

template <class Node>
Node *doublyInsertAtTail(Node *&head, Node *&tail, Node *newNode) {
  if (head == nullptr) {
    head = newNode;
    return newNode;
  }
  tail->next = newNode;
  newNode->prev = tail;
  return newNode;
}

template <class Node> void singlyRemove(Node *&head, Node *&node) {
  if (node == head) {
    head = head->next;
    free(node);
    return;
  }
  for (Node *current = head; current != nullptr; current = current->next) {
    if (current->next == node) {
      std::cout << "found node\n";
      current->next = node->next;
      free(node);
    }
  }
}

template <class Node> void doublyRemove(Node *&head, Node *&tail, Node *&node) {
  if (node == head) {
    head = head->next;
    node = nullptr;
    return;
  }
  if (node == tail) {
    tail = tail->prev;
    node = nullptr;
    return;
  }
  node->prev->next = node->next;
  node->next->prev = node->prev;
  free(node);
  return;
}

template <class Node> void deleteList(Node *&head) {
  if (head == nullptr)
    return;
  deleteList(head->next);
  free(head);
}
