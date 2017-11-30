package com.cl.week11;

import java.util.HashMap;
import java.util.Iterator;
import java.util.Scanner;

public class CountDigital {
	public static void main(String[] args) {
		Scanner in = new Scanner(System.in);
		HashMap<Integer, Integer> hm = new HashMap<Integer, Integer>();
		String str = new String(in.nextLine());
		String[] strs = str.split(" ");
		in.close();
		int i=0;
		while (i<strs.length) {
			int num = Integer.parseInt(strs[i]);
			if (hm.containsKey(num)) {
				int cnt = hm.get(num);
				cnt++;
				hm.put(num, cnt);
			} else {
				hm.put(num, 1);
					
			}
			i++;
		}
		Iterator<Integer> numIter = hm.keySet().iterator();
		while(numIter.hasNext()) {
			int num = numIter.next();
			System.out.println(num+":"+hm.get(num));
		}
		
	}
}
