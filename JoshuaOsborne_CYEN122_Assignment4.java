import java.lang.Math;
/**
 * @author Joshua Osborne
 * This code find a a pythagorean triple whose sum is 1000 and prints a product
 * This problem was presented as a ProjectEuler puzzle
 */
class Pythagorean
{
	public static void main(String [] args)
	{
		boolean found = false;
		int a = 0;
		int b = 0;
		int c = 0;
		int solution = 0;
		while(!found)
		{
			for (a = 1; (a < 710) && !found; a++)
			{
				for (b = 1; (b < 710) && !found; b++)
				{
					c = findC(a, b);
					if (c != -1)
					{
						if (sum(a, b, c) == 1000)
						{
							found =  true;
							solution = a*b*c;
						}
					}				
				}
			}
		}
		System.out.println(solution);
	}
	
	public static int sum(int a, int b, int c)
	{
		return a + b + c;
	}
	
	public static int findC(int a, int b)
	{
		double c = Math.sqrt((double)(a*a) + (double)(b*b));
		if (c%1 != 0)
			return -1;
		else
			return (int) c;
	}
}