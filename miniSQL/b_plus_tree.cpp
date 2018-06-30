//
// Created by ktwzj on 2018/6/19.
//

#include "b_plus_tree.h"
namespace IndexManager{
    template <class T, class KeyType>
    BPlusTree<T, KeyType>::BPlusTree() {

    }
    template < BPlusNodeChar, CharKey>
    BPlusTree<BPlusNodeChar, CharKey>::BPlusTree() {
        numOfKey = MAX_CHAR;
    }

    template <class T, class KeyType>
    BPlusTree< T, KeyType>::~BPlusTree() {
        std::cout << "B plus tree is cleaned.";
    }

    template <class T, class KeyType>
    bool BPlusTree< T,  KeyType>::insert(KeyType key) {
        T r;
        readBPlusNode(rootPosition,r);
        if (r.numOfKey == maxChildrenNum)
        {
            T newRoot;
            newRoot.numOfKey = 0;
            newRoot.isLeaf = false;
            newRoot.pointers[0] = rootPosition;
            splitBPlusNode(newRoot,r,0);
            writeBPlusNode(rootPosition, r);
            rootPosition = createBPlusNode();
            writeBPlusNode(rootPosition, newRoot);
        }
//        insertKey(root, tar);
    }
    template <class T, class KeyType>
    int BPlusTree<T, KeyType>::readBPlusNode(int address, T *bPlusNode) {
//        bPlusNode =
    }

    template <class T, class KeyType>
    int BPlusTree<T, KeyType>::writeBPlusNode(int address, T *bPlusNode) {
        return 1;
    }

    template <class T, class KeyType>
    int BPlusTree<T, KeyType>::writeBPlusNode(int address, T bPlusNode) {
        return 1;
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
            readBPlusNode(x.pointers[i], &y);

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
        else
        {

            for (int j = x.numOfKey; j > i; j--)
            {
                x.key[j] = x.key[j - 1];
                x.pointers[j] = x.pointers[j - 1];
            }
            x.key[i] = target.key;
            x.numOfKey++;
            x.pointers[i] = offset + ( numOfNode << 12 );

            writeBPlusNode(address, x);

        }
    }

    template <class T, class KeyType>
    void BPlusTree<T, KeyType>::splitBPlusNode(T child, T parent, int num) {
        int half = maxChildrenNum / 2;

        int i;

        for (i = parent.numOfKey; i > num; i--)
        {
            parent.key[i] = parent.key[i - 1];
            parent.pointers[i + 1] = parent.pointers[i];
        }
        parent.numOfKey++;

        T t;

        addressType address = createBPlusNode();

        parent.key[num] = child.key[half];
        parent.pointers[num + 1] = address;

        for (i = half + 1; i < maxChildrenNum; i++)
        {
            t.key[i - half - 1] = child.key[i];
            t.pointers[i - half - 1] = child.pointers[i];
        }

        t.numOfKey = maxChildrenNum - half - 1;
        t.pointers[t.numOfKey] = child.pointers[maxChildrenNum];

        t.isLeaf = child.isLeaf;

        child.numOfKey = half;

        if (child.isLeaf)
        {
            child.numOfKey++;
            t.pointers[maxChildrenNum] = child.pointers[maxChildrenNum];
            child.pointers[maxChildrenNum] = address;
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
        }
    }

    template <class T, class KeyType>
    void BPlusTree<T,KeyType>::recursiveRemove(KeyType target, addressType address) {
        int i, j;
        T x;
        readBPlusNode(address,&x);

        for (i = 0; i < x.numOfKey && target.numOfKey > x.key[i]; i++);

        if (i < x.numOfKey && x.key[i] == target.key)  //�ڵ�ǰ�ڵ��ҵ��ؼ���
        {

            if (!x.isLeaf)     //���ڽڵ��ҵ��ؼ���
            {
                T child;
//                BplusNodeInt child;
                addressType childAdd = x.pointers[i];
                readBPlusNode(x.pointers[i], &child);
                if (child.isLeaf)     //���������Ҷ�ڵ�
                {
                    if (child.numOfKey > maxChildrenNum / 2) {
                        x.key[i] = child.key[child.numOfKey - 2];
                        child.numOfKey--;
                        writeBPlusNode(address, &x);
                        writeBPlusNode(x.pointers[i], &child);
                        return;
                    } else {
                        if (i > 0)      //�����ֵܽڵ�
                        {
                            T leftChild;
                            readBPlusNode(x.pointers[i - 1], &leftChild);

                            if (leftChild.numOfKey > maxChildrenNum / 2) {
                                for (j = child.numOfKey; j > 0; j--) {
                                    child.key[j] = child.key[j - 1];
                                    child.pointers[j] = child.pointers[j - 1];
                                }

                                child.key[0] = x.key[i - 1];
                                child.pointers[0] = leftChild.pointers[leftChild.numOfKey - 1];

                                child.numOfKey++;

                                leftChild.numOfKey--;

                                x.key[i - 1] = leftChild.key[leftChild.numOfKey - 1];
                                x.key[i] = child.key[child.numOfKey - 2];

                                writeBPlusNode(address, &x);
                                writeBPlusNode(x.pointers[i - 1], &leftChild);
                                writeBPlusNode(x.pointers[i], &child);

                            } else    //���C
                            {
                                for (j = 0; j < child.numOfKey; j++) {
                                    leftChild.key[leftChild.numOfKey + j] = child.key[j];
                                    leftChild.pointers[leftChild.numOfKey + j] = child.pointers[j];
                                }
                                leftChild.numOfKey += child.numOfKey;

                                leftChild.pointers[maxChildrenNum] = child.pointers[maxChildrenNum];

                                clearNode(childAdd);
                                //�ͷ�child�ڵ�ռ�õĿռ�x.Pointer[i]

                                for (j = i - 1; j < x.numOfKey - 1; j++) {
                                    x.key[j] = x.key[j + 1];
                                    x.pointers[j + 1] = x.pointers[j + 2];
                                }
                                x.numOfKey--;

                                x.key[i - 1] = leftChild.key[leftChild.numOfKey - 2];

                                writeBPlusNode(address, x);
                                writeBPlusNode(x.pointers[i - 1], leftChild);

                                i--;

                            }


                        } else      //ֻ�����ֵܽڵ�
                        {
                            T rightChild;
                            addressType rightChildAddress = x.pointers[i + 1];
                            readBPlusNode(x.pointers[i + 1], rightChild);

                            if (rightChild.numOfKey > maxChildrenNum / 2)        //���D
                            {
                                x.key[i] = rightChild.key[0];
                                child.key[child.numOfKey] = rightChild.key[0];
                                child.pointers[child.numOfKey] = rightChild.pointers[0];
                                child.numOfKey++;

                                for (j = 0; j < rightChild.numOfKey - 1; j++) {
                                    rightChild.key[j] = rightChild.key[j + 1];
                                    rightChild.pointers[j] = rightChild.pointers[j + 1];
                                }

                                rightChild.numOfKey--;

                                writeBPlusNode(address, x);
                                writeBPlusNode(x.pointers[i], child);
                                writeBPlusNode(x.pointers[i + 1], rightChild);

                            } else    //���E
                            {
                                for (j = 0; j < rightChild.numOfKey; j++) {
                                    child.key[child.numOfKey + j] = rightChild.key[j];
                                    child.pointers[child.numOfKey + j] = rightChild.pointers[j];
                                }
                                child.numOfKey += rightChild.numOfKey;

                                child.pointers[maxChildrenNum] = rightChild.pointers[maxChildrenNum];

                                clearNode(rightChildAddress);
                                //�ͷ�rbchildռ�õĿռ�x.Pointer[i+1]

                                for (j = i; j < x.numOfKey - 1; j++) {
                                    x.key[j] = x.key[j + 1];
                                    x.pointers[j + 1] = x.pointers[j + 2];
                                }
                                x.numOfKey--;

                                writeBPlusNode(address, x);
                                writeBPlusNode(x.pointers[i], child);

                            }

                        }

                    }

                } else      //���F
                {

                    KeyType target2;
//                    target2.indexFileName = target.indexFileName;
                    target2.key = target.key;
                    target2.type = SINGLE;
                    addressType resultAdd;
                    vector <index_info> result;
                    result = searchBplusTree(target2, resultAdd);

                    T last;

                    read
                    readBPlusNode(resultAdd, last);

                    x.key[i] = last.key[last.numOfKey - 2];

                    writeBPlusNode(address, x);


                    if (child.numOfKey > maxChildrenNum / 2)        //���H
                    {

                    } else          //�����ӽڵ�Ĺؼ�������������,���ֵܽڵ��ĳһ���ؼ�����������
                    {
                        if (i > 0)  //x.key[i]�����ֵ�
                        {
                            T lbchild;
                            readBPlusNode(x.pointers[i - 1], lbchild);

                            if (lbchild.numOfKey > maxChildrenNum / 2)       //���I
                            {
                                for (j = child.numOfKey; j > 0; j--) {
                                    child.key[j] = child.key[j - 1];
                                    child.pointers[j + 1] = child.pointers[j];
                                }
                                child.pointers[1] = child.pointers[0];
                                child.key[0] = x.key[i - 1];
                                child.pointers[0] = lbchild.pointers[lbchild.numOfKey];

                                child.numOfKey++;

                                x.key[i - 1] = lbchild.key[lbchild.numOfKey - 1];
                                lbchild.numOfKey--;

                                writeBPlusNode(address, x);
                                writeBPlusNode(x.pointers[i - 1], lbchild);
                                writeBPlusNode(x.pointers[i], child);
                            } else        //���J
                            {
                                lbchild.key[lbchild.numOfKey] = x.key[i - 1];   //�����ӽڵ㸴�Ƶ������ֵܵ�ĩβ
                                lbchild.numOfKey++;

                                for (j = 0; j < child.numOfKey; j++)      //��child�ڵ㿽����lbchild�ڵ��ĩβ,
                                {
                                    lbchild.key[lbchild.numOfKey + j] = child.key[j];
                                    lbchild.pointers[lbchild.numOfKey + j] = child.pointers[j];
                                }
                                lbchild.pointers[lbchild.numOfKey + j] = child.pointers[j];
                                lbchild.numOfKey += child.numOfKey;        //�Ѿ���child������lbchild�ڵ�

                                clearNode(childAdd);
                                //�ͷ�child�ڵ�Ĵ洢�ռ�,x.Pointer[i]


                                //���ҵ��ؼ��ֵĺ���child��ؼ������ֵܵĺ���lbchild�ϲ���,���ùؼ���ǰ��,ʹ��ǰ�ڵ�Ĺؼ��ּ���һ��
                                for (j = i - 1; j < x.numOfKey - 1; j++) {
                                    x.key[j] = x.key[j + 1];
                                    x.pointers[j + 1] = x.pointers[j + 2];
                                }
                                x.numOfKey--;

                                writeBPlusNode(address, x);
                                writeBPlusNode(x.pointers[i - 1], lbchild);

                                i--;

                            }

                        } else        //����x.key[i]ֻ�����ֵ�
                        {
                            T rbchild;
                            addressType rbchildAdd = x.pointers[i + 1];
                            readBPlusNode(x.pointers[i + 1], rbchild);

                            if (rbchild.numOfKey > maxChildrenNum / 2)     //���K
                            {

                                child.key[child.numOfKey] = x.key[i];
                                child.numOfKey++;

                                child.pointers[child.numOfKey] = rbchild.pointers[0];
                                x.key[i] = rbchild.key[0];

                                for (j = 0; j < rbchild.numOfKey - 1; j++) {
                                    rbchild.key[j] = rbchild.key[j + 1];
                                    rbchild.pointers[j] = rbchild.pointers[j + 1];
                                }
                                rbchild.pointers[j] = rbchild.pointers[j + 1];
                                rbchild.numOfKey--;

                                writeBPlusNode(address, x);
                                writeBPlusNode(x.pointers[i], child);
                                writeBPlusNode(x.pointers[i + 1], rbchild);

                            } else        //���L
                            {
                                child.key[child.numOfKey] = x.key[i];
                                child.numOfKey++;

                                for (j = 0; j < rbchild.numOfKey; j++)     //��rbchild�ڵ�ϲ���child�ڵ��
                                {
                                    child.key[child.numOfKey + j] = rbchild.key[j];
                                    child.pointers[child.numOfKey + j] = rbchild.pointers[j];
                                }
                                child.pointers[child.numOfKey + j] = rbchild.pointers[j];

                                child.numOfKey = child.numOfKey + rbchild.numOfKey;

                                clearNode(rbchildAdd);

                                for (j = i; j < x.numOfKey - 1; j++)    //��ǰ���ؼ���֮��Ĺؼ�������һλ,ʹ�ýڵ�Ĺؼ���������һ
                                {
                                    x.key[j] = x.key[j + 1];
                                    x.pointers[j + 1] = x.pointers[j + 2];
                                }
                                x.numOfKey--;

                                writeBPlusNode(address, x);
                                writeBPlusNode(x.pointers[i], child);

                            }

                        }
                    }

                }

                recursiveRemove(x.pointers[i], target);
            }
            else  //情况G
            {
                for (j = i; j < x.numOfKey - 1; j++)
                {
                    x.key[j] = x.key[j + 1];
                    x.pointers[j] = x.pointers[j + 1];
                }
                x.numOfKey--;

                writeBPlusNode(address, x);

                return;
            }

        }
        else        //在当前节点没找到关键字
        {
            if (!x.isLeaf)    //没找到关键字,则关键字必然包含在以Pointer[i]为根的子树中
            {
                T child;
                addressType childAdd = x.pointers[i];
                readBPlusNode(x.pointers[i], child);

                if (!child.isLeaf)      //如果其孩子节点是内节点
                {
                    if (child.numOfKey > maxChildrenNum / 2)        //情况H
                    {

                    }
                    else          //否则孩子节点的关键字数量不过半,则将兄弟节点的某一个关键字移至孩子
                    {
                        if (i > 0)  //x.key[i]有左兄弟
                        {
                            T leftChild;
                            readBPlusNode(x.pointers[i - 1], leftChild);

                            if (leftChild.numOfKey > maxChildrenNum / 2)       //情况I
                            {
                                for (j = child.numOfKey; j > 0; j--)
                                {
                                    child.key[j] = child.key[j - 1];
                                    child.pointers[j + 1] = child.pointers[j];
                                }
                                child.pointers[1] = child.pointers[0];
                                child.key[0] = x.key[i - 1];
                                child.pointers[0] = leftChild.pointers[leftChild.numOfKey];

                                child.numOfKey++;

                                x.key[i - 1] = leftChild.key[leftChild.numOfKey - 1];
                                leftChild.numOfKey--;

                                writeBPlusNode(address, &x);
                                writeBPlusNode(x.pointers[i - 1], &leftChild);
                                writeBPlusNode(x.pointers[i], &child);
                            }
                            else        //情况J
                            {
                                leftChild.key[leftChild.numOfKey] = x.key[i - 1];   //将孩子节点复制到其左兄弟的末尾
                                leftChild.numOfKey++;

                                for (j = 0; j < child.numOfKey; j++)      //将child节点拷贝到lbchild节点的末尾,
                                {
                                    leftChild.key[leftChild.numOfKey + j] = child.key[j];
                                    leftChild.pointers[leftChild.numOfKey + j] = child.pointers[j];
                                }
                                leftChild.pointers[leftChild.numOfKey + j] = child.pointers[j];
                                leftChild.numOfKey += child.numOfKey;        //已经将child拷贝到lbchild节点


                                clearNode(childAdd);
                                //释放child节点的存储空间,x.Pointer[i]


                                //将找到关键字的孩子child与关键字左兄弟的孩子lbchild合并后,将该关键字前移,使当前节点的关键字减少一个
                                for (j = i - 1; j < x.numOfKey - 1; j++)
                                {
                                    x.key[j] = x.key[j + 1];
                                    x.pointers[j + 1] = x.pointers[j + 2];
                                }
                                x.numOfKey--;

                                writeBPlusNode(address, &x);
                                writeBPlusNode(x.pointers[i - 1], &leftChild);

                                i--;

                            }

                        }
                        else        //否则x.key[i]只有右兄弟
                        {
                            T rbchild;
                            addressType rbchildAdd = x.pointers[i + 1];
                            readBPlusNode(x.pointers[i + 1], rbchild);

                            if (rbchild.numOfKey > MAX_FLOAT / 2)     //情况K
                            {

                                child.key[child.numOfKey] = x.key[i];
                                child.numOfKey++;

                                child.pointers[child.numOfKey] = rbchild.pointers[0];
                                x.key[i] = rbchild.key[0];

                                for (j = 0; j < rbchild.numOfKey - 1; j++)
                                {
                                    rbchild.key[j] = rbchild.key[j + 1];
                                    rbchild.pointers[j] = rbchild.pointers[j + 1];
                                }
                                rbchild.pointers[j] = rbchild.pointers[j + 1];
                                rbchild.numOfKey--;

                                writeBPlusNode(address, &x);
                                writeBPlusNode(x.pointers[i], &child);
                                writeBPlusNode(x.pointers[i + 1], &rbchild);

                            }
                            else        //情况L
                            {
                                child.key[child.numOfKey] = x.key[i];
                                child.numOfKey++;

                                for (j = 0; j < rbchild.numOfKey; j++)     //将rbchild节点合并到child节点后
                                {
                                    child.key[child.numOfKey + j] = rbchild.key[j];
                                    child.pointers[child.numOfKey + j] = rbchild.pointers[j];
                                }
                                child.pointers[child.numOfKey + j] = rbchild.pointers[j];

                                child.numOfKey += rbchild.numOfKey;

                                clearNode(rbchildAdd);
                                //释放rbchild节点所占用的空间,x,Pointer[i+1]

                                for (j = i; j < x.numOfKey - 1; j++)    //当前将关键字之后的关键字左移一位,使该节点的关键字数量减一
                                {
                                    x.key[j] = x.key[j + 1];
                                    x.pointers[j + 1] = x.pointers[j + 2];
                                }
                                x.numOfKey--;

                                writeBPlusNode(address, &x);
                                writeBPlusNode(x.pointers[i], &child);

                            }

                        }
                    }
                }
                else  //否则其孩子节点是外节点
                {
                    if (child.numOfKey > MAX_FLOAT / 2)  //情况M
                    {

                    }
                    else        //否则孩子节点不到半满
                    {
                        if (i > 0) //有左兄弟
                        {
                            T lbchild;
                            readBPlusNode(x.pointers[i - 1], lbchild);

                            if (lbchild.numOfKey > MAX_FLOAT / 2)       //情况N
                            {
                                for (j = child.numOfKey; j > 0; j--)
                                {
                                    child.key[j] = child.key[j - 1];
                                    child.pointers[j] = child.pointers[j - 1];
                                }
                                child.key[0] = x.key[i - 1];
                                child.pointers[0] = lbchild.pointers[lbchild.numOfKey - 1];
                                child.numOfKey++;
                                lbchild.numOfKey--;

                                x.key[i - 1] = lbchild.key[lbchild.numOfKey - 1];

                                writeBPlusNode(x.pointers[i - 1], &lbchild);
                                writeBPlusNode(x.pointers[i], &child);
                                writeBPlusNode(address, &x);

                            }
                            else        //情况O
                            {

                                for (j = 0; j < child.numOfKey; j++)        //与左兄弟孩子节点合并
                                {
                                    lbchild.key[lbchild.numOfKey + j] = child.key[j];
                                    lbchild.pointers[lbchild.numOfKey + j] = child.pointers[j];
                                }
                                lbchild.numOfKey += child.numOfKey;

                                lbchild.pointers[MAX_FLOAT] = child.pointers[MAX_FLOAT];

                                clearNode(childAdd);
                                //释放child占用的空间x.Pointer[i]

                                for (j = i - 1; j < x.numOfKey - 1; j++)
                                {
                                    x.key[j] = x.key[j + 1];
                                    x.pointers[j + 1] = x.pointers[j + 2];
                                }

                                x.numOfKey--;

                                writeBPlusNode(x.pointers[i - 1], &lbchild);
                                writeBPlusNode(address, &x);

                                i--;

                            }

                        }
                        else        //否则只有右兄弟
                        {
                            T rbchild;
                            addressType rbchildAdd = x.pointers[i + 1];
                            readBPlusNode(x.pointers[i + 1], rbchild);

                            if (rbchild.numOfKey > MAX_FLOAT / 2)       //情况P
                            {
                                x.key[i] = rbchild.key[0];
                                child.key[child.numOfKey] = rbchild.key[0];
                                child.pointers[child.numOfKey] = rbchild.pointers[0];
                                child.numOfKey++;

                                for (j = 0; j < rbchild.numOfKey - 1; j++)
                                {
                                    rbchild.key[j] = rbchild.key[j + 1];
                                    rbchild.pointers[j] = rbchild.pointers[j + 1];
                                }
                                rbchild.numOfKey--;

                                writeBPlusNode(address, x);
                                writeBPlusNode(x.pointers[i + 1], rbchild);
                                writeBPlusNode(x.pointers[i], child);

                            }
                            else        //情况Q
                            {
                                for (j = 0; j < rbchild.numOfKey; j++)
                                {
                                    child.key[child.numOfKey + j] = rbchild.key[j];
                                    child.pointers[child.numOfKey + j] = rbchild.pointers[j];
                                }
                                child.numOfKey += rbchild.numOfKey;
                                child.pointers[MAX_FLOAT] = rbchild.pointers[MAX_FLOAT];

//                                clearNode(rbchildAdd);
                                //释放rbchild占用的空间x.Pointer[i+1]

                                for (j = i; j < x.numOfKey - 1; j++)
                                {
                                    x.key[j] = x.key[j + 1];
                                    x.pointers[j + 1] = x.pointers[j + 2];
                                }
                                x.numOfKey--;

                                writeBPlusNode(address, x);
                                writeBPlusNode(x.pointers[i], &child);


                            }

                        }

                    }

                }

                recursiveRemove(x.pointers[i], target);
            }


        }
    }

    template <class T, class KeyType>
    void BPlusTree<T,KeyType>::clearNode(addressType node){
    }

    template <class T, class KeyType>
    int BPlusTree<T,KeyType>::createBPlusNode(){

    }

    template <class T, class KeyType>
    int BPlusTree<T,KeyType>::createBPlusTreeFile(std::string filename, std::string path) {
        return 1;
    }

}
