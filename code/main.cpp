#include<bits/stdc++.h>
#include<unistd.h>
#include <dirent.h>
using namespace std;
#define mp make_pair
#define pb push_back

//some predefinded things
vector<string>builtin_command{"cd","help","quit","environ","history","clr","echo","dir","myshell","pause","cat"};
map<string, string> builtin_command_details{
                                              { "cd", " <directory> -> Change the current default directory to <directory>. If the <directory> argument is not present, report the current directory. If the directory doesn’t exist an appropriate error should be reported. This command should change the PWD environment variable." },
                                              { "clr", " -> Clear the screen" },
                                              { "dir", " <directory> -> List the contents of directory <directory>" },
                                              { "environ"," -> List all the environment strings of the current shell and bash shell" },
                                              { "echo"," <comment> -> Display <comment> on the display followed by a new line. (Multiple spaces/tabs may be reduced to a single space)" },
                                              { "pause"," -> Pause Operation of shell until ‘Enter’ is pressed."},
                                              { "help"," -> Display User Manual."},
                                              { "quit"," -> Quit the shell."},
                                              { "history"," -> Display a list of previously executed commands"},
                                              { "myshell batchfile"," -> execute the command in filenamed `batchfile`"}
                                            };
queue<string>queue;
string path=string(getcwd(NULL, 0));
string history_file_path=string(getcwd(NULL, 0))+"/History.txt";

void initilize_shell()
{
    cout << "\033[2J\033[1;1H";
    string main="****************************************************";
    cout<<main<<endl;
    cout<<"***********"<<"YOU ARE WELCOME IN MY SHELL"<<"**************"<<endl;
    string username = getenv("USER");
    cout<<"***********USER : ";
    cout<<username;
    for(int i=0;i<(20-username.length());i++)cout<<" ";
    cout<<"**************"<<endl;
    cout<<main<<endl;
}

string resolv_path(string s){
  if(s[0]=='/')return s;
  else {
    string temp=getcwd(NULL, 0);
    return temp+"/"+s;
  }
}

void builtin_cd(vector<string> &TOKENS){
  int n=TOKENS.size();
  if(n>2)cout<<"ERROR : Number of argument provided with `cd` are wrong. only 1 or 2 argument are expetecd with `cd`."<<endl;
  else if(n==1)cout<<getcwd(NULL, 0)<<endl;
  else{
    //chdir return 0 on succes
    string path=resolv_path(TOKENS[1]);
    if(chdir(path.c_str())!=0)cout<<"ERROR : Specified directory `"<<path<<"` doesn’t exist."<<endl;
  }
}

void builtin_dir(vector<string> &TOKENS){
  int n=TOKENS.size();
  string path;
  if(n>2){
    cout<<"ERROR : Number of argument provided with `dir` are wrong. only 1 or 2 argument are expetecd with `cd`."<<endl;
    return ;
  }
  else if (n==1)path=getcwd(NULL,0);
  else path=resolv_path(TOKENS[1]);
  //open directory and store correspoding pointer
  DIR *d=opendir(path.c_str());
  struct dirent *dir;
  if(d)
  {
      while((dir = readdir(d)))cout<<dir->d_name<<endl;
      closedir(d);
  }
  else cout<<"ERROR : Specified directory `"<<path<<"` doesn’t exist."<<endl;
}

vector<string> tokenize(string str){
  string temp;
  vector<string>v;
  for(auto i:str){
    if(i!=' ' and i!='\t')temp.pb(i);
    else if(!temp.empty()){
      v.pb(temp);
      temp.clear();
    }
  }
  if(!temp.empty()){
    v.pb(temp);
    temp.clear();
  }
  return v;
}

bool Is_builtin_command(string s){
  return find(builtin_command.begin(),builtin_command.end(),s)!=builtin_command.end();
}

void manage_history(string s){
  std::ofstream outfile;
  outfile.open(history_file_path, std::ios_base::app); // append instead of overwrite
  outfile << s;
  outfile << "\n";
}

int file_present(string filename){
    DIR* d = opendir(filename.c_str());

    if(d != NULL)
    {
     closedir(d);
     cout<<"ERROR : Specified file path `"<<filename<<"` is a directory, not a file."<<endl;
     return 2;
    }
    // ENOTDIR error come when a file was specified instead of directory.
    if(errno == ENOTDIR)return 1;

    cout<<"ERROR : Specified file path `"<<filename<<"` doesn’t exist."<<endl;
    return 3;
}

void run_the_command(string s,char **env);

void run_builtin_command(vector<string> &TOKENS,char **env){
  if(TOKENS[0]=="cd"){
    builtin_cd(TOKENS);
  }
  else if(TOKENS[0]=="quit"){
    cout<<"GOOD BYE!"<<endl;
    exit(0);
  }
  else if(TOKENS[0]=="help"){
    cout<<"******************************************************"<<endl;
    cout<<"WELCOME TO HELP GUIDE!"<<endl;
    cout<<"COMMAND SUPPORTED ARE :"<<endl<<endl;
    int x=0;
    for(auto i:builtin_command_details){
      x++;
      cout<<x<<". ";
      cout<<i.first<<i.second<<endl;
    }
    cout<<"******************************************************"<<endl;
  }
  else if(TOKENS[0]=="environ"){
    cout<<endl<<"=========== BASH SHELL ENVIROMENT ================"<<endl;
    while (*env)cout<<*env++<<endl;
    cout<<endl<<"=========== CURRENT SHELL ENVIROMENT ================"<<endl;
    cout<<"PWD="<<getcwd(NULL, 0)<<endl;
    cout<<"shell="<<path<<"/a.out"<<endl;
  }
  else if(TOKENS[0]=="history"){
    fstream fp;
    string filename = history_file_path,line;
    fp.open(filename.c_str());
    while (getline(fp,line))cout<<line<<endl;
  }
  else if (TOKENS[0]=="clr") {
    cout << "\033[2J\033[1;1H";
  }
  else if (TOKENS[0]=="echo") {
    for(int i=1;i<TOKENS.size();i++)cout<<TOKENS[i]<<" ";
    cout<<endl;
  }
  else if(TOKENS[0]=="dir"){
    builtin_dir(TOKENS);
  }
  else if (TOKENS[0]=="myshell") {
    string filename = "batchfile";
    if(TOKENS.size()!=2){
      cout<<"ERROR : Number of argument provided with `myshell` are wrong. Only 2 argument are expetecd with `myshell`."<<endl;
    }
    else if (TOKENS[1]!="batchfile") {
      cout<<"ERROR : Argument provided with `myshell` is wrong. Only `batchfile` keyword is expetecd with `myshell`."<<endl;
    }
    else if (file_present(filename)==1) {
      if (!file_present(filename))cout<<"ERROR : Specified file `batchfile` doesn't exist in current location."<<endl;
      fstream fp;
      string line;
      fp.open(filename.c_str());
      while (getline(fp,line))run_the_command(line,env);
    }
  }
  else if (TOKENS[0]=="pause") {
    char ch='a';
    cout<<"Currently SHELL is paused, Press ENTER to resume."<<endl;
    while(ch!='\n')ch=getchar();
  }
  //bonus
  else if (TOKENS[0]=="cat") {
    string filename = resolv_path(TOKENS[1]);
    if(TOKENS.size()!=2)cout<<"ERROR : Number of argument provided with `cat` are wrong. Only 2 argument are expetecd with `cat`."<<endl;
    else if (file_present(filename)==1) {
      fstream fp;
      string line;
      fp.open(filename.c_str());
      while (getline(fp,line))cout<<line<<endl;
    }
  }
}

void run_the_command(string s,char **env){
  vector<string> TOKENS=tokenize(s);
  if(TOKENS.size()!=0){
    manage_history(s);
    if (Is_builtin_command(TOKENS[0])) {
      run_builtin_command(TOKENS,env);
    }
    else{
      cout<<"ERROR : INVALID COMMAND `";
      for(auto i=0;i<TOKENS.size();i++)if(i==TOKENS.size()-1)cout<<TOKENS[i];else cout<<TOKENS[i]<<" ";
      cout<<"`."<<endl;
    }
  }
}

int main(int argc, char **argv, char **env) {
  initilize_shell();
  string s;
  while(1){
    cout << "\033[1;35m"+string(getcwd(NULL, 0))+"$ "+"\033[0m";
    getline(cin, s);
    run_the_command(s,env);
  }
  return 0;
}
