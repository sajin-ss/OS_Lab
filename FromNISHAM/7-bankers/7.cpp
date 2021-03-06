#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#define max 20
using namespace std;

void print(vector<int> a)
{
  auto i=a.begin();
  auto k=a.end();
  //cout<<"\n";
  for(;i!=k;++i)
    {
      cout<<*i<<" ";
    }
  cout<<"\n";
}

class matrix
{
public:
  int r;
  int c;
  int m[max][max];
  void read()
  {
    //cout<<"dim : ";
    cin>>r>>c;
    for(int i=0;i<r;++i)
      for(int j=0;j<c;++j)
	cin>>m[i][j];
  }
  matrix operator -(matrix b)
  {
    matrix d;
    d.r=r;
    d.c=c;
    for(int i=0;i<r;++i)
      for(int j=0;j<c;++j)
	d.m[i][j]=m[i][j]-b.m[i][j];
    return d;
  }
  void print()
  {
    for(int i=0;i<r;++i)
      {
	for(int j=0;j<c;++j)
	  cout<<m[i][j]<<" ";    
	cout<<"\n";
      }
  }
  int find(vector<int> row, vector<bool> finish)
  {
    for(int i=0;i<r;++i)
      {
	int f=1;
	for(int j=0;j<c;++j)
	  {
	    if(m[i][j]>row[j])
	      f=0;
	  }
	if(f && !finish[i]){
	  cout<<i<<"th row : ";
	  ::print(getRow(i));
	  ::print(row);
	  return i;
	}
      }
    return -1;
  }
  void chrow(int row, vector<int> nrow)
  {
    for(int j=0;j<c;++j)
      m[row][j]=nrow[j];
  }
  vector<int> getRow(int row)
  {
    vector<int> a;
    for(int j=0;j<c;++j)
      a.push_back(m[row][j]);
    return a;
  }
};

vector<int> minusv(vector<int> a, vector<int> b)
{
  auto i=a.begin();
  auto j=b.begin();
  auto k=a.end();
  auto l=b.end();
  vector<int> res;
  for(;i!=k&&j!=l;++i,++j)
    {
      res.push_back(*i-*j);
    }
  return res;
}
vector<int> plusv(vector<int> a, vector<int> b)
{
  auto i=a.begin();
  auto j=b.begin();
  auto k=a.end();
  auto l=b.end();
  vector<int> res;
  for(;i!=k&&j!=l;++i,++j)
    {
      res.push_back(*i+*j);
    }
  return res;
}

vector<int> read()
{
  vector<int> temp;
  int n;
  cin>>n;
  for(int i=0;i<n;++i){
    int t;
    cin>>t;
    temp.push_back(t);
  }
      return temp;
}
class bankers
{
public:
  int r,c;
  matrix allocated, total, need;
  vector<int> avail;
  
  bool request(int row, vector<int> req)
  {
    for(int i=0;i<c;++i){

      if(req[i]>need.m[row][i])
	{
	  cout<<"Request greater than need";
	  return false;
	}
      
      if(req[i]>avail[i])
	{
	  cout<<"Request greater than available";
	  return false;
	}
    }
    cout<<"\n Allocating resources to process "<<row<<" : ";
    print(req);
    avail = minusv(avail, req);
    allocated.chrow(row,plusv(allocated.getRow(row),req));
    need.chrow(row,minusv(need.getRow(row),req));
    auto temp = safety();
    if(!temp.first)
      {
	cout<<"\n Allocation is unsafe, rolling back changes";
	avail = plusv(avail, req);
	allocated.chrow(row,minusv(allocated.getRow(row),req));
	need.chrow(row,plusv(need.getRow(row),req));	
      }
    else
      {
	cout<<" new Safe sequence : ";
	print(temp.second);
      }
  }
  pair<bool,vector<int> > safety()
  {
    vector<int> work= avail;
    vector<bool> finish;
    for(int i=0;i<r;++i)
      finish.push_back(false);
    
    vector<int> safeSequence;
    while(find(finish.begin(),finish.end(),false)!=finish.end()){
      int row=need.find(work,finish);
      if(row==-1){
	auto temp=*new pair<bool,vector<int> >;
	temp.first=false;
	temp.second=*new vector<int>;
	return temp;
      }
      else
	{
	  for(int j=0;j<c;++j)
	    {
	      work[j]+=allocated.m[row][j];
	    }
	  finish[row]=true;
	  safeSequence.push_back(row);
	}
    }
    auto temp=*new pair<bool,vector<int> >;
    temp.first=true;
    temp.second=safeSequence;
    return temp;
  }
  bankers(matrix allocated, matrix total,vector<int> avail)
  {
    this->r=allocated.r;
    this->c=allocated.c;
    this->allocated = allocated;
    this->total = total;
    this->avail = avail;
    this->need = this->total - this->allocated;
    auto safe = safety();
    if(!safe.first)
      cout<<"Initial State is not safe\n";
    else
      {
	cout<<"\nSafe sequence is : ";
	print(safe.second);
      }
  }
  void describe()
  {
    cout<<endl;
    cout<<"Allocated"<<endl;
    allocated.print();
    cout<<endl<<"Total"<<endl;
    total.print();
    cout<<endl<<"Need"<<endl;
    need.print();
    cout<<endl<<"Available"<<endl;
    print(avail);
    
  }
};
int main()
{
  freopen("input","r",stdin);
  matrix al, tot;
  al.read();
  tot.read();
  vector<int> res=read();

  bankers b(al,tot,res);
  b.describe();
  int process;
  cin>>process;
  vector<int> req=read();
  b.request(process,req);
  cout<<"After state";
  b.describe();
  return 0;
}


/*
INPUT

5 3
0 1 0
2 0 0
3 0 2
2 1 1
0 0 2

5 3
7 5 3
3 2 2
9 0 2
2 2 2
4 3 3

3
3 3 2

0
3
3 3 2

OUTPUT

1th row : 1 2 2 
3 3 2 
3th row : 0 1 1 
5 3 2 
0th row : 7 4 3 
7 4 3 
2th row : 6 0 0 
7 5 3 
4th row : 4 3 1 
10 5 5 

Safe sequence is : 1 3 0 2 4 

Allocated
0 1 0 
2 0 0 
3 0 2 
2 1 1 
0 0 2 

Total
7 5 3 
3 2 2 
9 0 2 
2 2 2 
4 3 3 

Need
7 4 3 
1 2 2 
6 0 0 
0 1 1 
4 3 1 

Available
3 3 2 

 Allocating resources to process 0 : 3 3 2 

 Allocation is unsafe, rolling back changesAfter state
Allocated
0 1 0 
2 0 0 
3 0 2 
2 1 1 
0 0 2 

Total
7 5 3 
3 2 2 
9 0 2 
2 2 2 
4 3 3 

Need
7 4 3 
1 2 2 
6 0 0 
0 1 1 
4 3 1 

Available
3 3 2 

*/
