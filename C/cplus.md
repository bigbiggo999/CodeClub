


[cin使用详情](https://blog.csdn.net/u011486738/article/details/82284581)











###结构体通过sort排序
```cpp

//重载运算符
typedef struct node{
    int num;
    int value;
    bool operator < (const node& N) const{
        return value>N.value;
    }
}Node;

int n;
vector<string> E[maxn];
vector<Node> shop;
int main(){
    
    cin.tie(0);
    ios::sync_with_stdio(false);
    cin>>n;
    shop.resize(n);
    UP(n){
        cin>>shop[i].num>>shop[i].value;
    }
    sort(shop.begin(), shop.end());
    cout<<shop[0].num<<" "<<shop[0].value;
    return 0;
}

//结构体外定义compare函数

typedef struct node{
    int num;
    int value;
    
}Node;
bool nodeComparsion(Node node1,Node node2){
    return node1.value>node2.value;
}

int n;
vector<string> E[maxn];
vector<Node> shop;
int main(){
    
    cin.tie(0);
    ios::sync_with_stdio(false);
    cin>>n;
    shop.resize(n);
//    int num,value;
    UP(n){
        cin>>shop[i].num>>shop[i].value;
    }
    sort(shop.begin(), shop.end(), nodeComparsion);
    cout<<shop[0].num<<" "<<shop[0].value;
    
    
    
    return 0;
}
```



###读取一行字符串按照空格分割
```cpp
cin.ignore();
    UP(n){
        getline(cin,temp);
        istringstream is(temp);
        while (is>>temp) {
            E[i].push_back(temp);
        }
    }
```

###从文件读入和输出到文件
```cpp
ofstream fout("sum.txt");
ifstream fin("in.txt");
int main(){
//    ios::sync_with_stdio(false);
    int a,b;
    fin>>a >>b;
    fout<< "hello"<<a<<b<<endl;
    return 0;
}

```


###交通规划
```cpp
#include <bits/stdc++.h>
typedef unsigned int uint32_t;
#define UP(x) for(uint32_t i=0;i<x;i++)
using namespace std;

const int maxn = 205;
vector<pair<int,int>> E[maxn];
int n,m;
int d[maxn],inq[maxn],fee[maxn];
void init(){
    UP(n+1){
        E[i].clear();
        d[i] = INT_MAX;
        fee[i] = INT_MAX;
        inq[i] = 0;
    }
}

int main(){
    cin>>n>>m;
    init();
    UP(m){
        int x,y,z;
        scanf("%d%d%d",&x,&y,&z);
        E[x].push_back(make_pair(y, z));
        E[y].push_back(make_pair(x, z));
    }
    int s = 1;
    queue<int> Q;
    Q.push(s);
    inq[s] = 1;
    d[s] = 0;
    fee[s] = 0;
    while (!Q.empty()) {
        int now = Q.front();
        Q.pop();
        inq[now] = 0;
        UP(E[now].size()){
            int v = E[now][i].first;
            if(d[v]>d[now] + E[now][i].second){
                d[v] = d[now] + E[now][i].second;
                fee[v] = E[now][i].second;
                if(inq[v]==1) continue;
                inq[v] = 1;
                Q.push(v);
            }else if(d[v] == d[now] + E[now][i].second){
                fee[v] = min(fee[v], E[now][i].second);
            }
        }
    }
    int sum = 0;
    for(int i = 1;i<=n;i++){
        sum+=fee[i];
    }
    cout<<sum<<endl;
}
```

###最大的矩形
```cpp
#include <bits/stdc++.h>
#include <algorithm>
typedef unsigned int uint32_t;
#define UP(x) for(uint32_t i=0;i<x;i++)
using namespace std;



vector<int> arr;
vector<vector<int>> mat;
int main(){
    ios::sync_with_stdio(false);
    int n=0;

    cin>>n;
    arr.resize(n);
    UP(n){
        cin>>arr[i];
    }
    mat.resize(n);
    UP(n){
        mat[i].resize(n);
    }
    UP(n){
        int min_height = arr[i];
        for(int j=i;j<n;j++){
            mat[i][j] = (min_height = min(arr[j],min_height));
        }
    }
    int res = 0;
    
    UP(n){
        for(int j=i;j<n;j++){
            res = max(static_cast<uint32_t>(res), static_cast<uint32_t>(mat[i][j]*(j-i+1)));
        }
    }
    cout<<res;
    return 0;
}
```

###raid5磁盘恢复

```cpp
#include<bits/stdc++.h>
#define UP(x) for(uint32_t i=0;i<x;i++)
typedef unsigned int uint32_t;
using namespace std;
uint32_t len = 0;
//异或操作
char getc(char c){
    if(c<10)
        return c+'0';
    else if(c<=16)
        return c+'A'-10;
    else if(c<='9')
        return c-'0';
    else if(c<='F')
        return c-'A'+10;
    return ' ';
}
void yh(string &a,string b){
    UP(8){
        a[i]=getc(getc(a[i])^getc(b[i]));
    }
}
void getdata(string dskd[],uint32_t n,uint32_t s,uint32_t l,uint32_t r){
    uint32_t level=r/((n-1)*s);                //获取读取的数据层数
    uint32_t curp=(n-level%n)-1;            //获取读取的数据层数P块所在位置
    uint32_t blk=level*s+r%s;                //获取数据所在硬盘的块索引
    uint32_t disk=r%((n-1)*s)/s+curp+1;        //获取读取的数据所在硬盘索引
    disk=disk%n;
    if(len<blk*8+8||len==0){                //如果获取的长度超过了硬盘数据长度，直接输出减号退出
        cout<<"-"<<endl;
        return;
    }
    if(dskd[disk].length()!=0)                 //如果当前盘存在，直接输出
        cout<<dskd[disk].substr(blk*8,8)<<endl;
    else if(dskd[disk].length()==0&&n-l==1){ //如果当前盘缺失，且可以恢复，则通过异或读取对应的数据
        string str="00000000";
        UP(n){
            if(i!=disk)
                yh(str,dskd[i].substr(blk*8,8));
        }
        cout<<str<<endl;
    }else
        cout<<"-"<<endl;                     //如果当前盘缺失且不可以恢复，输出减号。
}
int main(){
    ios::sync_with_stdio(false);
    uint32_t n,s,l; //n个磁盘, s个块为一个条带,l个完整
    cin>>n>>s>>l;
    string dskd[1010];
    uint32_t dpos;
    UP(l){
        cin>>dpos;
        cin>>dskd[dpos];
    }
    uint32_t m;
    cin>>m;
    uint32_t a[1000];
    UP(m){
        cin>>a[i];
    }
    UP(n){
        if(dskd[i].length()!=0){
            len = dskd[i].length();
            break;
        }
    }
    UP(m){
        getdata(dskd,n,s,l,a[i]);
    }
    return 0;
}

```

###24点游戏

```cpp
#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <map>



using namespace std;

int calculate(int a,int b,char c);

int main(){
    int n;
    cin>>n;
    
    string line;
    string temp;
    stack<char> stk;
    stack<int> result_stk;
    vector<int> result;

    char topChar;
    while(n>=1){
        cin>>line;
        temp = "";
        for(auto c:line){
            if(stk.empty()) topChar = NULL;
            else topChar = stk.top();
            switch (c) {
                case '+':
                    if(stk.empty()) stk.push(c);
                    else{
                        temp+=topChar;
                        stk.pop();
                        stk.push('+');
                    }
                    break;
                case '-':
                    if(stk.empty()) stk.push(c);
                    else{
                        temp+=topChar;
                        stk.pop();
                        stk.push('-');
                    }
                    break;
                case 'x':
                        if(stk.empty()) stk.push('*');
                        else if(topChar=='-'||topChar=='+'){
                            stk.push('*');
                        }else{
                            temp+=topChar;
                            stk.pop();
                            stk.push('*');
                        }
                        break;
                case '/':
                    if(stk.empty()) stk.push('/');
                    else if(topChar=='-'||topChar=='+'){
                        stk.push('/');
                    }else{
                        temp+=topChar;
                        stk.pop();
                        stk.push('/');
                    }
                    break;
                default:
                    temp+=c;
                    break;
            }
        }
        while(!stk.empty()){
            temp+=stk.top();
            stk.pop();
        }
        for(auto c:temp){
            if(c>='1'&&c<='9'){
                result_stk.push(c-'0');
            }else{
                int b = result_stk.top();
                result_stk.pop();
                int a = result_stk.top();
                result_stk.pop();
                result_stk.push(calculate(a, b, c));
            }
        }
        result.push_back(result_stk.top());
        result_stk.pop();
        n--;
    }
    for(int i=0;i<result.size();i++){
        if(result[i]==24) cout<<"Yes"<<endl;
        else cout<<"No"<<endl;
    }

}


int calculate(int a,int b,char c){
    switch (c) {
        case '-':
            return a-b;
            break;
        case '+':
            return a+b;
            break;
        case '*':
            return a*b;
            break;
        case '/':
            return a/b;
            break;
        default:
            return 0;
            break;
    }
}



//    map<char, int > operations;
//    operations.insert(pair<char,int>('+', 1));
//    operations.insert(pair<char,int>('-', 1));
//    operations.insert(pair<char,int>('x', 2));
//    operations.insert(pair<char,int>('/', 2));
```




###[字符串匹配算法](https://blog.csdn.net/starstar1992/article/details/54913261/)

```cpp
int kmp(string s, string p){
    int i = 0,k = -1;
    int next[p.size()];
    getPMT(p, next);
    for(int k = 0;k<p.size();k++){
        cout <<next[k]<<" ";
    }
    cout<<endl;
    
    for(;i<s.size();i++){
        while (k>-1 && p[k+1] != s[i]) {
            k = next[k];
        }
        if(p[k+1]==s[i]){
            k++;
        }
        if(k == p.size()-1) return i-k;
    }
    return -1;

}

void getPMT(string p,int pmt[]){
    int size = int(p.size());
    pmt[0] = -1;
    int k = -1;
    for(int i = 1;i<size;i++){
        while (k>-1 && p[k+1]!=p[i]) {
            k = pmt[k];
        }
        if(p[k+1] == p[i]) {
            k++;
        }
        pmt[i] = k;
        
    }
}

```




###有面值分别为1，3，5的三种硬币若干，需要凑成11元最少需要多少硬币，凑成n元最少需要多少硬币？
```cpp
int main(){
    int n;
    while (cin>>n) {
        //n++;
        if(n==-1) break;
        if(n<=0) return 0;
        vector<int> min(n+1);
        vector<int> value = {1,3,5};
        min[0] = 0;
        for(int i = 1;i<=n;i++){
            min[i] = min[i-1]+1;
            for(int j = 0;j<value.size();j++){
                if(i<value[j]) break;
                if(min[i-value[j]]<min[i-1]){
                    min[i] = min[i-value[j]]+1;
                }
            }
        }

        cout<<min[n]<<endl;
    }

    return 0;
}

```


###最长回文子串

```cpp
// Dynamic Programming
string longestPalindrome(string s){
    int size = int(s.size());
    if(s.size()==0) return "";
    vector<vector<bool>> dp(size+1,vector<bool>(size+1));
    int left = 0,right = 0;
    for(int i = size-2;i>=0;i--){
        dp[i][i] = true;
        for(int j = i+1;j<size;j++){
            dp[i][j] = (s[i] == s[j]&&(j-i<3||dp[i+1][j-1]));
            if(dp[i][j]&&(right-left<j-i)){
                left=i;
                right=j;
            }
        }

    }
    return s.substr(left,right-left+1);
}



//暴力解法
string longestPalindrome(string s){
    size_t size = s.size();
    string result;
    string rever(s);
    reverse(rever.begin(), rever.end());
    int l = 0;
    for(int i = 0;i<size;i++){
        for(int j = 0;j<size;j++){
            int length = 0;
            int n = i;
            int m = j;
            while(m<size&&n<size){
                if(s[n]!=rever[m]) break;
                else {
                    length++;
                    m++;
                    n++;
                }
            }
            string temp = s.substr(i,length);
            string reverTemp = temp;
            reverse(reverTemp.begin(), reverTemp.end());
            bool sameInex = (strstr(s.c_str(),temp.c_str())==strstr(s.c_str(),reverTemp.c_str()));
            if(length>=l&&sameInex){
                l = length;
                result = temp;
            }
        }
    }
    return result;
}
```


###找两个有序数组的中位数
```cpp


//将两数组合并然后求中位数

double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2){
    if(nums1.size() == 0)
        return MedofArray(nums2);
    if(nums2.size() == 0)
        return MedofArray(nums1);
    vector<int> num3;
    int size = (nums1.size()+nums2.size());
    int mid = size/2;
    int flag = !(size%2);
    int i,m1,m2,cur;
    double a,b;
    for(i=m1=m2=0;i<size;i++){
        a = m1<nums1.size()?nums1[m1]:INT_MAX;
        b = m2<nums2.size()?nums2[m2]:INT_MAX;
        if(a<b){
            num3.push_back(nums1[m1]);
            m1++;
        }else
        {
            num3.push_back(nums2[m2]);
            m2++;
        }
        if(i == mid)
            break;
    }
    
    return (num3[mid]+num3[mid-flag])/2.0;
}

double MedofArray(vector<int>& nums){
    int mid = nums.size()/2;
    int flag = !(nums.size()%2);
    return (nums[mid] + nums[mid-flag])/2.0;
    
}








//

#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
using namespace std;


double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2);

int main(){
    vector<int> nums1={1,2,4,7};
    vector<int> nums2 = { 2,3,4,5,6,7,8,9};
    // 1 2 3
    // 1 2  5 10 13
    cout<<findMedianSortedArrays(nums1, nums2)<<endl;
    return 0;

}

double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
    const int n1 = nums1.size();
    const int n2 = nums2.size();
    if(n1>n2) return findMedianSortedArrays(nums2, nums1);
    const int k = (n1 + n2 + 1)/2;
    int left = 0;
    int right = n1;
    while(left < right){
        const int m1 = left + (right - left)/2;
        const int m2 = k - m1;
        if(nums1[m1]<nums2[m2-1])
            left = m1 + 1;
        else
            right = m1;
    }
    const int m1 = left;
    const int m2 = k - left;
    const int c1 = max(m1 <= 0 ? INT_MIN:nums1[m1-1],
                       m2 <= 0 ? INT_MIN:nums2[m2-1]);
    if((n1 + n2)%2 == 1)
        return c1;
    const int c2 = min(m1 >= n1 ? INT_MAX: nums1[m1],
                       m2 >= n2 ? INT_MAX : nums2[m2]);
    return (c1 + c2) * 0.5;
}

```




###cout使用测试
```cpp
#include <iostream>
#include <iomanip>
using namespace std;

int main(){
    int i = 90;
    cout<< hex<<i<<dec<<i<<endl;
    cout<<setiosflags(ios::uppercase);
    cout<<hex<<i<<endl;
    double k = 99.11354;
    cout<<setiosflags(ios::fixed)<<setprecision(3)<<k<<endl;
    cout<<showpos<<k<<endl;
    cout <<resetiosflags(ios::fixed|ios::showpos)<<dec<<defaultfloat;
    cout << "默认显示：" << endl;
    for (int i = 1; i < 1000; i *= 10){
        cout<<setiosflags(ios::left);
        cout <<setw(8) << i << ':' << setw(8) << i * 10 << endl;
    }
    cout << setfill('0') << setw(8) << 1.22 << endl;
    cout<<"\abc\108\\D\t\'"<<endl;
    return 0;
}

```

###求数据中的最大值，最小值，中位数
```cpp
#include <iostream>
#include <iomanip>

using namespace std;

void findMedium(int a[],int length);

int main(int argc, const char * argv[]) {
    int length;
    cin>>length;
    if(length<=0) return 0;
    int a[length+1];

    for(int i = 0;i<=length-1;i++){
        cin>>a[i];
    }
    findMedium(a, length);

    return 0;
}

void findMedium(int a[],int length){
    int min,max;
    min = max = 0;
    double medium = 0.0;
    int mediumInt = 0;
    if(length==1) {
        cout <<a[0]<<" "<<a[0]<<" "<<a[0]<<endl;
        return;
    }
    if(a[0]<=a[length-1]){
        min = a[0];
        max = a[length-1];
        cout <<max<<" ";

        if(length%2==0){
            medium = (a[length/2-1]+a[length/2])*1.0/2;
            mediumInt = medium;
            if(mediumInt-medium!= 0){
                cout<<fixed<<setprecision(1)<<medium<<" ";
            }else{
                cout<<mediumInt<<" ";
            }
            cout<<min;
        }else{
            medium = a[length/2];
            cout<<medium<<" "<<min;
        }
    }
    else if(a[0]>a[length-1]){ //  1 1 1 1
        max = a[0];
        min = a[length-1];
        cout <<max<<" ";

        if(length%2==0){
            medium = (a[length/2-1]+a[length/2])*1.0/2;
            mediumInt = medium;
            if(mediumInt-medium!= 0){
                cout<<fixed<<setprecision(1)<<medium<<" ";
            }else{
                cout<<mediumInt<<" ";
            }
            cout<<min;
        }else{
            medium = a[length/2];
            cout<<medium<<" "<<min;
        }
    }
}

```



