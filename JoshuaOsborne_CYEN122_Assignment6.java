import java.util.Iterator;
import java.util.BitSet;

/**
 *@author Joshua Osborne
 * This code finds the sum of all prime numbers from 1 to 2 million
 * This was a challenge presented as a ProjectEuler problem
 *
 */
class Summation
{
	private static BitSet sieve; //using the Sieve of Eratosthenes
	private static final int maxNumber = 2000000;
	private static double maxNumberSquareroot = Math.sqrt(maxNumber);
	private static long sum = 0;
	
	public static void main(String[] args)
	{
		sieve = new BitSet(maxNumber); //prime numbers will be true; nonprime is false. The index IS the number.
		sieve.set(2, maxNumber);
		for (int i = 4; i < maxNumber; i += 2)
		{
			sieve.flip(i); //makes all even numbers after 2 false
		}
		
		for (int i = 3; i < sieve.length(); i+=2) //i starting at three so as to skip 2
		{
			//prime is i
			if (sieve.get(i) && i < maxNumberSquareroot)
			{
				for (int j = i*i; j < maxNumber; j += 2*i) //j iterates over every second multiple
															   //of the prime starting after prime^2
															   //since anything composite behind that would have
															   //had a factor of a prime number checked before
				{
					sieve.clear(j);
				}
			}
		}
		for (int i = 0; i < sieve.length(); i++)
		{
			sum += sieve.get(i) ? i : 0;
		}
		System.out.println(sum);
	}
}