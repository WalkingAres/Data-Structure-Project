#ifndef STACK_HPP
#define STACK_HPP

#include <iostream>

using std::cin;
using std::cout;
using std::endl;

template<typename Obj>
class Stack{
    // the node of the stack
    struct Node{
        Node() : next(nullptr) {}

        Node(const Obj &ele, Node *t) : obj(ele), next(t) {}
        
        Obj obj;
        Node *next;
    };
public:
    // constructor
    // initialize a new empty stack
    Stack() : _top(new Node()), length(0) {
        id = ++count;
    }

    // destructor
    ~Stack() {
        Node *temp = _top;
        // delete all the element in the stack(including the _top pointer)
        while(_top != nullptr) {
            temp = _top->next;
            delete _top;
            _top = temp;
        }
        length = 0; // reset the length
        --count;
    }

    // push the new element into the top of the stack
    void push(const Obj &ele) noexcept {
        Node *newPtr = new Node(ele, _top->next); // initialize the new node
        _top->next = newPtr; // refresh the top of the stack
        ++length; // increment the length of the stack
    }
    
    // pop the top element out of the stack
    void pop() {
        // raise the exception when it is an empty stack
        if (length == 0)
            throw std::underflow_error("Pop from empty stack!");
        Node *temp = _top->next; 
        _top->next = temp->next; // refresh the top
        delete temp; // delete the top of the stack
        --length; // decrement the length of the stack
    }

    // determine if the stack is empty
    bool empty() noexcept {
        return length == 0;
    }

    // return the const reference of the top element
    const Obj& top() const {
        if (length == 0)
            throw std::underflow_error("There is no elements in the empty stack!");
        return (_top->next)->obj;
    }

    // return the reference of the top element
    Obj& top() {
        if (length == 0)
            throw std::underflow_error("There is no elements in the empty stack!");
        return (_top->next)->obj;
    }

    // return the reference of the top element
    int size() const noexcept{
        return length;
    }

    int getId() const noexcept{
        return id;
    }

	// TODO：输出缓冲轨道的火车车厢情况
	// id为缓冲轨道的轨道号码
    void display() {
		cout << "\n ++++++++++++\n";
		cout << " +  STACK " << id << " +\n";
		cout << " ++++++++++++\n";
        if (empty()) {
			// 轨道为空的时候
            cout << "===== NULL =====" << endl;
            return ;
        }
        Node *temp = _top->next;
		// 输出车厢号
        while (temp != nullptr) {
            // 车厢连接符
			cout << "<@"<< temp->obj<<"==";
			//
            temp = temp->next;
        }
        cout << "\n";
    }

    bool operator<(const Stack<Obj> &other) const {
        return top() < other.top();
    }
 
    static int count;
private:
    Node *_top; // the pointer points to the top element of the stack
    int length; // store the size of the stack
    int id;

};

// To support some features of RBTree
template<typename Obj>
int Stack<Obj>::count = 0;

template<typename Obj>
bool operator<(const Obj lhs, const Stack<Obj> &rhs) {
    return lhs < rhs.top();    
}

template<typename Obj>
bool operator>(const Obj lhs, const Stack<Obj> &rhs) {
    return lhs > rhs.top();    
}

template<typename Obj>
bool operator==(const Obj lhs, const Stack<Obj> &rhs) {
    return lhs == rhs.top();    
}

#endif