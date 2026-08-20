#include <stdio.h>
#define INF 999999
#define MAX 20
#define HCAP (MAX*MAX)

int n;
int graph[MAX][MAX];
int dist[MAX];
int hops[MAX];
int parent[MAX];
int visited[MAX];

int hv[HCAP], hd[HCAP], hsize;

void swp(int i,int j){
        int t=hv[i]; hv[i]=hv[j]; hv[j]=t;
        t=hd[i]; hd[i]=hd[j]; hd[j]=t;
}

void upheap(int idx){
        int p;
        while(idx>0){
                p=(idx-1)/2;
                if(hd[p]<=hd[idx]) break;
                swp(p,idx);
                idx=p;
        }
}

void downheap(int idx){
        int l,r,small;
        while(1){
                l=2*idx+1; r=2*idx+2; small=idx;
                if(l<hsize && hd[l]<hd[small]) small=l;
                if(r<hsize && hd[r]<hd[small]) small=r;
                if(small==idx) break;
                swp(idx,small);
                idx=small;
        }
}

void push(int v,int d){
        if(hsize>=HCAP) return;
        hv[hsize]=v; hd[hsize]=d;
        upheap(hsize);
        hsize++;
}

int popmin(){
        int v=hv[0];
        hsize--;
        hv[0]=hv[hsize];
        hd[0]=hd[hsize];
        downheap(0);
        return v;
}

void readGraph(){
        int i,j,edges,u,v,w;

        printf("Enter number of nodes: ");
        scanf("%d",&n);

        for(i=0;i<n;i++)
                for(j=0;j<n;j++)
                        graph[i][j] = (i==j)?0:-1;

        printf("Enter number of edges: ");
        scanf("%d",&edges);

        for(i=0;i<edges;i++){
                printf("Edge %d - enter node1,node2,weight: ",i+1);
                scanf("%d %d %d",&u,&v,&w);
                if(u<0 || u>=n || v<0 || v>=n || w<0) {
                    printf("Invalid edge, skipping\n");
                    i--;
                    continue;
                }
                graph[u][v]=w;
                graph[v][u]=w;
        }
}

void showGraph(){
        int i,j;
        printf("\nAdjacency Matrix (-1 = no link)\n\n    ");
        for(i=0;i<n;i++) printf("%4d",i);
        printf("\n");
        for(i=0;i<n;i++){
                printf("%3d ",i);
                for(j=0;j<n;j++) printf("%4d",graph[i][j]);
                printf("\n");
        }
}

int findNextHop(int src,int dst){
        int cur=dst;
        if(src==dst) return src;
        while(parent[cur]!=-1 && parent[cur]!=src)
                cur=parent[cur];
        if(parent[cur]==-1) return -1;
        return cur;
}

void showPath(int dst){
        int path[MAX],cnt=0,i;
        while(dst!=-1){
                path[cnt++]=dst;
                dst=parent[dst];
        }
        for(i=cnt-1;i>=0;i--){
                printf("%d",path[i]);
                if(i!=0) printf(" -> ");
        }
}

void showTable(int src){
        int i,nh;
        printf("\nRouting Table for Node %d (%d entries)\n",src,n-1);
        printf("Dest\tCost\tPrevNode\tNextHop\tHops\tPath\n");
        for(i=0;i<n;i++){
                if(i==src) continue;
                nh=findNextHop(src,i);
                printf("%d\t",i);
                if(dist[i]==INF) printf("INF\t"); else printf("%d\t",dist[i]);
                if(parent[i]==-1) printf("-\t\t"); else printf("%d\t\t",parent[i]);
                if(nh==-1) printf("-\t"); else printf("%d\t",nh);
                if(hops[i]==INF) printf("-\t"); else printf("%d\t",hops[i]);
                if(dist[i]==INF) printf("-\n");
                else { showPath(i); printf("\n"); }
        }
}

void dijkstra(int src){
        int i,u,v,nd;

        for(i=0;i<n;i++){
                dist[i]=INF; hops[i]=INF; parent[i]=-1; visited[i]=0;
        }
        dist[src]=0; hops[src]=0;
        hsize=0;
        push(src,0);

        while(hsize>0){
                // greedy pick - always move to the unvisited node with the smallest distance found so far,
                // regardless of how long the path becomes after that node - never revisit this choice
                u=popmin();
                if(visited[u]) continue;
                visited[u]=1;

                for(v=0;v<n;v++){
                        if(!visited[v] && graph[u][v]>0){
                                nd=dist[u]+graph[u][v];
                                if(nd<dist[v]){
                                        dist[v]=nd;
                                        hops[v]=hops[u]+1;
                                        parent[v]=u;
                                        push(v,nd);
                                }
                        }
                }
        }
        showTable(src);
}

int main(){
        int src;

        readGraph();
        showGraph();

        for(src=0;src<n;src++){
            dijkstra(src);
        }

        return 0;
}

/*
[24bcs174@mepcolinux ex4]$gcc p2.c
[24bcs174@mepcolinux ex4]$./a.out
Enter number of nodes: 9
Enter number of edges: 14
Edge 1 - enter node1,node2,weight: 0 1 4
Edge 2 - enter node1,node2,weight: 1 2 11
Edge 3 - enter node1,node2,weight: 0 2 8
Edge 4 - enter node1,node2,weight: 1 3 8
Edge 5 - enter node1,node2,weight: 3 4 2
Edge 6 - enter node1,node2,weight: 4 5 6
Edge 7 - enter node1,node2,weight: 2 5 1
Edge 8 - enter node1,node2,weight: 2 4 7
Edge 9 - enter node1,node2,weight: 3 6 7
Edge 10 - enter node1,node2,weight: 6 7 9
Edge 11 - enter node1,node2,weight: 7 8 10
Edge 12 - enter node1,node2,weight: 5 8 2
Edge 13 - enter node1,node2,weight: 3 8 4
Edge 14 - enter node1,node2,weight: 6 8 14

Adjacency Matrix (-1 = no link)

       0   1   2   3   4   5   6   7   8
  0    0   4   8  -1  -1  -1  -1  -1  -1
  1    4   0  11   8  -1  -1  -1  -1  -1
  2    8  11   0  -1   7   1  -1  -1  -1
  3   -1   8  -1   0   2  -1   7  -1   4
  4   -1  -1   7   2   0   6  -1  -1  -1
  5   -1  -1   1  -1   6   0  -1  -1   2
  6   -1  -1  -1   7  -1  -1   0   9  14
  7   -1  -1  -1  -1  -1  -1   9   0  10
  8   -1  -1  -1   4  -1   2  14  10   0

Routing Table for Node 0 (8 entries)
Dest	Cost	PrevNode	NextHop	Hops	Path
1	4	0		1	1	0 -> 1
2	8	0		2	1	0 -> 2
3	12	1		1	2	0 -> 1 -> 3
4	14	3		1	3	0 -> 1 -> 3 -> 4
5	9	2		2	2	0 -> 2 -> 5
6	19	3		1	3	0 -> 1 -> 3 -> 6
7	21	8		2	4	0 -> 2 -> 5 -> 8 -> 7
8	11	5		2	3	0 -> 2 -> 5 -> 8

Routing Table for Node 1 (8 entries)
Dest	Cost	PrevNode	NextHop	Hops	Path
0	4	1		0	1	1 -> 0
2	11	1		2	1	1 -> 2
3	8	1		3	1	1 -> 3
4	10	3		3	2	1 -> 3 -> 4
5	12	2		2	2	1 -> 2 -> 5
6	15	3		3	2	1 -> 3 -> 6
7	22	8		3	3	1 -> 3 -> 8 -> 7
8	12	3		3	2	1 -> 3 -> 8

Routing Table for Node 2 (8 entries)
Dest	Cost	PrevNode	NextHop	Hops	Path
0	8	2		0	1	2 -> 0
1	11	2		1	1	2 -> 1
3	7	8		5	3	2 -> 5 -> 8 -> 3
4	7	2		4	1	2 -> 4
5	1	2		5	1	2 -> 5
6	14	3		5	4	2 -> 5 -> 8 -> 3 -> 6
7	13	8		5	3	2 -> 5 -> 8 -> 7
8	3	5		5	2	2 -> 5 -> 8

Routing Table for Node 3 (8 entries)
Dest	Cost	PrevNode	NextHop	Hops	Path
0	12	1		1	2	3 -> 1 -> 0
1	8	3		1	1	3 -> 1
2	7	5		8	3	3 -> 8 -> 5 -> 2
4	2	3		4	1	3 -> 4
5	6	8		8	2	3 -> 8 -> 5
6	7	3		6	1	3 -> 6
7	14	8		8	2	3 -> 8 -> 7
8	4	3		8	1	3 -> 8

Routing Table for Node 4 (8 entries)
Dest	Cost	PrevNode	NextHop	Hops	Path
0	14	1		3	3	4 -> 3 -> 1 -> 0
1	10	3		3	2	4 -> 3 -> 1
2	7	4		2	1	4 -> 2
3	2	4		3	1	4 -> 3
5	6	4		5	1	4 -> 5
6	9	3		3	2	4 -> 3 -> 6
7	16	8		3	3	4 -> 3 -> 8 -> 7
8	6	3		3	2	4 -> 3 -> 8

Routing Table for Node 5 (8 entries)
Dest	Cost	PrevNode	NextHop	Hops	Path
0	9	2		2	2	5 -> 2 -> 0
1	12	2		2	2	5 -> 2 -> 1
2	1	5		2	1	5 -> 2
3	6	8		8	2	5 -> 8 -> 3
4	6	5		4	1	5 -> 4
6	13	3		8	3	5 -> 8 -> 3 -> 6
7	12	8		8	2	5 -> 8 -> 7
8	2	5		8	1	5 -> 8

Routing Table for Node 6 (8 entries)
Dest	Cost	PrevNode	NextHop	Hops	Path
0	19	1		3	3	6 -> 3 -> 1 -> 0
1	15	3		3	2	6 -> 3 -> 1
2	14	5		3	4	6 -> 3 -> 8 -> 5 -> 2
3	7	6		3	1	6 -> 3
4	9	3		3	2	6 -> 3 -> 4
5	13	8		3	3	6 -> 3 -> 8 -> 5
7	9	6		7	1	6 -> 7
8	11	3		3	2	6 -> 3 -> 8

Routing Table for Node 7 (8 entries)
Dest	Cost	PrevNode	NextHop	Hops	Path
0	21	2		8	4	7 -> 8 -> 5 -> 2 -> 0
1	22	3		8	3	7 -> 8 -> 3 -> 1
2	13	5		8	3	7 -> 8 -> 5 -> 2
3	14	8		8	2	7 -> 8 -> 3
4	16	3		8	3	7 -> 8 -> 3 -> 4
5	12	8		8	2	7 -> 8 -> 5
6	9	7		6	1	7 -> 6
8	10	7		8	1	7 -> 8

Routing Table for Node 8 (8 entries)
Dest	Cost	PrevNode	NextHop	Hops	Path
0	11	2		5	3	8 -> 5 -> 2 -> 0
1	12	3		3	2	8 -> 3 -> 1
2	3	5		5	2	8 -> 5 -> 2
3	4	8		3	1	8 -> 3
4	6	3		3	2	8 -> 3 -> 4
5	2	8		5	1	8 -> 5
6	11	3		3	2	8 -> 3 -> 6
7	10	8		7	1	8 -> 7
*/
