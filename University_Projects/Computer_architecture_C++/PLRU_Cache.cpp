// headers
#include <iostream>
#include "conio.h"
#include <iomanip>
#include <stddef.h>
#include <vector>

/* This program is a simple simulation of the "Pseudo Least Recently Used"
   cache replacement policy. */

using namespace std;


// address trace
int addresses[] = {0x0000, 0x0004, 0x000c, 0x2200, 0x00d0, 0x00e0,
				   0x1130, 0x0028, 0x113c, 0x2204, 0x0010, 0x0020,
				   0x0004, 0x0040, 0x2208, 0x0008, 0x00a0, 0x0004,
				   0x1104, 0x0028, 0x000c, 0x0084, 0x000c, 0x3390,
				   0x00b0, 0x1100, 0x0028, 0x0064, 0x0070, 0x00d0,
			       0x0008, 0x3394};

// integer log2
int log2(int n)
{
	unsigned int tmp = n;
	int result = 0;
	while (tmp >>= 1) ++result;
	return result;
}

struct node
{
	int flag;
	node *left;
	node *right;
	int nTag;
};

/*-------------------------------------------------------------------------------------*/

class PLRUtree
{
	public:
		PLRUtree();
		node *find_LRU_tag();
		void destroy_tree();
		void buildNewTree(int height);
		void updateLRU(int index);
		vector<node*> leafList;
		int h;

	private:
		void buildTree(int height, node *leaf);
		void destroy_tree(node *leaf);
		node *find_LRU_tag(node *leaf);
		node *root;
};

// constructor
void PLRUtree::buildTree(int height, node *leaf)
{
	if (height > 0)
	{
		node *lNode = new node;
		lNode->flag = 0;
		lNode->left = NULL;
		lNode->right = NULL;
		node *rNode = new node;
		rNode->flag = 0;
		rNode->left = NULL;
		rNode->right = NULL;
		if ((height - 1) == 0)
		{
			lNode->nTag = 0xA0000000;
			rNode->nTag = 0xA0000000;
			leafList.push_back(lNode);
			leafList.push_back(rNode);
		}
		else
		{
			lNode->nTag = NULL;
			rNode->nTag = NULL;
		}
		leaf->left = lNode;
		leaf->right = rNode;
		buildTree((height - 1), lNode);
		buildTree((height - 1), rNode);
	}
}

void PLRUtree::buildNewTree(int height)
{
	root = new node;
	root->flag = 0;
	root->left = NULL;
	root->right = NULL;
	h = height;
	if (height == 0)
	{
		root->nTag = 0xA0000000;
		leafList.push_back(root);
	}
	else
	{
		root->nTag = NULL;
	}
	buildTree(height, root);
}

PLRUtree::PLRUtree()
{
	root = NULL;
}

// destructor
void PLRUtree::destroy_tree(node *leaf)
{
	if (leaf != NULL)
	{
		destroy_tree(leaf->left);
		destroy_tree(leaf->right);
		delete leaf;
	}
}

node *PLRUtree::find_LRU_tag(node *leaf)
{
	if (leaf != NULL)
	{
		if (leaf->left == NULL)
		{
			return leaf;
		}
		else if (leaf->flag == 0)
		{
			leaf->flag = 1;
			return find_LRU_tag(leaf->left);
		}
		else
		{
			leaf->flag = 0;
			return find_LRU_tag(leaf->right);
		}
	}
	else
	{
		return NULL;
	}
}

void PLRUtree::updateLRU(int index)
{
	int bit;
	node *leaf = root;
	for (int i = 0; i < h; i++)
	{
		bit = index & (1 << (h - i - 1));
		bit = bit >> (h - i - 1);
		if (leaf->flag == bit)
		{
			leaf->flag = leaf->flag ^ 1;
		}
		if (bit == 1)
		{
			leaf = leaf->right;
		}
		else
		{
			leaf = leaf->left;
		}
	}
}

void PLRUtree::destroy_tree()
{
	destroy_tree(root);
}

node *PLRUtree::find_LRU_tag()
{
	return find_LRU_tag(root);
}

/*-------------------------------------------------------------------------------------*/
class Cache
{
public:
	int L, N, K;
	PLRUtree *treeList;
	
    // constructor...
    Cache(int newL, int newN, int newK)
    {
		L = newL;
		N = newN;
		K = newK;

		treeList = new PLRUtree[N];
		for (int i = 0; i < N; i++)
		{
			treeList[i].buildNewTree(log2(K));
		}
    }

	~Cache()
	{
		for (int i = 0; i < N; i++)
		{
			treeList[i].destroy_tree();
		}
	}

	bool hitTest(int addr)
	{
		int setNo = (addr >> log2(L)) & (N - 1);
		int tagTarget = addr >> log2(L);
		
		PLRUtree myTree = treeList[setNo];

		for(int i = 0; i < K; i++)
		{
			if(myTree.leafList[i] == tagTarget)
			{
				myTree.updateLRU(i); // make the tag that was hit the most recently used tag
				return true;
			}
		}
		
		node *tmp = myTree.find_LRU_tag();
		tmp->nTag = tagTarget; // replace least recently used tag

		return false;
	}
};

/*-------------------------------------------------------------------------------------*/

int main()
{
	int configs[4][3] = { {16, 8, 1}, {16, 4, 2}, {16, 2, 4}, {16, 1, 8} };

	int trace_length = (sizeof(addresses) / sizeof(int));

	for(int i = 0; i < 4; i++)
	{
		int hitNo = 0;
		Cache *myCache = new Cache(configs[i][0], configs[i][1], configs[i][2]);
		cout << "Cache configuration: L = " << configs[i][0] << ", N = " << configs[i][1] << ", K = " << configs[i][2] << "." << endl;
		int trace_length = (sizeof(addresses)/sizeof(int));
		for(int j = 0; j < trace_length; j++)
		{
			if(myCache->hitTest(addresses[j]))
			{
				hitNo++;
			}
		}
		cout << "Accesses: " << trace_length << " Hits: " << hitNo << " Misses: " << (trace_length - hitNo) << "." << endl;
		delete myCache;
	}

    _getch();
    return 0;
}
