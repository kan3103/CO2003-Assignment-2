#include "main.h"

int MAXSIZE;

inline bool sortByFrequency(const std::pair<char, int>& a, const std::pair<char, int>& b) {
    if(a.second!=b.second)
    return a.second > b.second;
    else return false;
}
inline string handle_name(string name){
    map<char, int> charCount;
    for (char c : name) {
        charCount[c]++;
    }
    vector<std::pair<char, int>> charCountVector(charCount.begin(), charCount.end());
    sort(charCountVector.begin(), charCountVector.end(), sortByFrequency);
    string s="";
    for (auto pair : charCountVector) {
        for(int i=0;i<pair.second;++i){
            s+=pair.first;
        };
    }
    return s;
}
class custormer{
    public:
    custormer(string name) : name(name), Left(nullptr),Right(nullptr),x_name(""){};
    custormer(string name, custormer* Left,custormer* Right): name(name), Left(nullptr),Right(nullptr),x_name(""){};
    string name;
    string x_name;
    custormer* Left;
    custormer* Right;
};

class Restaurant{
    public:
    Restaurant(string name): name_res(name){};
    ~Restaurant(){};

    string name_res;
    custormer* head;
    string add(string name){
        return handle_name(name);
    }
};

class Huffman{
    public:
    Huffman(){
        Gojo = new Restaurant("Gojo");
        Sukuna = new Restaurant("Sukuna");
    }
    Restaurant* Gojo;
    Restaurant* Sukuna;

    string LAPSE(string name){
        return Gojo->add(name);
    };
};

inline void simulate(string filename)
{
    Huffman* res= new Huffman();
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
            cout<<res->LAPSE(name)<<endl;
        }
    }
}