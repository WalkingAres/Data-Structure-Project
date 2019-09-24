#ifndef RBTREE_H
#define RBTREE_H

template <typename Object>
class RBTree
{
public:

	enum class RBTColor { RED, BLACK };
	struct RBTNode {
		Object* key; // 结点键值 
		RBTNode* left; // 左儿子
		RBTNode* right; // 右儿子
		RBTNode* parent; // 双亲
		RBTColor color;	// 颜色
	};

	RBTree(); // 构造函数
	~RBTree(); // 析构函数

	// Retrieve函数用于查找【栈顶元素大于或等于key】中最小的栈(即栈顶元素最小)并返回栈指针
	// Usage:1.查找栈顶元素大于key的对应栈并在栈中push新的车厢
	//		 2.查找树内是否存在等于key的对应栈并将该车厢pop出栈
	// Attention: 函数将会把返回的栈指针对应的节点在树中删除，在栈操作完成后需要重新使用
	//		 Insert函数进行插入，便于操作/节省空间/防止空栈入树
	Object* Retrieve(int key);

	// Insert函数用于在红黑树中插入新的节点
	// Attention: Retrieve函数传递的参数是一个具体的车厢编号，而Insert函数传递的是一个完整
	//		 的栈指针
	void Insert(Object* obj); // 插入

	// 返回树内栈顶元素最小/最大的栈，同时不删除对应的结点
	Object* Minimum();
	Object* Maximum();
	bool empty() {
		return root == nil || root == nullptr;
	}

private:

	void LeftRotate(RBTNode* x); // 左旋
	void RightRotate(RBTNode* x); // 右旋

	void InsertFixup(RBTNode* node); // 插入修复

	RBTNode* Minimum(RBTNode* node); // 查找node结点的后继
	void Transplant(RBTNode* u, RBTNode* v); // 子树移植v→u
	void Delete(RBTNode* key); // 删除
	void DeleteFixup(RBTNode* node); // 删除修复

private:

	RBTNode* root; // 根节点
	RBTNode* nil; // 叶节点
};

template <typename Object>
Object* RBTree<Object>::Minimum()
{
	RBTNode* node = root;
	while (node->left != nil)
	{
		node = node->left;
	}
	return node->key;
}

template <typename Object>
Object* RBTree<Object>::Maximum()
{
	RBTNode* node = root;
	while (node->right != nil)
	{
		node = node->right;
	}
	return node->key;
}

template <typename Object>
RBTree<Object>::RBTree()
{
	nil = new RBTNode{ nullptr, nullptr, nullptr, nullptr, RBTColor::BLACK };
	root = nil;
}

template <typename Object>
RBTree<Object>::~RBTree()
{
	while (root != nil) Delete(root);
	delete nil;
}

template <typename Object>
void RBTree<Object>::LeftRotate(RBTNode* x)
{
	RBTNode* y = x->right;
	x->right = y->left;
	if (y->left != nil) y->left->parent = x;
	y->parent = x->parent;
	if (x->parent == nil) root = y;
	else if (x == x->parent->left) x->parent->left = y;
	else x->parent->right = y;
	y->left = x;
	x->parent = y;
}

template <typename Object>
void RBTree<Object>::RightRotate(RBTNode* y)
{
	RBTNode* x = y->left;
	y->left = x->right;
	if (x->right != nil) x->right->parent = y;
	x->parent = y->parent;
	if (y->parent == nil) root = x;
	else if (y == y->parent->left) y->parent->left = x;
	else y->parent->right = x;
	x->right = y;
	y->parent = x;
}

template <typename Object>
void RBTree<Object>::Insert(Object* obj)
{
	RBTNode* z = new RBTNode{ obj, nullptr, nullptr, nullptr, RBTColor::BLACK };
	RBTNode* y = nil;
	RBTNode* x = root;
	while (x != nil)
	{
		y = x;
		if (*(z->key) < *(x->key)) x = x->left;
		else x = x->right;
	}
	z->parent = y;
	if (y == nil) root = z;
	else if (*(z->key) < *(y->key)) y->left = z;
	else y->right = z;
	z->left = nil;
	z->right = nil;
	z->color = RBTColor::RED;
	InsertFixup(z);
}

template <typename Object>
void RBTree<Object>::InsertFixup(RBTNode* z)
{
	// case 1: z的叔结点y是红色的
	// case 2: z的叔结点y是黑色的且z是一个右孩子
	// case 3: z的叔结点y是黑色的且z是一个左孩子
	while (z->parent->color == RBTColor::RED && z->parent != nil)
	{
		if (z->parent == z->parent->parent->left)
		{
			RBTNode* y = z->parent->parent->right;
			if (y->color == RBTColor::RED)
			{
				z->parent->color = RBTColor::BLACK;		// case 1
				y->color = RBTColor::BLACK;				// case 1
				z->parent->parent->color = RBTColor::RED;		// case 1
				z = z->parent->parent;					// case 1
			}
			else
			{
				if (z == z->parent->right)
				{
					z = z->parent;							// case 2
					LeftRotate(z);							// case 2
				}
				z->parent->color = RBTColor::BLACK;			// case 3
				z->parent->parent->color = RBTColor::RED;	// case 3
				RightRotate(z->parent->parent);				// case 3
			}
		}
		else
		{
			RBTNode* y = z->parent->parent->left;
			if (y->color == RBTColor::RED)
			{
				z->parent->color = RBTColor::BLACK;
				y->color = RBTColor::BLACK;
				z->parent->parent->color = RBTColor::RED;
				z = z->parent->parent;
			}
			else
			{
				if (z == z->parent->left)
				{
					z = z->parent;
					RightRotate(z);
				}
				z->parent->color = RBTColor::BLACK;
				z->parent->parent->color = RBTColor::RED;
				LeftRotate(z->parent->parent);
			}
		}
	}
	root->color = RBTColor::BLACK;
}

template <typename Object>
void RBTree<Object>::Transplant(RBTNode* u, RBTNode* v)
{
	if (u->parent == nil) root = v;
	else if (u == u->parent->left) u->parent->left = v;
	else u->parent->right = v;
	if (v != nil) v->parent = u->parent;
}

template <typename Object>
typename RBTree<Object>::RBTNode* RBTree<Object>::Minimum(RBTNode* node)
{
	while (node->left != nil) node = node->left;
	return node;
}

template <typename Object>
void RBTree<Object>::Delete(RBTNode* z)
{
	RBTNode* x = nullptr;
	RBTNode* y = z;
	RBTColor y_original_color = y->color;
	if (z->left == nil)
	{
		x = z->right;
		Transplant(z, z->right);
	}
	else if (z->right == nil)
	{
		x = z->left;
		Transplant(z, z->left);
	}
	else
	{
		y = Minimum(z->right);
		y_original_color = y->color;
		x = y->right;
		if (y->parent == z)
			x->parent = y;
		else
		{
			Transplant(y, y->right);
			y->right = z->right;
			y->right->parent = y;
		}
		Transplant(z, y);
		y->left = z->left;
		y->left->parent = y;
		y->color = z->color;

		z->parent = nullptr;
		z->right = nullptr;
		z->left = nullptr;
		delete z;
	}
	if (y_original_color == RBTColor::BLACK)
		DeleteFixup(x);
}

template <typename Object>
void RBTree<Object>::DeleteFixup(RBTNode* x)
{
	// case 1: x的兄弟结点w是红色的
	// case 2: x的兄弟结点w是黑色的，而且w的两个子结点都是黑色的
	// case 3: x的兄弟结点w是黑色的，w的左孩子是红色的，w的右孩子是黑色的
	// case 4: x的兄弟结点w是黑色的，w的右孩子是红色的
	RBTNode* w;
	while (x != root && x != nil && x->color == RBTColor::BLACK)
	{
		if (x == x->parent->left)
		{
			w = x->parent->right;
			if (w->color == RBTColor::RED)
			{
				w->color = RBTColor::BLACK;						// case 1
				x->parent->color = RBTColor::RED;				// case 1
				LeftRotate(x->parent);							// case 1
				w = x->parent->right;							// case 1
			}
			if (w->left->color == RBTColor::BLACK && w->right->color == RBTColor::BLACK)
			{
				w->color = RBTColor::RED;						// case 2
				x = x->parent;									// case 2
			}
			else
			{
				if (w->right->color == RBTColor::BLACK)
				{
					w->left->color = RBTColor::BLACK;			// case 3
					w->color = RBTColor::RED;					// case 3
					RightRotate(w);								// case 3
					w = x->parent->right;						// case 3
				}
				w->color = x->parent->color;					// case 4
				x->parent->color = RBTColor::BLACK;				// case 4
				w->right->color = RBTColor::BLACK;				// case 4
				LeftRotate(x->parent);							// case 4
				x = root;										// case 4
			}
		}
		else
		{
			w = x->parent->left;
			if (w->color == RBTColor::RED)
			{
				w->color = RBTColor::BLACK;
				x->parent->color = RBTColor::RED;
				RightRotate(x->parent);
				w = x->parent->left;
			}
			if (w->left->color == RBTColor::BLACK && w->right->color == RBTColor::BLACK)
			{
				w->color = RBTColor::RED;
				x = x->parent;
			}
			else
			{
				if (w->left->color == RBTColor::BLACK)
				{
					w->color = RBTColor::RED;
					w->right->color = RBTColor::BLACK;
					LeftRotate(w);
					w = x->parent->left;
				}
				w->color = x->parent->color;
				x->parent->color = RBTColor::BLACK;
				w->left->color = RBTColor::BLACK;
				RightRotate(x->parent);
				x = root;
			}
		}
	}
	x->color = RBTColor::BLACK;
}

template <typename Object>
Object* RBTree<Object>::Retrieve(int key)
{
	RBTNode* tmpn = root;
	while (true)
	{
		if (key < *(tmpn->key))
		{
			if (tmpn->left == nil || key > * (tmpn->left->key))
			{
				Object* res = tmpn->key;
				Delete(tmpn);
				return res;
			}
			else if (key == *(tmpn->left->key))
			{
				Object* res = tmpn->left->key;
				Delete(tmpn->left);
				return res;
			}
			else
			{
				tmpn = tmpn->left;
			}
		}
		else if (key == *(tmpn->key))
		{
			Object* res = tmpn->key;
			Delete(tmpn);
			return res;
		}
		else
		{
			if (tmpn->right != nil)
				tmpn = tmpn->right;
			else
				return nullptr;
		}
	}
}

#endif