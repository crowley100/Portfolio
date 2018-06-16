
public class UnionFind {
	
	 private int[] id;
	 private int[] sz;
	 
	 public UnionFind(int N)
	 {
		 id = new int[N];
		 sz = new int[N];
		 for (int i = 0; i < N; i++)
		 {
			 id[i] = i;
		 	 sz[i] = 1; // each tree initially has 1 element...
		 }
     }
	 
	
	 public int find(int i)
	 {
		 while (i != id[i])
		 {
			 id[i] = id[id[i]];
			 i = id[i];
		 }
		 return i; 
	 }
	
	 public void union(int p, int q)
	 {
		 int i = find(p);
		 int j = find(q);
		 // weighting
		 if( i == j ) return;
		 if(sz[i] < sz[j])
		 {
			 id[i] = j; sz[j] += sz[i];
		 }
		 else
		 {
			 id[j] = i; sz[i] += sz[j];
		 }
	 }
	 
	 public boolean connected(int p, int q)
	 {
		 return find(p) == find(q); 
	 }
	 
	 public int getSize(int index)
	 {
		 return sz[index];
	 }
}
