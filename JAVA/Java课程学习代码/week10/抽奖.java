package com.cl.week10;

public class Lotto {
	public static void main(String[] args) {
		int i = 0, j = 0;
		String[][] prize = new String[10][];
		prize[0] = new String[] { "apple", "benench", "thinkpad", "iphone" };
		String str = null;
		int k = 4,c = 0;
		while (i <= 3) {
			j = 0;
			str = prize[i][(int) (Math.random() * (k--))];
			System.out.println("第"+c+"次抽到的奖品为:"+str);
			//错误代码//prize[i+1] = new String[prize[i].length];
			prize[i+1] = new String[prize[i].length-1];
			/*
			每次开辟的空间都为4个,所以为赋值的空间默认值为null,但是下面要进行比较,所以会产生null和null的比较
			但这种比较会产生java.lang.NullPointerException错误.
			 
			 */
			for (String s : prize[i]) {
				if (!s.equals(str)) {
					prize[i+1][j] = s;
					j++;
				}
			}
			i++;
			c++;
		}

	}
}
