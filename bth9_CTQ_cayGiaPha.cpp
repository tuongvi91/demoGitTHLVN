#include <iostream>
#include <string>
#include <queue>


using namespace std;

struct Person
{
	string name;
	int yearOfBirth;
};

struct FT //Family Tree
{
	Person data;
	FT *child, *sibling;
};

FT* createNode(Person p, FT* child, FT* sibling)
{
	return new FT{p, child, sibling};
}

FT* createFT()
{
	FT *n1, *n2, *n3, *n4, *n5, *n6, *n7, *n8;
	n1 = createNode({"Nguyen H", 1970}, NULL, NULL);
	n2 = createNode({"Nguyen G", 1965}, NULL, n1);
	n3 = createNode({"Nguyen F", 1965}, NULL, NULL);
	n4 = createNode({"Nguyen E", 1960}, NULL, n3);
	n5 = createNode({"Nguyen D", 1955}, NULL, n4);
	n6 = createNode({"Nguyen C", 1935}, n2, NULL);
	n7 = createNode({"Nguyen B", 1930}, n5, n6);
	n8 = createNode({"Nguyen A", 1900}, n7, NULL);
	return n8;
}

void printFTBFS(FT* root)
{
	queue<FT*> q; FT *p;
	if (root)
	{
		q.push(root);
		while (!q.empty())
		{
			p = q.front(); q.pop();
			cout << p->data.name << " " << p->data.yearOfBirth << endl;
			p = p->child;
			while (p)
			{
				q.push(p);
				p = p->sibling;
			}	
		}
	}
}

//dem so nguoi trong cay 
int countPersons(FT *root)
{
	if (root == NULL) return 0;
	else
	{
		int count=1;
		FT *r = root->child;
		while (r != NULL)
		{
			count=count+countPersons(r);
			r=r->sibling;
		}
		return count;
	} 
}
//Tính so the he
int height(FT *root)
{
	if (root==NULL) return 0;
	else
	{
		int h, max=0;
		FT *r=root->child;
		while (r!= NULL)
		{
			h=height(r);
			if (h>max) max=h;
			r=r->sibling;
		}
		return h+1;
	}
}
//Tìm mot nguoi theo tên
FT* search(FT *root, string name)
{
	FT *r, *result;
	if(root==NULL) return NULL;
	else
	{
		if (root->data.name==name) return root;
		else
		{
			r = root->child;
			while (r)
			{
				result = search(r, name);
				if (result != NULL)
				return result;
				else r=r->sibling;
			}			
		}
		return NULL;
	}
}

//kiem tra nguoi ten Y co phai con nguoi ten X khong?
bool isParent(FT* root, string x, string y)
{
	FT *r;
	r=search(root, x);
	if (r==NULL) return false;
	else 
	{
		FT *p=r->child;
		while (p != NULL)
		{
			if (p->data.name==y) return true;
			else 
			{
				p=p->sibling;
			}
		}	
		return false;	
	}
}

//Thêm ngu?i q vào con c?a ngu?i tên x trong cây.
void insertChild(FT* root, Person p, string x)
{
	FT *r = search(root, x);
	if (r != NULL) 
	{
		FT *q = createNode(p, NULL, NULL);
		FT *p1 = NULL, *r2=r->child;
		while (r2!=NULL && r2->data.yearOfBirth <p.yearOfBirth)
		{
			p1=r2;
			r2=r2->sibling;
		}
		if (p1 ==NULL) 
		{
			q->sibling=r->child;
			r->child=q;
		}
		else
		{
			q->sibling=r2;
			p1->sibling=q;
		}
	}
}

//liet ke con chau cua mot nguoi co ho ten x
void printDescendants(FT* root, string x)
{
	FT *r = search(root, x);
	if (r!= NULL) 
	{
		printFTBFS(r);
	}
	
}

//in nhung nguoi thuoc the he thu k:
void printByLevel(FT* root, int k)
{
	if (root == NULL) return ;
	else if (root != NULL)
	{
		if (k==1) 
			{
				cout<<root->data.name<<" - "<<root->data.yearOfBirth<<endl;
			}
		else
		{
			FT *p=root->child;
			while (p!=NULL)
			{
				printByLevel(p, k-1);
				p=p->sibling;
			}
		}					
	}
	 
}

//tinh bac cua cay
//duyet tung nut, moi nut dem so nut con, sau do chon nut con lon nhat
int degree(FT* root)
{
	int max=0, count;
	if (root==NULL)	return 0;
	else
	{ FT *r=root;
		count =0;
		r=root->child;
		while (r!= NULL)
		{
			count ++;
			r=r->sibling;
		}
		r=root->child;
		while (r!= NULL)
			{
				int d = degree(r);
				if (d>count) count=d;
				r=r->sibling;
			}	
	}
	return count;
}



//xoa nguoi tren cay co ten X

FT * findParent(FT* root, string x, FT *p)
{
	FT *r=root;
	FT *p1;
		if  (r!= NULL)
		{
			FT *rt = r; 
			if (isParent(rt, rt->data.name ,p->data.name)==true) p1=rt;
			rt = r->child;
			while (rt != NULL)
			{
				findParent(rt,  x, p);
				rt=rt->sibling;
			}
		}	
		return p1;	
}

void deleteByName(FT* root, string x)
{
	FT *p, *p1, *p2, *r;
	if (root!=NULL)
	{
		p=search(root, x);
		p2=p->sibling;
		p1 = findParent(root, x, p);
	}
	if (p1->child->data.name == p->data.name) p->sibling= p1->child; else p->sibling= p2->sibling;
	delete(p);


}


int main()
{
	struct Person;
	struct FT *root, *r;
	int count, h;
	string name, y, x;
	root=createFT();
	cout<<"cay gia pha: "<<endl;
	printFTBFS(root);
//	cout <<"so nguoi trong cay gia pha: ";
//	count=countPersons(root);
//	cout<<count<<endl;
//	cout <<"so the he trong cay gia pha: ";
//	h = height(root);
//	cout<<h<<endl;
//	
//	name="Nguyen A N";
//	r=search(root, name);
//	if (r==NULL) 
//	cout<<"khong co nguoi co ten "<<name<<endl;
//	else
//	cout<<"co nguoi can tim: "<<r<<endl;
//	
//	x="Nguyen A";
//	y="Nguyen C";
//	cout<<"nguoi '"<<y<<"' la con cua nguoi ten '"<<x<<"' :"<<isParent(root, x, y)<<endl;
//	
//	insertChild(root,  {"Nguyen Y", 2000},  x);
//	cout<<"cay gia pha: "<<endl;
//	printFTBFS(root);
//	cout<<"con chau cua "<<x<<" la: "<<endl;
//	printDescendants(root, x);
//	int k=2;
//	cout<<"nhung nguoi thuoc the he thu "<<k<<endl;
//	printByLevel(root, k);
//	h= degree(root);
//	cout<<"bac cua cay: "<<h<<endl;
	deleteByName(root, "Nguyen B");
	cout<<"cay gia pha sau khi xoa: "<<endl;
	printFTBFS(root);
}
