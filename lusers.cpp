#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

/*#define FILE_LOGIN_DEFS "/etc/login.defs"*/
#define FILE_LOGIN_DEFS "./login.defs.test"
#define FILE_PASSWD "/etc/passwd"

vector<string> split(string &s, char delim)
{
    vector<string> vtoken;
    string token;
    stringstream stream(s);
    while(getline(stream, token, delim)) {
        vtoken.push_back(token);
    } 
    return vtoken;    
}


void dbg_print_vector(vector<string> v) 
{
    for(int i=0; i<v.size(); i++)
        cout << "[" << v[i] << "]";
    cout << "\n" ;
}

int find_minuid(ifstream &logindefs, char delimeter) 
{
    vector<string> toks;
    string line;

    int vec_size;
    int minuid = 0;

    while(getline(logindefs, line) ) {
        toks = split(line, delimeter); 
        //dbg_print_vector(toks);
        vec_size = toks.size();
        if(vec_size && toks[0] == "UID_MIN") {
            istringstream (toks[vec_size-1]) >> minuid;
            cout << toks[0] << " = " << minuid << "\n";
        }
    }
    

}

int get_minuid(ifstream &logindefs) 
{
    int minuid = 0;

    minuid = find_minuid(logindefs, ' ');
    /*if(! minuid)*/ 
    {
        logindefs.clear();
        logindefs.seekg(0, ios::beg);
        minuid = find_minuid(logindefs, '\t');
    }

    return minuid;
}

int main() 
{
    ifstream logindefs(FILE_LOGIN_DEFS);
    ifstream pwd(FILE_PASSWD);
    int minuid = 0;

    if(!logindefs.is_open()) {
        cout << "Unable to open file " << FILE_LOGIN_DEFS << "\n";
        return 1; 
    }

    minuid = get_minuid(logindefs);

    logindefs.close();
}
