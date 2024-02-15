#include "../db/skiplist.h"
#include "../util/arena.h"
#include <iostream>
#include <set>

using namespace std;
using namespace leveldb;

typedef uint64_t Key;

struct Comparator {
  int operator()(const Key& a, const Key& b) const {
    if (a < b) {
      return -1;
    } else if (a > b) {
      return +1;
    } else {
      return 0;
    }
  }
};

int main(){
    const int N = 2000;
    const int R = 5000;
    Random rnd(1000);
    std::set<Key> keys;
    Arena arena;
    Comparator cmp;
    SkipList<Key, Comparator> list(cmp, &arena);
    for (int i = 0; i < N; i++) {
        Key key = rnd.Next() % R;
        if (keys.insert(key).second) {
            list.Insert(key);
        }
    }

    for (int i = 0; i < R; i++) {
        if (list.Contains(i)&&!keys.count(i)) {
            std::cout<<"insert data error"<<std::endl;
        }
    }

    // Forward iteration test
    for (int i = 0; i < R; i++) {
        SkipList<Key, Comparator>::Iterator iter(&list);
        iter.Seek(i);

        // Compare against model iterator
        std::set<Key>::iterator model_iter = keys.lower_bound(i);
        for (int j = 0; j < 3; j++) {
            if (model_iter == keys.end()) {
                break;
            } else {
                ++model_iter;
                iter.Next();
            }
        }
    }

    // Backward iteration test

    SkipList<Key, Comparator>::Iterator iter(&list);
    iter.SeekToLast();

    // Compare against model iterator
    for (std::set<Key>::reverse_iterator model_iter = keys.rbegin();
        model_iter != keys.rend(); ++model_iter) {
        if(!iter.Valid()){
            std::cout<<"backward iteration error"<<std::endl;
        }
        if(*model_iter!=iter.key()){
            std::cout<<"not equal"<<std::endl;
        }
        iter.Prev();
    }

    std::cout<<"test successfully"<<std::endl;
}