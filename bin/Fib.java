
public class Fib {

	
	public static String  hello() {
		 
		return  "hello ! " ;
	}
	
	
	public static int fib ( int a , int b ) {
		if( a==10 ) return a+b ;
		System.err.print( a );
		System.err.print( " " );
		System.err.println( b );
		return fib ( a+1 , a+b )  ;
	}
	
	
	
	
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		 System.err.println( hello() );
		 int n=10 ;
		 int p=n;
		 fib (0 ,0 ) ;
          
        
           
		
	}

}
