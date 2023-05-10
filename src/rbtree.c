#include "rbtree.h"

#include <stdlib.h>

// 00:52 추가 : 노드 생성(메모리할당, 선언, key, color 까지만 초기화)
node_t *new_node_t(const key_t k, const color_t c) {
  node_t *n = (node_t *)malloc(sizeof(node_t));
  n -> key = k;
  n -> color = c;

  return n;
}
rbtree *new_rbtree(void) {
  
  /* 05-08 23:30
    initialize struct if needed
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
  nil->color = RBTREE_BLACK;
  // node_t *nil = new_node_t(NULL, RBTREE_BLACK);
  // 
  /*
    -> rbtree.c:6:32: note: expected ‘key_t’ {aka ‘const int’} but argument is of type ‘void *’
    int * 대신 void * 되어버려서 캐스팅 오류
  */
  t->root = nil;
  t->nil = nil;
  
  return t;
}

// 연결된 모든 노드 제거(메모리해제)
void delete_all_node_t(node_t *p, const node_t *nil) {
  /*  05-08 23:40 ~ 05-09 00:21
      DONE : 연결된 노드를 후위순회 삭제 ( l -> r -> now )
  */
  if (p->left != nil) {
    delete_all_node_t(p->left, nil);
  }

  if (p->right != nil) {
    delete_all_node_t(p->right, nil);
  }
  free(p);
  
}

void delete_rbtree(rbtree *t) {
  /* 05-08 23:40 ~ 
    reclaim the tree nodes's memory
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
  free(nil);

  // 트리 삭제(메모리 해제)
  free(t);
}

// TREE-SUCCESSOR : 오른쪽 서브트리 최솟값으로 표현. 직후원소
// TREE-PREDECESSOR : 왼쪽 서브트리 최댓값으로 표현. 직전원소

// TRANSPLANT (ERASE SUB RUTINE) FOR RBTREE (NULL -> SENTINEL NODE)
void rbtree_transplant(rbtree *t, node_t *u, node_t *v) {
  if (u->parent == t->nil){
    t->root = v;
  } else if (u == u->parent->left){
    u->parent->left = v;
  } else {
    u->parent->right = v;
  }
  v->parent = u->parent;
}
// LEFT-ROTATE (x.right != T.nil)
void rbtree_left_rotate(rbtree *t, node_t *x) {
  node_t *y = x->right;
  x->right = y->left;
  if(y->left != t->nil) {
    y->left->parent = x;
  }
  y->parent =x->parent;
  if(x->parent == t->nil) {
    t->root = y;
  } else if(x == x->parent->left){
    x->parent->left = y;
  } else {
    x->parent->right = y;
  }
  y->left = x;
  x->parent = y;
}
// RIGHT-ROTATE (y.left != T.nil)
void rbtree_right_rotate(rbtree *t, node_t *y) {
  node_t x = y->left;
  y->left = x->right;
  if(x->right != t->nil){
    x->right->parent = y;
  }
  x->parent = y->parent;
  if(y->parent == t->nil){
    t->root = x;
  }else if(y== y->parent->left){
    y->parent->left = x;
  } else {
    y->parent->right = x;
  }
  x->right = y;
  y->parent = x;
}

// INSERT-FIXUP
void rbtree_insert_fixup(rbtree *t, node_t *current) {
  node_t *z = current;  // 작업 기준이 되는 node z
  while(z->parent->color == RBTREE_RED){
    if(z->parent == z->parent->parent->left){
      node_t *y = z->parent->parent->right;
      if(y->color == RBTREE_RED){
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      } else if(z== z->parent->right){
        z = z->parent;
        rbtree_left_rotate(t,z);
      } else {
        z->parent->color = RBTREE_BLACK;
        z->parent->color = RBTREE_RED;
        rbtree_right_rotate(t,z->parent->parent);
      }
    } else {
      node_t *y = z->parent->parent->left;
      if(y->color == RBTREE_RED){
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      } else if(z== z->parent->left){
        z = z->parent;
        rbtree_left_rotate(t,z);
      } else {
        z->parent->color = RBTREE_BLACK;
        z->parent->color = RBTREE_RED;
        rbtree_right_rotate(t,z->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}
// DELETE-FIXUP
void rbtree_delete_fixup(rbtree *t, node_t x) {
  
} 

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // implement insert
  /* 05-09 00:28 ~
    노드 추가하고, 트리에 알맞게 관계 적용해주고, 트리 정합성 체크하여 변경작업
    리턴 : 새로 추가한 노드


  */
  node_t *z = new_node_t(key, RBTREE_RED);
  node_t *y = t->nil;
  node_t *x = t->root;

  while(x != t->nil){
    y = x;
    if(z->key < x->key){
      x = x->left;
    } else {
      x = x->right;
    }
  }
  z->parent = y;
  if (y== t->nil){
    t->root = z;
  } else if (z->key < y->key) {
    y->left = z;
  } else {
    y->right = z;
  }
  z->left = t->nil;
  z->right = t->nil;
  //z->color = RBTREE_RED
  rbtree_insert_fixup(t,z);
  return z;
}
node_t *rbtree_node_find(const node_t *p, const key_t key, const node_t *nil) {

  if(p == nil || key == p->key){
    return p;
  }

  if (key < p->key){
    return rbtree_node_find(p->left,key,nil);
  } else {
    return rbtree_node_find(p->right,key,nil);
  }
}
node_t *rbtree_find(const rbtree *t, const key_t key) {
  // implement find

  node_t *result = rbtree_node_find(t->root,key,t->nil);
  if (result == t->nil){
    return NULL;
  } else {
    return result;
  }

}
// 추가 : 노드기준 min
node_t *subtree_min(const node_t *root, const node_t *nil) {
  /*

    이진검색트리에서 최소의 키를 가지는 원소
    nil을 만날 때까지 루트로부터 왼쪽 자식을 계속 따라감.
    (nil이 아니라고 가정하는) 주어진 노드 x를 루트로 가지느 서브트리의 최소 원소에 대한 서브트리를 리턴
  */
  node_t *x = root;
  while(x->left !=nil) {
    x = x->left;
  }
  return x;
}

node_t *rbtree_min(const rbtree *t) {
  /*
    implement min (TREE-MINIMUM(x))

    이진검색트리에서 최소의 키를 가지는 원소
    nil을 만날 때까지 루트로부터 왼쪽 자식을 계속 따라감.
    (nil이 아니라고 가정하는) 주어진 노드 x를 루트로 가지느 서브트리의 최소 원소에 대한 서브트리를 리턴

    + 트리에 노드가 없으면 NULL 리턴
  */
  if (t->root != t->nil) {
    return subtree_min(t->root, t->nil);
  } else {
    return NULL;
  }
}

// 추가 : 노드기준 max
node_t *subtree_max(const node_t *root, const node_t *nil) {
  /*
    이진검색트리에서 최대의 키를 가지는 원소
    nil을 만날 때까지 루트로부터 오른쪽 자식을 계속 따라감.
    (nil이 아니라고 가정하는) 주어진 노드 x를 루트로 가지느 서브트리의 최대 원소에 대한 서브트리를 리턴
  */
  node_t *x = root;
  while(x->right !=nil) {
    x = x->right;
  }
  return x;
}
node_t *rbtree_max(const rbtree *t) {
  /*
    implement max (TREE-MAXIMUM(x))

    이진검색트리에서 최대의 키를 가지는 원소
    nil을 만날 때까지 루트로부터 오른쪽 자식을 계속 따라감.
    (nil이 아니라고 가정하는) 주어진 노드 x를 루트로 가지느 서브트리의 최대 원소에 대한 서브트리를 리턴

    + 트리에 노드가 없으면 NULL 리턴
  */
  if (t->root != t->nil) {
    return subtree_max(t->root, t->nil);
  } else {
    return NULL;
  }
}

int rbtree_erase(rbtree *t, node_t *z) {
  /*
    implement erase (RB-DELETE)
  */
  node_t y = z;
  node_t x;
  color_t y_original_color = y->color;
  if(z->left == t->nil) {
    x = z->right;
    rbtree_transplant(t, z, z->right);
  } else if (z->right == t->nil) {
    x = z->left;
    rbtree_transplant(t,z,z->left);
  } else {
    y=subtree_min(z->right);
    y_original_color = y->color;
    x= y->right;
    if(y->parent == z){
      x->parent = y;
    } else {
      rbtree_transplant(t,y,y->right);
      y->right = z->right;
      y->right->parent = y;
    }
    rbtree_transplant(t,z,y);
    y->left = z->left;
    y->left->parent = y;
    y->color = z->color;
  }

  if(y_original_color == RBTREE_BLACK){
    // RB_DELETE_FIXUP(T,x);
  }
  free(z);  // 처리 완료 후, z 노드 삭제(메모리해제)
  return 0; // 정상처리를 의미
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
