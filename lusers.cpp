#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

#include <pwd.h>

using namespace std;

#define FILE_LOGIN_DEFS "/etc/login.defs"
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
    for(unsigned int i=0; i<v.size(); i++)
        cout << "[" << v[i] << "]";
    cout << "\n" ;
}

void find_minuid(ifstream &logindefs, char delimeter, unsigned int &minuid, unsigned int &maxuid)
{
    vector<string> toks;
    string line;

    int vec_size;

    while(getline(logindefs, line) ) {
        toks = split(line, delimeter);
        //dbg_print_vector(toks);
        vec_size = toks.size();
        if(vec_size && toks[0] == "UID_MIN") {
            minuid = stoi(toks[vec_size-1]);
            //cout << toks[0] << " = " << minuid << "\n";
        } else if(vec_size && toks[0] == "UID_MAX") {
            maxuid = stoi(toks[vec_size-1]);
            //cout << toks[0] << " = " << maxuid << "\n";
        }

    }
}

void get_minmaxuid(ifstream &logindefs, unsigned int &minuid, unsigned int &maxuid)
{

    // Assume word delimeter is a space
    find_minuid(logindefs, ' ', minuid, maxuid);
    if(! minuid)
    {
        logindefs.clear();
        logindefs.seekg(0, ios::beg);

        // Assume word delimeter is tab
        find_minuid(logindefs, '\t', minuid, maxuid);
    }
}

int main()
{
    ifstream logindefs(FILE_LOGIN_DEFS);
    unsigned int minuid = 0;
    unsigned int maxuid = -1;
    struct passwd *pwent = NULL;

    if(!logindefs.is_open()) {
        cout << "Unable to open file " << FILE_LOGIN_DEFS << " !\n";
        return 1;
    }

    get_minmaxuid(logindefs, minuid, maxuid);
    if(! minuid) {
        cout << "UID_MIN not found!\n";
        return 1;
    }

    cout << "List of users with " << minuid << " <= uid <= " << maxuid << ":\n";
    pwent = getpwent();
    while(pwent) {
        if(pwent->pw_uid >= minuid && pwent->pw_uid <= maxuid) {
            cout << pwent->pw_name << " " << pwent->pw_uid <<  " " << pwent->pw_shell << "\n";
        }
        pwent = getpwent();
    }
    endpwent();

    logindefs.close();
}
