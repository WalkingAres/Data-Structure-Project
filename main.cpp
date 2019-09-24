#include <iostream>
#include "Stack.hpp"
#include "RBTree.hpp"
#include <iomanip>
#include<windows.h>

using namespace std;

// ��׼����
//int que[] = {3, 6, 9, 2, 4, 7, 1, 8, 5};

// ��д����ʹ��
typedef Stack<int> SInt;

// ��ʼ�����ջ�ĺ����
RBTree<SInt> sTop;

//��ʾ����𳵳������
void Welcome() {
	cout << "******************************************\n";
	cout << "**                                      **\n";
	cout << "**  Welcome to Train Dispatching Center **\n";
	cout << "**                                      **\n";
	cout << "******************************************\n";
	cout << "\n\nPlease input the number of trains:";
}

//number state true=coming false=arrive
void Train(int const & number,bool state){
	cout << "\n==========\n";
	cout << "===  " << setiosflags(ios::left) << setw(3) << number << "===";

	if (state) cout << "    COMING!\n";
	else cout << "    ARRIVE!\n";

	cout << "============\n";
	cout << " **     **  \n";
}


int main() {
    int count = 0, outque = 1, eCount = 0, n, c;
	// TODO������𳵳������
	Welcome();
	cin >> n;

    int *que = new int[n];
    int *Empty = new int[n];
	SInt** arr = new SInt* [n];



	/* TODO������𳵳����������
    ** e.g. {3, 6, 9, 2, 4, 7, 1, 8, 5}
	** 3Ϊ���Ƚ������Ļ�
	*/
	cout << "Input the order( e.g. 4 1 3 2 , means #4 first in ):";
	for (int i = 0; i < n; i++) {
        cin >> que[i];
    }

	// ��ʼ���㷨
    for (int j = 0; j < n; j++) {
        c = que[j];
		// TODO��������ڽ���ĳ���� cΪ�����
		Train(c,true);
		Sleep(1000);
        // determine if the coming train is the exact train we want
        if (c == outque) {
			// TODO������ͨ��
			Train(c,false);
			Sleep(1000);
            ++outque;
        }
        else {
            // check if the set is empty
            if (sTop.empty()) {
                // check if we have some empty stacks which are allocated previously
                if (eCount == 0) {
                    // allocate new stack
                    SInt *newPtr = new SInt;
                    arr[++count] = newPtr;
                    newPtr->push(c);
                    sTop.Insert(newPtr);
                }
                else {
                    // reuse the allocated stacks
                    arr[Empty[1]]->push(c);
                    sTop.Insert(arr[Empty[1]]);
                    for (int q = 1; q < eCount; ++q) {
                        Empty[q] = Empty[q - 1];
                    }
                    --eCount;
                }
            }
            else {
                if ((sTop.Maximum())->top() < c) {
                    if (eCount == 0) {
                        SInt *newPtr = new SInt;
                        newPtr->push(c);
                        arr[++count] = newPtr;
                        sTop.Insert(newPtr);
                    }
                    else {
                        arr[Empty[1]]->push(c);
                        sTop.Insert(arr[Empty[1]]);
                        for (int q = 1; q < eCount; ++q) {
                            Empty[q] = Empty[q - 1];
                        }
                        --eCount;
                    }                    
                }
                else{
                    auto temp = sTop.Retrieve(c);
                    temp->push(c);
                    sTop.Insert(temp);
                }
            }
        }

        while (!sTop.empty() && (sTop.Minimum()->top() == outque)) {
			// TODO��������ڽ���ĳ���� cΪ�����
			Train(outque,false);
			Sleep(1000);
			//
            auto temp = sTop.Retrieve(outque++);
            temp->pop();
            if (!temp->empty())
                sTop.Insert(temp);
            else {
                int element = temp->getId();
                int q;
                for (q = 1; q <= eCount; q++) {
                    if (element < Empty[q]) {
                        for (int t = eCount; t >= q; --t) {
                            Empty[t + 1] = Empty[t];
                        }
                        break;
                    }
                }
                Empty[q] = element;
                ++eCount;
            }

        }

        for (int i = 1; i <= count; i++) {
			// TODO����������ڻ������ĳ������
			// һ��ջ��һ��������
			// ����Stack.hpp �鿴
                arr[i]->display();
        }
        cout << "\n\n";
		Sleep(1000);
    }

	for (int i = 1; i <= count; i++) {
		delete arr[i];
	}

	delete []arr;
    delete []que;
    delete []Empty;
    
    return 0;
}