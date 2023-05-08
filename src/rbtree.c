#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  
  /* 05-08 23:30
    TODO: initialize struct if needed
    new_rbree should return rbtree struct with null root node
    
    rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));

    트리 생성 : 
      트리 생성, 메모리할당
      nil 노드 생성, 메모리 할당
      nil 노드 색상 설정(RBTREE_BLACK)
      생성한 트리의 nil에 nil 포인터 연결(nil 노드의 주소 저장)
      생성한 트리의 root에 nil 포인터 연결(nil 노드의 주소 저장)

      생성한 트리 포인터 리턴
  */

  rbtree *t = (rbtree *)malloc(sizeof(rbtree));
  node_t *nil = (node_t *)malloc(sizeof(node_t));
  nil -> color = RBTREE_BLACK;
  t -> root = nil;
  t -> nil = nil;
  
  return t;
}

// 노드 제거(메모리해제)
void delete_all_node_t(node_t *p, const node_t *nil) {
  /*
      TODO : 연결된 노드를 후위순회 삭제 ( l -> r -> now )
  */
  if (p->left != nil) {
    delete_node_t(p->left, nil);
  }

  if (p->right != nil) {
    delete_node_t(p->right, nil);
  }
  free(p);
  
}

void delete_rbtree(rbtree *t) {
  /* 05-08 23:40 ~ 
    TODO: reclaim the tree nodes's memory
    노드가 남아있으면 삭제 -> 후위 순회 해야 자손을 다 삭제하고 마지막으로 자신 삭제 가능
    nil 노드 삭제 ( root의 )

    -> nil 노드부터 자손노드 후위순회 삭제 ( nil -> root -> ... 자녀가 nil이면 실질적인 leaf 노드. 삭제 시작)

    if :루트 포인터가 nil이 아니라면
      (반복,재귀)root부터 후위순회, 자손 삭제. 삭제기준 : 현재 노드의 l, r이 전부 nil (leaf node)
      삭제 = 메모리 해제 free(current node)
    
    (모든 노드 삭제 후) nil 노드 메모리해제

    다 삭제 후
    트리 포인터 기반으로 메모리 해제
  */
  node_t *root = t->root;
  node_t *nil = t->nil;

  // 노드가 남아있으면 루트부터 후위순회 삭제(메모리 해제)
  if (root != nil) {
    delete_all_node_t(root,nil);
  }
  // nil 노드 삭제(메모리 해제)
  free(nil)

  // 트리 삭제(메모리 해제)
  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
