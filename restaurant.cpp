#include "main.h"

int MAXSIZE;

// Xử lý mã hóa tên
inline bool sortByFrequency(const std::pair<char, int>& a, const std::pair<char, int>& b) {
    if(a.second!=b.second)
    return a.second > b.second;
    else return false;
}
inline string handle_name(string& name){
    map<char, int> charCount;
    for (char c : name) {
        charCount[c]++;
    }
    vector<std::pair<char, int>> charCountVector(charCount.begin(), charCount.end());
    if(charCountVector.size()<3) return "";
    sort(charCountVector.begin(), charCountVector.end(), sortByFrequency);
    string s="";
    name="";
    for (auto pair : charCountVector) {
        for(int i=0;i<pair.second;++i){
            name+=pair.first;
            if(pair.first>='a'&&pair.first<='z'){
                s+=(pair.first+pair.second<='z')?pair.first+pair.second:pair.first+pair.second-26;
            }
            else{
                s+=(pair.first+pair.second<='Z')?pair.first+pair.second:pair.first+pair.second-26;
            }
        };
    }
    return s;
}

// 
class custormer{
    public:
    custormer(string name) : name(name), Left(nullptr),Right(nullptr),x_name(""){};
    custormer(string name, custormer* Left,custormer* Right): name(name), Left(nullptr),Right(nullptr),x_name(""){};
    string name;
    string x_name;
    custormer* Left;
    custormer* Right;
    custormer* Parent;
};
//
class Restaurant{
    public:
    Restaurant(string name): name_res(name),MAXSIZE(MAXSIZE){};
    Restaurant(){}
    ~Restaurant(){};
    string name_res;
    int MAXSIZE;
};
class GOJO :public Restaurant{
    public:
    GOJO() : Restaurant("GOJO") {};
};
class SUKUNA :public Restaurant{
    public:
    SUKUNA() : Restaurant("SUKUNA"){};
};
//
// class Huffman{
//     public:
//     Huffman():root(nullptr),Gojo(new GOJO()),Sukuna(new SUKUNA()) {};
//     ~Huffman(){
//         delete Gojo;
//         delete Sukuna;
//     }
//     Restaurant* Gojo;
//     Restaurant* Sukuna;
//     custormer* root;
//     string LAPSE(string name){
//         if(!handle_name(name)) return "";
//         int result;
//         if(result>=0){
//             Restaurant* restau = new GOJO();
//         }
//     };
// };

enum Balance{
    EH=0,
    LH=-1,
    RH=1
};

class HuffNode {
    public:
    virtual ~HuffNode() {} // Base destructor
    virtual int weight() = 0; // Return frequency
    virtual bool isLeaf() = 0; // Determine type
    int height;
};
class LeafNode : public HuffNode {
    private:
    char it; // Value
    int wgt; // Weight
    public:
    LeafNode(const char& val, int freq) // Constructor
    { it = val; wgt = freq; height=1;}
    int weight() { return wgt; }
    char val() { return it; }
    bool isLeaf() { return true; }
};
class IntlNode : public HuffNode {
    private:
    HuffNode* lc; // Left child
    HuffNode* rc; // Right child
    int wgt; // Subtree weight
    public:
    IntlNode(HuffNode* l, HuffNode* r){
        wgt = l->weight() + r->weight(); lc = l; rc = r; 
        this->height=(lc->height>rc->height)? lc->height+1:rc->height+1;
    }
    int weight() { return wgt; }
    bool isLeaf() { return false; }
    HuffNode* left() const { return lc; }
    void setLeft(HuffNode* b)
    { lc = (HuffNode*)b; }
    HuffNode* right() const { return rc; }
    void setRight(HuffNode* b)
    { rc = (HuffNode*)b; }
};

class HuffTree {
    private:
    HuffNode* Root; // Tree root
    public:
    HuffTree(char& val, int freq) // Leaf constructor
    { Root = new LeafNode(val, freq); }
    // Internal node constructor
    HuffTree(HuffTree* l, HuffTree* r)
    { Root = new IntlNode(l->root(), r->root()); }
    ~HuffTree() {} // Destructor
    HuffNode* root() { return Root; } // Get root
    int weight() { return Root->weight(); } // Root weight
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

struct minTreeComp {
    bool operator()( HuffTree* lhs,  HuffTree* rhs) const {
        if(lhs->weight()!= rhs->weight())
        return lhs->weight() > rhs->weight(); // Change to < for min heap, > for max heap
        else return false;
    }
};

// Build a Huffman tree from a collection of frequencies

HuffTree* buildHuff(HuffTree** TreeArray, int count) {
    std::priority_queue<HuffTree*, std::vector<HuffTree*>, minTreeComp> forest(TreeArray, TreeArray+ count);
    HuffTree *temp1, *temp2, *temp3 = nullptr;
    while (forest.size() > 1) {
        temp1 = forest.top(); forest.pop();
        temp2 = forest.top(); forest.pop();
        temp3 = new HuffTree(temp1, temp2);
        forest.push(temp3);
        // Note: No need to manually delete temp1 and temp2; they are now owned by the priority_queue
    }
    return temp3;
}
inline void simulate(string filename)
{
    ifstream ss(filename);
	string str, maxsize, name;
    int result, num;
	while(ss >> str)
	{ 
		if(str == "MAXSIZE")
		{
			ss >> maxsize;
			MAXSIZE = stoi(maxsize); 
    	}
        else if(str == "LAPSE")
        {
            ss>>name;
            string s=handle_name(name);
            int n=20;
            HuffTree** arr = new HuffTree*[s.length()];
            int count=0;
            int k=1;
            for(int i=0;i<s.length()-1;++i){
                if(s[i]==s[i+1]){
                    k++;
                }
                else{
                    arr[count]=new HuffTree(s[i],k);
                    k=1;
                    count++;
                };
            }
            arr[count]=new HuffTree(s[s.length()-1],k);
            count++;
            HuffTree* huff=buildHuff(arr,count);
            HuffNode* ok=huff->root();
            while(!ok->isLeaf()){
                ok=dynamic_cast<IntlNode*>(ok)->left();
            };
            huff->print();
        }
        else if(str == "KOKUSEN"){}
        else if(str == "KEITEIKEN"){}
        else if(str == "HAND"){}
        else if(str == "LIMITLESS"){}
        else if(str == "CLEAVE"){};
    }
}