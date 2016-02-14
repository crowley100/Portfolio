/*****************************************************************************
 * CountiesMap
 *
 * @version 1.0
 * @author Lorcan Reidy
 *
 * The constructor of this class loads a MapImage object.
 * The constructor then computes the regions with the same colour on the map.
 * We call these regions <b>counties</b>.
 * 
 * Once the counties have been computed, their size can be obtained by calling
 * the method <code>getCountySize</code>.
 *
 * This implementation is based on the data-structure/algorithm: 
 * 
 * 	Union-Find
 * 
 * 	To ensure the data-structure/algorithm ran efficiently, I used a weighted quick union
 * 	with path compression. This means that whenever pixels were connected, the smaller tree
 *  of pixels was connected below the larger tree. This means the height of the Union-Find tree
 *  will be shorter, and it will run more efficiently. After computing the root of a particular node
 *  path compression causes each examined node in that path to point to it's grandparent (root).
 *  This ensures the tree remains flat.
 *  
 *  Memory use:
 *  	N + N + N + 13(int variables) = 3N + 13
 *  	Therefore, memory usage is O[N]
 *  
 *  	justification: instance of MapImage, map, stores N pixels
 *  				   instance of UnionFind, myUnionFind, stores 2N pixels ( sz and id arrays )
 *  				   N + 2N = 3N
 *  				   memory used for integer variables is negligible when using Big-Oh notation
 *  				   as is the coefficient of N (3).
 *  				   This results in memory usage of O[N].
 *  	
 *  Amortized run time of methods:
 *  	((N + Mlog*N) + (log*N) + (Nlog*N)) / (M + 1)
 *  	therefore, amortized run time is O[((N + Mlg*N) + (log*N) + (Nlog*N)) / M]
 *  
 *  Worst-case asymptotic run time of methods:
 *  	CountiesMap: O[Nlog*N]
 *
 *  	justification: With a MapImage of N pixels being passed as a parameter,
 *  				   the method will index through every pixel in the image,
 *  				   calling the union method from UnionFind. The union method has a run time of log*N
 *  				   and is called for each pixel, resulting in a total run time of Nlog*N.
 *  
 *  	getCountySize: O[log*N]
 *  
 *  	justification: getCountySize calls the find method from UnionFind.
 *  				   resulting in a total run time of log*N
 *  
 *  	UnionFind: O[N + log*N + log*N]
 * 
 * 		justification: UnionFind's constructor, passed a parameter of N, will loop N times.
 * 					   The find method takes time proportional to the depth of the node being searched for.
 * 					   Depth of any node x is at most logN. With path compression the run time is reduced to log*N.
 * 					   The union method executes a constant number of lines of code, however it calls the
 * 					   find method twice.
 *
 *****************************************************************************/
public class CountiesMap
{
	  private final MapImage map;
	  private final int width;
	  private final int height;
	  private final UnionFind myUnionFind;
	
	  /**
	   * The constructor does all the map calculations.
	   * The parameter of the class contains a map of counties of a country.
	   * There is no text on the map. It has only single-colour regions.
	   * Some of these single-colour regions represent counties (you don't know which ones).
	   * There might be other regions on the map such as lakes, oceans, islands etc.
	   *
	   * @param map this is a MapImage object
	   */
	  public CountiesMap(MapImage map)
	  { 
		    this.map = map;
		    this.width = map.getWidth();
		    this.height = map.getHeight();
		    myUnionFind = new UnionFind(this.width * this.height);
		    
		    int x;
		    int y;
		    int pixelColor;
		    int pixelIndex;
		    int prevPixelColor;
		    int prevPixelIndex;
		    int belowPixelColor;
		    int belowPixelIndex;
		    
		    pixelIndex = getIndex(map.getMinX(), map.getMinY());
		    pixelColor = map.getRGB(map.getMinX(), map.getMinY());
		    
		    for(y = map.getMinY(); y < this.height-1; y++)  
		    {
			    for(x = (map.getMinX()+1); x < this.width; x++)
			    {
			    	prevPixelIndex = pixelIndex;
			    	prevPixelColor = pixelColor;
			    	pixelColor = map.getRGB(x, y); 
			    	pixelIndex = getIndex(x, y);
			    	
			    	
			    	belowPixelColor = map.getRGB(x, y+1); // pixel below to ensure continuity with previous row
			    	belowPixelIndex = getIndex(x, y+1);
			    	
			    	if(prevPixelColor == pixelColor)
			    	{
			    		myUnionFind.union(prevPixelIndex, pixelIndex);
			    	}
			    	if(belowPixelColor == pixelColor)
			    	{
			    		myUnionFind.union(belowPixelIndex, pixelIndex);
			    	}
			    }
		    }
		    
	  }
	
	  /**
	   * This method returns the size in pixels of the region which includes the point (x,y).
	   *
	   * @param x the x-coordinate of the point in the region.
	   * @param y the y-coordinate of the point in the region.
	   * @return the size of the region in pixels.
	   */
	  public int getCountySize(int x, int y)
	  {	  	
		    int index = getIndex(x, y);
		    int root = myUnionFind.find(index);
		    int size = myUnionFind.getSize(root);
		    return size;
	  }
	
	  /**
	   * This method can be used to convert the map's (x,y) coordinates to a unique linear index.
	   * Suppose we want to store all pixels of the map in a one-dimentional array.
	   * Then the array will have to have size (map.getHeight() * map.getWidth()).
	   * Pixel (x,y) will be at position getIndex(x,y) in the array.
	   *
	   * @param x the x-coordinate of the pixel.
	   * @param y the y-coordinate of the pixel.
	   * @return the index in a 1-dimentional array corresponding to pixel (x,y).
	   */
	  private int getIndex(int x, int y)
	  {
		  return y * map.getWidth()  +  x;
	  }

}
