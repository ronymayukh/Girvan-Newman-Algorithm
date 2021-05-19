#include <bits/stdc++.h>
using namespace std;

class Graph{
    
    public:
    
    unordered_set<string> nodes;
    unordered_map<string,vector<string>> adjList;

    void addNodes(string a){
        nodes.insert(a);
    }
    
    void addEdge(string a, string b){
        
        if(nodes.find(a) == nodes.end() || nodes.find(b) == nodes.end()){
            cout<<"No such nodes\n";
            return;
        }
        
        adjList[a].push_back(b);
        adjList[b].push_back(a);
    }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    void DFS(string u, unordered_map<string,bool> &visited){
        
        visited[u] = true;
        cout<<u<<" --> ";
        
        for(int i=0;i<adjList[u].size();i++){
            cout<<adjList[u][i]<<", ";
        }
        cout<<"\n";
        
        for(int i=0;i<adjList[u].size();i++){
            
            if(visited[adjList[u][i]] == false){
                DFS(adjList[u][i],visited);
            }
            
        }
        
        
    }
    
/////////////////////////////////////////////////////////////////////////////////////////////////////////    
    int findNumberOfClusters(){
        
        int num = 0;
        
        unordered_map<string,bool> visited;
        
        for(string i : nodes){
            visited[i] = false;
        }
        
        for(string i : nodes){
            if(!visited[i]){
                num++;
                cout<<"CLUSTER "<<num<<"\n";
                DFS(i,visited);
                cout<<"\n";
            }
        }
        
        return num;
        
    }
    
//////////////////////////////////////////////////////////////////////////////////////////////////////////////    
    void removeEdge(string a, string b){
        cout<<"REMOVING EDGE ("<<a<<","<<b<<")\n";
        cout<<"-----------------------------------------------------------------\n";
        adjList[a].erase(find(adjList[a].begin(),adjList[a].end(),b));
        adjList[b].erase(find(adjList[b].begin(),adjList[b].end(),a));
        
    }
    
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static bool sortBetweeness(vector<string> v1, vector<string> v2){
        
        if(v1[0] == v2[0]){
            return v1[1]<v2[1];
        }
        
        return v1[0]<v2[0];
        
    }
    
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 void printBetweeness(unordered_map<string,unordered_map<string,double>> betweeness){
        
        cout<<"BETWEENESS TABLE: \n";
        string a;
        string b;
        double max_betweeness = DBL_MIN;
        
        vector<vector<string>> result;
        
        for(string i : nodes){
            for(string j : nodes){
                
                if(max_betweeness < betweeness[i][j]){
                        max_betweeness = betweeness[i][j];
                        a = i;
                        b = j;
                    }
                
                if(betweeness[i][j] == 0)
                    continue;
                
                if(i<j){
                    vector<string> temp;
                    temp.push_back(i);
                    temp.push_back(j);
                    temp.push_back(to_string(betweeness[i][j]));
                    
                    result.push_back(temp);
                    
                    
                }
                
            }
        }
        
        sort(result.begin(),result.end(),sortBetweeness);
        
        for(int i=0;i<result.size();i++){
            cout<<"("<<result[i][0]<<","<<result[i][1]<<") -> "<<result[i][2]<<"\n";
        }
        
        removeEdge(a,b);
        
    }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool isSamePath(vector<string> v1, vector<string> v2){
    
    if(v1.size() != v2.size()){
        return false;
    }
    
    for(int i=0;i<v1.size();i++){
        if(v1[i] != v2[i]){
            return false;
        }
    }
    
    return true;
    
}
    
    
    
    
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    static bool sortPath(vector<string> v1, vector<string> v2){
        
        if(v1[0] == v2[0]){
            return v1[v1.size()-1]<v2[v2.size()-1];
        }
        
        return v1[0]<v2[0];
    }
    
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////    
    
    void printPaths(vector<vector<string>> allPaths){
        
        int n = allPaths.size();
        sort(allPaths.begin(),allPaths.end(),sortPath);
        
        vector<vector<string>> finalPath;
        vector<string> temp = allPaths[0];
        finalPath.push_back(temp);
        
        for(int i=1;i<allPaths.size();i++){
            
            if(!isSamePath(temp,allPaths[i])){
                temp = allPaths[i];
                finalPath.push_back(temp);
            }
            
        }
        
        for(int i=0; i<n;i++){
            for(int j=0;j<finalPath[i].size()-1;j++){
                cout<<finalPath[i][j]<<"-> ";
            }
            cout<<finalPath[i][finalPath[i].size()-1]<<"\n";
        }
    }
    
    
    
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////    
    void getPathHelper(vector<string> currentPath, vector<vector<string>> &allPaths){
        int n = currentPath.size();

        
        if(currentPath[0]>=currentPath[n-1]){
            return;
        }
        
       allPaths.push_back(currentPath);
    }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////    
    void getPath(string a, unordered_map<string, vector<string>> child, vector<string> currentPath, vector<vector<string>> &allPaths){
        
        if(child[a].size() == 0){
            currentPath.push_back(a);
            getPathHelper(currentPath,allPaths);
            return;
        }
        
        currentPath.push_back(a);
        getPathHelper(currentPath,allPaths);
        
        for(string x : child[a]){
            
            getPath(x,child,currentPath,allPaths);
            
        }
 
    }
    
    
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////    
    void findBetweenNessPerSource(string u, unordered_map<string,unordered_map<string,double>> &betweeness, vector<vector<string>> &allPaths)
    {
        unordered_map<string, int> level;
        unordered_map<string, bool> visit;
        unordered_map<string, vector<string>> child;
        
        for(string x : nodes){
            level[x] = 0;
            visit[x] = false;
        }
        

    queue<string> q;
    q.push(u);
    level[u] = 1;
    while(!q.empty())
    {
        string v = q.front();
        q.pop();
        visit[v] = true;
        for(int j=0; j<adjList[v].size(); j++)
        {
            if((level[adjList[v][j]] == 0 || level[adjList[v][j]] > level[v]) && visit[adjList[v][j]] == 0)
            {
                q.push(adjList[v][j]);
                child[v].push_back(adjList[v][j]);
                level[adjList[v][j]] = level[v] + 1;
            }
        }
    }

    unordered_map<string,vector<string>> parent;

    for(string j : nodes)
        for(int k = 0; k < child[j].size(); k++)
            parent[child[j][k]].push_back(j);


    getPath(u,child,vector<string>(),allPaths);

    queue<string> temp2;

    unordered_map<string, double> temp;
    unordered_map<string, double> visitFrCredit;
    
    for(string i : nodes){
        temp[i] = 0;
        visitFrCredit[i] = 0;
    }
    
    for(string i : nodes)
        if(child[i].size() == 0)
        {

            visitFrCredit[i] = 1;
            temp[i] = 1;
            temp2.push(i);
        }

    unordered_map<string, int> temp3;
    for(string i : nodes)
        temp3[i] = child[i].size();


    while(!temp2.empty())
    {
        string m = temp2.front();
        temp2.pop();

        for(int i=0; i<parent[m].size(); i++)
        {
            temp[parent[m][i]] += temp[m]/parent[m].size();
            temp3[parent[m][i]]--;
            if(temp3[parent[m][i]]==0 && child[parent[m][i]].size() !=0)
                temp[parent[m][i]]++;
            if(visitFrCredit[parent[m][i]] == 0)
            {
                temp2.push(parent[m][i]);
                visitFrCredit[parent[m][i]] = 1;
            }
        }
    }

    temp[u] = 0;
        for(string i : nodes)
        {
            if(temp[i] == 0)
                continue;
            for(int j = 0; j<parent[i].size(); j++)
            {
                betweeness[i][parent[i][j]] += temp[i]/parent[i].size();
            }
        }
    }
    
///////////////////////////////////////////////////////////////////////////////////////////////////////////    
    void girvan_newman_helper(){
        unordered_map<string,unordered_map<string,double>> betweeness;
    
        for(string i : nodes){
            for(string j : nodes){
            betweeness[i][j] = 0;
            }
        }
        
        vector<vector<string>> allPaths;
        
        for(string i : nodes){

            findBetweenNessPerSource(i,betweeness,allPaths);
        }
        
        printPaths(allPaths);
        printBetweeness(betweeness);
        
        
        
        
    }
    
////////////////////////////////////////////////////////////////////////////////////////////////////////////    
    void girvan_newman(){
        
        int n = nodes.size();
        int countRound = 0;
        int numberCluster;
       
       cout<<"After Round "<<countRound<<"\n";
            numberCluster = findNumberOfClusters();
            cout<<"----------------------------------------------------------------------\n";
       
        while(numberCluster < n){

            girvan_newman_helper();
            countRound++;
            cout<<"After Round "<<countRound<<"\n";
            numberCluster = findNumberOfClusters();
            cout<<"----------------------------------------------------------------------\n";

        }
        
    }    

    
};



int main()
{
    Graph g;
    
    int n,m;
    cin>>n>>m;
    string A,B;
    
    for(int i=0;i<n;i++){
        cin>>A;
        g.addNodes(A);
    }
    
    for(int i=0;i<m;i++){
        cin>>A>>B;
        g.addEdge(A,B);
    }

    g.girvan_newman();

    return 0;
}
