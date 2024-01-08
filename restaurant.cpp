#include "main.h"

inline bool sortByFrequency(const std::pair<char, int>& a, const std::pair<char, int>& b) {
    if(a.second!=b.second)
    return a.second < b.second;
    else if(((a.first>='a') && (b.first>='a'))||(a.first<='Z'&&b.first<='Z')) return a.first<b.first;
    return a.first>b.first;
}
inline  vector<std::pair<char, int>>  handle_name(string& name,bool& check){
    unordered_map<char, int> charCount;
    for (char c : name) {
        charCount[c]++;
    }
    vector<std::pair<char, int>> charCountVector(charCount.begin(), charCount.end());
    if(charCountVector.size()<3) {
        check=true;
        return charCountVector;
    } 
    string s="";
    unordered_map<char,int> encode;
    int k =name.length();
    for(int i=0;i<k;++i){
        if(name[i]>='a'&&name[i]<='z')
            name[i]=(name[i]+(charCount[name[i]]%26)<='z')?name[i]+(charCount[name[i]]%26):name[i]+(charCount[name[i]]%26)-26;
        else  name[i]=(name[i]+(charCount[name[i]]%26)<='Z')?name[i]+(charCount[name[i]]%26):name[i]+(charCount[name[i]]%26)-26;
    }
    for (auto pair : charCountVector) {
        if(pair.first>='a'&&pair.first<='z'){
            encode[(pair.first+(pair.second%26)<='z')?pair.first+(pair.second%26):pair.first+(pair.second%26)-26]+=pair.second;
        }
        else if(pair.first>='A'&&pair.first<='Z') {
            encode[(pair.first+(pair.second%26)<='Z'&&pair.first+(pair.second%26)>='A')?pair.first+(pair.second%26):pair.first+(pair.second%26)-26]+=pair.second;
        };
    }
    
    vector<std::pair<char, int>> charCountVector2(encode.begin(), encode.end());
    sort(charCountVector2.begin(), charCountVector2.end(), sortByFrequency);
    check=false;
    return charCountVector2;
}
class HuffNode {
    public:
    string binary;
    virtual ~HuffNode() {} // Base destructor
    virtual int weight() = 0; // Return frequency
    virtual bool isLeaf() = 0; // Determine type
    int height;
};
class LeafNode : public HuffNode {
    public:
    char it; // Value
    int wgt; // Weight
    LeafNode(const char& val, int freq) // Constructor
    { it = val; wgt = freq; height=1;binary="";}
    int weight() { return wgt; }
    char val() { return it; }
    bool isLeaf() { return true; }
};
class IntlNode : public HuffNode {
    public:
    HuffNode* lc; // Left child
    HuffNode* rc; // Right child
    int wgt; // Subtree weight
    IntlNode(HuffNode* l, HuffNode* r){
        wgt = l->weight() + r->weight(); lc = l; rc = r; 
        this->height=(lc->height>rc->height)? lc->height+1:rc->height+1;
        binary="";
    }
    int weight() { return wgt; }
    bool isLeaf() { return false; }
    HuffNode* left() const { return lc; }
    void setLeft(HuffNode* b)
    { lc = (HuffNode*)b; }
    HuffNode* right() const { return rc; }
    void setRight(HuffNode* b)
    { rc = (HuffNode*)b; }
    void set_height(){
        height=(this->lc->height>this->rc->height)? this->lc->height+1:this->rc->height+1;
    }
};

class HuffTree {
    public:
    HuffNode* Root; // Tree root
    int stt;
    HuffTree(char& val, int freq){ // Leaf constructor
        Root = new LeafNode(val, freq); 
    }
    HuffTree(char& val, int freq,int stt){ // Leaf constructor 
        Root = new LeafNode(val, freq); 
        this->stt=stt;
    }
    // Internal node constructor
    HuffTree(HuffTree* l, HuffTree* r){ 
        Root = new IntlNode(l->root(), r->root()); 
    }
    HuffTree(HuffTree* l, HuffTree* r,int stt){ 
        Root = new IntlNode(l->root(), r->root()); 
        this->stt=stt;
    }
    ~HuffTree() {
        queue<HuffNode*>tmp;
        if(Root) tmp.push(Root);
        while(!tmp.empty()){
            if(tmp.front()->isLeaf()){
                HuffNode*temp =tmp.front();
                tmp.pop();
                delete temp;
            }
            else{
                tmp.push(((IntlNode*)tmp.front())->left());
                tmp.push(((IntlNode*)tmp.front())->right());
                HuffNode*temp =tmp.front();
                tmp.pop();
                delete temp;
                
            }
        }
    } // Destructor
    HuffNode* del(HuffNode* node){
        if(!node) return nullptr;
        HuffNode* temp=node;
        if(node->isLeaf()){
            delete node;
            
            node=nullptr;
            return nullptr;
        }
        else{
            if(dynamic_cast<IntlNode*>(temp)->left()){
                temp=dynamic_cast<IntlNode*>(temp)->left();
                delete node;
                
                node =nullptr;
                return temp;
            }
            else{
                temp=dynamic_cast<IntlNode*>(temp)->right();
                delete node;
                
                node=nullptr;
                return temp;
            }
        };
        return nullptr;

    }
    HuffNode* root() { 
        return Root; 
    } // Get root
    void setRoot(HuffNode* root){
        this->Root=root;
    }
    int weight() { 
        return Root->weight(); 
    } // Root weight
    
};
class custormer{
    public:
    custormer(string name,int result,int ID) : name(name),res(result),ID(ID),parent(nullptr),left(nullptr),right(nullptr){};
    string name;
    int res;
    int ID;
    int sum_nodego;
    custormer* parent;
    custormer* left;
    custormer* right;
};
class BST{
    public:
    custormer* root;
    vector<custormer*> save;
    int count;
    BST(custormer* cus){
        root=cus;
        root->sum_nodego=1;
        count++;
        save.push_back(cus);
    };
    BST(){
        root=nullptr;
        count=0;
    }
    ~BST(){
        while(count>0){
            custormer* temp=save[save.size()-1];
            save.pop_back();
            count--;
            delete temp;
        }
    }
    void push(custormer* cus){
        custormer* temp=root;
        if(!temp){
            root=cus;
            root->sum_nodego=1;
            count++;
            save.push_back(cus);
            return;
        }
        while(temp->left||temp->right){
            if(cus->res>=temp->res) {
                if(!temp->right) {
                    temp->right=cus; 
                    cus->sum_nodego=1;
                    temp->sum_nodego+=1;
                    count++;
                    save.push_back(cus);
                    return;
                }
                else {
                    temp->sum_nodego+=1;
                    temp=temp->right;
                }
            }
            else {
                if(!temp->left) {
                    temp->left=cus; 
                    cus->sum_nodego=1;
                    temp->sum_nodego+=1;
                    count++;
                    save.push_back(cus);
                    return;
                }
                else {
                    temp->sum_nodego+=1;
                    temp=temp->left;
                }
            };
        }
        if(temp->res>cus->res){
            temp->left=cus;
            cus->sum_nodego=1;
            temp->sum_nodego+=1;
            count++;
            save.push_back(cus);
            return;
        }
        else{
            temp->right=cus;
            cus->sum_nodego=1;
            temp->sum_nodego+=1;
            count++;
            save.push_back(cus);
            return;
        }
    }
    custormer* deletehelp(custormer* root, int value){
        if(!root) return nullptr;
        
        if(value<root->res){
            root->left=deletehelp(root->left,value);
            root->sum_nodego--;
            return root;
        }
        else if(value>root->res){
            root->right=deletehelp(root->right,value);
            root->sum_nodego--;
            return root;
        }
        else{
            if(!root->left){
                custormer*temp=root->right;
                root=nullptr;
                count--;
                return temp;
            };
            if(!root->right){
                custormer*temp=root->left;
                root=nullptr;
                count--;
                return temp;
            };
            custormer* min=root->right;
            while(min->left){
                min=min->left;
            }
            min->sum_nodego=root->sum_nodego-1;
            min->right = deletehelp(root->right, min->res);
            min->left=root->left;
            return min;
        }
    }
};
inline unsigned long long C(int r, int n) {
    if (r > n - r) {
        r = n - r;
    }
    unsigned long long result = 1;
    for (int i = 0; i < r; i++) {
        result *= (n - i);
        result /= (i + 1);
    }
    return result;
}

inline void LIMITLESS(custormer* cus){
    if(!cus) return;
    LIMITLESS(cus->left);
    cout<<cus->res<<"\n";
    LIMITLESS(cus->right);
}
class GOJO {
public:
    int MAXSIZE;
    vector<BST*>hash;

    GOJO(int MAXSIZE) : MAXSIZE(MAXSIZE) {
        for (int i = 0; i < MAXSIZE; ++i) {
            BST* temp= new BST();
            hash.push_back(temp);
        }
    }

    ~GOJO() {
        for (int i = MAXSIZE-1; i >-1; --i) {
            BST* temp=hash[i];
            hash.pop_back();
            delete temp;
        }
    }

    void add_cus(custormer* cus, int ID) {
        hash[(ID-1) % MAXSIZE]->push(cus);
    }
    long long KOKU(custormer* cus){
        if(!cus) return 1;
        if(cus->sum_nodego==1||cus->sum_nodego==2) return 1%MAXSIZE;
        int sum_left=cus->left?cus->left->sum_nodego:0;
        int sum_right=cus->right?cus->right->sum_nodego:0;
        long long x=KOKU(cus->right)%MAXSIZE;
        long long y=KOKU(cus->left)%MAXSIZE;
        long long z=C(sum_left,sum_left+sum_right)%MAXSIZE;
        return(x*y*z)%MAXSIZE;
    }
};
class Sukuna_node{
    public:
    int stt;
    int count;
    int kv;
    int vitri;
    vector<custormer*> fifo;
    Sukuna_node(int kv,int vitri) : stt(-1),count(0) , kv(kv),vitri(vitri){}
    ~Sukuna_node(){
        while(fifo.size()>0){
            delete fifo[fifo.size()-1];    
            fifo.pop_back();
        }
    }
    void add_cus(custormer* cus){
        fifo.push_back(cus);
        stt=0;
        count++;
    }
    void del_cus(){
        fifo.erase(fifo.begin());
        count--;
        stt=0;
    }
};
class SUKUNA {
    public:
    int MAXSIZE;
    int count;
    vector<Sukuna_node*> root_su;
    vector<Sukuna_node*> stt;
    SUKUNA(int MAXSIZE) : MAXSIZE(MAXSIZE),count(0){
        for(int i=1;i<=MAXSIZE;++i){
            Sukuna_node* su_node=new Sukuna_node(i,-1);
            stt.push_back(su_node);
        }
    };
    ~SUKUNA(){
        while(stt.size()>0){
            Sukuna_node* temp= stt[stt.size()-1];
            delete temp;
            stt.pop_back();
        }
    }
    void push_cus(custormer* cus,int kv){
        if(stt[kv-1]->count==0) {
            root_su.push_back(stt[kv-1]);
            stt[kv-1]->vitri=count;
        };
        count=root_su.size();
        stt[kv-1]->add_cus(cus);
        reheapdown(stt[kv-1]->vitri);
        reheapup(stt[kv-1]->vitri);
        int n =root_su.size();
        for(int i=0;i<n;++i){
            root_su[i]->stt++;
        }
    }
    void reheapup(int count){
        if(count==0) return ;
        if(root_su[(count-1)/2]->count>root_su[count]->count){
            Sukuna_node* tmp=root_su[(count-1)/2];
            root_su[(count-1)/2]=root_su[count];
            root_su[(count-1)/2]->vitri=(count-1)/2;
            root_su[count]=tmp;
            root_su[count]->vitri=count;
            return reheapup((count-1)/2);
        };
    };
    void reheapdown(int count){
        if(count>=this->count/2||2*count+1>=this->count) return;
        int l=(2*count+1<this->count)?root_su[2*count+1]->count:root_su[count]->count+1;
        int r=(2*count+2<this->count)?root_su[2*count+2]->count:root_su[count]->count+1;
        if(root_su[count]->count>l||root_su[count]->count>r){
            if(l>r){
                Sukuna_node* tmp=root_su[2*count+2];
                root_su[2*count+2]=root_su[count];
                root_su[count]=tmp;
                root_su[count]->vitri=count;
                root_su[2*count+2]->vitri=2*count+2;
                return reheapdown(2*count+2);
            }
            else if(l<r){
                Sukuna_node* tmp=root_su[2*count+1];
                root_su[2*count+1]=root_su[count];
                root_su[count]=tmp;
                root_su[count]->vitri=count;
                root_su[2*count+1]->vitri=2*count+1;
                return reheapdown(2*count+1);
            }
            else{
                if(root_su[2*count+1]->stt>root_su[2*count+2]->stt){
                    Sukuna_node* tmp=root_su[2*count+1];
                    root_su[2*count+1]=root_su[count];
                    root_su[count]=tmp;
                    root_su[count]->vitri=count;
                    root_su[2*count+1]->vitri=2*count+1;
                    return reheapdown(2*count+1);
                }
                else{
                    Sukuna_node* tmp=root_su[2*count+2];
                    root_su[2*count+2]=root_su[count];
                    root_su[count]=tmp;
                    root_su[count]->vitri=count;
                    root_su[2*count+2]->vitri=2*count+2;
                    return reheapdown(2*count+2);
                }
            }
        }
        else if(root_su[count]->count==l||root_su[count]->count==r){
            if(l==r){
                if(root_su[2*count+1]->stt>root_su[2*count+2]->stt&&root_su[2*count+1]->stt>root_su[count]->stt){
                    Sukuna_node* tmp=root_su[2*count+1];
                    root_su[2*count+1]=root_su[count];
                    root_su[count]=tmp;
                    root_su[count]->vitri=count;
                    root_su[2*count+1]->vitri=2*count+1;
                    return reheapdown(2*count+1);
                }
                else if(root_su[2*count+1]->stt<root_su[2*count+2]->stt&&root_su[2*count+2]->stt>root_su[count]->stt){
                    Sukuna_node* tmp=root_su[2*count+2];
                    root_su[2*count+2]=root_su[count];
                    root_su[count]=tmp;
                    root_su[count]->vitri=count;
                    root_su[2*count+2]->vitri=2*count+2;
                    return reheapdown(2*count+2);
                }
            }
            else if(l>r&&root_su[2*count+2]->stt>root_su[count]->stt){
                Sukuna_node* tmp=root_su[2*count+2];
                    root_su[2*count+2]=root_su[count];
                    root_su[count]=tmp;
                    root_su[count]->vitri=count;
                    root_su[2*count+2]->vitri=2*count+2;
                    return reheapdown(2*count+2);
            }
            else if(r>l&&root_su[2*count+1]->stt>root_su[count]->stt){
                Sukuna_node* tmp=root_su[2*count+1];
                root_su[2*count+1]=root_su[count];
                root_su[count]=tmp;
                root_su[count]->vitri=count;
                root_su[2*count+1]->vitri=2*count+1;
                return reheapdown(2*count+1);
            }
        }
    }
    void reheapDown(vector<Sukuna_node*>& maxHeap, int numberOfElements, int index)
    {
        int left = index * 2 + 1;
        int right = index * 2 + 2;
        int largest = index;
        if (left < numberOfElements && (maxHeap[left]->count < maxHeap[largest]->count||
        (maxHeap[left]->count == maxHeap[largest]->count&&maxHeap[left]->stt>maxHeap[largest]->stt))) 
        largest = left;
        if (right < numberOfElements && (maxHeap[right]->count < maxHeap[largest]->count||
        (maxHeap[right]->count == maxHeap[largest]->count&&maxHeap[right]->stt>maxHeap[largest]->stt))) 
        largest = right;
        if (largest != index)
        {
            swap(maxHeap[largest], maxHeap[index]);
            reheapDown(maxHeap, numberOfElements,largest);
        }
    }
    void KEITEIKEN(int num){
        if(count==0) return;
        vector<Sukuna_node*> temp;
        vector<Sukuna_node*> su = root_su;
        queue<custormer*> xoa;
        int kan=num;
        while(kan>0&&!su.empty()){
            temp.push_back(su[0]);
            su[0]=su[su.size()-1];
            su.pop_back();
            reheapDown(su,su.size(),0);
            kan--;
        }
        int z=temp.size();
        for (int i=0;i<z;++i){
            for(int j=0;j<num;++j){
                xoa.push(temp[i]->fifo[0]);
                temp[i]->del_cus();
                if(temp[i]->count==0) break;
            }
            int n =root_su.size();
            for(int k=0;k<n;++k){
                root_su[k]->stt++;
            }
            reheapup(temp[i]->vitri);
            if(temp[i]->count==0){
                root_su[0]=root_su[root_su.size()-1];
                root_su[0]->vitri=0;
                root_su.pop_back();
                count--;
                reheapdown(0);
            }
        }
        while(!xoa.empty()){
            cout<<xoa.front()->res<<"-"<<xoa.front()->ID<<"\n";
            custormer* h=xoa.front();
            xoa.pop();
            delete h;
        }
    }
    void CLEAVE(int num,int n){
        int h=root_su.size();
        if(num>=h) return;
        int k = root_su[num]->fifo.size();
        int z=n;
        for(int i=k-1;i>-1;--i){
            if(z==0) break;
            cout<<root_su[num]->fifo[i]->ID<<"-"<<root_su[num]->fifo[i]->res<<"\n";
            z--;
        }
        CLEAVE(2*num+1,n);
        CLEAVE(2*num+2,n);
    }
};
struct minTreeComp {
    bool operator()( HuffTree* lhs,  HuffTree* rhs)  {
        if(lhs->weight()==rhs->weight()) return lhs->stt>rhs->stt;
        return lhs->weight()>rhs->weight();
    }
};

// Build a Huffman tree from a collection of frequencies
inline HuffNode* rotateRight(HuffNode* root) {
    if(!root||root->isLeaf()) return root;
    if(dynamic_cast<IntlNode*>(root)->left()) {
        HuffNode *tmp=dynamic_cast<IntlNode*>(root)->left();
        dynamic_cast<IntlNode*>(root)->setLeft(dynamic_cast<IntlNode*>(tmp)->right());
        dynamic_cast<IntlNode*>(tmp)->setRight(root);
        dynamic_cast<IntlNode*>(root)->set_height();
        dynamic_cast<IntlNode*>(tmp)->set_height();
        root=tmp;
    };
    return root;
};

inline HuffNode* rotateLeft(HuffNode* root) {
    if(!root||root->isLeaf()) return root;
    if(dynamic_cast<IntlNode*>(root)->right()) {
        HuffNode*tmp=dynamic_cast<IntlNode*>(root)->right();
        dynamic_cast<IntlNode*>(root)->setRight(dynamic_cast<IntlNode*>(tmp)->left());
        dynamic_cast<IntlNode*>(tmp)->setLeft(root);
        dynamic_cast<IntlNode*>(root)->set_height();
        dynamic_cast<IntlNode*>(tmp)->set_height();
        root=tmp;
    }
    return root;
};
inline bool check_balance(HuffNode*& root,int& check){
    if(!root||root->isLeaf()||check>=3) return false;
    HuffNode* temp_left=dynamic_cast<IntlNode*>(root)->left();
    HuffNode* temp_right=dynamic_cast<IntlNode*>(root)->right();
    if(temp_left->height>temp_right->height+1){
        if(dynamic_cast<IntlNode*>(temp_left)->left()->height<dynamic_cast<IntlNode*>(temp_left)->right()->height){
            temp_left=rotateLeft(temp_left);
            dynamic_cast<IntlNode*>(root)->setLeft(temp_left);
        }
        root=rotateRight(root);
        check++;
        return true;
    }
    else if(temp_left->height+1<temp_right->height){
        if(dynamic_cast<IntlNode*>(temp_right)->left()->height>dynamic_cast<IntlNode*>(temp_right)->right()->height){
            temp_right=rotateRight(temp_right);
            dynamic_cast<IntlNode*>(root)->setRight(temp_right);
        }
        root=rotateLeft(root);
        check++;
        return true;
    };
    if(check>=3) return true;
    temp_left=dynamic_cast<IntlNode*>(root)->left();
    if(check_balance(temp_left,check)){
        dynamic_cast<IntlNode*>(root)->setLeft(temp_left);
        dynamic_cast<IntlNode*>(root)->set_height();
        return true;
    }
    if(check>=3) return true;
    temp_right=dynamic_cast<IntlNode*>(root)->right();
    if(check_balance(temp_right,check)){
        dynamic_cast<IntlNode*>(root)->setRight(temp_right);
        dynamic_cast<IntlNode*>(root)->set_height();
        return true;
    }
    
    return false;
}
inline HuffTree* buildHuff(HuffTree** TreeArray, int count,string & s) {
    std::priority_queue<HuffTree*, std::vector<HuffTree*>, minTreeComp> forest;
    for(int i=0;i<count;++i){
        forest.push(TreeArray[i]);
    }
    HuffTree *temp1, *temp2, *temp3 = nullptr;
    if(forest.size()==1) {
        temp3=forest.top();
        forest.pop();
    };
    while (forest.size() > 1) {
        temp1 = forest.top(); forest.pop();
        temp2 = forest.top(); forest.pop();
        count++;
        temp3 = new HuffTree(temp1, temp2,count);
        temp1->Root=nullptr;
        temp2->Root=nullptr;
        delete temp1;   
        delete temp2; 
        HuffNode* tmp=temp3->root();
        int check=0;
        while(check_balance(tmp,check)&&check<3){}
        temp3->setRoot(tmp);
        forest.push(temp3);
    }
    HuffNode* root_temp=temp3->root();
    if (root_temp->height==1){
        s="0";
        return temp3;
    }
    queue<HuffNode*> bin;
    map<char,string>bin_help;
    bin.push(root_temp);
    while(!bin.empty()){
        if(!bin.front()->isLeaf()){
            dynamic_cast<IntlNode*>(bin.front())->left()->binary=bin.front()->binary+"0";
            dynamic_cast<IntlNode*>(bin.front())->right()->binary=bin.front()->binary+"1";
            bin.push(dynamic_cast<IntlNode*>(bin.front())->left());
            bin.push(dynamic_cast<IntlNode*>(bin.front())->right());
        }
        else{
            bin_help[dynamic_cast<LeafNode*>(bin.front())->val()]=bin.front()->binary;
        }
        bin.pop();
    }
    string temp="";
    for(int i=s.length()-1;i>-1;--i){
        temp=bin_help[s[i]]+temp;
        if(temp.length()>=10) break;
    }
    int start=temp.length();
    s="";
    for(int i=start-1; i>-1;--i){
        s+=temp[i];
        if(s.length()>=10) break;
    }

    return temp3;
}
inline int binaryToDecimal(long long binaryNumber) {
    int decimalNumber = 0, i = 0, remainder;
    
    while (binaryNumber != 0) {
        remainder = binaryNumber % 10;
        binaryNumber /= 10;
        decimalNumber += remainder * pow(2, i);
        ++i;
    }
    
    return decimalNumber;
}
inline void Hand(HuffNode* node){
    if(!node) return ;
    if(node->isLeaf())
    cout<<dynamic_cast<LeafNode*>(node)->val()<<"\n";
    else {
        Hand(dynamic_cast<IntlNode*>(node)->left());
        cout<<dynamic_cast<IntlNode*>(node)->weight()<<"\n";
        Hand(dynamic_cast<IntlNode*>(node)->right());
    }
}
inline bool post_order(int* arr,custormer* cus,int &t){
    if(!cus) return false;
    post_order(arr,cus->left,t);
    post_order(arr,cus->right,t);
    arr[t]=cus->res;
    t++;
    return true;
}


inline void simulate(string filename)
{
    ifstream ss(filename);
    GOJO* gojo;
    SUKUNA* sukuna;
	string str, maxsize;
    HuffTree* hand_temp=nullptr;
	while(ss >> str)
	{ 
		if(str == "MAXSIZE")
		{
			ss >> maxsize;
			int MAXSIZE = stoi(maxsize); 
            gojo=new GOJO(MAXSIZE);
            sukuna=new SUKUNA(MAXSIZE);
    	}
        else if(str == "LAPSE")
        {
            string name;
            ss>>name;
            string x_name=name;
            bool check;
            vector<std::pair<char, int>> s=handle_name(x_name,check);
            if(check) continue;
            HuffTree** arr = new HuffTree*[s.size()];
            int count=0;
            int k=s.size();
            for(int i=0;i<k;++i){
                arr[count]=new HuffTree(s[i].first,s[i].second,count+1);
                count++;
            }
            string temp=x_name;
            string temp2=x_name;
            // cout<<endl<<x_name<<endl;
            if(hand_temp) delete hand_temp;
            hand_temp=buildHuff(arr,count,temp);
            delete []arr;
            // hand_temp->print();
            int result=binaryToDecimal(stol(temp));
            // cout<<result<<endl;
            if(result%2){
                int ID=result%gojo->MAXSIZE+1;
                custormer* cus=new custormer(name,result,ID);
                gojo->add_cus(cus,ID);
            }
            else{
                int ID=result%sukuna->MAXSIZE+1;
                custormer* cus=new custormer(name,result,ID);
                sukuna->push_cus(cus,ID);
            }
        }
        else if(str == "KOKUSEN"){
            for(int i=0;i<gojo->MAXSIZE;++i){
                if(gojo->hash[i]->count!=0){
                    BST* temp=gojo->hash[i];
                    // int t=0;
                    // post_order(x,temp->root,t);
                    int xoa=gojo->KOKU(temp->root);
                    while(xoa>0){
                        temp->root=temp->deletehelp(temp->root,temp->save[0]->res);
                        if(!temp->save.empty()) {
                            custormer* tm=temp->save[0];
                            temp->save.erase(temp->save.begin());
                            delete tm;
                        }
                        xoa--;
                    }
                }
            }
        }
        else if(str == "KEITEIKEN"){
            int num;
            ss>>num;
            sukuna->KEITEIKEN(num);
        }
        else if(str == "HAND"){
            if(!hand_temp) continue;
            HuffNode* temp=hand_temp->root();
            Hand(temp);
        }
        else if(str == "LIMITLESS"){
            int num;
            ss>>num;
            if(num>gojo->MAXSIZE||num<=0) continue;
            BST* temp= gojo->hash[num-1];
            LIMITLESS(temp->root);
        }
        else if(str == "CLEAVE"){
            int num;
            ss>>num;
            sukuna->CLEAVE(0,num);
        };
    };
    if(gojo) delete gojo;
    if(sukuna) delete sukuna;
    if(hand_temp) delete hand_temp;
}