class Hanoi
{
	private static int count;
	
	public static void main (String[] args)
	{
		count = 0;
		move(10, 'A', 'B', 'C');
		System.out.println(count + " number of moves");
	}
	
	public static void move(int disks, char from, char spare, char dest)
	{
		if (disks == 1)
		{
			System.out.println(from + " -> " + dest);
			count++;
		}
		else
		{
			move(disks-1, from, dest, spare);
			move(1, from, spare, dest);
			move(disks-1, spare, from, dest);
		}
	}
}