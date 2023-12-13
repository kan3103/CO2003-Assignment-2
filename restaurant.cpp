#include "main.h"
inline bool sortByFrequency(const std::pair<char, int>& a, const std::pair<char, int>& b) {
    if(a.second!=b.second)
    return a.second < b.second;
    else if(a.first>='a'&&b.first>='a'||(a.first<='Z'&&b.first<='Z')) return a.first<b.first;
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
    // sort(charCountVector.begin(), charCountVector.end(), sortByFrequency);
    string s="";
    unordered_map<char,int> encode;
    for(int i=0;i<name.length();++i){
        if(name[i]>='a'&&name[i]<='z')
            name[i]=(name[i]+charCount[name[i]]<='z')?name[i]+charCount[name[i]]:name[i]+charCount[name[i]]-26;
        else  name[i]=(name[i]+charCount[name[i]]<='Z')?name[i]+charCount[name[i]]:name[i]+charCount[name[i]]-26;
    }
    for (auto pair : charCountVector) {
        if(pair.first>='a'&&pair.first<='z'){
            encode[(pair.first+pair.second<='z')?pair.first+pair.second:pair.first+pair.second-26]+=pair.second;
        }
        else{
            encode[(pair.first+pair.second<='Z')?pair.first+pair.second:pair.first+pair.second-26]+=pair.second;
        };
    }
    
    vector<std::pair<char, int>> charCountVector2(encode.begin(), encode.end());
    sort(charCountVector2.begin(), charCountVector2.end(), sortByFrequency);
    for (auto pair : charCountVector2){
        for(int i=0;i<pair.second;++i){
            s+=pair.first;
        }
    }
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
    public:
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
    public:
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
    public:
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
        HuffNode*temp=Root;
        while(temp){

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
    void print(){
        HuffNode* temp=Root;
        queue<HuffNode*> tmp;
        if(!temp->isLeaf()){
            cout<<dynamic_cast<IntlNode*>(temp)->weight();
            tmp.push(dynamic_cast<IntlNode*>(temp)->left());
            tmp.push(dynamic_cast<IntlNode*>(temp)->right());
        }
        else cout<<dynamic_cast<LeafNode*>(temp)->val();
        while(!tmp.empty()){
            if(!tmp.front()->isLeaf()){
                cout<<" "<<dynamic_cast<IntlNode*>(tmp.front())->weight();
                tmp.push(dynamic_cast<IntlNode*>(tmp.front())->left());
                tmp.push(dynamic_cast<IntlNode*>(tmp.front())->right());
                tmp.pop();
            }
            else {
                cout<<" Leaf: "<<dynamic_cast<LeafNode*>(tmp.front())->val();
                tmp.pop();
            }
        };
    }
};
class custormer{
    public:
    custormer(string name,int result,HuffTree* hufftree,int ID) : name(name),res(result),hufftree(hufftree),ID(ID),parent(nullptr),left(nullptr),right(nullptr){};
    string name;
    int res;
    HuffTree* hufftree;
    int ID;
    custormer* parent;
    custormer* left;
    custormer* right;
};
class BST{
    public:
    custormer* root;
    int count;
    BST(custormer* cus){
        root=cus;
        count++;
    };
    BST(){
        root=nullptr;
        count=0;
    }
    void push(custormer* cus){
        custormer* temp=root;
        if(!temp){
            root=cus;
            count++;
            return;
        }
        while(temp->left||temp->right){
            if(cus->res>=temp->res) {
                if(!temp->right) {
                    temp->right=cus; 
                    count++;
                    return;
                }
                else temp=temp->right;
            }
            else {
                if(!temp->left) {
                    temp->left=cus; 
                    count++;
                    return;
                }
                else temp=temp->left;
            };
        }
        if(temp->res>cus->res){
            temp->left=cus;
            count++;
            return;
        }
        else{
            temp->right=cus;
            count++;
            return;
        }
    }
    void print(){
        cout<<root->name;
    }
};
class GOJO {
public:
    int MAXSIZE;
    BST** hash;

    GOJO(int MAXSIZE) : MAXSIZE(MAXSIZE) {
        hash = new BST*[MAXSIZE];
        for (int i = 0; i < MAXSIZE; ++i) {
            hash[i] = new BST();
        }
    }

    ~GOJO() {
        for (int i = 0; i < MAXSIZE; ++i) {
            delete hash[i];
        }
        delete[] hash;
    }

    void add_cus(custormer* cus, int ID) {
        hash[(ID-1) % MAXSIZE]->push(cus);
    }
};
class Sukuna_node{
    public:
    int stt;
    int count;
    int weight;
    Sukuna_node(int kv) : stt(kv),count(0){}
    custormer* cus;
    void add_cus(custormer* cus){
        this->cus=cus;
        count++;
    }
};
class SUKUNA {
    public:
    int MAXSIZE;
    int count;
    Sukuna_node** root_su;
    SUKUNA(int MAXSIZE) : MAXSIZE(MAXSIZE),count(0){
        root_su=new Sukuna_node*[15];
    };
    void push_cus(custormer* cus,int kv){
        root_su[count]=new Sukuna_node(kv);
        root_su[count]->add_cus(cus);
        reheapup(count);
        count++;
    }
    void reheapup(int kv){
        if(kv==0) return ;
        if(root_su[(kv-1)/2]->weight>root_su[kv]->weight){
            Sukuna_node* tmp=root_su[(kv-1)/2];
            root_su[(kv-1)/2]=root_su[kv];
            root_su[kv]=tmp;
            return reheapup((kv-1)/2);
        };
    };

};
struct minTreeComp {
    bool operator()( HuffTree* lhs,  HuffTree* rhs)  {
        if(lhs->weight()==rhs->weight()) return lhs->stt>rhs->stt;
        return lhs->weight()>rhs->weight();
    }
};

// Build a Huffman tree from a collection of frequencies
HuffNode* rotateRight(HuffNode* root) {
    if(!root||root->isLeaf()) return root;
    if(dynamic_cast<IntlNode*>(root)->left()) {
        HuffNode *tmp=dynamic_cast<IntlNode*>(root)->left();
        dynamic_cast<IntlNode*>(root)->setLeft(dynamic_cast<IntlNode*>(tmp)->right());
        dynamic_cast<IntlNode*>(tmp)->setRight(root);
        dynamic_cast<IntlNode*>(tmp)->set_height();
        dynamic_cast<IntlNode*>(root)->set_height();
        root=tmp;
    };
    return root;
};

HuffNode* rotateLeft(HuffNode* root) {
    if(!root||root->isLeaf()) return root;
    if(dynamic_cast<IntlNode*>(root)->right()) {
        HuffNode*tmp=dynamic_cast<IntlNode*>(root)->right();
        dynamic_cast<IntlNode*>(root)->setRight(dynamic_cast<IntlNode*>(tmp)->left());
        dynamic_cast<IntlNode*>(tmp)->setLeft(root);
        dynamic_cast<IntlNode*>(tmp)->set_height();
        dynamic_cast<IntlNode*>(root)->set_height();
        root=tmp;
    }
    return root;
};
HuffNode* check_balance(HuffNode*& root,int& check){
    if(!root||root->isLeaf()||check>=3) return root;
    HuffNode* temp_left=dynamic_cast<IntlNode*>(root)->left();
    HuffNode* temp_right=dynamic_cast<IntlNode*>(root)->right();
    if(temp_left->height>temp_right->height+1){
        if(dynamic_cast<IntlNode*>(temp_left)->left()->height<dynamic_cast<IntlNode*>(temp_left)->right()->height){
            temp_left=rotateLeft(temp_left);
            dynamic_cast<IntlNode*>(root)->setLeft(temp_left);
        }
        if(check>=3) return root;
        root=rotateRight(root);
        check++;
    }
    else if(temp_left->height+1<temp_right->height){
        if(dynamic_cast<IntlNode*>(temp_right)->left()->height>dynamic_cast<IntlNode*>(temp_right)->right()->height){
            temp_right=rotateRight(temp_right);
            dynamic_cast<IntlNode*>(root)->setRight(temp_right);
        }
        if(check>=3) return root;
        root=rotateLeft(root);
        check++;
    };
    if(check>=3) return root;
    temp_left=dynamic_cast<IntlNode*>(root)->left();
    check_balance(temp_left,check);
    dynamic_cast<IntlNode*>(root)->setLeft(temp_left);
    if(check>=3) return root;
    temp_right=dynamic_cast<IntlNode*>(root)->right();
    check_balance(temp_right,check);
    dynamic_cast<IntlNode*>(root)->setRight(temp_right);
    dynamic_cast<IntlNode*>(root)->set_height();
    
    return root;
}
HuffTree* buildHuff(HuffTree** TreeArray, int count,string & s) {
    std::priority_queue<HuffTree*, std::vector<HuffTree*>, minTreeComp> forest;
    for(int i=0;i<count-1;++i){
        forest.push(TreeArray[i]);
    }
    HuffTree *temp1, *temp2, *temp3 = nullptr;
    while (forest.size() > 1) {
        temp1 = forest.top(); forest.pop();
        temp2 = forest.top(); forest.pop();
        temp3 = new HuffTree(temp1, temp2,count);
        count++;
        HuffNode* tmp=temp3->root();
        int check=0;
        check_balance(tmp,check);
        temp3->setRoot(tmp);
        forest.push(temp3);
    }
    HuffNode* root_temp=temp3->root();
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
        temp+=bin_help[s[i]];
        if(temp.length()>=10) break;
    }
    int k=10;
    int start=temp.length();
    s=temp.substr(0,min(k,start));
    return temp3;
}
int binaryToDecimal(long long binaryNumber) {
    int decimalNumber = 0, i = 0, remainder;
    
    while (binaryNumber != 0) {
        remainder = binaryNumber % 10;
        binaryNumber /= 10;
        decimalNumber += remainder * pow(2, i);
        ++i;
    }
    
    return decimalNumber;
}
void Hand(HuffNode* node){
    if(!node) return ;
    if(node->isLeaf())
    cout<<dynamic_cast<LeafNode*>(node)->val()<<endl;
    else {
        Hand(dynamic_cast<IntlNode*>(node)->left());
        cout<<dynamic_cast<IntlNode*>(node)->weight()<<endl;
        Hand(dynamic_cast<IntlNode*>(node)->right());
    }
}
bool post_order(int* arr,custormer* cus,int &t){
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
    custormer* new_cus;
	string str, maxsize, name;
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
            ss>>name;
            string x_name=name;
            bool check;
            vector<std::pair<char, int>> s=handle_name(x_name,check);
            if(check) continue;
            int n=20;
            HuffTree** arr = new HuffTree*[s.size()];
            int count=0;
            int k=1;
            for(int i=0;i<s.size();++i){
                arr[count]=new HuffTree(s[i].first,s[i].second,count+1);
                count++;
            }
            arr[count]=new HuffTree(s[s.size()-1].first,s[s.size()-1].second);
            count++;
            string temp=x_name;
            HuffTree* huff=buildHuff(arr,count,temp);
            HuffNode* ok=huff->root();
            int result=binaryToDecimal(stol(temp));
            cout<<result<<endl;
            if(result%2){
                int ID=result%gojo->MAXSIZE+1;
                custormer* cus=new custormer(name,result,huff,ID);
                new_cus=cus;
                gojo->add_cus(cus,ID);
            }
            else{
                int ID=result%sukuna->MAXSIZE+1;
                custormer* cus=new custormer(name,result,huff,ID);
                new_cus=cus;
                sukuna->push_cus(cus,ID);
            }
        }
        else if(str == "KOKUSEN"){
            for(int i=0;i<gojo->MAXSIZE;++i){
                if(gojo->hash[i]->count!=0){
                    BST* temp=gojo->hash[i];
                    int* x= new int[gojo->hash[i]->count];
                    int t=0;
                    post_order(x,gojo->hash[i]->root,t);
                    for(int j=0 ;j<gojo->hash[i]->count;++j){
                        cout<<x[j]<<" ";
                    }
                }
            }
        }
        else if(str == "KEITEIKEN"){}
        else if(str == "HAND"){
            if(!new_cus) continue;
            HuffTree* hf=new_cus->hufftree;
            HuffNode* temp=hf->root();
            cout<<endl;
            Hand(temp);
        }
        else if(str == "LIMITLESS"){}
        else if(str == "CLEAVE"){};
    }
}