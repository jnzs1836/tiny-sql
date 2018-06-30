//
// Created by ktwzj on 2018/6/15.
//

#ifndef INDEXMANAGER_BPLUSTREE_H
#define INDEXMANAGER_BPLUSTREE_H

#include <iostream>
#include "tree.h"
#include <vector>
#include <algorithm>
#include "logger.h"
/*
 * Define constants
 */
#define MAX_CHAR  15
#define MAX_INT   509
#define MAX_FLOAT 509
#define ADDRESS_BITS 12
#define OFFSET_BITS 20
#define F_OK 0
#define SINGLE 0
#define EQUAL 0
#define NOT_EQUAL 1
#define GREATER 3
#define GREATER_OR_EQUAL 5
#define LESS 2
#define LESS_OR_EQUAL 4
#define NOTLESS 2
#define NOTLARGER 3
//#define LESS 2

//#define EQUAL 5
#define NOT_FOUND -1

#define TO_GREATER_NOT_INCLUDE 10
#define TO_SMALLER_NOT_INCLUDE 9
#define TO_GREATER_INCLUDE 8
#define TO_SMALLER_INCLUDE 7
#define MY_CHAR_TYPE_SIZE 255
#define MAX_NODE_NUM 100

//typedef struct IndexManager::
typedef int addressType;
namespace IndexManager {

	


    struct MyCharType{
        char data[MY_CHAR_TYPE_SIZE];
        bool operator==(const MyCharType b) const{
            int i = 0;
            bool flag = true;
            for(i = 0; i < MY_CHAR_TYPE_SIZE; i++){
                if(data[i] == 0 && b.data[i] == 0){
                    break;
                }
                if(data[i] != b.data[i]){
                    flag = false;
                }
            }
            return flag;
		};
		bool operator<(const MyCharType b) const {
			auto myString = std::string(data);
			auto bString = std::string(b.data);
			return myString < bString;
		};
		bool operator>(const MyCharType b) const {
			auto myString = std::string(data);
			auto bString = std::string(b.data);
			return myString > bString;
		};
		bool operator<=(const MyCharType b) const {
			auto myString = std::string(data);
			auto bString = std::string(b.data);
			return myString <= bString;
		};
		bool operator>=(const MyCharType b) const {
			auto myString = std::string(data);
			auto bString = std::string(b.data);
			return myString >= bString;
		};
	} ;

	
    typedef struct {
        bool used[MAX_NODE_NUM];
    } NodeMemoryTable;
    typedef struct {
        int num;
        int rootPosition;
        char msg[10];
        NodeMemoryTable nodeMemoryTable;
    } MetaData;

    typedef struct
    {
        int key[MAX_INT];       //key value
        int pointers[MAX_INT]; //in interior node it ponit to the child node
        addressType next;
        addressType prev;
        //in leaf node, the low 12 bit is the offset number
        //              the high 20 bit is the block number
        int  numOfKey;              //the number of existing key
        bool isLeaf;           //will be true if the node is leaf node, otherwise it will be false
    } BPlusNode;
    typedef struct
    {
        int key[MAX_INT + 1];       //key value
        int pointers[MAX_INT + 2]; //in interior node it ponit to the child node

        //in leaf node, the low 12 bit is the offset number
        //              the high 20 bit is the block number
        int  numOfKey;              //the number of existing key
        bool isLeaf;           //will be true if the node is leaf node, otherwise it will be false
    } BPlusNodeInt;
    typedef struct
    {
        MyCharType key[MAX_CHAR + 1];       //key value
        int pointers[MAX_CHAR + 2]; //in interior node it ponit to the child node
        //in leaf node, the low 12 bit is the offset number
        //              the high 20 bit is the block number
        int  numOfKey;              //the number of existing key
        bool isLeaf;           //will be true if the node is leaf node, otherwise it will be false
    } BPlusNodeChar;

    typedef struct
    {
        float key[MAX_FLOAT + 1];       //key value
        int pointers[MAX_FLOAT + 2]; //in interior node it ponit to the child node

        //in leaf node, the low 12 bit is the offset number
        //              the high 20 bit is the block number
        int  numOfKey;              //the number of existing key
        bool isLeaf;           //will be true if the node is leaf node, otherwise it will be false
    } BPlusNodeFloat;

    typedef struct {
        MyCharType key;
        int data;
        int type;
    } CharKey;

    typedef struct{
        int key;
        addressType data;
        int type;
    }IntKey;
    typedef struct {
        float key;
        addressType data;
        int type;
    } FloatKey;
    enum TraversalType {include_first,include_last,include_both,exclude_both};
    template <class T, class KeyType>
    class BPlusTree :public Tree<T,KeyType> {
    public:
		int initNewTree();
        bool removeKey(KeyType key);
        bool adjustAfterRemove(T pNode);
        BPlusTree(void *memory);
        ~BPlusTree();
        bool insert(KeyType key);
        bool remove(KeyType key);
//        bool update(KeyType oldKey, KeyType newKey);
//        bool search();
        void test();
        void splitBPlusNode( T* child, T* parent, int num);
        addressType searchNode(KeyType key);
        std::vector<addressType> searchNodes(KeyType key);

    private:
        void adjustLeafNodesFromRightToLeft(T* father, T*leftChild, T* rightChild,addressType fatherAddress,int splitKey);
        void adjustLeafNodesFromLeftToRight(T* father, T*leftChild, T* rightChild,addressType fatherAddress,int splitKey);
        void adjustNonLeafNodesFromRightToLeft(T* father, T*leftChild, T* rightChild,addressType fatherAddress,int splitKey);
        void adjustNonLeafNodesFromLeftToRight(T* father, T*leftChild, T* rightChild,addressType fatherAddress,int splitKey);
        void mergeLeafNodes(T* father, T* leftChild, T* rightChild,addressType fatherAddress,int splitKey);
        void mergeNonLeafNodes(T* father, T* leftChild, T* rightChild,addressType fatherAddress,int splitKey);
        void emptyTree();
        void deleteKey(KeyType target);
        int nextPointer;
        T findLeftestNode();
        T recursiveFindLeftestNode(addressType address);
        T findNode(KeyType target);
        T recursiveFindNode(KeyType target, addressType address);
        void setEmptyNode(T* node);
        void clearNode(addressType address);
        int nodesOffset;
        int nodeSize;
        Logger logger;
        void *memoryAddress;
        MetaData metaData;
        bool isEnd(T* node);
        T *convertAddress(addressType address);
        addressType mapAddress(addressType rawAddress);
        std::vector<addressType> traversalLinks(T* anchor, int type, KeyType target);
        std::vector<addressType> traversalLinksInterval(T* first, T* last, KeyType firstKey, KeyType lastKey, TraversalType traversalType);
//        void clearNode(addressType node);
        int maxChildrenNum;
        void recursiveInsert(KeyType& target, addressType address);
        int recursiveRemove(KeyType target, addressType address);
        addressType recursiveSearchNode(KeyType target, addressType address);
        std::vector<addressType> recursiveSearchNodes(KeyType key);
        int offset;
        int numOfNode;
        int createBPlusNode();
        long rootPosition;
//        fstream ioFile;
        int readBPlusNode(int address, T* bPlusNode);
        int writeBPlusNode(int address, T* bPlusNode);
        int writeBPlusNode(int address, T bPlusNode);
        int createBPlusTreeFile(std::string filename, std::string path);
    };
    template <class T, class KeyType>
    BPlusTree<T, KeyType>::BPlusTree(void *memory) {
        nodeSize = sizeof(T);
        nodesOffset = sizeof(MetaData);
        maxChildrenNum = 100;
        memoryAddress = memory;
        maxChildrenNum = 4;
        memcpy(&metaData,memory,sizeof(MetaData));
        logger =  Logger();
        logger.info(metaData.msg);
        nextPointer = maxChildrenNum + 1;
		rootPosition = metaData.rootPosition;
    }

    template <class T, class KeyType>
    BPlusTree< T, KeyType>::~BPlusTree() {
		memcpy(memoryAddress, &metaData, sizeof(MetaData));
        std::cout << "B plus tree is cleaned.";
    }

    template <class T, class KeyType>
    void BPlusTree< T,  KeyType>::test() {
        addressType  a = createBPlusNode();
        std::cout << "a: " << a << std::endl;
        T node;
        writeBPlusNode(a,&node);
        addressType  v = createBPlusNode();
        std::cout << "v: " << v << std::endl;
        KeyType key;
        key.data = 1;
        key.key = 2;
        insert(key);
        KeyType key2;
        key2.data = 2;
        key2.key = 3;
        KeyType key3;
        key3.data = 3;
        key3.key = 5;
        KeyType key4;
        key4.data = 4;
        key4.key = 9;
        KeyType key5;
        key5.data = 5;
        key5.key = 8;
        key5.type = LESS;
        KeyType key6;
        key6.data = 6;
        key6.key = 10;
        KeyType key7;
        key7.data = 7;
        key7.key = 11;
        KeyType key8;
        key8.data = 8;
        key8.key = 19;
        T rootNode;
//        readBPlusNode(5,&demo);
        insert(key2);
        insert(key3);
        insert(key4);
        insert(key5);
        insert(key6);
        insert(key7);
        insert(key8);
        remove(key);

        readBPlusNode(rootPosition,&rootNode);
        T leftNode;
        T rightNode;
        readBPlusNode(rootNode.pointers[0],&leftNode);
        readBPlusNode(rootNode.pointers[1],&rightNode);
        mergeLeafNodes(&rootNode,&leftNode,&rightNode,rootPosition,0);
//        adjustLeafNodesFromLeftToRight(&rootNode,&leftNode,&rightNode,rootPosition,0);
//        deleteKey(key2);
//        auto found = searchNodes(key5);
//        for (auto it = found.begin(); it != found.end();it++)
//        {
//            std::cout << (*it) << std::endl;
//            cout << (*it).x << endl;
//            cout << (*it).y << endl;
//        }
        std::cout << "heloo";

    }


    template <class T, class KeyType>
    void BPlusTree< T,  KeyType>::deleteKey(KeyType target){
        auto leftestNode = findLeftestNode();
        std::vector<KeyType> keeps;
        T currentNode = leftestNode;

        while(currentNode.pointers[nextPointer] >= 0 ){
            for(int i = 0; i < currentNode.numOfKey; i++){
                if(currentNode.key[i] != target.key){
                KeyType get;
                get.key = currentNode.key[i];
                get.data = currentNode.pointers[i];
                keeps.push_back(get);

                }
            }
            readBPlusNode(currentNode.pointers[nextPointer],&currentNode);
        }
        for(int i = 0; i < currentNode.numOfKey; i++){
            if(currentNode.key[i] != target.key){
                KeyType get;
                get.key = currentNode.key[i];
                get.data = currentNode.pointers[i];
                keeps.push_back(get);

            }
        }
        emptyTree();
        for (auto it = keeps.begin(); it != keeps.end();it++)
        {
            insert(*it);
        }

    };
	template <class T, class KeyType>
	int BPlusTree< T, KeyType>::initNewTree() {
		metaData.num = 0;
		for (int i = 0; i < MAX_NODE_NUM; i++) {
			metaData.nodeMemoryTable.used[i] = false;
		}
		metaData.num = 0;
		memcpy(memoryAddress, &metaData, sizeof(MetaData));
	}


    template <class T, class KeyType>
    void BPlusTree< T,  KeyType>::emptyTree() {
        rootPosition = 0;
        for(int i =0; i < MAX_NODE_NUM; i++){
            metaData.nodeMemoryTable.used[i] = false;
        }
        metaData.rootPosition = -1;
        metaData.num = 0;
        numOfNode = 0;

    }
    template <class T, class KeyType>
    bool BPlusTree< T,  KeyType>::insert(KeyType key) {
        T r;
        if(metaData.num == 0){
            rootPosition = createBPlusNode();
            metaData.rootPosition = rootPosition;
            T root;
            root.numOfKey = 1;
            root.isLeaf = true;
            root.key[0] = key.key;
            root.pointers[0] = key.data;
            root.pointers[nextPointer] = -1;
            writeBPlusNode(rootPosition,root);
            //metaData.num++;
			memcpy(memoryAddress, &metaData, sizeof(MetaData));
            return true;
        }

        T currentNode;
        readBPlusNode(rootPosition,&currentNode);
        std::vector<T> nodePath;
        std::vector<int> pList;
        std::vector<addressType> addressList;
        auto currentAddress = rootPosition;
        int flag;
        while(!currentNode.isLeaf){

            flag = currentNode.numOfKey;
            for(int i = 0; i < currentNode.numOfKey; i++){
                if(currentNode.key[i] > key.key ){
                    flag = i;
                    break;
                }
            }
            addressList.push_back(currentAddress);
            T get = currentNode;
            nodePath.push_back(get);
            pList.push_back(flag);
            /*
             * Next Node
             */
            currentAddress = currentNode.pointers[flag];
            readBPlusNode(currentAddress,&currentNode);
        }

        int keyPos = currentNode.numOfKey;
        for(int i = 0; i < currentNode.numOfKey; i++){
            if(currentNode.key[i] > key.key){
                keyPos = i;
                break;
            }
        }
        for(int i = currentNode.numOfKey; i >keyPos; i-- ){
            currentNode.key[i] = currentNode.key[i-1];
            currentNode.pointers[i] = currentNode.pointers[i-1];
        }
        currentNode.numOfKey++;
        currentNode.key[keyPos] = key.key;
        currentNode.pointers[keyPos] = key.data;
        writeBPlusNode(currentAddress,currentNode);
//        auto child = nodePath.back();
//        nodePath.pop_back();
//        T father = nodePath.back();
//        auto childNum = pList.back();
//        pList.pop_back();


        /*
         * Solve the leaf node
         */

//        for(int i = currentNode.numOfKey; i >keyPos; i-- ){
//            child.key[i] = child.key[i-1];
//            child.pointers[i+1] = child.pointers[i];
//            child.numOfKey++;
//        }

//        child.pointers[keyPos] = key.data;
//        if(keyPos == 0){
//            auto father_num = pList.back();
//            father.key[father_num] = child.key[0];
//        }
//        T father = nodePath.back();
        T father;
//        T addressLeaf = addressList.back();
//        writeBPlusNode(addressLeaf,child);
        flag = keyPos;
        while(!nodePath.empty()){
            father = nodePath.back();
            nodePath.pop_back();
            auto childIndex = pList.back();
            auto address = addressList.back();
            addressList.pop_back();
//            if(flag == 0){
//                father.key[childIndex] = key.key;
//            }
            if(currentNode.numOfKey > maxChildrenNum){
                splitBPlusNode(&currentNode,&father,childIndex);
            }else{

            }

            writeBPlusNode(currentAddress,currentNode);
            writeBPlusNode(address,father);
            currentNode = father;
            currentAddress = address;
            flag = pList.back();
//            childNum = pList.back();
            pList.pop_back();
        }
        /*
         * The root Node
         */
        if(currentNode.numOfKey > maxChildrenNum){
            T newRoot;
            auto newRootPosition = createBPlusNode();
            newRoot.pointers[0] = rootPosition;
            newRoot.numOfKey = 0;
            newRoot.isLeaf = false;
            splitBPlusNode(&currentNode,&newRoot,0);
            writeBPlusNode(newRootPosition,newRoot);
            writeBPlusNode(rootPosition,currentNode);
            rootPosition = newRootPosition;
			metaData.rootPosition = rootPosition;
        }
		memcpy(memoryAddress, &metaData, sizeof(MetaData));
        return true;


//
//        readBPlusNode(rootPosition,&r);
//        if (r.numOfKey == maxChildrenNum)
//        {
//            T newRoot;
//            newRoot.numOfKey = 0;
//            newRoot.isLeaf = false;
//            newRoot.pointers[0] = rootPosition;
//            splitBPlusNode(newRoot,r,0);
//            writeBPlusNode(rootPosition, r);
//            rootPosition = createBPlusNode();
//            writeBPlusNode(rootPosition, newRoot);
//        }
//        insertKey(root, tar);
    }
    template <class T, class KeyType>
    int BPlusTree<T, KeyType>::readBPlusNode(int address, T *bPlusNode) {
        if(!metaData.nodeMemoryTable.used[address]){
            std::cout << "you are getting " << address << std::endl;
        }
        auto myOffset = address * nodeSize + nodesOffset;
        auto myAddress = (T*)(myOffset + (char*)memoryAddress);
        memcpy(bPlusNode,myAddress,nodeSize);
        return 1;
//        bPlusNode =
    }


    template <class T, class KeyType>
    void BPlusTree<T, KeyType>::clearNode(addressType address){
	
		metaData.nodeMemoryTable.used[address] = false;
		metaData.num--;
		memcpy(memoryAddress, &metaData, sizeof(MetaData));
    };


    template <class T, class KeyType>
    int BPlusTree<T, KeyType>::writeBPlusNode(int address, T *bPlusNode) {
        auto myOffset = address * nodeSize + nodesOffset;
        auto myAddress = (T*)(myOffset + (char*)memoryAddress);
        memcpy(myAddress,bPlusNode,nodeSize);
		memcpy(memoryAddress, &metaData, sizeof(MetaData));
        return 1;
    }

    template <class T, class KeyType>
    int BPlusTree<T, KeyType>::writeBPlusNode(int address, T bPlusNode) {
        auto myOffset = address * nodeSize + nodesOffset;
        auto myAddress = (T*)(myOffset + (char*)memoryAddress);
        memcpy(myAddress,&bPlusNode,nodeSize);
		memcpy(memoryAddress, &metaData, sizeof(MetaData));
        return 1;
    }

    template <class T, class KeyType>
    void BPlusTree<T, KeyType>::setEmptyNode(T* node) {
        for(int i = 0; i < maxChildrenNum; i++){
            node->key[i] = 0;
        }
    }
    template <class T, class KeyType>
    void BPlusTree<T, KeyType>::recursiveInsert(KeyType& target, addressType address) {
        T x;
        readBPlusNode(address, x);
        int i;
        for ( i = 0; i < x.numOfKey && x.key[i] < target.key; i++);

        if (i < x.numOfKey && x.isLeaf && x.key[i] == target.key)
        {
            return;
        }
        if (!x.isLeaf)
        {
            T y;
            readBPlusNode(x.pointers[i], y);

            if (y.numOfKey == maxChildrenNum)
            {
                splitBPlusNode(x,y,i);
//                splitBplusNodeInt(x, y, i);
                writeBPlusNode(address, &x);
                writeBPlusNode(x.pointers[i], &y);
            }
            if (target.key <= x.key[i] || i == x.numOfKey)
            {
                recursiveInsert(target, x.pointers[i]);
            }
            else
            {
                recursiveInsert(target, x.pointers[i+1]);
            }
        }
        else          //�����Ҷ�ڵ�,��ֱ�ӽ��ؼ��ֲ���key������
        {

            for (int j = x.numOfKey; j > i; j--)
            {
                x.key[j] = x.key[j - 1];
                x.pointers[j] = x.pointers[j - 1];
            }
            x.key[i] = target.key;
            x.numOfKey++;
            x.pointers[i] = offset + ( numOfNode << 12 );
//                    target.index.offset + (target.index.num << 12);

            writeBPlusNode(address, x);

        }
    }

    template <class T, class KeyType>
    void BPlusTree<T, KeyType>::splitBPlusNode(T *child, T *parent, int num) {
        int half = maxChildrenNum / 2;

        int i;

        for (i = parent->numOfKey; i > num; i--)
        {
            parent->key[i] = parent->key[i - 1];
            parent->pointers[i + 1] = parent->pointers[i];
        }
        parent->numOfKey++;

        T t;
        addressType address = createBPlusNode();

        parent->key[num] = child->key[half];
        parent->pointers[num + 1] = address;

        for (i = half + 1; i < maxChildrenNum + 1; i++)
        {
            t.key[i - half ] = child->key[i];
            t.pointers[i - half ] = child->pointers[i];
        }
        t.key[0] = child->key[half];

        t.numOfKey = maxChildrenNum + 1 - half ;
        t.pointers[t.numOfKey] = child->pointers[maxChildrenNum];

        t.isLeaf = child->isLeaf;

        child->numOfKey = half;

        if (child->isLeaf)
        {
            t.pointers[0] = child->pointers[half];
//            child->numOfKey++;
            t.pointers[nextPointer] = child->pointers[nextPointer];
            child->pointers[nextPointer] = address;
        }

        writeBPlusNode(address,t);
    }


    template <class T, class KeyType>
    bool BPlusTree<T,KeyType>::remove(KeyType key) {
        recursiveRemove(key,rootPosition);
        T root;
        readBPlusNode(rootPosition,&root);

        if (!root.isLeaf && root.numOfKey == 0)
        {
            clearNode(rootPosition);
            rootPosition = root.pointers[0];
//            root = rootnode.Pointer[0];
        }
        return true;
    }

    template <class T, class KeyType>
    int BPlusTree<T,KeyType>::recursiveRemove(KeyType target, addressType address) {
        int i, j;
        T x;
        readBPlusNode(address,&x);

        for (i = 0; i < x.numOfKey;  i++){
            if(target.key <= x.key[i]){
                break;
            }
        }

        if (true)  //?????
        {

            if (!x.isLeaf)     //���ڽڵ��ҵ��ؼ���
            {
                T child;
//                BplusNodeInt child;
                addressType childAdd = x.pointers[i];
                readBPlusNode(x.pointers[i], &child);
                if (child.isLeaf)     // child is a leaf node
                {
                    int foundKey = -1;
                    for(int h = 0; h < child.numOfKey; h++ ){
                        if(child.key[h] == target.key){
                            foundKey = h;
                            break;
                        }
                    }
                    if(foundKey<0){
                        return -1;
                        // not found;
                    }else{
                        for(int h = foundKey; h < child.numOfKey - 1; h++){
                            child.key[h] = child.key[h+1];
                            child.pointers[h] = child.pointers[h+1];
                        }
                        child.numOfKey--;
                    }

                    if (child.numOfKey > maxChildrenNum / 2) {
//                        x.key[i] = child.key[child.numOfKey - 2];
//                        child.numOfKey--;
                        writeBPlusNode(address, &x);
                        writeBPlusNode(x.pointers[i], &child);
                        return 1;
                    } else {
                        if (i > 0)      // left sibling exist
                        {
                            T leftChild;
                            readBPlusNode(x.pointers[i - 1], &leftChild);

                            if (leftChild.numOfKey > maxChildrenNum / 2) {
                                adjustLeafNodesFromLeftToRight(&x,&leftChild,&child,address,i-1);
                            } else    // need merging
                            {
                                mergeLeafNodes(&x,&leftChild,&child,address,i-1);
                            }
                        } else      //left sibling does not exist.
                        {
                            T rightChild;
                            addressType rightChildAddress = x.pointers[i + 1];
                            readBPlusNode(x.pointers[i + 1], &rightChild);

                            if (rightChild.numOfKey > maxChildrenNum / 2)        //adjust
                            {
                                adjustLeafNodesFromLeftToRight(&x,&child,&rightChild,address,i);

                            } else    //need merging
                            {
                               mergeLeafNodes(&x,&child,&rightChild,address,i);
                            }

                        }

                    }
                } else      // the child is not a leaf node
                {
                    recursiveRemove(target,childAdd);

//                    KeyType target2;
//                    target2.indexFileName = target.indexFileName;
//                    target2.key = target.key;
//                    target2.type = SINGLE;
//                    addressType resultAdd;
//                    vector <index_info> result;
//                    result = searchBplusTree(target2, resultAdd);

//                    T last;

//                    read
//                    readBPlusNode(resultAdd, &last);
//
//                    x.key[i] = last.key[last.numOfKey - 2];

//                    writeBPlusNode(address, x);


                    if (child.numOfKey > maxChildrenNum / 2)        //do not need to adjust
                    {

                    } else          // do
                    {
                        if (i > 0)  // there is a left sibling existing
                        {
                            T leftChild;
                            readBPlusNode(x.pointers[i - 1], &leftChild);

                            if (leftChild.numOfKey > maxChildrenNum / 2)       // adjust
                            {
                                adjustNonLeafNodesFromLeftToRight(&x,&leftChild,&child,address,i-1);
                            } else        //need merging
                            {
                                mergeNonLeafNodes(&x,&leftChild,&child,address,i-1);
                                i--;
                            }

                        } else        //����x.key[i]ֻ�����ֵ�
                        {
                            T rightChild;
                            addressType rightChildAdd = x.pointers[i + 1];
                            readBPlusNode(x.pointers[i + 1], &rightChild);

                            if (rightChild.numOfKey > maxChildrenNum / 2)     //adjust
                            {
                                adjustNonLeafNodesFromRightToLeft(&x,&child,&rightChild,address,i);
                            } else        //need merging.
                            {
                                mergeNonLeafNodes(&x,&child,&rightChild,address,i);
                            }

                        }
                    }

                }
//                recursiveRemove( target, x.pointers[i]);
            }
            else  // the father is a leaf node.
            {
                int foundKey = -1;
                for(int h = 0; h < x.numOfKey; h++ ){
                    if(x.key[h] == target.key){
                        foundKey = h;
                        break;
                    }
                }
                if(foundKey<0){
                    return -1;
                    // not found;
                }else{
                    for(int h = foundKey; h < x.numOfKey - 1; h++){
                        x.key[h] = x.key[h+1];
                        x.pointers[h] = x.pointers[h+1];
                    }
                    x.numOfKey--;
                }
                writeBPlusNode(address, x);
                return 1;
            }
        }
        else        //在当前节点没找到关键字
        {

        }
    }

//    template <class T, class KeyType>
//    void BPlusTree<T,KeyType>::clearNode(addressType node){
//    }


    template <class T, class KeyType>
    int BPlusTree<T,KeyType>::createBPlusNode(){
        int firstMatch = -1;
        int flag = 0;
        for(int i = 0; i < MAX_NODE_NUM; i++){
            if(metaData.nodeMemoryTable.used[i] == false){
                firstMatch = i;
                flag = 1;
                break;
            }
        }
        if(flag){
            metaData.nodeMemoryTable.used[firstMatch]  = true;
			metaData.num++;
            return firstMatch;
        }else{
            return -1;
        }
    }

    template <class T, class KeyType>
    int BPlusTree<T,KeyType>::createBPlusTreeFile(std::string filename, std::string path) {
        return 1;
    }

    template <class T, class KeyType>
    T BPlusTree<T,KeyType>::findLeftestNode() {
        return recursiveFindLeftestNode(rootPosition);
    }
    template <class T, class KeyType>
    T BPlusTree<T,KeyType>::recursiveFindLeftestNode(addressType address){
        T node;
        readBPlusNode(address,&node);
        if( node.isLeaf){
            return node;
        }else{
            return recursiveFindLeftestNode(node.pointers[0]);

        }
    };
    template< class T, class KeyType>
    T BPlusTree<T,KeyType>::findNode(KeyType target) {
        return recursiveFindNode(target,metaData.rootPosition);
    }

    template< class T, class KeyType>
    T BPlusTree<T,KeyType>::recursiveFindNode(KeyType target, addressType address) {
        int i;
        T node;
        readBPlusNode(address,&node);
        for( i = 0; i < node.numOfKey && node.key[i] <= target.key; i++);
        if( node.isLeaf){
            if(node.key[i - 1] == target.key){
                return node;
            }else{
                T demo;
                demo.numOfKey=-1;
                return demo;
            }
        }else{
            return recursiveFindNode(target,node.pointers[i]);

        }
    }

    template< class T, class KeyType>
    addressType BPlusTree<T,KeyType>::searchNode(KeyType key){
//        T root;
//        readBPlusNode(rootPosition,&root);
        return recursiveSearchNode(key,rootPosition);
    };

    template< class T, class KeyType>
    addressType BPlusTree<T,KeyType>::recursiveSearchNode(KeyType target, addressType address){
        int i;
        T node;
        readBPlusNode(address,&node);
        for( i = 0; i < node.numOfKey && node.key[i] < target.key; i++);
        if( node.isLeaf){
            if(node.key[i] == target.key){
                return node.pointers[i];
            }else{
                return NOT_FOUND;
            }
        }else{
            return recursiveSearchNode(target,node.pointers[i]);

        }
    };
    template<class T, class KeyType>
    std::vector<addressType> BPlusTree<T,KeyType>::searchNodes(KeyType key){
        auto equalNode = findNode(key);
        std::vector<addressType > found;
        if(key.type == EQUAL){
            addressType foundOne = recursiveSearchNode(key,rootPosition);
            found.push_back(foundOne);
        }else if(key.type == GREATER){
            found = traversalLinks(&equalNode,TO_GREATER_NOT_INCLUDE,key);
        }else if(key.type == GREATER_OR_EQUAL){
            found = traversalLinks(&equalNode,TO_GREATER_INCLUDE,key);
        }else if (key.type == LESS){
            auto leftestNode = findLeftestNode();
            KeyType tmp;
            tmp.type = -1;
            found = traversalLinksInterval(&leftestNode,&equalNode,tmp,key,include_first);
        }else if(key.type == LESS_OR_EQUAL){
            auto leftestNode = findLeftestNode();
            KeyType tmp;
            tmp.type = -1;
            found = traversalLinksInterval(&leftestNode,&equalNode,tmp,key,include_both);
        }
        return found;
    }

    template <class T, class KeyType>
    addressType BPlusTree<T,KeyType>::mapAddress(addressType rawAddress) {
        return 1;
    }
    template <class T, class KeyType>
    T *BPlusTree<T,KeyType>::convertAddress(addressType address){
        return (T*)address;
    };


    template<class T, class KeyType>
    bool BPlusTree<T,KeyType>::isEnd(T* node){
        if(node->pointers[nextPointer] < 0 ){
            return true;
        }else{
            return false;
        }
    }


    template<class T, class KeyType>
    std::vector<addressType> BPlusTree<T,KeyType>::recursiveSearchNodes(KeyType key){

    }
    template<class T, class KeyType>
    std::vector<addressType> BPlusTree<T,KeyType>::traversalLinks(T *anchor, int type, KeyType target) {
        std::vector<addressType> found;
        T currentNode;
        memcpy(&currentNode,anchor,sizeof(T));

        if(type == TO_GREATER_INCLUDE){
            int flag = 0;
            for(int i = 0; i < currentNode.numOfKey; i++){
                if( target.key == currentNode.key[i]){
                    flag = 1;
                }
                if(flag){
                    found.push_back(currentNode.pointers[i]);
                }
            }
            readBPlusNode(currentNode.pointers[nextPointer],&currentNode);
            do{
                for(int i = 0; i < currentNode.numOfKey; i++){
                    found.push_back(currentNode.pointers[i]);

                }
                if(currentNode.pointers[nextPointer] < 0){
                    break;
                }
                readBPlusNode(currentNode.pointers[nextPointer],&currentNode);
            }while(!isEnd(&currentNode));
        }else if(type == TO_GREATER_NOT_INCLUDE){
            int flag = 0;
            for(int i = 0; i < currentNode.numOfKey; i++){
                if(flag){
                    found.push_back(currentNode.pointers[i]);
                }
                if( target.key == currentNode.key[i]){
                    flag = 1;
                }
            }
            readBPlusNode(currentNode.pointers[nextPointer],&currentNode);
            do{
                for(int i = 0; i < currentNode.numOfKey; i++){
                    found.push_back(currentNode.pointers[i]);
                }
                if(currentNode.pointers[nextPointer] < 0){
                    break;
                }
                readBPlusNode(currentNode.pointers[nextPointer],&currentNode);
            }while(!isEnd(&currentNode));
        }else if(type == TO_SMALLER_INCLUDE){
            do{
                for(int i = 0; i < currentNode.numOfKey; i++){
                    found.push_back(currentNode.key[i]);
                }
//                currentNode = convertAddress(currentNode->prev);
            }while(!isEnd(&currentNode));
        }else if(type == TO_SMALLER_NOT_INCLUDE){
//            currentNode = convertAddress(currentNode->prev);
            do{
                for(int i = 0; i < currentNode.numOfKey; i++){
                    found.push_back(currentNode.key[i]);
                }
//                currentNode = convertAddress(currentNode->prev);
            }while(!isEnd(&currentNode));
        }else{

        }
        return found;
    }
    template<class T, class KeyType>
    std::vector<addressType> BPlusTree<T,KeyType>::traversalLinksInterval(T* first, T* last, KeyType firstKey, KeyType lastKey, TraversalType traversalType){
        T currentNode;
        memcpy(&currentNode,first, sizeof(T));
        std::vector<addressType> found;
//        addressType endAddress = last->prev->pointers[nextPointer];
//        addressType beginAddress = first->pointers[nextPointer]->last;
        if(traversalType == include_both){
            auto flag = 0;
            if(firstKey.type == -1){
                flag = 1;
            }
            for(int i =0; i < first->numOfKey; i++){
                if(flag == 0){
                    if(firstKey.key == first->key[i]){
                        flag = 1;
                    }
                }
                if(flag == 1){
                    found.push_back(first->pointers[i]);
                }
                if(first->key[i] == lastKey.key){
//                    found.push_back(first->pointers[i]);
                    return found;
                }
            }
            T tmp;
            readBPlusNode(currentNode.pointers[nextPointer],&currentNode);
            while(currentNode.pointers[nextPointer] == last->pointers[nextPointer]){
                for(int i = 0; i < currentNode.numOfKey; i++){
                    found.push_back(currentNode.pointers[i]);
                }
                readBPlusNode(currentNode.pointers[nextPointer],&currentNode);
            }
            flag = 1;
            for(int i = 0; i < last->numOfKey; i++){
                if(flag == 1){
                    found.push_back(last->pointers[i]);
                }
                if(flag == 1){
                    if(lastKey.key == last->key[i]){
                        flag = 0;
                    }
                }
            }

        }else if(traversalType == include_first){
            auto flag = 0;
            if(firstKey.type == -1){
                flag = 1;
            }
            for(int i =0; i < first->numOfKey; i++){
                if(flag == 0){
                    if(firstKey.key == first->key[i]){
                        flag = 1;
                    }
                }
                if(first->key[i] == lastKey.key){
//                    found.push_back(first->pointers[i]);
                    return found;
                }
                if(flag == 1){
                    found.push_back(first->pointers[i]);
                }
            }
            T tmp;
            readBPlusNode(currentNode.pointers[nextPointer],&currentNode);
            while(currentNode.pointers[nextPointer] != last->pointers[nextPointer]){
                for(int i = 0; i < currentNode.numOfKey; i++){
                    found.push_back(currentNode.pointers[i]);
                }
                readBPlusNode(currentNode.pointers[nextPointer],&currentNode);
            }
            flag = 1;
            for(int i = 0; i < last->numOfKey; i++){
                if(flag == 1){
                    if(lastKey.key == last->key[i]){
                        flag = 0;
                    }
                }
                if(flag == 1){
                    found.push_back(last->pointers[i]);
                }
            }
        }else if(traversalType == include_last){
            readBPlusNode(first->pointers[nextPointer],&currentNode);
            do{
                for(int i = 0; i < currentNode.numOfKey; i++){
                    found.push_back(currentNode.pointers[i]);
                }
            }while(currentNode.pointers[nextPointer] != last->pointers[nextPointer] );
        }
    return found;
    };

//    template<class T, class KeyType>
//    bool BPlusTree<T,KeyType>::removeKey(KeyType key){
//        searchNodeParse snp;
//        if(metaData.num == 0)
//        {
////            std::cout << "ERROR: In deleteKey, no nodes in the tree " << fileName << "!" << endl;
//            return false;
//        }
//        else
//        {
//            auto foundNode = findNode(key);
//            auto leftestNode = findLeftestNode();
////            findToLeaf(root, key, snp);
//            if(!leftestNode)
//            {
//                std::cout << "ERROR: In deleteKey, no keys in the tree " <<std::endl;
//                return false;
//            }
//            else // find the key in the leaf node
//            {
//                if(metaData.num == 1)
//                {
//                    int flag = 0;
//                    for(int i = 0; i < foundNode.numOfKey; i++){
//                        if(flag == 0){
//                            if(foundNode.key[i] == key.node){
//                                flag = 1;
//                            }
//                        }else{
//                            foundNode.key[i] = foundNode.key[i+1];
//                            foundNode.pointers[i] = foundNode.pointers[i+1];
//                        }
//
//                    }
//                    foundNode.numOfKey--;
////                    snp.pNode->removeAt(snp.index);
//                    metaData.num;
////                    keyCount --;
//                    return adjustAfterRemove(foundNode);
//                }
//                else
//                {
//                    if(snp.index == 0 && leafHead != snp.pNode) // the key exist in the branch.
//                    {
//                        // go to upper level to update the branch level
//                        size_t index = 0;
//
//                        Node now_parent = snp.pNode->parent;
//                        bool if_found_inBranch = now_parent->search(key,index);
//                        while(!if_found_inBranch)
//                        {
//                            if(now_parent->parent)
//                                now_parent = now_parent->parent;
//                            else
//                            {
//                                break;
//                            }
//                            if_found_inBranch = now_parent->search(key,index);
//                        }// end of search in the branch
//
//                        now_parent -> keys[index] = snp.pNode->keys[1];
//
//                        snp.pNode->removeAt(snp.index);
//                        keyCount--;
//                        return adjustAfterDelete(snp.pNode);
//
//                    }
//                    else //this key must just exist in the leaf too.
//                    {
//                        snp.pNode->removeAt(snp.index);
//                        keyCount--;
//                        return adjustAfterDelete(snp.pNode);
//                    }
//                }
//            }
//        }
//    }

    template<class T, class KeyType>
    void BPlusTree<T, KeyType>::mergeLeafNodes(T *father, T *leftChild, T *rightChild,addressType fatherAddress, int splitKey){
        for ( int j = 0; j < rightChild->numOfKey; j++)
        {
            leftChild->key[leftChild->numOfKey + j] = rightChild->key[j];
            leftChild->pointers[leftChild->numOfKey + j] = rightChild->pointers[j];
        }
        leftChild->numOfKey += rightChild->numOfKey;

        leftChild->pointers[nextPointer] = rightChild->pointers[nextPointer];

        clearNode(father->pointers[splitKey + 1]);
        //释放rightChild占用的空间x.Pointer[i+1]

        for (int j = splitKey; j < father->numOfKey - 1; j++)
        {
            father->key[j] = father->key[j + 1];
            father->pointers[j + 1] = father->pointers[j + 2];
        }
        father->numOfKey--;
        writeBPlusNode(fatherAddress,*father);
        writeBPlusNode(father->pointers[splitKey],*leftChild);
    }

    template<class T, class KeyType>
    void BPlusTree<T, KeyType>::mergeNonLeafNodes(T *father, T *leftChild, T *rightChild,addressType fatherAddress, int splitKey){
        leftChild->key[leftChild->numOfKey] = father->key[splitKey];
        leftChild->numOfKey++;
        for ( int j = 0; j < rightChild->numOfKey; j++)
        {
            leftChild->key[leftChild->numOfKey + j] = rightChild->key[j];
            leftChild->pointers[leftChild->numOfKey + j] = rightChild->pointers[j];
        }
        leftChild->pointers[leftChild->numOfKey + rightChild->numOfKey] = rightChild->pointers[rightChild->numOfKey];
        leftChild->numOfKey += rightChild->numOfKey;


        clearNode(father->pointers[splitKey + 1]);

        for (int j = splitKey; j < father->numOfKey - 1; j++)
        {
            father->key[j] = father->key[j + 1];
            father->pointers[j + 1] = father->pointers[j + 2];
        }
        father->numOfKey--;

        writeBPlusNode(fatherAddress,*father);
        writeBPlusNode(father->pointers[splitKey],*leftChild);
    }

    template<class T, class KeyType>
    void  BPlusTree<T, KeyType>::adjustLeafNodesFromLeftToRight(T* father, T*leftChild, T* rightChild,addressType fatherAddress,int splitKey){
        for (int j = rightChild->numOfKey; j > 0; j--)
        {
            rightChild->key[j] = rightChild->key[j - 1];
            rightChild->pointers[j] = rightChild->pointers[j - 1];
        }
        rightChild->key[0] =leftChild->key[leftChild->numOfKey - 1];
        rightChild->pointers[0] =leftChild->pointers[leftChild->numOfKey - 1];

        rightChild->numOfKey++;
        leftChild->numOfKey--;

        father->key[splitKey] = rightChild->key[0];

        writeBPlusNode(fatherAddress, father);
        writeBPlusNode(father->pointers[splitKey], leftChild);
        writeBPlusNode(father->pointers[splitKey+ 1], rightChild);
    };


    template<class T, class KeyType>
    void BPlusTree<T, KeyType>::adjustLeafNodesFromRightToLeft(T* father, T*leftChild, T* rightChild,addressType fatherAddress,int splitKey) {
        father->key[splitKey] = rightChild->key[1];

        leftChild->key[leftChild->numOfKey] = rightChild->key[0];
        leftChild->pointers[leftChild->numOfKey] = rightChild->pointers[0];
        leftChild->numOfKey++;

        for (int j = 0; j < rightChild->numOfKey - 1; j++)
        {
            rightChild->key[j] = rightChild->key[j + 1];
            rightChild->pointers[j] = rightChild->pointers[j + 1];
        }



        rightChild->numOfKey--;

        writeBPlusNode(fatherAddress, father);
        writeBPlusNode(father->pointers[splitKey], leftChild);
        writeBPlusNode(father->pointers[splitKey + 1], rightChild);
    }

    template<class T, class KeyType>
    void BPlusTree<T, KeyType>::adjustNonLeafNodesFromRightToLeft(T *father, T *leftChild, T *rightChild, addressType fatherAddress,
                                                      int splitKey){
//        father->key[splitKey + 1] = rightChild->key[1];
        leftChild->key[leftChild->numOfKey] = father->key[splitKey];
        leftChild->pointers[leftChild->numOfKey] = rightChild->pointers[0];
        leftChild->numOfKey++;
        father->key[splitKey] = rightChild->key[0];

        for (int j = 0; j < rightChild->numOfKey - 1; j++)
        {
            rightChild->key[j] = rightChild->key[j + 1];
            rightChild->pointers[j] = rightChild->pointers[j + 1];
        }
        rightChild->pointers[rightChild->numOfKey] = rightChild->pointers[rightChild->numOfKey + 1];

        rightChild->numOfKey--;

        writeBPlusNode(fatherAddress, father);
        writeBPlusNode(father->pointers[splitKey], leftChild);
        writeBPlusNode(father->pointers[splitKey + 1], rightChild);
    }

    template<class T, class KeyType>
    void BPlusTree<T, KeyType>::adjustNonLeafNodesFromLeftToRight(T *father, T *leftChild, T *rightChild, addressType fatherAddress,
                                                      int splitKey){
        for (int j = rightChild->numOfKey; j > 0; j--)
        {
            rightChild->key[j] = rightChild->key[j - 1];
            rightChild->pointers[j + 1] = rightChild->pointers[j];
        }
        rightChild->pointers[1] = rightChild->pointers[0];
        rightChild->numOfKey++;
        rightChild->pointers[0] = leftChild->pointers[leftChild->numOfKey];
        rightChild->numOfKey++;

        father->key[splitKey] = leftChild->key[leftChild->numOfKey - 1];
        leftChild->numOfKey--;

        writeBPlusNode(fatherAddress, father);
        writeBPlusNode(father->pointers[splitKey], leftChild);
        writeBPlusNode(father->pointers[splitKey + 1], rightChild);
    }









};
#endif //INDEXMANAGER_BPLUSTREE_H
