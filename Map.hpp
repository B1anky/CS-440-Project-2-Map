#ifndef MAPXXYZZ
#define MAPXXYZZ

#include <utility>
#include <stdlib.h>
#include <stdio.h>
#include <random>
#include <vector>
#include <iostream>
#include <stdexcept>

namespace cs540{
	template <typename Key_T, typename Mapped_T>
	class Map{
		public:
			typedef typename std::pair<const Key_T, Mapped_T> ValueType;	

			class SkipList{
				public:
					class Node{
						public:
							Node(){
								value = nullptr;
								next.emplace_back(nullptr);
								prev.emplace_back(nullptr);
							}

							Node(size_t levels){
								value = nullptr;
								for (size_t i = 0; i < levels; ++i) next.emplace_back(nullptr);
								for (size_t i = 0; i < levels; ++i) prev.emplace_back(nullptr);
							}

							Node(const Key_T key, size_t levels){
								value = new ValueType(key, std::numeric_limits<Mapped_T>::min());
								for (size_t i = 0; i < levels; ++i) next.emplace_back(nullptr);
								for (size_t i = 0; i < levels; ++i) prev.emplace_back(nullptr);
							}


							Node(const ValueType &input, size_t levels){
								value = new ValueType(input);
								for (size_t i = 0; i < levels; ++i) next.emplace_back(nullptr);
								for (size_t i = 0; i < levels; ++i) prev.emplace_back(nullptr);
							}

							Node(const Node & other){
								if(other.value){
									value = new ValueType(*other.value);
									for (size_t i = 0; i < other.next.size(); ++i) next.emplace_back(nullptr);
									for (size_t i = 0; i < other.next.size(); ++i) prev.emplace_back(nullptr);
								}else{
									value = nullptr;
									for (size_t i = 0; i < 16; ++i) next.emplace_back(nullptr);
									for (size_t i = 0; i < 16; ++i) prev.emplace_back(nullptr);
								}
							}

							Node &operator=(const Node & other){
								if(other != this){
									~Node();
									value = new ValueType(other.value);
									prev = other->prev;
									next = other->next;
								}
								return *this;
							}

							~Node(){
								delete(value);
							}

							ValueType *value; //where value->first = Key_T and value->second = Mapped_T
							std::vector<Node*> next;
							std::vector<Node*> prev;
					};

					size_t highestLevel; //index 0 corresponds to size of base level, extends up
					float promoteChance;
					size_t curHighestLevel;
					Node* head; // each index should always have prev as nullptr
					Node* tail; // each index should always have next as nullptr
					size_t size;

					SkipList(){
						promoteChance = .5; // 50% chance starting
						highestLevel = 32;
						curHighestLevel = 0;
						size = 0;

						head = new Node(highestLevel);
						tail = new Node(highestLevel);

						for(size_t i = 0; i < head->next.size(); ++i){
							head->prev.at(i) = nullptr;
							head->next.at(i) = tail;
							tail->prev.at(i) = head;
							tail->next.at(i) = nullptr;
						}
					}

					SkipList(const ValueType & value){
						promoteChance = .5; // 50% chance starting
						highestLevel = 32;
						curHighestLevel = 0;
						size = 1;

						head = new Node(highestLevel);
						tail = new Node(highestLevel);
						Node* newNode = new Node(value);

						for(size_t i = 0; i < head->next.size(); ++i){
							head->next[i] = newNode; 
							newNode->prev[i] = head;
							newNode->next[i] = tail;
							tail->prev[i] = newNode;
						}
					}

					SkipList &operator=(const SkipList & other){
						if(this != &other){
							Node* cur;
							Node* next;
							cur = head;
							while(cur){
								next = cur->next[0];
								delete(cur);
								cur = next;
							}

							promoteChance = .5; // 50% chance starting
							highestLevel = 32;
							curHighestLevel = 0;
							size = 0;

							head = new Node(highestLevel);
							tail = new Node(highestLevel);

							for(size_t i = 0; i < head->next.size(); ++i){
								head->prev.at(i) = nullptr;
								head->next.at(i) = tail;
								tail->prev.at(i) = head;
								tail->next.at(i) = nullptr;
							}

							Node* it = other.head;
							while(it != other.tail){
								if(it->value != nullptr){
									insert(*it->value);
								}
								it = it->next[0];
							}
						}
						return *this;
					}

					~SkipList(){
						//iterate to the bottom level and delete all nodes in order like a linked list
						Node* cur;
						Node* next;
						cur = head;
						while(cur){
							next = cur->next[0];
							delete(cur);
							cur = next;
						}
						highestLevel = 0;
						size = 0;
					}

					//Credit to randomLevel() code to http://codereview.stackexchange.com/questions/116345/skip-list-implementation
					size_t randomLevel(){
						size_t value = 1;
						while((((double)std::rand()/RAND_MAX)) < promoteChance && std::abs(value) < curHighestLevel + 1 && curHighestLevel + 1 <= highestLevel){
							value++;
						}
						if(value > curHighestLevel){
							curHighestLevel++;
						}

						return abs(value);
					}

					size_t nodeLevel(const std::vector<Node*>& node){
						size_t curLevel = 1;

						if(node[0] == tail){
							return curLevel;
						}

						for(size_t i = 0; i < node.size(); ++i){
							if(node[i] != nullptr && node[i]->value != nullptr){
								++curLevel;
							}else{
								break;
							}
						}
						return curLevel;
					}

					void print(){
						Node* list = head;
						size_t curLevel = nodeLevel(list->next);
						int i = curLevel;
						for(i = curLevel; i >= 0; --i){
							list = head->next.at(i);
							std::cout << "level: " << i << ":";
							while(list->next.at(i) != nullptr/* && list->next.at(i) != tail*/){
								std::cout << "(" << list->value->first << "," << list->value->second << ")";
								list = list->next.at(i);
							}
							std::cout << std::endl;
						}
					}

					Node* createNode(ValueType value, size_t level){
						return new Node(value, level);
					}

					Node* find(const Key_T key){
						Node* x = head;
						size_t curMax = nodeLevel(head->next);

						for(size_t i = curMax; i-- > 0;){
							while(x->next[i] != nullptr && x->next[i]->value != nullptr){
								if(x->next[i]->value->first < key){
									x = x->next[i];
								}else{
									break;
								}
							}
						}

						x = x->next[0];

						if(x){
							if(x->value != nullptr){
								if(x->value->first == key){
									return x;
								}else{
									return nullptr;
								}
							}else{
								return nullptr;
							}
						}else{
							return nullptr;
						}
					}

					void insert(const ValueType item){
						Node* x = nullptr;

						x = find(item.first);
						if(x){
							return;
						}

						std::vector<Node*> update(head->next);
						size_t curMax = nodeLevel(head->next);
						x = head;

						for(size_t i = curMax; i-- > 0;){
							while(x->next[i] != nullptr){
								if(x->next[i]->value != nullptr){
									if(x->next[i]->value->first < item.first){
										x = x->next[i];
									}else{
										break;
									}
								}else{
									break;
								}
							}
							update[i] = x;
						}

						x = x->next[0];

						bool add = false;
						size_t newLevel = 1;
						if(x){
							if(x->value == nullptr){
								add = true;
							}else{
								if(x->value->first != item.first){
									add = true;
								}
							}
						}

						if(add){
							newLevel = randomLevel();
							size_t curLevel = nodeLevel(update);

							if(newLevel > curLevel){
								for(size_t i = curLevel + 1; i < newLevel; ++i){
									update[i] = head;
								}
							}
							Node* newNode = createNode(item, newLevel);

							for(size_t i = 0; i < newLevel; ++i){
								if(update[i] == tail){
						 			newNode->next[i] = tail;
						 			newNode->prev[i] = tail->prev[i];
						 			tail->prev[i]->next[i] = newNode;
						 			tail->prev[i] = newNode;
								}else if(update[i] == head){
									newNode->prev[i] = head;
									head->next[i]->prev[i] = newNode;
									newNode->next[i] = head->next[i];
									head->next[i] = newNode;
								}else{
									newNode->next[i] = update[i]->next[i];
									update[i]->next[i]->prev[i] = newNode;
									newNode->prev[i] = update[i];
									update[i]->next[i] = newNode;
								}
							}
							size++;
						}
					}

					void erase(const Key_T key){
						std::vector<Node*> iter(head->next);
						Node* cur = head;
						size_t curMax = nodeLevel(head->next);

						for(size_t i = curMax; i-- > 0;){
							while(cur->next[i] != nullptr && cur->next[i]->value != nullptr){
								if(cur->next[i]->value->first < key){
									cur = cur->next[i];
								}else{
									break;
								}
							}
							iter.at(i) = cur;
						}

						cur = cur->next.at(0);

						// update pointers and delete node
						if(cur->value != nullptr){ 
							if (cur->value->first == key) {
								for (size_t i = 0; i < iter.size(); ++i) {
									if (iter.at(i)->next.at(i) != cur) {
										break;
									}
									if(size > 1){
										iter.at(i)->next.at(i) = cur->next.at(i);
										cur->next.at(i)->prev.at(i) = iter.at(i);
									}else{
										head->next[i] = tail;
										tail->prev[i] = head;			
									}
								}
								delete cur;    
								size--;    
							}
						}  
					}
			};

			class Node;
			class ConstIterator;
			class ReverseIterator;

			class Iterator{
				public:
					typename SkipList::Node *p;

					Iterator(){
						this->p = nullptr;
					}

					Iterator(typename SkipList::Node * p){
						this->p = p;
					}

					Iterator &operator++(){
						this->p = p->next[0];
						return *this;
					}

					Iterator operator++(int){
						this->p = p->next[0];
						return *this;
					}

					Iterator &operator--(){
						this->p = p->prev[0];
						return *this;
					}

					Iterator operator--(int){
						this->p = p->prev[0];
						return *this;
					}

					ValueType &operator*() const{
						return p->value;
					}

					ValueType *operator->() const{
						return p->value;
					}

					friend bool operator!=(const typename Map<Key_T, Mapped_T>::Iterator& lhs, 
						const typename Map<Key_T, Mapped_T>::Iterator& rhs){
								return(lhs.p != rhs.p);
					}

					friend bool operator!=(const typename Map<Key_T, Mapped_T>::Iterator& lhs, 
						const typename Map<Key_T, Mapped_T>::ConstIterator& rhs){
								return(lhs.p != rhs.p);
					}

					friend bool operator!=(const typename Map<Key_T, Mapped_T>::Iterator& lhs, 
						const typename Map<Key_T, Mapped_T>::ReverseIterator& rhs){
								return(lhs.p != rhs.p);
					}

					friend bool operator==(const typename Map<Key_T, Mapped_T>::Iterator& lhs, 
						const typename Map<Key_T, Mapped_T>::Iterator& rhs){
								return(lhs.p == rhs.p);
					}

					friend bool operator==(const typename Map<Key_T, Mapped_T>::Iterator& lhs, 
						const typename Map<Key_T, Mapped_T>::ConstIterator& rhs){
								return(lhs.p == rhs.p);
					}

					friend bool operator==(const typename Map<Key_T, Mapped_T>::Iterator& lhs, 
						const typename Map<Key_T, Mapped_T>::ReverseIterator& rhs){
								return(lhs.p == rhs.p);
					}

};
			class ConstIterator{
				public:
					typename SkipList::Node *p;
					ConstIterator(){
						this->p = nullptr;
					}

					ConstIterator(const Iterator & other){
						this->p = other.p;
					}
				
					ConstIterator &operator++(){
						this->p = p->next[0];
						return *this;
					}

					ConstIterator operator++(int){
						this->p = p->next[0];
						return *this;
					}

					ConstIterator &operator--(){
						this->p = p->prev[0];
						return *this;
					}

					ConstIterator operator--(int){
						this->p = p->prev[0];
						return *this;
					}

					const ValueType &operator*() const{
						return p->value;
					}

					const ValueType *operator->() const{
						return p->value;
					}

					friend bool operator!=(const typename Map<Key_T, Mapped_T>::ConstIterator& lhs, 
						const typename Map<Key_T, Mapped_T>::ConstIterator& rhs){
								return(lhs.p != rhs.p);
					}

					friend bool operator!=(const typename Map<Key_T, Mapped_T>::ConstIterator& lhs, 
						const typename Map<Key_T, Mapped_T>::Iterator& rhs){
								return(lhs.p != rhs.p);
					}

					friend bool operator!=(const typename Map<Key_T, Mapped_T>::ConstIterator& lhs, 
						const typename Map<Key_T, Mapped_T>::ReverseIterator& rhs){
								return(lhs.p != rhs.p);
					}

					friend bool operator==(const typename Map<Key_T, Mapped_T>::ConstIterator& lhs, 
						const typename Map<Key_T, Mapped_T>::ConstIterator& rhs){
								return(lhs.p == rhs.p);
					}

					friend bool operator==(const typename Map<Key_T, Mapped_T>::ConstIterator& lhs, 
						const typename Map<Key_T, Mapped_T>::Iterator& rhs){
								return(lhs.p == rhs.p);
					}

					friend bool operator==(const typename Map<Key_T, Mapped_T>::ConstIterator& lhs, 
						const typename Map<Key_T, Mapped_T>::ReverseIterator& rhs){
								return(lhs.p == rhs.p);
					}
			};

			class ReverseIterator{
				public:
					typename SkipList::Node* p;

					ReverseIterator(){
						this->p = nullptr;
					}

					ReverseIterator &operator++(){
						this->p = p->prev[0];
						return *this;
					}

					ReverseIterator operator++(int){
						this->p = p->prev[0];
						return *this;
					}

					ReverseIterator &operator--(){
						this->p = p->next[0];
						return *this;
					}

					ReverseIterator operator--(int){
						this->p = p->next[0];
						return *this;
					}

					ValueType &operator*() const{
						return p->value;
					}

					ValueType *operator->() const{
						return p->value;
					}

					friend bool operator!=(const typename Map<Key_T, Mapped_T>::ReverseIterator& lhs, 
						const typename Map<Key_T, Mapped_T>::ReverseIterator& rhs){
								return(lhs.p != rhs.p);
					}

					friend bool operator!=(const typename Map<Key_T, Mapped_T>::ReverseIterator& lhs, 
						const typename Map<Key_T, Mapped_T>::Iterator& rhs){
								return(lhs.p != rhs.p);
					}

					friend bool operator!=(const typename Map<Key_T, Mapped_T>::ReverseIterator& lhs, 
						const typename Map<Key_T, Mapped_T>::ConstIterator& rhs){
								return(lhs.p != rhs.p);
					}

					friend bool operator==(const typename Map<Key_T, Mapped_T>::ReverseIterator& lhs, 
						const typename Map<Key_T, Mapped_T>::ReverseIterator& rhs){
								return(lhs.p == rhs.p);
					}

					friend bool operator==(const typename Map<Key_T, Mapped_T>::ReverseIterator& lhs, 
						const typename Map<Key_T, Mapped_T>::Iterator& rhs){
								return(lhs.p == rhs.p);
					}

					friend bool operator==(const typename Map<Key_T, Mapped_T>::ReverseIterator& lhs, 
						const typename Map<Key_T, Mapped_T>::ConstIterator& rhs){
								return(lhs.p == rhs.p);
					}
			};

			SkipList *skipList;
			Iterator it;
			ConstIterator c_it;
			ReverseIterator r_it;

			Map(){
				skipList = new SkipList();
			}

			Map(const Map<Key_T, Mapped_T> & other){
				skipList = new SkipList();
				*skipList = *other.skipList;
			}

			Map &operator=(const Map<Key_T, Mapped_T> & other){
				if(!skipList){
					skipList = new SkipList();
				}
				*skipList = *other.skipList;
				return *this;
			}

			Map(std::initializer_list<std::pair<const Key_T, Mapped_T>> init){
				skipList = new SkipList();
				for(auto pair: init){
					skipList->insert(pair);
				}
			}

			~Map(){
				delete(skipList);
			}

			size_t size() const{
				return skipList->size;
			}

			bool empty() const{
				if(skipList->size == 0){
					return true;
				}else{
					return false;
				}
			}

			Iterator begin(){
				it.p = skipList->head->next[0];
				return it;
			}

			Iterator end(){
				it.p = skipList->tail;
				return it;
			}

			ConstIterator begin() const{
				ConstIterator c_it;
				c_it.p = skipList->head->next[0];
				return c_it;
			}

			ConstIterator end() const{
				ConstIterator c_it;
				c_it.p = skipList->tail;
				return c_it;
			}

			ReverseIterator rbegin(){
				ReverseIterator r_it;
				r_it.p = skipList->tail->prev[0];
				return r_it;
			}

			ReverseIterator rend(){
				ReverseIterator r_it;
				r_it.p = skipList->head;
				return r_it;
			}

			Iterator find(const Key_T & key){
				Iterator it;
				typename SkipList::Node *node = skipList->find(key);
				if(node){
					it.p = node;
				}else{
					it = end();
				}
				return it;
			}

			ConstIterator find(const Key_T & key) const{
				ConstIterator c_it;
				typename SkipList::Node *node = skipList->find(key);
				if(node){
					c_it.p = node;
				}else{
					c_it.p = end();
				}
				return c_it;
			}

			Mapped_T &at(const Key_T & key){
				typename SkipList::Node *node = skipList->find(key);
				if(node == nullptr){
					throw std::out_of_range("map::at");
				}
				return node->value->second;
			}

			const Mapped_T &at(const Key_T & key) const{
				typename SkipList::Node *node = skipList->find(key);
				if(node == nullptr){
					throw std::out_of_range("map::at");
				}
				return node->value->second;
			}

			Mapped_T &operator[](const Key_T & key){
				typename SkipList::Node *node = skipList->find(key);
				if(node == nullptr){
					insert(std::make_pair(key, Mapped_T()));
					node = skipList->find(key);
				}
				
				return node->value->second;	
			}

			std::pair<Iterator, bool> insert(const ValueType & value){
				Iterator it;
				bool inserted = false;
				typename SkipList::Node *node = skipList->find(value.first);
				if(node){
					it.p = node;
				}else{
					skipList->insert(value);
					it.p = skipList->find(value.first);
					inserted = true;
				}
				return std::make_pair(it, inserted);
			}

			template <typename IT_T>
			void insert(IT_T range_beg, IT_T range_end){
				for(auto it = range_beg; it != range_end; ++it){
					skipList->insert(*it);
				}
			}

			void erase(Iterator pos){
				skipList->erase(pos->first);
			}

			void erase(const Key_T & key){
				typename SkipList::Node *node = skipList->find(key);
				if(node == nullptr){
					throw std::out_of_range("map::erase");
				}else{
					skipList->erase(key);
				}
			}

			void clear(){
				delete(skipList);
				skipList = new SkipList();
			}

			void print(){
				skipList->print();
			}

			friend bool operator==(const Map<Key_T,Mapped_T> &lhs, const Map<Key_T,Mapped_T> &rhs){
				typename Map<Key_T, Mapped_T>::ConstIterator leftMapIt = lhs.begin();
				typename Map<Key_T, Mapped_T>::ConstIterator rightMapIt = rhs.begin();
				if(lhs.size() != rhs.size()){
					return false;
				}
				while(leftMapIt != lhs.end()){
					if(leftMapIt.p->value->second == rightMapIt.p->value->second){
						leftMapIt++;
						rightMapIt++;
					}else{
						return false;
					}
				}
				return true;
			}
			
			friend bool operator!=(const Map<Key_T,Mapped_T> &lhs, const Map<Key_T,Mapped_T> &rhs){
				if(lhs.size() != rhs.size()){
					return true;
				}
				return !(lhs == rhs);
			}

			friend bool operator<(const Map<Key_T,Mapped_T> &lhs, const Map<Key_T,Mapped_T> &rhs){
				typename Map<Key_T, Mapped_T>::ConstIterator leftMapIt = lhs.begin();
				typename Map<Key_T, Mapped_T>::ConstIterator rightMapIt = rhs.begin();
				while(leftMapIt != lhs.end()){
					if(leftMapIt.p->value->second < rightMapIt.p->value->second){
						leftMapIt++;
						rightMapIt++;
					}else{
						return false;
					}
				}
				return true;
			}
	};
}
#endif
